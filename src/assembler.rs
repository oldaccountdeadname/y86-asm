/*
DONE
 halt
 nop
 rrmovq
 irmovq
 rmmovq
TODO
 mrmovq
 OPq
 jXX
 cmovXX
 call
 ret
 pushq
 popq

 also comments and error handling
*/
use std::collections::HashMap;
use std::fs;
use std::io::{self, BufRead};
use std::str::FromStr;

use crate::util::MaybeLined;

pub type Result<T> = std::result::Result<T, MaybeLined<Error>>;

/// This is analogous to a single assembly file. It stores a symbol table of
/// labels and their positions within self, and an in-memory byte array of the
/// assembly.
pub struct AsmUnit {
    pub symbol_table: HashMap<String, u64>,
    pub asm: Vec<u8>,
}

#[derive(Debug)]
pub enum Error {
    BadRegister(String),
    BadInstruction(String),
    BadInt(String, u8),
    IncompleteStatement,
    IoFailed(io::Error),
}

impl AsmUnit {
    pub fn read_asm(path: &'_ str) -> Result<Self> {
        let file = fs::File::open(path)
            .map_err(|x| MaybeLined::no_line(Error::IoFailed(x)))?;

        let file = io::BufReader::new(file);

        let mut builder = Builder::empty();

        for (line_no, line) in file.lines().enumerate() {
            let line = match line {
                Ok(l) => l,
                Err(x) => {
                    return Err(MaybeLined::no_line(Error::IoFailed(x)));
                },
            };

            for word in line.split_whitespace() {
                // we want to allow separating instructions with semicolons
                for token in word.split(';') {
                    if let Err(e) = builder.instruct(token) {
                        return Err(MaybeLined::new(line_no + 1, e));
                    }
                }
            }

            if builder.state() != &State::Ready {
                return Err(MaybeLined::new(
                    line_no + 1, Error::IncompleteStatement
                ));
            }
        }

        Ok(builder.unit())
    }
}

enum Instruction {
    Halt,
    Nop,
    RrMovq(Register, Register),
    IrMovq(u64, Register),
    Ret,
}

#[repr(u8)]
#[derive(Copy, Clone, PartialEq)]
enum Register {
    Rax = 0,
    Rcx,
    Rdx,
    Rbx,
    Rsp,
    Rbp,
    Rsi,
    Rdi,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,

    /// NoReg indicates 0x0f, and is not an indication that a register is
    /// unspecified.
    NoReg,
}

/// The builder is a finite state automata that may be fed input one logical
/// word (whitespace delimited text) at a time.
struct Builder {
    inner: AsmUnit,
    state: State,
}

/// An enumeration of all states the parser may be in.
#[derive(PartialEq)]
enum State {
    Ready,
    RrMovq(Option<Register>, Option<Register>),
    IrMovq(Option<u64>, Option<Register>),
}

impl Builder {
    pub fn empty() -> Self {
        Self {
            state: State::Ready,
            inner: AsmUnit {
                symbol_table: HashMap::new(),
                asm: Vec::new(),
            },
        }
    }

    pub fn instruct(&mut self, word: &'_ str) -> std::result::Result<(), Error> {
        if let Some(ins) = self.state.next(word)? {
            let bytes: Vec<u8> = ins.into();
            self.inner.asm.extend(bytes);
        }

        Ok(())
    }

    pub fn unit(self) -> AsmUnit {
        self.inner
    }

    pub fn state(&'_ self) -> &'_ State {
        &self.state
    }
}

impl State {
    pub fn next(
        &mut self, word: &'_ str
    ) -> std::result::Result<Option<Instruction>, Error> {
        match self {
            Self::Ready => match word {
                "halt" => Ok(Some(Instruction::Halt)),
                "nop" => Ok(Some(Instruction::Nop)),
                "ret" => Ok(Some(Instruction::Ret)),
                "rrmovq" => {
                    *self = Self::RrMovq(None, None);
                    Ok(None)
                }
                "irmovq" => {
                    *self = Self::IrMovq(None, None);
                    Ok(None)
                }
                w => Err(Error::BadInstruction(w.to_string())),
            },
            Self::RrMovq(None, None) => {
                let without_comma = &word[..(word.len() - 1)];
                let register = Register::from_symbolic(without_comma)?;

                *self = Self::RrMovq(Some(register), None);

                Ok(None)
            }
            Self::RrMovq(Some(x), None) => {
                let register = Register::from_symbolic(word)?;
                let ins = Instruction::RrMovq(*x, register);
                *self = Self::Ready;
                Ok(Some(ins))
            }
            Self::IrMovq(None, None) => {
                let without_comma = &word[..(word.len() - 1)];
                *self = Self::IrMovq(
                    Some(
                        u64::from_str(without_comma)
                            .map_err(|_| Error::BadInt(word.to_string(), 10))?,
                    ),
                    None,
                );

                Ok(None)
            }
            Self::IrMovq(Some(i), None) => {
                let register = Register::from_symbolic(word)?;
                let ins = Instruction::IrMovq(*i, register);
                *self = Self::Ready;
                Ok(Some(ins))
            }

            _ => unreachable!(),
        }
    }
}

impl Into<Vec<u8>> for Instruction {
    fn into(self) -> Vec<u8> {
        let mut ins = match self {
            Self::Halt => vec![0x00],
            Self::Nop => vec![0x10],
            Self::Ret => vec![0x90],
            Self::RrMovq(from, to) => {
                let mut base = vec![0x20, 0x00];
                base.push((from as u8) << 4 | to as u8);
                base
            }
            Self::IrMovq(i, r) => {
                let mut base = vec![0x30];
                base.push((r as u8) | 0xf0);
                base.extend(i.to_le_bytes());
                base
            }
        };

        // 16 byte offsets
        ins.resize(16, 0);
        ins
    }
}

impl Register {
    pub fn from_symbolic(sym: &'_ str) -> std::result::Result<Self, Error> {
        match sym {
            "%rax" => Ok(Self::Rax),
            "%rcx" => Ok(Self::Rcx),
            "%rdx" => Ok(Self::Rdx),
            "%rbx" => Ok(Self::Rbx),
            "%rsp" => Ok(Self::Rsp),
            "%rbp" => Ok(Self::Rbp),
            "%rsi" => Ok(Self::Rsi),
            "%rdi" => Ok(Self::Rdi),
            "%r8" => Ok(Self::R8),
            "%r9" => Ok(Self::R9),
            "%r10" => Ok(Self::R10),
            "%r11" => Ok(Self::R11),
            "%r12" => Ok(Self::R12),
            "%r13" => Ok(Self::R13),
            "%r14" => Ok(Self::R14),
            _ => Err(Error::BadRegister(sym.to_string())),
        }
    }
}

impl From<io::Error> for Error {
    fn from(err: io::Error) -> Self {
        Error::IoFailed(err)
    }
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "\x1b[0;31m{}\x1b[0;m",
            match self {
                Self::BadRegister(s) => format!("Expected a register, but got `{}`", s),
                Self::BadInstruction(s) => format!("Expected an instruction, but got `{}`.", s),
                Self::BadInt(got, base) =>
                    format!("Expected integer with base {}, but got `{}`.", base, got),
                Self::IncompleteStatement => "Statement is incomplete.".to_string(),

                Self::IoFailed(e) => format!("Couldn't perform necessary IO ({})", e),
            }
        )
    }
}

#[cfg(test)]
mod tests {
    use std::fs::remove_file;
    use std::fs::File;
    use std::io::Write;

    use super::AsmUnit;

    #[test]
    fn assemble_seq_no_args() {
        const PATH: &'static str = "/tmp/y86-asm_assemble_with_no_args";
        let data = "nop\nnop;nop\nret\nhalt\n";

        File::create(PATH)
            .unwrap()
            .write_all(&data.bytes().collect::<Vec<u8>>())
            .ok();

        assert_eq!(
            AsmUnit::read_asm(PATH).unwrap().asm,
            &[
                0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ],
        );

        remove_file(PATH).unwrap();
    }

    #[test]
    fn assemble_rrmovq() {
        const PATH: &'static str = "/tmp/y86-asm_assemble_rrmovq";
        let data = "rrmovq %r10, %r11";

        File::create(PATH)
            .unwrap()
            .write_all(&data.bytes().collect::<Vec<u8>>())
            .ok();

        assert_eq!(
            AsmUnit::read_asm(PATH).unwrap().asm,
            &[
                0x20, 0x00, 0xab, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00
            ],
        );

        remove_file(PATH).unwrap();
    }

    #[test]
    fn assemble_irmovq_base10() {
        const PATH: &'static str = "/tmp/y86-asm_assemble_irmovq";
        let data = "irmovq 1, %rax";

        File::create(PATH)
            .unwrap()
            .write_all(&data.bytes().collect::<Vec<u8>>())
            .ok();

        assert_eq!(
            AsmUnit::read_asm(PATH).unwrap().asm,
            &[
                0x30, 0xf0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00
            ],
        );

        remove_file(PATH).unwrap();
    }
}
