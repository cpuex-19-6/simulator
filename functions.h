#ifndef FUNCTIONS_INCLUDE
#define FUNCTIONS_INCLUDE

#include <stdint.h>

uint32_t downto(uint32_t u, int n, int m);

int c2b_8(uint8_t *dest, char *source, size_t size);
int c2b_32(uint32_t *dest, char *source, size_t size);

void b2c_8(char *dest, int8_t source);
void b2c_32(char *dest, int32_t source);

void print_binary(uint32_t instr);

void endian_wrapper(void *a, void *b, size_t size);

#endif
