/**
 * halt
 * nop
 * _______
 * rrmovq |
 * irmovq |
 * rmmovq |----->mov
 * mrmovq |
 * _______|
 * _____
 * addq |
 * subq |
 * andq |----->OP
 * xorq |
 * _____|
 * ____
 * jmp |
 * jle |
 * jl  |
 * je  |
 * jne |------>jump
 * jge |
 * jg  |
 * ____|
 * _______
 * cmovle |
 * cmovl  |
 * cmove  |
 * cmovne |------->cmov
 * cmovge |
 * cmovg  |
 * _______|
 * 
 * call
 * ret
 * pushq
 * popq
 **/

