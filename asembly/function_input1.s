.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
x: .quad 0
y: .quad 0
r: .quad 0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $10, %rax
movq %rax, x(%rip)
movq $20, %rax
movq %rax, y(%rip)
movq %rax, r(%rip)
movq  r(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
