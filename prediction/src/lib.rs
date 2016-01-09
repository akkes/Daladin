///Represents a Makrov chain.
struct Makrov {
    number_of_chains: u32,
    sensibility: u32,
    values: Vec<Vec<u32>>,
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
        }
    }

    fn initValues(number_of_chains: u32) -> Vec<Vec<u32>> {
        return vec![vec![std::u32::MAX / number_of_chains; number_of_chains as usize]; number_of_chains as usize]
    }

    fn printValues(&self) {
        println!("{:?}", self.values);
    }

    fn isValid(&self) -> bool {
        return true
    }
    /*fn fillValues(self) {
        self.values = vec![vec![200, 100, 500], vec![200, 200, 600], vec![100, 100, 800]];
    }*/
}
#[test]
fn it_works() {
    let mTest = Makrov::new(4, 10);
    mTest.printValues();
    let sum = (1..5).fold(0, |sum, x| sum + x);
    //sum +=
    println!("{:?}", sum);
    //println!("{:?}", hello);
}
