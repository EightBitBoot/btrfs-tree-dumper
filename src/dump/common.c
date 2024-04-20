#include "btd/dump/common.h"

#include <stdio.h>

void btd_dump_byte_array(int fd, uint8_t *array, size_t size, int flags)
{
    // Uppercase is default
    char *format = ((flags & BTD_DBA_LOWERCASE) ? "%02x" : "%02X");

    if(flags & BTD_DBA_BIG_ENDIAN) {
        // Big endian
        size_t start_index = 0;

        if(flags & BTD_DBA_SKIP_LEADING_ZEROS) {
            // Skip leading zeros; default is to print leading zeros
            while(start_index < size && array[start_index] == 0) {
                start_index++;
            }
        }

        for(size_t i = start_index; i < size; i++) {
            dprintf(fd, format, array[i]);
        }
    }
    else {
        // Little endian; default
        size_t start_index = 0;

        if(flags & BTD_DBA_SKIP_LEADING_ZEROS) {
            // Skip leading zeros; default is to print leading zeros
            while(start_index < size && array[size - 1 - start_index] == 0) {
                start_index++;
            }
        }

        for(size_t i = start_index; i < size; i++) {
            dprintf(fd, format, array[size - 1 - i]);
        }
    }
}

void btd_dump_uuid(int fd, uint8_t *uuid, size_t size)
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

void btd_dump_key(int fd, struct btrfs_disk_key *key)
{
    // NOTE(Adin): The keys are stored little endian on disk
    //             so this function will only work on little
    //             endian cpus

    dprintf(fd, "{ objectid: 0x%016llX, type: 0x%02hhX, offset: 0x%016llX }",
           key->objectid,
           key->type,
           key->offset);
}
