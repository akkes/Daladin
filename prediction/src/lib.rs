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
    fn new(number_of_chains: u32, sensibility: u32) -> Makrov {
        Makrov {
            number_of_chains : number_of_chains,
            sensibility : sensibility,
            values : Makrov::initValues(number_of_chains),
            actual_node : 0 /*TODO: Discuss if it's a construtor parameter or not.*/
        }
    }

    fn initValues(number_of_chains: u32) -> Vec<Vec<u32>> {
        return vec![vec![std::u32::MAX / number_of_chains; number_of_chains as usize]; number_of_chains as usize]
    }

    fn printValues(&self) {
        println!("{:?}", self.values);
    }

    fn isValid(&self) -> bool {
        /*TODO: Gérer le fait que avec certains nombres la répartition merde*/
        //println!("{:?}", std::u32::MAX);
        //println!("{:?}", self.values[1].iter().fold(0, |sum, val| sum+val));
        return self.values[1].iter().fold(0, |sum, val| sum+val) == std::u32::MAX
    }
    /*fn fillValues(self) {
        self.values = vec![vec![200, 100, 500], vec![200, 200, 600], vec![100, 100, 800]];
    }*/
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
}
#[test]
fn it_works() {
    let mut mTest = Makrov::new(4, 10);
    /*mTest.printValues();
    let arr = vec![1, 1, 1, 1, 1];
    let sum = arr.iter().fold(0, |sum, val| sum + val);
    //sum +=
    println!("{:?}", sum);
    //println!("{:?}", hello);*/
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
    println!("{:?}", mTest.get_next_node());
}
