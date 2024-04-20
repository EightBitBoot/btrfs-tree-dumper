#pragma once

#include <btrfs/ctree.h>

void term_dump_super(int fd, struct btrfs_super_block *super);
void term_dump_header(int fd, struct btrfs_header *header);
void term_dump_item(int fd, struct btrfs_item *item);
void btd_dump_key(int fd, struct btrfs_disk_key *key);
void term_dump_key_ptr(int fd, struct btrfs_key_ptr *ptr);