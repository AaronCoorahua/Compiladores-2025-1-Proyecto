.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
i: .quad 0
resultfor: .quad 0
countback: .quad 0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $0, %rax
movq %rax, resultfor(%rip)
movq  resultfor(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
