#ifndef PROMPT_H
#define PROMPT_H

#include "!TYPES.h"

// Reads a line into `buf` (capacity `cap`) with cursor movement support.
// Returns  0 on Enter (success).
// Returns -1 on Escape (caller should treat as user abort).
i32 prompt_read(char *buf, usize cap);

#endif
