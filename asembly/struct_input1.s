.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
d.r1: .double 0.0
d.x1: .double 0.0
d.y1: .double 0.0
e.r2: .quad   0
e.x2: .quad   0
e.y2: .quad   0
LC0: .double 10.2
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movsd LC0(%rip), %xmm0
movsd %xmm0, d.x1(%rip)
movq $13, %rax
cvtsi2sd %rax, %xmm0
movq  %rax,  e.x2(%rip)
movsd d.x1(%rip), %xmm0
movq e.x2(%rip), %rax
cvtsi2sd %rax, %xmm1
addsd %xmm1, %xmm0
movsd %xmm0, d.y1(%rip)
movsd d.y1(%rip), %xmm0
leaq print_float_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
