#ifndef CPU_INSTR_H
#define CPU_INSTR_H

#include "types.h"

// 内联函数是什么? 有什么好处?
// 内联函数是一种特殊的函数，它在被调用的地方展开，而不是通过函数调用的方式进行调用。
// 内联函数的好处是可以减少函数调用的开销，提高程序的执行效率。
static inline void cli(void) {
    // cli 指令用于关闭中断
    __asm__ __volatile__("cli");
}

static inline void sti(void) {
    // sti 指令用于开启中断
    __asm__ __volatile__("sti");
}

static inline uint8_t inb(uint16_t port) {
    uint8_t data;
    // inb 指令用于从指定端口读取一个字节的数据
    // [p] 表示端口号，[v] 表示数据
    __asm__ __volatile__("inb %[p], %[v]" : [v]"=a"(data) : [p]"d"(port));
    return data;
}

static inline void outb(uint16_t port, uint8_t data) {
    // outb 指令用于向指定端口写入一个字节的数据
    // [p] 表示端口号，[v] 表示数据
    __asm__ __volatile__("outb %[v], %[p]" : : [v]"a"(data), [p]"d"(port));
}

static inline void lgdt(uint32_t start, uint16_t size) {
    
    // gdtr 结构体用于存储全局描述符表的起始地址和大小
    // gdtr.limit 用于存储全局描述符表的大小
    // gdtr.start15_0 用于存储全局描述符表的起始地址的低 16 位
    // gdtr.start31_16 用于存储全局描述符表的起始地址的高 16 位
    
    struct {
        uint16_t limit;
        uint16_t start15_0;
        uint16_t start31_16;
    }gdtr;

    gdtr.start31_16 = start >> 16;
    gdtr.start15_0 = start & 0xffff;
    gdtr.limit = size - 1;

    // lgdt 指令用于加载全局描述符表
    // [g] 表示 gdtr 结构体
    // [g]"m"(gdtr) 表示将 gdtr 结构体的地址传递给 lgdt 指令
    __asm__ __volatile__("lgdt %[g]" : : [g]"m"(gdtr));
}


static inline uint32_t read_cr0 (void) {
    uint32_t cr0;
    // cr0 寄存器用于存储控制寄存器 0 的值
    // mov 指令用于将 cr0 寄存器的值传递给 data 变量
    __asm__ __volatile__("mov %%cr0, %[v]" : [v]"=r"(cr0));
    return cr0;
}

static inline void write_cr0 (uint32_t data) {
    // cr0 寄存器用于存储控制寄存器 0 的值
    // mov 指令用于将 data 变量的值传递给 cr0 寄存器
    __asm__ __volatile__("mov %[v], %%cr0" : : [v]"r"(data));
}

static inline void far_jump(uint32_t selector, uint32_t offset) {
    uint32_t addr[] = {offset, selector};
    // far 指令用于远跳转
    __asm__ __volatile__("ljmpl *(%[a])"::[a]"r"(addr));
}

static inline uint16_t inw(uint16_t  port) {
	uint16_t rv;
	__asm__ __volatile__("in %1, %0" : "=a" (rv) : "dN" (port));
	return rv;
}


# endif