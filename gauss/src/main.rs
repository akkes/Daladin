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

#[test]
fn it_works() {
}


fn main() {

    //println!("{:?}", tm);
    println!("Hello, world!");
    println!("{}", gauss(7.85, 0.6, 8.0, 1.5));
}
