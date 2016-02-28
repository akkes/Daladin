extern crate rustbox;

mod lib;

use rustbox::{Color, RustBox};
use rustbox::Key;
use std::default::Default;
use std::error::Error;
use std::io;
fn main() {
    let mut feedback = String::new();
    let genres = vec!["Classique", "Electro", "Rock", "Jazz"];
    let mut past_genres : Vec<u32> = vec![0, 0, 0, 0];
    let mut test = lib::Makrov::new(4, 100000, 0);
    let mut actual_node = test.get_actual_node();
    // loop {
    //     test.printValues();
    //     println!("Tu écoutes du {}. C'est bien ?", genres[actual_node]);
    //     io::stdin().read_line(&mut feedback)
    //         .ok()
    //         .expect("Failed");
    //     if feedback == "oui\n" {
    //         test.apply_feedback(true, actual_node);
    //     } else {
    //         test.apply_feedback(false, actual_node);
    //     }
    //     feedback = String::new();
    //     actual_node = test.get_next_node();
    // }

    let rustbox = match RustBox::init(Default::default()) {
        Result::Ok(v) => v,
        Result::Err(e) => panic!("{}", e),
    };
    let width = rustbox.width();
    let height = rustbox.height();
    let header = "Daladin prediction | Q : Quitter | Y : J'aime | N : J'aime pas";
    let listening_str = "Genre en cours d'écoute :";
    let info_str = "Informations sur la chaîne : ";
    let past_str = "Genres passés : ";
    rustbox.print((width/2)-header.len()/2, 1, rustbox::RB_BOLD, Color::White, Color::Black, header);
    loop {
        rustbox.present();
        rustbox.clear();
        rustbox.print((width/2)-header.len()/2, 1, rustbox::RB_BOLD, Color::White, Color::Black, header);

        rustbox.print(1, (height/3)-1, rustbox::RB_BOLD, Color::White, Color::Black, listening_str);
        rustbox.print(1, height/3, rustbox::RB_BOLD, Color::White, Color::Black, genres[actual_node]);

        rustbox.print(1*(width/4), (height/3)-1, rustbox::RB_BOLD, Color::White, Color::Black, past_str);
        for i in 0..genres.len() {
            rustbox.print(1*(width/4), (height/3)+i, rustbox::RB_BOLD, Color::White, Color::Black, &*(past_genres[i].to_string() + " " + &genres[i].to_string()));
        }
        rustbox.print(1*(width/4), (height/3)+genres.len(), rustbox::RB_BOLD, Color::White, Color::Black, &*("Total :".to_string() + &past_genres.iter().fold(0, |sum, val| sum+val).to_string()));

        rustbox.print(1*(width/4), (height/3)+genres.len()+2, rustbox::RB_BOLD, Color::White, Color::Black, &*("Barre de progression à faire, pour mieux visualiser la répartition".to_string()));

        rustbox.print(3*(width/4), (height/3)-1, rustbox::RB_BOLD, Color::White, Color::Black, info_str);
        rustbox.print(3*(width/4), (height/3), rustbox::RB_BOLD, Color::White, Color::Black, &*("Sensibilité : ".to_string() + &test.get_sensibility().to_string()));
        rustbox.print(3*(width/4), (height/3)+1, rustbox::RB_BOLD, Color::White, Color::Black,  &*("Nombre de noeuds : ".to_string() + &test.get_number_of_chains().to_string()));
        rustbox.print(3*(width/4), (height/3)+1, rustbox::RB_BOLD, Color::White, Color::Black,  &*("Matrice d'adjacence : TODO ".to_string()));
        match rustbox.poll_event(false) {
            Ok(rustbox::Event::KeyEvent(key)) => {
                match key {
                    Key::Char('q') => { break; }
                    Key::Char('y') => { //rustbox.print(10, 10, rustbox::RB_BOLD, Color::White, Color::Black, "tu m");
                                        //test.apply_feedback(true, actual_node);
                                      }
                    Key::Char('n') => { //rustbox.print(10, 10, rustbox::RB_BOLD, Color::White, Color::Black, "batar");
                                        //test.apply_feedback(false, actual_node);
                                      }
                    _ => {}
                }
            },
            Err(e) => panic!("{}", e.description()),
            _ => { }
        };
        past_genres[actual_node] += 1;
        actual_node = test.get_next_node();
    }
}
