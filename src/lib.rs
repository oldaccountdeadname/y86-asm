static USAGE: &'static str = "\
    y86-asm [--help] | [-o|--output] <filename> [input] [input] [input]...

y86-asm assembles and links multiple y86 assembly plain text files. Pass
filenames as arguments to the CLI. If no output filename is specified with the
--output or -o flag, y86asm writes to a.out.
";

pub mod driver;

mod assembler;
mod util;

use driver::RunConf;

#[derive(Debug)]
pub enum Args {
    Assemble(RunConf),
    Help(i32),
}

impl Args {
    pub fn parse<I>(args: I) -> Self
    where
        I: Iterator<Item = String>,
    {
        let mut arg_is_output = false;
        let mut output = None;
        let mut inputs = Vec::new();
        for arg in args {
            match arg.as_str() {
                "--help" | "-h" => return Self::Help(0),
                "--output" | "-o" => {
                    arg_is_output = true;
                }
                _ if arg_is_output => {
                    output = Some(arg);
                    arg_is_output = false;
                }
                _ => {
                    inputs.push(arg);
                }
            }
        }

        if inputs.is_empty() {
            Self::Help(1)
        } else {
            Self::Assemble(RunConf::new(
                inputs,
                output.unwrap_or(String::from("a.out")),
            ))
        }
    }

    pub fn exec(self) -> i32 {
        match self {
            Self::Help(i) => {
                eprintln!("{}", USAGE);
                i
            }
            Self::Assemble(ac) => ac.make(),
        }
    }
}
