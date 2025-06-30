.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
a: .quad 0
b: .quad 0
c: .quad 0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $10, %rax
movq %rax, a(%rip)
movq $5, %rax
movq %rax, b(%rip)
movq  a(%rip), %rax
pushq %rax
movq  b(%rip), %rax
pushq %rax
movq $2, %rax
cvtsi2sd %rax, %xmm0
movq %rax, %rbx
popq %rax
imulq %rbx, %rax
movq %rax, %rbx
popq %rax
addq %rbx, %rax
movq %rax, c(%rip)
movq $0, %rax
popq %rbp
ret
