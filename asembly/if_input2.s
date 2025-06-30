.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
x: .quad 0
y: .quad 0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $8, %rax
movq %rax, x(%rip)
movq $3, %rax
movq %rax, y(%rip)
movq  x(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq  y(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
