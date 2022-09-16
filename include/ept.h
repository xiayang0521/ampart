#ifndef HAVE_EPT_H
#define HAVE_EPT_H
#include "common.h"

/* Local */

#include "dts.h"

/* Definition */

#define EPT_PARTITION_GAP_GENERIC     0x800000U    // 8M
#define EPT_PARTITION_GAP_RESERVED    0x2000000U   // 32M

#define EPT_PARTITION_BOOTLOADER_SIZE 0x400000U    // 4M
#define EPT_PARTITION_BOOTLOADER_NAME "bootloader"
#define EPT_PARTITION_RESERVED_NAME   "reserved"
#define EPT_PARTITION_RESERVED_SIZE   0x4000000U   // 64M
#define EPT_PARTITION_CACHE_NAME      "cache"
#define EPT_PARTITION_CACHE_SIZE      0U
#define EPT_PARTITION_ENV_NAME        "env"
#define EPT_PARTITION_ENV_SIZE        0x800000U    // 8M

#define EPT_HEADER_MAGIC_UINT32       (uint32_t)0x0054504DU

/* Structure */

struct 
    ept_header {
        union { // 4
            char        magic_string[4];
            uint32_t    magic_uint32;
        };
        union { // 12
            char        version_string[12];
            uint32_t    version_uint32[3];
        };
        uint32_t        partitions_count;   // 4
        uint32_t        checksum;   // 4
    }; // 24

struct 
    ept_partition {
        char        name[MAX_PARTITION_NAME_LENGTH];
        uint64_t    size;
        uint64_t    offset;
        uint32_t    mask_flags;
        uint32_t    padding;
    }; // 40

struct 
    ept_table {
    union {
        struct ept_header       header; // 24
        struct {
            union { // 4
                char            magic_string[4];
                uint32_t        magic_uint32;
            };
            union { // 12
                char            version_string[12];
                uint32_t        version_uint32[3];
            };
            uint32_t            partitions_count;   // 4
            uint32_t            checksum;   // 4
        };
    };
    union {
        struct ept_partition    partitions[MAX_PARTITIONS_COUNT]; // 40 * 32
        char                    partition_names[MAX_PARTITIONS_COUNT][sizeof(struct ept_partition)];
    };
}; // 1304

/* Variable */

// extern struct ept_table const  ept_table_empty;

/* Function */

// uint32_t
//     ept_checksum(
//         struct ept_partition const * const  partitions, 
//         int const                           partitions_count
//     );

int 
    ept_compare_table(
        struct ept_table const *    table_a, 
        struct ept_table const *    table_b
    );

int
    ept_eclone_parse(
        int const                   argc,
        char const * const * const  argv,
        struct ept_table * const    table,
        size_t const                capacity
    );

int
    ept_is_not_pedantic(
        struct ept_table const *    table
    );

uint64_t 
    ept_get_capacity(
        struct ept_table const *    table
    );

int
    ept_read_and_report(
        struct ept_table *  table,
        int                 fd,
        size_t              size
    );

void 
    ept_report(
        struct ept_table const *    table
    );

int
    ept_table_from_dtb(
        struct ept_table *  table,
        uint8_t const *     dtb,
        size_t              dtb_size,
        uint64_t            capacity
    );

int
    ept_table_from_dts_partitions_helper(
        struct ept_table * const                table,
        struct dts_partitions_helper const *    phelper, 
        uint64_t const                          capacity
    );

int 
    ept_valid_table(
        struct ept_table const *    table
    );

#endif 