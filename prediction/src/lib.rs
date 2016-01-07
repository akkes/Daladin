///Represents a Makrov chain.
struct Makrov {
    number_of_chains: u32,
    sensibility: f32,
    //values: Vec<Vec<f32>>
}
impl Makrov {
    ///Creates a new Makrov chain.
    /// # Arguments
    ///
    /// * `number_of_chains` - Number of chains the Makrov chain will have. Each chain can be a podcast ID.
    /// * `sensibility` - This will afflict how much the probability values will evolve depending on the user feedback.
    fn new(number_of_chains: u32, sensibility: f32) -> Makrov {
        Makrov {
            number_of_chains : number_of_chains,
            sensibility : sensibility,
            /*for i in 0..number_of_chains-1 {

            }*/
        }
    }
}
#[test]
fn it_works() {
    let hello = "Hello!";
    println!("{:?}", hello);
}
