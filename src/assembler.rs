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

/// This is analogous to a single assembly file. It stores a symbol table of
/// labels and their positions within self, and an in-memory byte array of the
/// assembly.
pub struct AsmUnit {
    pub symbol_table: HashMap<String, u64>,
    pub asm: Vec<u8>,
}

impl AsmUnit {
    pub fn read_asm(path: &'_ str) -> Result<Self, io::Error> {
        let file = fs::File::open(path)?;
        let file = io::BufReader::new(file);

        let mut builder = Builder::empty();

        for line in file.lines() {
            for word in line?.split_whitespace() {
                // we want to allow separating instructions with semicolons
                for token in word.split(';') {
                    builder.instruct(token);
                }
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
#[derive(Copy, Clone)]
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

    pub fn instruct(&mut self, word: &'_ str) {
        if let Some(ins) = self.state.next(word) {
            let bytes: Vec<u8> = ins.into();
            self.inner.asm.extend(bytes);
        }
    }

    pub fn unit(self) -> AsmUnit {
        self.inner
    }
}

impl State {
    pub fn next(&mut self, word: &'_ str) -> Option<Instruction> {
        match self {
            Self::Ready => match word {
                "halt" => Some(Instruction::Halt),
                "nop" => Some(Instruction::Nop),
                "ret" => Some(Instruction::Ret),
                "rrmovq" => {
                    *self = Self::RrMovq(None, None);
                    None
                }
                "irmovq" => {
                    *self = Self::IrMovq(None, None);
                    None
                }
                _ => todo!(),
            },
            Self::RrMovq(None, None) => {
                let without_comma = &word[..(word.len() - 1)];
                *self = Self::RrMovq(Some(Register::from_symbolic(without_comma)), None);

                None
            }
            Self::RrMovq(Some(x), None) => {
                let ins = Instruction::RrMovq(*x, Register::from_symbolic(word));
                *self = Self::Ready;
                Some(ins)
            }
            Self::IrMovq(None, None) => {
                let without_comma = &word[..(word.len() - 1)];
                *self = Self::IrMovq(
                    Some(u64::from_str(without_comma).expect("base 10 number")),
                    None,
                );

                None
            }
            Self::IrMovq(Some(i), None) => {
                let ins = Instruction::IrMovq(*i, Register::from_symbolic(word));
                *self = Self::Ready;
                Some(ins)
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
    pub fn from_symbolic(sym: &'_ str) -> Self {
        match sym {
            "%rax" => Self::Rax,
            "%rcx" => Self::Rcx,
            "%rdx" => Self::Rdx,
            "%rbx" => Self::Rbx,
            "%rsp" => Self::Rsp,
            "%rbp" => Self::Rbp,
            "%rsi" => Self::Rsi,
            "%rdi" => Self::Rdi,
            "%r8" => Self::R8,
            "%r9" => Self::R9,
            "%r10" => Self::R10,
            "%r11" => Self::R11,
            "%r12" => Self::R12,
            "%r13" => Self::R13,
            "%r14" => Self::R14,
            _ => todo!("haha error handling what a funny joke haha"),
        }
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
