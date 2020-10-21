#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <tmmintrin.h>
#include <memory.h>
#include <stdint.h>
#include <math.h>

#define MAX_SIZE 0x10000000
#define REG_SIZE 0x10
static const int bits2shift = (int)log2((double)REG_SIZE);

size_t read_file(const char*, uint8_t**, size_t*);
int reverse(uint8_t*, const size_t);
size_t calc_alignment(void*, const uint8_t);
void print_hex(const uint8_t*, const size_t);

int main(int argc, char** argv)
{
    if (argc < 2) {
        puts("Provide the filename!");
        return -1;
    }
    uint8_t *buf = NULL;
    size_t offset;
    const size_t size = read_file(argv[1], &buf, &offset);
    const int chars_left = reverse(buf+offset, size);
    print_hex(buf+offset-chars_left, size);
    free(buf);

    return EXIT_SUCCESS;
}

size_t read_file(const char *fname, uint8_t **buf, size_t *offset)
{
    FILE* file = fopen(fname, "rb");
    if (!file) {
        puts("Error opening file!");
       return -1; 
    }
    fseek(file, 0L, SEEK_END);    
    size_t size = ftell(file);
    rewind(file);
    if (size > MAX_SIZE) {
        puts("File exceeding maximum size!");
        fclose(file);
        return -1;
    }
    if (!(*buf = malloc(size + ((REG_SIZE<<1)-0x1)))) {
        puts("Buffer allocation failed!");
        fclose(file);
        return -1;
    } 
    *offset = calc_alignment((void*)*buf, REG_SIZE) + REG_SIZE;
    if (fread((void*)(*buf+*offset), 1, size, file) != size) {
        puts("Error reading file!");
        fclose(file);
        return -1;
    }
    fclose(file);
    return size;
}

int reverse(uint8_t* buf, const size_t size)
{
    __m128i xmm0, xmm1, xmm2;
    xmm2 = _mm_set_epi8(0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                        0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf);
    const size_t size_q = size & (~(REG_SIZE-0x1));
    size_t i = 0ul;
    for (size_t j = size_q - REG_SIZE; i < j; i+=REG_SIZE, j-=REG_SIZE) {
        xmm0 = _mm_load_si128((__m128i*)(buf + i));
        xmm1 = _mm_load_si128((__m128i*)(buf + j));
        xmm0 = _mm_shuffle_epi8(xmm0, xmm2);
        xmm1 = _mm_shuffle_epi8(xmm1, xmm2);
        _mm_store_si128((__m128i*)(buf + i), xmm1);
        _mm_store_si128((__m128i*)(buf + j), xmm0);
    } 
    if (size_q & REG_SIZE) {
        xmm0 = _mm_load_si128((__m128i*)(buf + i));
        xmm0 = _mm_shuffle_epi8(xmm0, xmm2);
        _mm_store_si128((__m128i*)(buf + i), xmm0);
    }
    for (size_t k = size_q, j = 1; k < size; k++, j++) {
        *(buf-j) = buf[k]; 
    }
    return (int)(size - size_q);
}

uint64_t calc_alignment(void* buf, const uint8_t align)
{
    const size_t addr = (size_t)buf;
    return (addr & (align-0x1));
}

void print_hex(const uint8_t *buf, const size_t size)
{
    const size_t num_lines = size >> bits2shift;
    size_t offset = 0lu;
    for (size_t i = 0; i < num_lines; i++) {
        
        for (uint8_t j = 0u; j < REG_SIZE; j++) {
            printf("%02X", buf[offset+j]);
            if (j&0x1) printf(" ");
        }
        offset  += REG_SIZE;
        printf("\n");
    }
    for (uint8_t j = offset; j < size; j++) {
        printf("%02X", buf[j]);
        if (j&0x1) printf(" ");
    }
    printf("\n");
}
