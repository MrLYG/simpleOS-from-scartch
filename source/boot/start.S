/**
 * 自己动手写操作系统
 *
 * 系统引导部分，启动时由硬件加载运行，然后完成对二级引导程序loader的加载
 * 该部分程序存储于磁盘的第1个扇区，在计算机启动时将会由BIOS加载到0x7c00处
 * 之后，将由BIOS跳转至0x7c00处开始运行
 * 
 * 作者：李述铜
 * 联系邮箱: 527676163@qq.com
 */
	#include "boot.h"

  	// 16位代码，务必加上
  	.code16
 	.text
	.global _start
	.extern boot_entry
_start:
	mov $0, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	mov %ax, %fs
	mov %ax, %gs

	mov $_start, %esp // 设置栈指针

read_loader: // 读取loader到内存
	mov $0x8000, %bx // 0x8000是loader的加载地址
	mov $0x2, %ah // 功能号：读磁盘
	mov $0x2, %cx // 读取2个扇区
	mov $64, %al // 读取1个扇区
	mov $0x0080, %dx // 0x0080是loader所在的磁头号和扇区号
	int $0x13 // 调用BIOS中断13h
	jc read_loader // 如果失败，重试
	jmp boot_entry // 跳转至loader
	
	// 引导结束段
	.section boot_end, "ax"
boot_sig: .byte 0x55, 0xaa
