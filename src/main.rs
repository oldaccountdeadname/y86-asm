use std::env;
use std::process;

use y86_asm::Args;

fn main() {
    let args = Args::parse(env::args().skip(1));
    process::exit(args.exec())
}
