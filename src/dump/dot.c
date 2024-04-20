#include "btd/dump/dot.h"

#include "btd/dump/common.h"

#include <stdio.h>
#include <stdlib.h>

#define BTD_DOT_NEWLINE "\\n"

#define BTD_DOT_DUMP_CSUM(fd, label, csum_data, size) \
    do {                                              \
        dprintf(fd, label ": 0x");                    \
        btd_dump_byte_array(fd, csum_data, size, BTD_DBA_SKIP_LEADING_ZEROS); \
        dprintf(fd, BTD_DOT_NEWLINE);                 \
    } while (0)

#define BTD_DOT_DUMP_UUID(fd, title, value, size) \
    do {                                          \
        dprintf(fd, title ": ");                  \
        btd_dump_uuid(fd, value, size);           \
        dprintf(fd, BTD_DOT_NEWLINE);             \
    } while(0)

#define BTD_DOT_DUMP_KEY(fd, label, key_ptr) \
    do {                                     \
        dprintf(fd, label ": ");             \
        btd_dump_key(fd, key_ptr);           \
        dprintf(fd, BTD_DOT_NEWLINE);        \
    } while(0)

#define BTD_DOT_DUMP_U8(fd, title, value)  dprintf(fd, title ": %hhu (0x%02X)" BTD_DOT_NEWLINE, value, value)
#define BTD_DOT_DUMP_U16(fd, title, value) dprintf(fd, title ": %hu (0x%02X)" BTD_DOT_NEWLINE, value, value)
#define BTD_DOT_DUMP_U32(fd, title, value) dprintf(fd, title ": %u (0x%08X)" BTD_DOT_NEWLINE,   value, value)
#define BTD_DOT_DUMP_U64(fd, title, value) dprintf(fd, title ": %llu (0x%016llX)" BTD_DOT_NEWLINE, value, value)

void dot_dump_header(int fd, uint64_t location_found, struct btrfs_header *header)
{
    dprintf(fd, "\"0x%016lX\" [label=\"", location_found);

    BTD_DOT_DUMP_CSUM(fd, "csum", header->csum, BTRFS_CSUM_SIZE);
    BTD_DOT_DUMP_UUID(fd, "fsid", header->fsid, BTRFS_FSID_SIZE);
    BTD_DOT_DUMP_U64(fd, "bytenr", header->bytenr);
    BTD_DOT_DUMP_U64(fd, "flags", header->flags);

    BTD_DOT_DUMP_UUID(fd, "chunk_tree_uuid", header->chunk_tree_uuid, BTRFS_FSID_SIZE);
    BTD_DOT_DUMP_U64(fd, "bytenr", header->bytenr);
    BTD_DOT_DUMP_U64(fd, "owner", header->owner);
    BTD_DOT_DUMP_U32(fd, "nritems", header->nritems);
    // Print last value without a newline at the end
    // TODO(Adin): Need a better way to toggle newline printing
    dprintf(fd, "level: %hhu (0x%02X)", header->level, header->level);

    dprintf(fd, "\"];\n");
}

void dot_dump_key_ptr(int fd, uint64_t node_found_in, struct btrfs_key_ptr *ptr)
{
    dprintf(fd, "\"0x%016lX\" -> \"0x%016llX\" [xlabel=\"", node_found_in, ptr->blockptr);
    BTD_DOT_DUMP_KEY(fd, "key", &ptr->key);
    dprintf(fd, "generation: %llu (0x%016llX)", ptr->generation, ptr->generation);
    dprintf(fd, "\"];\n");
}