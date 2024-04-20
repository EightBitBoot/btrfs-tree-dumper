#pragma once

#include <stdint.h>

#include <btrfs/ctree.h>

void dot_dump_header(int fd, uint64_t location_found, struct btrfs_header *header);
void dot_dump_key_ptr(int fd, uint64_t node_found_in, struct btrfs_key_ptr *key_ptr);