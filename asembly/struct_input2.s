.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
d.r: .quad   0
d.x: .quad   0
d.y: .quad   0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $10, %rax
cvtsi2sd %rax, %xmm0
movq  %rax,  d.x(%rip)
movq $20, %rax
cvtsi2sd %rax, %xmm0
movq  %rax,  d.y(%rip)
movq  %rax,  d.r(%rip)
movq  d.r(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
