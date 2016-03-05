extern crate rand;
extern crate time;

use self::time::Duration;
use self::time::Tm;
use self::rand::Rng;
use std::cmp::PartialOrd;
///Represents a Makrov chain.
#[no_mangle]
pub struct Makrov {
    number_of_chains: u32,
    sensibility: u32,
    values: Vec<Vec<f64>>,
    actual_node : usize, //Is it useful ?
}
impl Makrov {
    ///Creates a new Makrov chain.
    /// # Arguments
    ///
    /// * `number_of_chains` - Number of chains the Makrov chain will have. Each chain can be a podcast ID.
    /// * `sensibility` - This will afflict how much the probability values will evolve depending on the user feedback.
    pub fn new(number_of_chains: u32, sensibility: u32, starting_node : usize) -> Makrov {
        Makrov {
            number_of_chains : number_of_chains,
            sensibility : sensibility,
            values : Makrov::init_values(number_of_chains, starting_node),
            actual_node : starting_node, /*It is.*/
        }
    }

    fn init_values(number_of_chains: u32, starting_node : usize) -> Vec<Vec<f64>> {
        //println!("INITIALISATION VALEURS A {:?}", std::u32:MAX);
        //println!("DBG : Reste a répartir en bruit {}", 1.0f64%number_of_chains);
        let mut ret = vec![vec![1.0f64/number_of_chains as f64; number_of_chains as usize]; number_of_chains as usize];
        // let mut rand_y = rand::thread_rng().gen_range(0, number_of_chains);
        // let mut rand_x;
        //Makrov::add_noise(self, MAX%number_of_chains);
        //println!("Reste de la division {:?}", MAX % number_of_chains);
        /*TODO : This should solve le souci du reste, mais ça marche toujours pas. */
        return ret;
    }
    /*fn add_noise(values : &Vec<Vec<u32>>, noise: u32) -> Vec<Vec<u32>> {
        println!("DBG : Ajout de {} de bruit ", noise);
        let mut ret = values.clone();
        let mut rand_y = rand::thread_rng().gen_range(0, values.len());
        for i in 0..noise {
            rand_y = rand::thread_rng().gen_range(0, values.len());
            ret[i as usize%values.len() as usize][rand_y as usize] += 1;
            println!("DBG : Valeur de bruit {} ajoutée en {} {}", i, i as usize%values.len(), rand_y);
            //println!("X {} Y {} VALUE {}", rand_x, rand_y, values[rand_x as usize][rand_y as usize]);
        }
        return ret;
    }*/
    pub fn printValues(&self) {
        println!("DBG : Contenu de self.values {:?}", self.values);
    }
    pub fn getValues(&self) -> Vec<Vec<f64>> {
        self.values.clone()
    }
    /*fn isValid(&self) -> bool {
        /*TODO: Rendre la fonction utile. Là on a juste un seuil.*/
        return self.values[0].iter().fold(0, |sum, val| sum+val) > MAX-self.number_of_chains;
    }*/
    pub fn sum(&self, line : usize) -> f64 {
        let ret = self.values[line].iter().fold(0f64, |sum, val| sum + val) as f64;
        return ret;
    }
    pub fn get_sensibility(&self) -> u32 {
        self.sensibility
    }
    pub fn get_actual_node(&self) -> usize {
        self.actual_node
    }
    pub fn get_number_of_chains(&self) -> u32 {
        self.number_of_chains
    }
    pub fn get_next_node(&mut self) -> usize {
        let mut random = rand::thread_rng().gen_range(0f64, 1f64);
        //println!("{:?}", random);
        /*TODO: Faire moins sale ave des filter() et fold()*/
        for i in 0..self.values.len() {
            //println!("Random : {:?} and values : {:?}", random, self.values[self.actual_node][i]);
            if random < self.values[self.actual_node][i] {
                self.actual_node = i;
                return i;
            } else {
                random -= self.values[self.actual_node][i];
            }
        }
        return 1;
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
    pub fn get_probability(self, from : usize, to : usize, duration : Duration, normal_hour : Tm) -> f64 {
        let a = self.values[from][to];
        let  mut hour = time::now();
        hour.tm_mday = 0;
        hour.tm_mon = 0;
        hour.tm_year = 0;
        hour.tm_yday = 0;
        hour.tm_utcoff = 0;
        hour.tm_nsec = 0;
        //println!("{:?}", normal_hour);
        //println!("{:?}", hour);
        let top_exp = -((normal_hour-hour).num_seconds() as f64).powi(2);
        let bot_exp = 2f64 * (duration.num_seconds() as f64/4f64).powi(2);
        let egblog = a*(top_exp/bot_exp).exp();
        return egblog;
    }
    /* NE FONCTIONNE PAS. Juste un c/c de la fonction que j'avais fait pour tester*/
    /*pub fn apply_feedback(&mut self, feedback : bool, startingNode : usize) {
        //println!("{:?}", feedback as f32 * sensibility);
        //TODO: Resultats bizarres quand on soustrait.
        //TODO: FIX OVERFLOWS
        //println!("DBG : Valeur de feedback {:?}", feedback);
        if feedback == false {
            self.values[startingNode][self.actual_node] -=  self.sensibility;
            for i in 0..self.values[startingNode].len() {
                if i != self.actual_node {
                    self.values[startingNode][i] += self.sensibility/self.values[startingNode].len() as u32;
                }
            }
        } else {
            self.values[startingNode][self.actual_node] +=  self.sensibility;
            for i in 0..self.values[startingNode].len() {
                if i != self.actual_node {
                    self.values[startingNode][i] -= self.sensibility/self.values[startingNode].len() as u32;
                }
            }
        };
        //self.values[startingNode][self.actual_node] +=  signedSensibility;
    }*/
}
#[test]
fn it_works() {
    let mut mTest = Makrov::new(4, 10000, 0);
    //println!("{:?}", mTest.printValues());
    //println!("{:?}", mTest.sum(0));
    println!("{:?}", mTest.get_probability(2, 3, Duration::minutes(90), time::strptime("1-14-47-26", "%w-%H-%M-%S").unwrap()));

}
