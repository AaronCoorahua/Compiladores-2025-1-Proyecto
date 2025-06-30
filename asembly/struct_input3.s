.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
d.x: .double 0.0
d.y: .quad   0
a: .double 0.0
r: .quad 0
LC0: .double 10.58
LC1: .double 4.2
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movsd LC0(%rip), %xmm0
movsd %xmm0, d.x(%rip)
movq $3, %rax
cvtsi2sd %rax, %xmm0
movq  %rax,  d.y(%rip)
movsd d.x(%rip), %xmm0
leaq print_float_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq  d.y(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movsd d.x(%rip), %xmm0
movsd LC1(%rip), %xmm0
movsd %xmm0, %xmm1
addsd %xmm1, %xmm0
movsd %xmm0, a(%rip)
movq %rax, r(%rip)
movsd a(%rip), %xmm0
leaq print_float_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq  r(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
