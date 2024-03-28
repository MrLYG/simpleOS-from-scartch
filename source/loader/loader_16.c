__asm__(".code16gcc");

#include "loader.h"

boot_info_t boot_info;

// 一个展示使用itoa函数和show_msg打印内存区域信息的例子
// Function prototype
static void print_entry_info(SMAP_entry_t* entry);
static void show_msg(const char *msg) {
    char c;

    //  内联汇编 0xe功能号是设置光标 0x10是BIOS的中断号
    while ((c = *msg++) != '\0') {
        __asm__ __volatile__(
            "mov $0xe, %%ah\n\t" // 设置光标
            "mov %[ch], %%al\n\t" // 设置字符
            "int $0x10"::[ch]"r"(c)
        );
    }
    
    
}

//https://wiki.osdev.org/Memory_Map_(x86)
static void detect_memory(void) {
    uint32_t contID = 0;
	int signature, bytes;
    SMAP_entry_t smap_entry;
    show_msg("Detecting memory...\r\n");
    
    boot_info.ram_region_count = 0;
    for (int i = 0; i < BOOT_RAM_REGION_MAX; i++) {
        SMAP_entry_t * entry = &smap_entry;
        __asm__ __volatile__("int  $0x15"
			: "=a"(signature), "=c"(bytes), "=b"(contID)
			: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(entry));
		if (signature != 0x534D4150) {
            show_msg("failed.\r\n");
			return;
		}

		if (bytes > 20 && (entry->ACPI & 0x0001) == 0){
			continue;
		}

        if (entry->Type == 1) {
            boot_info.ram_region_cfg[boot_info.ram_region_count].start = entry->BaseL;
            boot_info.ram_region_cfg[boot_info.ram_region_count].size = entry->LengthL;
            boot_info.ram_region_count++;
            print_entry_info(entry);
        }

		if (contID == 0) {
			break;
		}
        
    }
    show_msg("Detect memory done.\r\n");
}
static void itoa(uint32_t value, char* str, uint32_t base) {
    char* hex = "0123456789ABCDEF";
    char* ptr = str;
    uint32_t num = value;

    // 处理0的特殊情况
    if(num == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    // 处理数字转换
    while(num > 0) {
        uint32_t remainder = num % base;
        *ptr++ = hex[remainder];
        num /= base;
    }
    *ptr = '\0'; // 添加字符串终止符

    // 反转字符串
    char* start = str;
    char* end = ptr - 1;
    while(start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}



static void print_entry_info(SMAP_entry_t* entry) {
    char buffer[64];

    // 打印基地址
    show_msg("Base: 0x");
    itoa(entry->BaseL, buffer, 16);
    show_msg(buffer);

    // 打印内存区域长度
    show_msg(", Length: 0x");
    itoa(entry->LengthL, buffer, 16);
    show_msg(buffer);

    // 打印类型
    show_msg(", Type: 0x");
    itoa(entry->Type, buffer, 16);
    show_msg(buffer);

    // 打印换行
    show_msg("\r\n");
}


/**
 * Global Descriptor Table
 * 0-4GB内存分段
*/
uint16_t gdt_table[][4] = {
    {0, 0, 0, 0}, // 0x00
    {0xFFFF, 0x0000,0x9a00,0x00cf}, // 0x08
    {0xFFFF, 0x0000,0x9200,0x00cf}, // 0x10
};



/*
    * 进入保护模式
    * 1. 关中断
    * 2. 打开A20地址线
    * 3.. 设置GDT
    * 4. 设置CR0的PE位
    * 5. 远跳转到保护模式
*/
static void enter_protect_mode (void) {
    cli();

    // 打开A20地址线
    uint8_t v = inb(0x92);
    outb(0x92, v | 0x2);

    /**
     * 设置GDT
     * GDT中的每一项称为描述符（Descriptor），包含了如下信息：
        基地址（Base Address）：被描述的内存区域的起始地址。
        段限（Segment Limit）：内存区域的大小。
        访问权限（Access Rights）：定义了哪些程序或任务可以访问该内存区域，以及它们的访问方式（如可读、可写、执行等）。
    */
    lgdt((uint32_t)gdt_table, sizeof(gdt_table));

    // 设置CR0的PE位
    uint16_t cr0 = read_cr0();
    cr0 |= 1 << 0; // 设置CR0的PE位为1
    write_cr0(cr0);

    // 远跳转到保护模式
    // 0x08是GDT中的第二个描述符，表示代码段
    far_jump(8, (uint32_t)protect_mode_entry);
}

void loader_entry(void) {
    show_msg("Hello, Yuangang Li!\r\n");
    detect_memory();
    enter_protect_mode();
    for(;;) {}
}