.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
i: .quad 0
r: .double 0.0
LC0: .double 3.14
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movq $4, %rax
movq %rax, i(%rip)
movsd LC0(%rip), %xmm0
movsd %xmm0, r(%rip)
movq i(%rip), %rax
cvtsi2sd %rax, %xmm0
movsd r(%rip), %xmm1
addsd %xmm1, %xmm0
leaq print_float_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
movq $0, %rax
popq %rbp
ret
