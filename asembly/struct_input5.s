.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
p.x: .quad   0
p.y: .quad   0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $3, %rax
cvtsi2sd %rax, %xmm0
movq  %rax,  p.x(%rip)
movq $2, %rax
cvtsi2sd %rax, %xmm0
movq  %rax,  p.y(%rip)
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
