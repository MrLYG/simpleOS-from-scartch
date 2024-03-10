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



// Move the function declaration outside of the detect_memory function
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



void loader_entry(void) {
    show_msg("Hello, Yuangang Li!\r\n");
    detect_memory();
    for(;;) {}
}