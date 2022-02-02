use crate::assembler::AsmUnit;

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
            Err(_) => {
                return 1;
            }
        };

        0
    }

    fn assemble(&self) -> Result<Vec<AsmUnit>, std::io::Error> {
        let mut units = Vec::with_capacity(self.inputs.len());

        for input in &self.inputs {
            units.push(AsmUnit::read_asm(input)?);
        }

        Ok(units)
    }
}
