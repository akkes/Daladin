extern crate rand;
extern crate time;

use self::time::Duration;
use self::time::Tm;
use self::rand::Rng;
use std::cmp::PartialOrd;
///Represents a Markov chain.
pub struct Markov {
    number_of_chains: u32,
    sensibility: f64,
    values: Vec<Vec<f64>>,
    durations: Vec<Duration>,
    hours: Vec<Tm>,
    previous_checked_node : usize,
    last_checked_node : usize, //Is it useful ?
}
impl Markov {
    ///Creates a new Markov chain.
    /// # Arguments
    ///
    /// * `number_of_chains` - Number of chains the Markov chain will have. Each chain can be a podcast ID.
    /// * `sensibility` - This will afflict how much the probability values will evolve depending on the user feedback.
    pub fn new(number_of_chains: u32, sensibility: f64, starting_node : usize) -> Markov {
        Markov {
            number_of_chains : number_of_chains,
            sensibility : sensibility,
            values : Markov::init_values(number_of_chains, starting_node),
            durations : vec![Duration::minutes(60);number_of_chains as usize],
            hours : vec![time::strptime("2-14-47-26", "%w-%H-%M-%S").unwrap();number_of_chains as usize],
            previous_checked_node : 0,
            last_checked_node : starting_node, /*It is.*/
        }
    }

    fn init_values(number_of_chains: u32, starting_node : usize) -> Vec<Vec<f64>> {
        let mut ret = vec![vec![1.0f64/number_of_chains as f64; number_of_chains as usize]; number_of_chains as usize];
        return ret;
    }
    pub fn printValues(&self) {
        println!("DBG : Contenu de self.values {:?}", self.values);
    }
    pub fn get_values(&self) -> Vec<Vec<f64>> {
        self.values.clone()
    }
    pub fn get_value(&self, from : usize, to : usize) -> f64 {
        self.values[from][to]
    }
    pub fn sum(&self, line : usize) -> f64 {
        let ret = self.values[line].iter().fold(0f64, |sum, val| sum + val) as f64;
        return ret;
    }
    pub fn get_sensibility(&self) -> f64 {
        self.sensibility
    }
    pub fn get_last_checked_node(&self) -> usize {
        self.last_checked_node
    }
    pub fn get_previous_checked_node(&self) -> usize {
        self.previous_checked_node
    }
    pub fn get_number_of_chains(&self) -> u32 {
        self.number_of_chains
    }
    /*pub fn get_next_node(&mut self) -> usize {
        let mut random = rand::thread_rng().gen_range(0f64, 1f64);
        //println!("{:?}", random);
        /*TODO: Faire moins sale ave des filter() et fold()*/
        for i in 0..self.values.len() {
            //println!("Random : {:?} and values : {:?}", random, self.values[self.last_checked_node][i]);
            if random < self.values[self.last_checked_node][i] {
                self.last_checked_node = i;
                return i;
            } else {
                random -= self.values[self.last_checked_node][i];
            }
        }
        return 1;
    }*/
    pub fn set_debug_nodes(&mut self) {
        self.previous_checked_node = 0;
        self.last_checked_node = 1;
    }
    pub fn set_value(&mut self, from : usize, to: usize, value : f64) {
        //TODO: Chack if float isn't 0<float<1
        //TODO : NOT OVERFLOW SAFE !
        //println!("{:?}", value.partial_cmp(&1f64));
        /*match value.partial_cmp(&1f64) {
            Greater => value = 1f64,

        };*/
        let delta = self.values[from][to] - value;
        for i in 0..self.values.len() {
            if i==to {
                self.values[from][to] = value;
            } else {
                self.values[from][i] += delta/(self.number_of_chains-1) as f64;
            }
        }
    }
    pub fn get_probability(&self, from : usize, to : usize) -> f64 {
        let a = self.values[from][to];
        let mut hour = time::now();
        hour.tm_mday = 0;
        hour.tm_mon = 0;
        hour.tm_year = 0;
        hour.tm_yday = 0;
        hour.tm_utcoff = 0;
        hour.tm_nsec = 0;
        //println!("{:?}", normal_hour);
        //println!("{:?}", hour);
        let top_exp = -((self.hours[to]-hour).num_seconds() as f64).powi(2);
        let bot_exp = 2f64 * (self.durations[to].num_seconds() as f64/4f64).powi(2);
        let egblog = a*(top_exp/bot_exp).exp();
        return egblog;
    }
    pub fn get_hour(&self, id : usize) -> Tm {
        self.hours[id]
    }
    pub fn set_hour(&mut self, id : usize, hour : Tm) {
        self.hours[id] = hour
    }
    pub fn get_duration(&self, id : usize) -> Duration {
        self.durations[id]
    }
    pub fn get_next_node(&mut self) -> usize {
        self.previous_checked_node = self.last_checked_node;
        let lcn = self.last_checked_node;
        let mut gaussian_probs = vec![0f64; self.number_of_chains as usize];
        //println!("Taille gauss {:?}", gaussian_probs.len());
        for i in 0..gaussian_probs.len() {
            gaussian_probs[i] = self.get_probability(lcn, i);
        }
        //println!("{:?}", gaussian_probs);
        let sum_probs = gaussian_probs.iter().fold(0f64, |sum, val| sum + val) as f64;
        //println!("{:?}", sum_probs); //TODO: Vérifier si c'est toujours ~1
        let mut random = rand::thread_rng().gen_range(0f64, sum_probs);
        for i in 0..self.number_of_chains {
            //println!("Random : {:?} and values : {:?}", random, self.values[self.last_checked_node][i]);
            if random < gaussian_probs[i as usize] {
                self.last_checked_node = i as usize;
                return i as usize;
            } else {
                random -= gaussian_probs[i as usize];
            }
            //return 1;
        }
        //println!("{:?}", self.get_hour(3));
        return 1;
    }
    pub fn add_node(&mut self) -> usize {
        //let default_prob = 1f64/(self.number_of_chains+1) as f64;
        for i in 0..self.number_of_chains {
            self.values[i as usize].push(0f64);
        }
        self.number_of_chains += 1;
        self.values.push(vec![1f64/self.number_of_chains as f64; self.number_of_chains as usize]);
        let noc = self.number_of_chains;
        for i in 0..noc {
            //println!("RESTE {:?}", 1f64/self.number_of_chains as f64);
            //self.values[i as usize][self.number_of_chains as usize - 1] = 1f64/self.number_of_chains as f64;
            self.set_value(i as usize, noc as usize -1, 1f64/noc as f64);
        }
        let  mut hour = time::now();
        hour.tm_mday = 0;
        hour.tm_mon = 0;
        hour.tm_year = 0;
        hour.tm_yday = 0;
        hour.tm_utcoff = 0;
        hour.tm_nsec = 0;
        self.hours.push(hour);
        self.durations.push(Duration::minutes(60)); //TODO: Make actual duration matter

        return self.number_of_chains as usize -1;
    }
    pub fn apply_feedback(&mut self, feedback : bool) { //TODO: Vérifier le bon fonctionnement du décalage temporel.
        let lcn = self.last_checked_node;
        let pcn = self.previous_checked_node;
        let mut sensibility_to_apply = self.sensibility;
        let mut new_value = self.get_value(pcn, lcn);
        if !feedback {
            sensibility_to_apply *= -1f64;
        }else{
            let mut now = time::now();
            now.tm_mday = 0;
            now.tm_mon = 0;
            now.tm_year = 0;
            now.tm_yday = 0;
            now.tm_utcoff = 0;
            now.tm_nsec = 0;
            let delta = now-self.hours[lcn];
            //println!("{:?}", delta);
            //println!("{:?}", self.hours[lcn] + delta);
            if(now<self.hours[lcn]) {
                self.hours[lcn] = self.hours[lcn] - delta/2;
            } else {
                self.hours[lcn] = self.hours[lcn] + delta/2;
            }
            self.hours[lcn].tm_mday = 0;
            self.hours[lcn].tm_mon = 0;
            self.hours[lcn].tm_year = 0;
            self.hours[lcn].tm_yday = 0;
            self.hours[lcn].tm_utcoff = 0;
            self.hours[lcn].tm_nsec = 0;
        }
        self.set_value(pcn, lcn, new_value + sensibility_to_apply);
    }

}

#[test]
fn it_works() {
    let mut mTest = Markov::new(3, 0.1, 0);
    mTest.set_debug_nodes();
    println!("{:?}", mTest.get_probability(0, 1));
    println!("{:?}", mTest.get_hour(1));
    mTest.apply_feedback(true);
    println!("{:?}", mTest.get_hour(1));

    mTest.apply_feedback(true);
    println!("{:?}", mTest.get_hour(1));

    mTest.apply_feedback(true);
    println!("{:?}", mTest.get_hour(1));

    mTest.apply_feedback(true);

    println!("{:?}", mTest.get_hour(1));
    mTest.apply_feedback(true);
    println!("{:?}", mTest.get_hour(1));

    mTest.apply_feedback(true);


    println!("{:?}", mTest.get_hour(1));
}
