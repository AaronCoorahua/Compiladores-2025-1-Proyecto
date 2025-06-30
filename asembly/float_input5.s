.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
x: .double 0.0
LC0: .double 4.75
.text
.globl main
main:
pushq %rbp
movq %rsp, %rbp
movsd LC0(%rip), %xmm0
movsd %xmm0, x(%rip)
movq $0, %rax
popq %rbp
ret
