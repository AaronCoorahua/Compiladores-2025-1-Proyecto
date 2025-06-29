.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
a: .float 0.0
b: .float 0.0
c: .float 0.0
LC0: .float 1.5
LC1: .float 2.25
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movss LC0(%rip), %xmm0
movss %xmm0, a(%rip)
movss LC1(%rip), %xmm0
movss %xmm0, b(%rip)
movss a(%rip), %xmm0
movss %xmm0, %xmm1
movss b(%rip), %xmm0
addss %xmm1, %xmm0
movss %xmm0, c(%rip)
movss c(%rip), %xmm0
cvtss2sd %xmm0, %xmm0
leaq print_float_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
