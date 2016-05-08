#ifndef MD5_INCLUDE_H
#define MD5_INCLUDE_H

#include <stddef.h>
#include <stdint.h>

typedef struct md5 {
    uint8_t buffer[64];
    uint32_t digest[4];
    uint64_t pos;
    uint64_t len;
} md5_t;

void md5_init(md5_t* md5);
void md5_update(md5_t* md5, uint8_t* data, size_t len);
void md5_finish(md5_t* md5);
void md5_transform(uint32_t* r, uint32_t* x);

char* md5(char* msg, size_t len);
void md5_compute(md5_t* md5);
void md5_print(md5_t* md5);

#endif
