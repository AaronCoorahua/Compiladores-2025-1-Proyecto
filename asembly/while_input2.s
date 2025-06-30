.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
i: .quad 0
total: .quad 0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $1, %rax
movq %rax, i(%rip)
movq $0, %rax
movq %rax, total(%rip)
movq  total(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
