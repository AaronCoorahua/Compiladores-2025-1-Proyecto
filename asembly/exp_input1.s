.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
a: .quad 0
r: .double 0.0
LC0: .double 2
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $2, %rax
movq %rax, a(%rip)
movq $-365796704, %rax
cvtsi2sd %rax, %xmm0
movsd LC0(%rip), %xmm0
movsd %xmm0, %xmm1
divsd %xmm1, %xmm0
movsd %xmm0, r(%rip)
movsd r(%rip), %xmm0
leaq print_float_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
