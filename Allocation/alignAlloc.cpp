
void* allocateAligned(size_t size_bytes, size_t alignment)
{
    using U8 = uint8_t;
    assert(alignment >= 1);
    assert(alignment <= 128);
    assert((alignment & (alignment - 1)) == 0); // pwr of 2
    // Determine total amount of memory to allocate.
    size_t expandedSize_bytes = size_bytes + alignment;
    // Allocate unaligned block & convert address to uintptr_t.
    uintptr_t rawAddress = reinterpret_cast<uintptr_t>(
            malloc(expandedSize_bytes));
    // Calculate the adjustment by masking off the lower bits
    // of the address, to determine how "misaligned" it is.
    size_t mask = (alignment - 1);
    uintptr_t misalignment = (rawAddress & mask);
    ptrdiff_t adjustment = alignment - misalignment;
    // Calculate the adjusted address.
    uintptr_t alignedAddress = rawAddress + adjustment;
        // Store the adjustment in the byte immediately
        // preceding the adjusted address.
        assert(adjustment < 256);
    U8* pAlignedMem = reinterpret_cast<U8*>(alignedAddress);
    pAlignedMem[-1] = static_cast<U8>(adjustment);
    return reinterpret_cast<void*>(pAlignedMem);
}

void freeAligned(void* pMem)
{
    using U8 = uint8_t;
    const U8* pAlignedMem
        = reinterpret_cast<const U8*>(pMem);
    uintptr_t alignedAddress
        = reinterpret_cast<uintptr_t>(pMem);
    ptrdiff_t adjustment
        = static_cast<ptrdiff_t>(pAlignedMem[-1]);
    uintptr_t rawAddress = alignedAddress - adjustment;
    void* pRawMem = reinterpret_cast<void*>(rawAddress);
    free(pRawMem);
}

int main()
{
    double* d = reinterpret_cast<double*>(aligned_alloc(128, 8));
    d[0] = 6.7;
    cout << d[0] << endl;
    freeAligned(d);

    return 0;
}

