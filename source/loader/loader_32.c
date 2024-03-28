#include "loader.h"

/**
 * 读取硬盘数据, 从指定的扇区读取指定数量的扇区数据, 并将数据写入到缓存中.
 * @param sector 起始扇区号
 * @param sector_count 扇区数
 * @param buf 缓存
 * 该方法实现使用LAB模式读取硬盘数据, 通过0x1F0~0x1F7端口进行数据的读取.
 * 1. 通过0x1F6端口设置硬盘的工作模式, 0xE0表示使用LAB模式
 * 2. 通过0x1F2~0x1F5端口设置LBA参数, 0x1F2端口设置扇区数, 0x1F3~0x1F5端口设置LBA参数
 *    LBA参数的0~7位存放的是扇区号, 8~15位存放的是柱面号, 16~23位存放的是磁头号, 24~27位存放的是扇区数
 * 3. 通过0x1F7端口发送读取命令
*/
static void read_disk(int sector, int sector_count, uint8_t * buf) {
    outb(0x1F6, (uint8_t) (0xE0));

	outb(0x1F2, (uint8_t) (sector_count >> 8));
    outb(0x1F3, (uint8_t) (sector >> 24));		// LBA参数的24~31位
    outb(0x1F4, (uint8_t) (0));					// LBA参数的32~39位
    outb(0x1F5, (uint8_t) (0));					// LBA参数的40~47位

    outb(0x1F2, (uint8_t) (sector_count));
	outb(0x1F3, (uint8_t) (sector));			// LBA参数的0~7位
	outb(0x1F4, (uint8_t) (sector >> 8));		// LBA参数的8~15位
	outb(0x1F5, (uint8_t) (sector >> 16));		// LBA参数的16~23位

	outb(0x1F7, (uint8_t) 0x24);

	// 读取数据
	uint16_t *data_buf = (uint16_t*) buf;
	while (sector_count-- > 0) {
		// 每次扇区读之前都要检查，等待数据就绪
		while ((inb(0x1F7) & 0x88) != 0x8) {}

		// 读取并将数据写入到缓存中
		for (int i = 0; i < SECTOR_SIZE / 2; i++) {
			*data_buf++ = inw(0x1F0);
		}
	}
}


void load_kernel (void) {
    // 读取内核镜像
    read_disk(100, 500, (uint8_t *)SYS_KERNEL_LOAD_ADDR);

    
    
}