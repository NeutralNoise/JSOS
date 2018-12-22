
.set MAGIC, 0x1badb002
.set FLAGS, (1 << 0 | 1 << 1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

.section .text
.extern kernelMain
.extern callConstructors
.global loader

loader:
#set up the kenel stack
  mov $kernel_stack,%esp
  #call the global var constructors
  call callConstructors
  #push the multiboot_struct and magic number to our kernelMain function
  push %eax
  push %ebx
  #call the kernel
  call kernelMain

_stop:
    cli
    hlt
    jmp _stop

.section .bss
.space 2*1024*1024; # 2MB statck pointer
kernel_stack:
