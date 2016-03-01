// Pour la date, on pourrait utiliser la lib time, qui a l'air assez sympa.
// https://doc.rust-lang.org/time/time/index.html
extern crate time;
//hour, normalhour et duration en minutes, et les heures : nbre de minutes depuis minuit.
fn gauss(hour : f64, p : f64, normal_hour : f64, duration : f32) -> f64{
    //tada
    //A tweak, marche chelou pour les contenus courts
    let a = p;
    let top_exp = -(normal_hour-hour).powi(2);
    let bot_exp = 2f64 * (duration as f64/4f64).powi(2);
    let egblog = a*(top_exp/bot_exp).exp();
    return egblog;
}



fn get_actual_time() {
    println!("{:?}", time::get_time());
}
#[test]
fn it_works() {
}


fn main() {

    //println!("{:?}", tm);
    println!("Hello, world!");
    let initial_time = time::strptime("0-07-00-00", "%w-%H-%M-%S").unwrap();
    let initial_time2  = time::strptime("0-08-00-00", "%w-%H-%M-%S").unwrap();
    let duration = time::Duration::minutes(90);
    //println!("{:?}", initial_time2-initial_time);
    //println!("{:?}", initial_time2);
    println!("{}", gauss(7.0, 0.6, 8.0, 1.5));
    println!("{}", gaussTm(initial_time, 0.6, initial_time2, duration));
}
