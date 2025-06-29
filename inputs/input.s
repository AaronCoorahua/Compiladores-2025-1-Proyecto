.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
a: .double 0.0
b: .double 0.0
c: .double 0.0
LC0: .double 6.3
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movsd LC0(%rip), %xmm0
movsd %xmm0, a(%rip)
movq $3, %rax
cvtsi2sd %rax, %xmm0
movsd %xmm0, b(%rip)
movsd a(%rip), %xmm0
movsd b(%rip), %xmm1
divsd %xmm1, %xmm0
movsd %xmm0, c(%rip)
movsd c(%rip), %xmm0
leaq print_float_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
