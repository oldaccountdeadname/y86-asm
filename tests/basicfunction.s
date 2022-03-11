jmp main

add:
	addq %rdi, %rsi
	rrmovq %rsi, %rax
	ret

main:
	irmovq 2, %rsi
	irmovq 2, %rdi
	call add
