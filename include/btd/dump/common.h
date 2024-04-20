#pragma once

#include <stdint.h>
#include <stddef.h>

#include <btrfs/ctree.h>

#define BTD_DBA_LOWERCASE (1U)
#define BTD_DBA_BIG_ENDIAN (2U)
#define BTD_DBA_SKIP_LEADING_ZEROS (4U)

void btd_dump_byte_array(int fd, uint8_t *array, size_t size, int flags);
void btd_dump_key(int fd, struct btrfs_disk_key *key);
void btd_dump_uuid(int fd, uint8_t *uuid, size_t size);