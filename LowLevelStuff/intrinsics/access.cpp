// To access 8-bit data:
#define _mm_extract_epi8(x, imm) \
((((imm) & 0x1) == 0) ?           \
_mm_extract_epi16((x), (imm) >> 1) & 0xff : \
_mm_extract_epi16(_mm_srli_epi16((x), 8), (imm) >> 1))

// For 16-bit data, use the following intrinsic:
int _mm_extract_epi16(__m128i a, int imm)
// To access 32-bit data:
#define _mm_extract_epi32(x, imm) \
_mm_cvtsi128_si32(_mm_srli_si128((x), 4 * (imm)))
// To access 64-bit data (Intel® 64 architecture only):
#define _mm_extract_epi64(x, imm) \
_mm_cvtsi128_si64(_mm_srli_si128((x), 8 * (imm)))


