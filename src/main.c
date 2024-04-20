#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

#include <sys/stat.h>

#include <btrfs/ctree.h>

#include "btd/common.h"
#include "btd/dump/term.h"
#include "btd/dump/dot.h"

void dump_tree_recursive(int fs_fd, int out_fd, uint64_t node_location)
{
    struct btrfs_header node_header;
    lseek(fs_fd, node_location, SEEK_SET);
    read(fs_fd, &node_header, sizeof(node_header));

    dot_dump_header(out_fd, node_location, &node_header);

    if(node_header.level > 0) {
        struct btrfs_key_ptr *ptrs = malloc(node_header.nritems * sizeof(ptrs));
        if(ptrs == NULL) {
            error("Failed to allocate ptrs for node 0x%016lX", node_location);
            return;
        }

        read(fs_fd, ptrs, node_header.nritems * sizeof(*ptrs));

        for(uint32_t i = 0; i < node_header.nritems; i++) {
            dot_dump_key_ptr(out_fd, node_location, &ptrs[i]);

            dump_tree_recursive(fs_fd, out_fd, ptrs[i].blockptr);
        }

        free(ptrs);
    }
}

void term_dump_tree_recursive(int fs_fd, int out_fd, uint64_t node_location)
{
    struct btrfs_header node_header;
    lseek(fs_fd, node_location, SEEK_SET);
    read(fs_fd, &node_header, sizeof(node_header));

    term_dump_header(out_fd, &node_header);
    dprintf(fs_fd, "\n");

    if(node_header.level > 0) {
        struct btrfs_key_ptr *ptrs = malloc(node_header.nritems * sizeof(ptrs));
        if(ptrs == NULL) {
            error("Failed to allocate ptrs for node 0x%016lX", node_location);
            return;
        }

        read(fs_fd, ptrs, node_header.nritems * sizeof(*ptrs));

        for(uint32_t i = 0; i < node_header.nritems; i++) {
            dprintf(fs_fd, "// -- 0x%016lX -----------------------------------\n", node_location);
            term_dump_key_ptr(out_fd, &ptrs[i]);
            dprintf(fs_fd, "\n");

            dump_tree_recursive(fs_fd, out_fd, ptrs[i].blockptr);
        }

        free(ptrs);
    }
}

int main(int argc, char **argv)
{
    if(argc < 2) {
        error("Usage: btrfs-tree-dump <path-to-filesystem>\n");
        return 1;
    }

    struct stat stats;
    if(stat(argv[1], &stats) != 0) {
        perror("stat");
        return 1;
    }

    int fd = 0;
    fd = open(argv[1], O_RDONLY);
    if(fd < 0) {
        perror("open");
        return 1;
    }

    lseek(fd, BTRFS_SUPER_INFO_OFFSET, SEEK_SET);

    struct btrfs_super_block super;
    read(fd, &super, sizeof(super));

    // Dump the super block
    term_dump_super(STDOUT_FILENO, &super);
    printf("\n");

    dump_tree_recursive(fd, STDOUT_FILENO, super.root);

    // lseek(fd, super.root, SEEK_SET);

    // struct btrfs_header root_header;
    // read(fd, &root_header, sizeof(root_header));

    // // Dump the root tree root header
    // term_dump_header(STDOUT_FILENO, &root_header);
    // printf("\n");

    // if(root_header.level > 0) {
    //     struct btrfs_key_ptr *root_ptrs = malloc(root_header.nritems * sizeof(*root_ptrs));
    //     read(fd, root_ptrs, root_header.nritems * sizeof(*root_ptrs));

    //     // Dump the root tree node key pointers
    //     for(uint32_t i = 0; i < root_header.nritems; i++) {
    //         term_dump_key_ptr(STDOUT_FILENO, &root_ptrs[i]);
    //         printf("\n");
    //     }
    // }
    // else {
    //     struct btrfs_item *root_items = malloc(root_header.nritems * sizeof(*root_items));

    //     // Dump the root tree node key pointers
    //     for(uint32_t i = 0; i < root_header.nritems; i++) {
    //         term_dump_item(STDOUT_FILENO, &root_items[i]);
    //         printf("\n");
    //     }
    // }

    if(close(fd) != 0) {
        perror("close");
        return 1;
    }
}