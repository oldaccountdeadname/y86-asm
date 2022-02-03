use std::fmt;
use std::fmt::Display;

#[derive(Debug)]
pub struct MaybeLined<T> {
    line: Option<usize>,
    inner: T,
}

impl<T> MaybeLined<T> {
    pub fn new(line: usize, inner: T) -> Self {
        Self {
            line: Some(line),
            inner
        }
    }

    pub fn no_line(inner: T) -> Self {
        Self {
            line: None,
            inner,
        }
    }
}

impl<T: Display> Display for MaybeLined<T> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self.line {
            Some(n) => write!(f, "[line {}]: {}", n, self.inner),
            None => write!(f, "{}", self.inner),
        }
    }
}
