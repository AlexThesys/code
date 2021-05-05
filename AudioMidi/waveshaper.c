#include <stdint.h>
#include <xmmintrin.h>
#include <emmintrin.h>

#define M_PI_4 0.785398163397448309616  // pi/4

// in c++ make these template parameters
float coef_pos = 1.0f; // 0.1f ... 20.0f
float coef_neg = 1.0f; // 0.1f ... 20.0f
int num_stages = 10; // 1 ... 10
int32_t invert_stages = 1; // 0 ... 1

inline float fast_atan(float x) {
	return M_PI_4 * x - x * ((float)((uint32_t)x & 0x7FFFFFFF) - 1.0f) * (0.2447f + 0.0663f * (float)((uint32_t)x & 0x7FFFFFFF));
}

#define fast_atan_simd(x) \
x = _mm_mul_ps(x,x);\
x = _mm_mul_ps(x, pi_4);\
abs_x = _mm_and_ps(x, (__m128)not_mask);\
temp = _mm_sub_ps(abs_x, one);\
x = _mm_mul_ps(x, temp);\
temp = _mm_mul_ps(abs_x, b);\
temp = _mm_add_ps(temp, a);\
x = _mm_mul_ps(x, temp); 

void waveshaper(float* buffer, int buf_len) {
    for (int i = 0; i < buf_len; i++) {
        float sample = buffer[i];
        for (int j = 0; j < num_stages; j++) {
			const int32_t mask = (int32_t)sample >> 0x1f;
            const float coeff = (~mask & (uint32_t)coef_pos) | (mask & (uint32_t)coef_neg);
            sample = (1.0f / fast_atan(coeff)) * fast_atan(coeff * sample);
            sample = (uint32_t)sample ^ (0x80000000 & ~((invert_stages & j) - 0x01));
        }
        buffer[i] = sample;
    }
}

void waveshaper_simd(float* buffer, int buf_len) {
	const int buf_len_simd = buf_len & 0x03;
	const __m128i sign_bit = _mm_set1_epi32(0x80000000);
	const __m128i not_sign_bit = _mm_set1_epi32(0x7FFFFFFF);
	const __m128 c_pos = _mm_set1_ps(coef_pos);
	const __m128 c_neg = _mm_set1_ps(coef_neg);
	const __m128i not_mask = _mm_set1_epi32(0xFFFFFFFF);
	const __m128 pi_4 = _mm_set1_ps(M_PI_4);
	const __m128 one = _mm_set1_ps(1.0f);
	const __m128 a = _mm_set1_ps(0.2447f);
	const __m128 b = _mm_set1_ps(0.0663f);

	// process
   for (int i = 0; i < buf_len_simd; i += 4) {
		__m128 sample = _mm_load_ps(&buffer[i]);
		for (int j = 0; j < num_stages; j++) {
			__m128i mask = _mm_srai_epi32((__m128i)sample, 0x1f);
			__m128 coef = _mm_and_ps((__m128)mask, c_neg);
			mask = _mm_xor_si128(mask, not_mask);
			mask = _mm_and_si128(mask, (__m128i)c_pos);
			coef = _mm_or_ps(coef, (__m128)mask);
			sample = _mm_mul_ps(sample, coef);
            __m128 abs_x, temp;
			fast_atan_simd(coef)
			coef = _mm_div_ps(one, coef);
			fast_atan_simd(sample)
			sample = _mm_mul_ps(sample, coef);
			const uint32_t invert = 0x80000000 & ~((invert_stages & j) - 0x01);
			const __m128i inv = _mm_set1_epi32(invert);
			sample = _mm_xor_ps(sample, (__m128)inv);
		}
		_mm_store_ps(&buffer[i], sample);
    }
	
	// process the rest
    for (int i = buf_len_simd; i < buf_len; i++) {
        float sample = buffer[i];
        for (int j = 0; j < num_stages; j++) {
			const int32_t mask = (int32_t)sample >> 0x1f;
            const float coeff = (~mask & (uint32_t)coef_pos) | (mask & (uint32_t)coef_neg);
            sample = (1.0f / fast_atan(coeff)) * fast_atan(coeff * sample);
            sample = (uint32_t)sample ^ (0x80000000 & ~((invert_stages & j) - 0x01));
        }
        buffer[i] = sample;
    }
}