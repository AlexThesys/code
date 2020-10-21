#include <emmintir.h>

uint32_t sum (uint16_t* first, uinst16_t* last)
{
    __m128i result = _mm_setzero_epi32();
    for (; first != last; first += 8){
        __m128i in = _mm_load_si128(reinterpret_cast<__m128i*>(first));
        __m128i lo = _mm_unpacklo_epi16(in, _mm_set_zero_si128());
        __m128i hi = _mm_unpackhi_epi16(in, _mm_set_zero_si128());

        result = _mm_add_epi32(result, lo);
        result = _mm_add_epi32(result, hi);
    }

    result = _mm_add_epi32(result,
            _mm_shuffle_epi32(result, _MM_SHUFFLE(1, 0, 3, 2)));
    result = _mm_add_epi32(result,
            _mm_shuffle_epi32(result, _MM_SHUFFLE(0, 1, 2, 3)));
    return _mm_cvtsi128_si32(result);
}
