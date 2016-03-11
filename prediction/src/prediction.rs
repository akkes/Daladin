extern crate libc;
extern crate rustc_serialize;
mod markov;
use libc::uint32_t;
use self::markov::Markov as Markov;
//use markov::Markov;
///Array of `Markov` chains
pub struct Register {
    register : Vec<Markov>
}

impl Register {
    ///Inits an empty `Register`.
    pub fn new() -> Register {
        Register {
            register : Vec::new(),
        }
    }
    ///Returns the number of `Markov` chains in the current `Register`
    pub fn get_size(&self) -> u32 {
        return self.register.len() as u32
    }
    /*pub fn get_markov(& self, id : usize) -> Markov {
        let ret = &self.register[id];
        //TODO!!!!
        return ret
    }*/
    ///Creates a 1-node `Markov` chain and returns its id
    pub fn add_radio(&mut self) -> usize {
        self.register.push(Markov::new(1, 0.1, 0));
        return self.register.len()-1; //The ID of the newly created Markov chain.
    }
    ///Adds a node onto the `Markov` with the id `markov_id`
    pub fn add_content(&mut self, markov_id : usize) -> usize {
        self.register[markov_id].add_node()
    }
    ///Gets the next node of the `Markov`, by calculating time-dependent probabilities
    pub fn get_next_content(&mut self, markov_id : usize) -> usize {
        self.register[markov_id].get_next_node()
    }
    ///Change the probability of the last transition depending on the `sensibility` and the actual time.
    pub fn apply_feedback(&mut self, markov_id : usize, feedback : bool) {
        self.register[markov_id].apply_feedback(feedback)
    }
}

//FOR FFI
#[no_mangle]
///FFI equivelent to `Register::new()`
pub extern fn register_new() -> *mut Register {
    Box::into_raw(Box::new(Register::new()))
}
///FFI equivelent to the Register destructor`
#[no_mangle]
pub extern fn register_free(ptr : *mut Register) {
    if ptr.is_null() { return }
    unsafe { Box::from_raw(ptr); }
}
///FFI equivelent to `Register::get_size()`
#[no_mangle]
pub extern fn register_get_size(ptr: *const Register) -> uint32_t {
    let register = unsafe {
        assert!(!ptr.is_null());
        &*ptr
    };
    register.get_size() as uint32_t
}
///FFI equivelent to `Register::add_radio()`
#[no_mangle]
pub extern fn register_add_radio(ptr : *mut Register) -> uint32_t {
    let register = unsafe {
        assert!(!ptr.is_null());
        &mut *ptr
    };
    return register.add_radio() as uint32_t
}
///FFI equivelent to `Register::add_content()`
#[no_mangle]
pub extern fn register_add_content(ptr : *mut Register, markov_id : uint32_t) -> uint32_t {
    let register = unsafe {
        assert!(!ptr.is_null());
        &mut *ptr
    };
    return register.add_content(markov_id as usize) as u32
}
///FFI equivelent to `Register::get_next_content()`
#[no_mangle]
pub extern fn register_get_next_content(ptr : *mut Register, markov_id : uint32_t) -> uint32_t {
    let register = unsafe {
        assert!(!ptr.is_null());
        &mut *ptr
    };
    return register.get_next_content(markov_id as usize) as u32;
}
///FFI equivelent to `Register::apply_feedback()`
#[no_mangle]
pub extern fn register_apply_feedback(ptr : *mut Register, markov_id : uint32_t, feedback : uint32_t) {
    let register = unsafe {
        assert!(!ptr.is_null());
        &mut *ptr
    };
    if feedback == 0 {
        register.apply_feedback(markov_id as usize, false);
    } else {
        register.apply_feedback(markov_id as usize, true);
    }
}
