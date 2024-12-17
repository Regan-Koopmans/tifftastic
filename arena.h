#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t *base;
  size_t size;
  size_t offset;
} Arena;

