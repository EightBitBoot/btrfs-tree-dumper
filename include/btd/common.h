#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BTD_BLOCK_SIZE (512U)

#define error(format, ...) fprintf(stderr, format, ##__VA_ARGS__)