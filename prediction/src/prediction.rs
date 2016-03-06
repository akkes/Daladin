extern crate libc;

mod markov;

use libc::uint32_t;
use self::markov::Markov as Markov;
//use markov::Markov;
pub struct Register {
    register : Vec<Markov>
}

impl Register {
    pub fn new() -> Register {
        Register {
            register : Vec::new(),
        }
    }
    pub fn get_size(&self) -> u32 {
        return self.register.len() as u32
    }
    pub fn add_radio(&mut self) -> usize {
        self.register.push(Markov::new(1, 0.1, 0));
        return self.register.len()-1; //The ID of the newly created Markov chain.
    }
    pub fn add_content(&mut self, markov_id : usize) -> usize {
        self.register[markov_id].add_node()
    }
}

//FOR FFI
#[no_mangle]
pub extern fn register_new() -> *mut Register {
    Box::into_raw(Box::new(Register::new()))
}

#[no_mangle]
pub extern fn register_free(ptr : *mut Register) {
    if ptr.is_null() { return }
    unsafe { Box::from_raw(ptr); }
}

#[no_mangle]
pub extern fn register_get_size(ptr: *const Register) -> uint32_t {
    let register = unsafe {
        assert!(!ptr.is_null());
        &*ptr
    };
    register.get_size() as uint32_t
}

#[no_mangle]
pub extern fn register_add_radio(ptr : *mut Register) -> uint32_t {
    let register = unsafe {
        assert!(!ptr.is_null());
        &mut *ptr
    };
    return register.add_radio() as uint32_t
}

#[no_mangle]
pub extern fn register_add_content(ptr : *mut Register, markov_id : uint32_t) -> uint32_t {
    let register = unsafe {
        assert!(!ptr.is_null());
        &mut *ptr
    };
    return register.add_content(markov_id as usize) as u32
}
