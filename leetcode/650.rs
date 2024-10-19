use std::collections::VecDeque;

pub fn min_steps(n: i32) -> i32 {
    let mut queue = VecDeque::new();
    queue.push_back((1, 0, 0));
    while let Some((a, b, c)) = queue.pop_front() {
        if a == n {
            return c;
        }
        if b != a {
            queue.push_back((a, a, c + 1));
        }
        if b != 0 && (a + b <= n) {
            queue.push_back((a + b, b, c + 1));
        }
    }
    unreachable!("Failure")
}

use std::env;
use std::process;

fn main() {
    let n = match env::args().nth(1) {
        None => 10,
        Some(arg) => match arg.parse() {
            Ok(x) if x > 0 => x,
            _ => {
                eprintln!("Optional input argument must be an integer greater than zero.");
                process::exit(1);
            }
        },
    };

    println!("{}", min_steps(n));
}
