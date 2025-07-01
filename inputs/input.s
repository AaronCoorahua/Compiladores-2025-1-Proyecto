.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
x: .quad 0
y: .quad 0
.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
movq $50, %rax
movq %rax, x(%rip)
movq $10, %rax
movq %rax, y(%rip)
movq  x(%rip), %rax
pushq %rax
movq  y(%rip), %rax
movq %rax, %rbx
popq %rax
cmpq %rbx, %rax
setg %al
movzbq %al, %rax
  cmpq $0, %rax
  je else_0
movq  x(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
  jmp endif_0
else_0:
movq  y(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
endif_0:
  movq $0, %rax
  popq %rbp
  ret
