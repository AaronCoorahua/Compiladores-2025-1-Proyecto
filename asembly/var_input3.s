.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
a: .quad 0
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $1, %rax
cvtsi2sd %rax, %xmm0
pushq %rax
movq $3, %rax
cvtsi2sd %rax, %xmm0
movq %rax, %rbx
popq %rax
subq %rbx, %rax
movq %rax, a(%rip)
movq  a(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
