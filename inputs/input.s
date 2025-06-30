.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
x: .quad 0
y: .quad 0
.text
.globl main
main:
  pushq %rbp
  movq  %rsp, %rbp
movq $5, %rax
movq %rax, x(%rip)
movq $10, %rax
movq %rax, y(%rip)
  movq $0, %rax
  popq %rbp
  ret
