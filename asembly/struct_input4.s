.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
p.x: .double 0.0
p.y: .quad   0
LC0: .double 3.14
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movsd LC0(%rip), %xmm0
movsd %xmm0, p.x(%rip)
movq $5, %rax
cvtsi2sd %rax, %xmm0
movq  %rax,  p.y(%rip)
movq $0, %rax
popq %rbp
ret
