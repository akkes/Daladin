extern crate rand;

use rand::Rng;
///Represents a Makrov chain.
struct Makrov {
    number_of_chains: u32,
    sensibility: u32,
    values: Vec<Vec<u32>>,
    actual_node : usize,
}
impl Makrov {
    ///Creates a new Makrov chain.
    /// # Arguments
    ///
    /// * `number_of_chains` - Number of chains the Makrov chain will have. Each chain can be a podcast ID.
    /// * `sensibility` - This will afflict how much the probability values will evolve depending on the user feedback.
    fn new(number_of_chains: u32, sensibility: u32, starting_node : usize) -> Makrov {
        Makrov {
            number_of_chains : number_of_chains,
            sensibility : sensibility,
            values : Makrov::init_values(number_of_chains, starting_node),
            actual_node : starting_node, /*It is.*/
        }
    }

    fn init_values(number_of_chains: u32, starting_node : usize) -> Vec<Vec<u32>> {
        //println!("INITIALISATION VALEURS A {:?}", std::u32:MAX);
        println!("Reste a répartir {:?}", std::u32::MAX%number_of_chains);
        let mut ret = vec![vec![std::u32::MAX / number_of_chains; number_of_chains as usize]; number_of_chains as usize];
        // let mut rand_y = rand::thread_rng().gen_range(0, number_of_chains);
        // let mut rand_x;
        //Makrov::add_noise(self, std::u32::MAX%number_of_chains);
        //println!("Reste de la division {:?}", std::u32::MAX % number_of_chains);
        /*TODO : This should solve le souci du reste, mais ça marche toujours pas. */
        return ret;
    }
    fn add_noise(values : &Vec<Vec<u32>>, noise: u32) -> Vec<Vec<u32>> {
        let mut ret = values.clone();
        let mut rand_y = rand::thread_rng().gen_range(0, values.len());
        let mut rand_x;
        for i in 0..noise {
            //println!("lskdj");
            rand_x = rand::thread_rng().gen_range(0, values.len());
            ret[rand_x as usize][rand_y as usize] += 1;
            //println!("X {} Y {} VALUE {}", rand_x, rand_y, values[rand_x as usize][rand_y as usize]);
        }
        return ret;
    }
    fn printValues(&self) {
        println!("{:?}", self.values);
    }
    fn isValid(&self) -> bool {
        /*TODO: Rendre la fonction utile. Là on a juste un seuil.*/
        return self.values[0].iter().fold(0, |sum, val| sum+val) > std::u32::MAX-self.number_of_chains;
    }
    fn get_sensibility(&self) -> u32 {
        return self.sensibility
    }

    fn get_next_node(&mut self) -> usize {
        let mut random = rand::thread_rng().gen_range(0u32, std::u32::MAX);
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
    /* NE FONCTIONNE PAS. Juste un c/c de la fonction que j'avais fait pour tester*/
    fn apply_feedback(&mut self, feedback : bool, startingNode : usize) {
        //println!("{:?}", feedback as f32 * sensibility);
        //TODO: Resultats bizarres quand on soustrait.
        println!("VOici le feedback {:?}", feedback);
        if feedback == false {
            self.values[startingNode][self.actual_node] -=  self.sensibility;
        } else {
            self.values[startingNode][self.actual_node] +=  self.sensibility;
        };
        //self.values[startingNode][self.actual_node] +=  signedSensibility;
    }
}
#[test]
fn it_works() {
    let mut mTest = Makrov::new(4, 10, 0);
    println!("{:?}", mTest.isValid());
    /*mTest.printValues();
    let arr = vec![1, 1, 1, 1, 1];
    let sum = arr.iter().fold(0, |sum, val| sum + val);
    //sum +=
    println!("{:?}", sum);
    //println!("{:?}", hello);*/
    //println!("{:?}", mTest.isValid());


    mTest.printValues();
    println!("{:?}", mTest.get_next_node());
    mTest.apply_feedback(true, 1);
    mTest.printValues();
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
}
