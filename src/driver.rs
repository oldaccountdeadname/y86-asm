use crate::assembler;

#[derive(Debug)]
pub struct RunConf {
    inputs: Vec<String>,
    output: String,
}

impl RunConf {
    pub fn new(inputs: Vec<String>, output: String) -> Self {
        Self { inputs, output }
    }

    pub fn make(self) -> i32 {
        let _asm = match self.assemble() {
            Ok(k) => k,
            Err(errors) => {
                for e in &errors {
                    eprintln!("Error occured while assembling: {}", e);
                }

                eprintln!("Aborted assembly due to {} errors.", errors.len());
                return 1;
            }
        };

        0
    }

    fn assemble(&self) -> assembler::Result<Vec<assembler::AsmUnit>> {
        let mut units = Vec::with_capacity(self.inputs.len());

        for input in &self.inputs {
            units.push(assembler::AsmUnit::read_asm(input)?);
        }

        Ok(units)
    }
}
