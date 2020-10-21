#include <array>
#include <memory>
#include <algorithm>
#include <cstring>
#include <xmmintrin.h>

extern bool loadIR(const char*, float*, size_t);

template <size_t SIZE=1024u>
class Convolver
{
    std::unique_ptr<std::array<float, SIZE>> ir_buffer;
    std::unique_ptr<std::array<float, SIZE>> delay_buffer[2];
    size_t dWriteIndex[2];
    size_t buffer_mask;
public:
    Convolver(const char*);
    void update(float*, const int) noexcept;
    void updateSIMD(float*, const int) noexcept;
};

template <size_t SIZE>
Convolver<SIZE>::Convolver(const char* filename)
{
    static_assert(!(SIZE & 0x3), "SIZE must be divisible by 4!\n");
    ir_buffer = std::make_unique<std::array<float, SIZE>>();
    delay_buffer[0] = std::make_unique<std::array<float, SIZE>>();
    delay_buffer[1] = std::make_unique<std::array<float, SIZE>>();
    memset(dWriteIndex, 0, 2*sizeof(size_t));
    buffer_mask = SIZE - 1u;
    loadIR(filename, ir_buffer.data(), SIZE);
    // std::reverse(ir_buffer.begin(), ir_buffer.end());
}

template <size_t SIZE>
void Convolver<SIZE>::update(float* buffer, const int ch) noexcept
{
    float xn  = *buffer;
    delay_buffer[ch]->at(dWriteIndex[ch]) = xn;
    size_t dReadIndex = dWriteIndex[ch];
    float yn_accum = 0.0f;

    for (size_t ir_index = 0; ir_index < ir_buffer->size(); ++ir_index){
        yn_accum += delay_buffer[ch]->at(dReadIndex) * ir_buffer->at(ir_index);

        dReadIndex = ++dReadIndex & buffer_mask;
    }

    dWriteIndex[ch] = --dWriteIndex[ch] & buffer_mask;

    *buffer = yn_accum;
}

template <size_t SIZE>
void Convolver<SIZE>::updateSIMD(float* buffer, const int ch) noexcept
{
    float xn  = *buffer;
    delay_buffer[ch]->at(dWriteIndex[ch]) = xn;
    size_t dReadIndex = dWriteIndex[ch];
    __m128 yn_accum = _mm_setzero_ps();

    for (size_t ir_index = 0; ir_index < ir_buffer->size(); ir_index += 4){
        size_t ind1 = (dReadIndex + 1) & buffer_mask;
        size_t ind2 = (dReadIndex + 2) & buffer_mask;
        size_t ind3 = (dReadIndex + 3) & buffer_mask;
        __m128 delayed = _mm_set_ps(delay_buffer[ind3], delay_buffer[ind2], delay_buffer[ind1], delay_buffer[dReadIndex]);
        __m128 ir = _mm_load_ps(&ir_buffer->at(ir_index));

        delayed = _mm_mul_ps(delayed, ir);

        yn_accum = _mm_add_ps(yn_accum, delayed);

        dReadIndex = (dReadIndex + 4) & buffer_mask;
    }

   yn_accum = _mm_add_ps(_mm_shuffle_ps(yn_accum, yn_accum,
                                        _MM_SHUFFLE(1, 0, 3, 2)),
                        yn_accum);
   yn_accum = _mm_add_ps(_mm_shuffle_ps(yn_accum, yn_accum,
                                        _MM_SHUFFLE(0, 1, 2, 3)),
                        yn_accum);

    *buffer = _mm_cvtss_f32(yn_accum);

    dWriteIndex[ch] = --dWriteIndex[ch] & buffer_mask;
}


