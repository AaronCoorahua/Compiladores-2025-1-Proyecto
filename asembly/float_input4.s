.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
