.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
p1.x: .quad   0
p1.y: .quad   0
p2.x: .quad   0
p2.y: .quad   0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $1, %rax
cvtsi2sd %rax, %xmm0
movq  %rax,  p1.x(%rip)
movq  p1.x(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
