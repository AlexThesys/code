template <size_t SIZE>
class BumpPtrAllocator
{
    static constexpr int SlabSize = 4096;
    void* slabs[SIZE];
    void *currPtr, *end;
    size_t slabCounter = 0;
public:
    BumpPtrAllocator() : currPtr(nullptr), end(nullptr) {}
    void* allocate(int size)
    {
        if (size >= (char*)end - (char*)currPtr) {
            currPtr = malloc(SlabSize);
            end = (char*)currPtr + SlabSize;
            slabs[slabCounter++] = currPtr;
        }
        void* ptr = currPtr;
        currPtr = (char*)currPtr + size;
        return ptr;
    }
    ~BumpPtrAllocator()
    {
        for (size_t i = 0; i < slabCounter; ++i)
            free(slabs[i]);
    }
    //...
};

int main()
{
    BumpPtrAllocator<8> bpa;

    int* a = reinterpret_cast<int*>(bpa.allocate(128*sizeof(int)));
    for (int i = 0; i < 128; ++i)
        a[i] = i;

    cout << a[0] << endl;
    cout << a[4] << endl;

    return 0;
}


