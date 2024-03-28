
  	// 16位代码，务必加上
  	.code16
 	.text
	.global _start
	.extern boot_entry
_start:
	jmp loader_entry

	.code32
	.text
	.global protect_mode_entry
	.extern load_kernel
protect_mode_entry:
	// 重新加载所有的数据段描述符
	mov $16, %ax		// 16为数据段选择子
	mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
	jmp $8, $load_kernel
