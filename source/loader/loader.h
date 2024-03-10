#ifndef LOADER_H
#define LOADER_H

#include "comm/types.h"
#include "comm/boot_info.h"

typedef struct SMAP_entry {
    uint32_t BaseL;
    uint32_t BaseH;
    uint32_t LengthL;
    uint32_t LengthH;
    uint32_t Type;
    uint32_t ACPI; // 0 is usable, 1 is reserved
}__attribute__((packed)) SMAP_entry_t;

#endif
