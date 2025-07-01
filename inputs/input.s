.data
print_int_fmt: .string "%ld\n"
print_float_fmt: .string "%f\n"
i: .quad 0
result: .quad 0
.text
.globl main
.text
main:
  pushq %rbp
  movq %rsp, %rbp
movq $1, %rax
movq %rax, result(%rip)
movq $2, %rax
cvtsi2sd %rax, %xmm0
  movq %rax, i(%rip)
for_loop_0:
  movq i(%rip), %rax
movq $4, %rax
cvtsi2sd %rax, %xmm0
  cmpq %rax, i(%rip)
  jg for_end_0
movq  result(%rip), %rax
pushq %rax
movq  i(%rip), %rax
movq %rax, %rbx
popq %rax
imulq %rbx, %rax
movq %rax, result(%rip)
  addq $1, i(%rip)
  jmp for_loop_0
for_end_0:
movq  result(%rip), %rax
movq %rax, %rsi
leaq print_int_fmt(%rip), %rdi
movb $1, %al
call printf@PLT
  movq $0, %rax
  popq %rbp
  ret
