.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
p.x: .double 0.0
p.y: .double 0.0
LC0: .double 1.5
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $4, %rax
cvtsi2sd %rax, %xmm0
movsd %xmm0, p.x(%rip)
movsd LC0(%rip), %xmm0
movsd %xmm0, p.y(%rip)
movsd p.y(%rip), %xmm0
movsd p.x(%rip), %xmm1
subsd %xmm1, %xmm0
leaq print_float_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
