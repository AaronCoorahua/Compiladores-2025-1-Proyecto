.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
a: .quad 0
b: .quad 0
resultado: .quad 0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $7, %rax
movq %rax, a(%rip)
movq $5, %rax
movq %rax, b(%rip)
movq %rax, resultado(%rip)
movq  resultado(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
