#include "btd/dump/term.h"

#include <stdio.h>
#include <assert.h>

#include "btd/common.h"
#include "btd/dump/common.h"

#define DUMP_CHECKSUM(fd, title, value, size) do {                       \
                                              dprintf(fd, title ": 0x"); \
                                              btd_dump_byte_array(fd, value, size, BTD_DBA_SKIP_LEADING_ZEROS); \
                                              dprintf(fd, "\n");         \
                                          } while(0)

#define DUMP_UUID(fd, title, value, size) do {                         \
                                          dprintf(fd, title ": ");      \
                                          btd_dump_uuid(fd, value, size); \
                                          dprintf(fd, "\n");            \
                                      } while(0)

#define DUMP_KEY(fd, title, key) do { \
                                 dprintf(fd, title ": "); \
                                 btd_dump_key(fd, key); \
                             } while(0)

#define DUMP_U8(fd, title, value)  dprintf(fd, title ": %hhu (0x%02X)\n", value, value)
#define DUMP_U16(fd, title, value) dprintf(fd, title ": %hu (0x%02X)\n", value, value)
#define DUMP_U32(fd, title, value) dprintf(fd, title ": %u (0x%08X)\n",   value, value)
#define DUMP_U64(fd, title, value) dprintf(fd, title ": %llu (0x%016llX)\n", value, value)

static void term_dump_uuid_impl(int fd, uint8_t *uuid, size_t size)
{
    // TODO(Adin): non-assert check for this
    assert(size == 16);

    // TODO(Adin): There is definitely a better way to do this
    btd_dump_byte_array(fd, uuid,      4, BTD_DBA_BIG_ENDIAN | BTD_DBA_LOWERCASE);
    dprintf(fd, "-");
    btd_dump_byte_array(fd, uuid + 4,  2, BTD_DBA_BIG_ENDIAN | BTD_DBA_LOWERCASE);
    dprintf(fd, "-");
    btd_dump_byte_array(fd, uuid + 6,  2, BTD_DBA_BIG_ENDIAN | BTD_DBA_LOWERCASE);
    dprintf(fd, "-");
    btd_dump_byte_array(fd, uuid + 8,  2, BTD_DBA_BIG_ENDIAN | BTD_DBA_LOWERCASE);
    dprintf(fd, "-");
    btd_dump_byte_array(fd, uuid + 10, 6, BTD_DBA_BIG_ENDIAN | BTD_DBA_LOWERCASE);
}

void term_dump_super(int fd, struct btrfs_super_block *super)
{
    // TODO(Adin): Also print pointers as byte offsets
    // TODO(Adin): Dump flag names
    // TODO(Adin): Dump ascii magic

    DUMP_CHECKSUM(fd, "csum", super->csum, BTRFS_CSUM_SIZE);
    DUMP_UUID(fd, "fsid", super->fsid, BTRFS_FSID_SIZE);
    DUMP_U64(fd, "bytenr", super->bytenr);
    DUMP_U64(fd, "flags", super->flags);

    DUMP_U64(fd, "magic", super->magic);
    DUMP_U64(fd, "generation", super->generation);
    DUMP_U64(fd, "root", super->root);
    DUMP_U64(fd, "chunk root", super->chunk_root);
    DUMP_U64(fd, "log_root", super->log_root);

    DUMP_U64(fd, "log_root_transid", super->log_root_transid);
    DUMP_U64(fd, "total_bytes", super->total_bytes);
    DUMP_U64(fd, "bytes_used", super->bytes_used);
    DUMP_U64(fd, "root_dir_objectid", super->root_dir_objectid);
    DUMP_U64(fd, "num_devices", super->num_devices);
    DUMP_U32(fd, "sectorsize", super->sectorsize);
    DUMP_U32(fd, "nodesize", super->nodesize);

    DUMP_U32(fd, "__unused_leafsize", super->__unused_leafsize);
    DUMP_U32(fd, "stripesize", super->stripesize);
    DUMP_U32(fd, "sys_chunk_array_size", super->sys_chunk_array_size);
    DUMP_U64(fd, "chunk_root_generation", super->chunk_root_generation);
    DUMP_U64(fd, "compat_flags", super->compat_flags);
    DUMP_U64(fd, "compat_ro_flags", super->compat_ro_flags);
    DUMP_U64(fd, "incompat_flags", super->incompat_flags);
    DUMP_U16(fd, "csum_type", super->csum_type);
    DUMP_U8(fd, "root_level", super->root_level);
    DUMP_U8(fd, "chunk_root_level", super->chunk_root_level);
    DUMP_U8(fd, "log_root_level", super->log_root_level);

    // TODO(fd, Adin): print dev_item

    dprintf(fd, "label: \"%s\"\n", super->label);

    DUMP_U64(fd, "cache_generation", super->cache_generation);
    DUMP_U64(fd, "uuid_tree_generation", super->uuid_tree_generation);

    DUMP_UUID(fd, "metadata_uuid", super->metadata_uuid, BTRFS_FSID_SIZE);

    DUMP_U64(fd, "block_group_root", super->block_group_root);
    DUMP_U64(fd, "block_group_root_generation", super->block_group_root_generation);
    DUMP_U8(fd, "block_group_root_level", super->block_group_root_level);

    // TODO(Adin): Future expansion fields (make optional with parameter)
}

void term_dump_header(int fd, struct btrfs_header *header)
{
    DUMP_CHECKSUM(fd, "csum", header->csum, BTRFS_CSUM_SIZE);
    DUMP_UUID(fd, "fsid", header->fsid, BTRFS_FSID_SIZE);
    DUMP_U64(fd, "bytenr", header->bytenr);
    DUMP_U64(fd, "byte_nr", header->flags);

    DUMP_UUID(fd, "chunk_tree_uuid", header->chunk_tree_uuid, BTRFS_UUID_SIZE);
    DUMP_U64(fd, "generation", header->generation);
    DUMP_U64(fd, "owner", header->owner);
    DUMP_U32(fd, "nritems", header->nritems);
    DUMP_U8(fd, "level", header->level);
}

void term_dump_item(int fd, struct btrfs_item *item)
{
    DUMP_KEY(fd, "key", &item->key);
    DUMP_U32(fd, "offset", item->offset);
    DUMP_U32(fd, "size", item->offset);
}

void term_dump_key_ptr(int fd, struct btrfs_key_ptr *ptr)
{
    DUMP_KEY(fd, "key", &ptr->key);
    DUMP_U64(fd, "blockptr", ptr->blockptr);
    DUMP_U64(fd, "generation", ptr->generation);
}