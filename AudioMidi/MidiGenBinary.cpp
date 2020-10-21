#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <random>
#include <chrono>
#include <ctype.h>
#include <cmath>
#include <tuple>

#define LINUX

constexpr int division = 1024;
constexpr int divUnit = division >> 2;

union VarLength
{
    unsigned short word;
    unsigned char bytes[2];
};

typedef struct
{
    unsigned char* note;
    VarLength* length;
} MidiData;

bool createMidiFile(unsigned int, MidiData*);
bool fillRandomizedBuffers(unsigned char*, VarLength*, unsigned int);
unsigned bswap_dword(unsigned a);
unsigned short bswap_word(unsigned short a);
VarLength cookVarLength(unsigned short val);
std::pair<unsigned char, unsigned short> calcNoteOct(unsigned short note);

struct MThd_data
{
    const unsigned char mthd[4] = {'M','T', 'h', 'd'};
    const unsigned int dataLen = bswap_dword(0x6);
    const unsigned short data[3] = {0x0, bswap_word(0x1), bswap_word(division)};
    const unsigned char padding[2];
};

struct MTrk_data
{
    const unsigned char mthd[4] = {'M','T', 'r', 'k'};
    unsigned int dataLen;
};

int main(int argc, char* argv[])
{
    unsigned int numNotes = 100;
    if (argc < 2) {
        puts("Put the number of notes to generate!");
    }
    else {
        if (isdigit(*argv[1]))
            numNotes = static_cast<unsigned>(atoi(argv[1]));
    }
    printf("Notes to create: %d\n", numNotes);

    MidiData midiData;
    if (!((midiData.note = (unsigned char*)malloc(sizeof(char)*numNotes))
        && (midiData.length = (VarLength*)malloc(sizeof(VarLength)*numNotes)))) {
        puts("Memory not allocated!");
        return 1;
    }

    if (!fillRandomizedBuffers(midiData.note, midiData.length, numNotes))
        return 1;

    if (createMidiFile(numNotes, &midiData))
        puts("Midi file succesfully created!");

    free(midiData.note);
    free(midiData.length);

    return 0;
}

bool createMidiFile(unsigned int count, MidiData* mData)
{
    FILE* file;
#ifdef LINUX
    if ((file = fopen("MidiData/midiFile.mid", "w")) == nullptr) return false;
#else
    if (fopen_s(&file, "midiFile.mid", "w")) return false;
#endif
    // write MThd header
    {
        MThd_data mtdhData{};
        fwrite(&mtdhData, 1, 14, file);
    }
    // write MTrk header
    {
        MTrk_data mtrkData{};
        // (numNotes+1) * (varLen + noteOff + 1 + noteOn)  + endOfTrack[]
        mtrkData.dataLen = (count+1) * (2 + 3 + 1 + 3) + 4;
        mtrkData.dataLen = bswap_dword(mtrkData.dataLen);
        fwrite(&mtrkData, 1, 8, file);
    }
    // write first noteOn event
    {
        //1 byte varLenght, 3 bytes event
        unsigned char firstNoteOn[] = {0x0, 0x90, mData->note[0], 0x7F};	// deltaTime+noteOn
        fwrite(firstNoteOn, 1, 4, file);
    }
    //write events loop
    {
        unsigned char noteData[] = {0x0, 0x0, 0x0};
        for (unsigned i = 0; i < count-1; ++i) {
            // write delta time
            fwrite(&mData->length[i], 2, 1, file);
            // write noteOff
            noteData[0] = 0x80;
            noteData[1] = mData->note[i];
            noteData[2] = 0x0;
            fwrite(noteData, 1, 3, file);
            // write zero deltaTime
            unsigned char zeroDelta = 0x0;
            fwrite(&zeroDelta, 1, 1, file);
            // write noteOn
            noteData[0] = 0x90;
            noteData[1] = mData->note[i+1];
            noteData[2] = 0x7F;
            fwrite(noteData, 1, 3, file);
        }
    }
    // write last noteOff event
    {
        fwrite(&mData->length[count-1], 2, 1, file);	// deltaTime
        unsigned char lastNoteOff[] = {0x80, mData->note[count-1], 0x0};	// noteOff
        fwrite(lastNoteOff, 1, 3, file);
    }
    // write end track
    {
        unsigned char endOfTrack[] = {0x0, 0xFF, 0x2F, 0x0};	// varLen8 + endOfTrack
        fwrite(endOfTrack, 1, 4, file);
    }
    fflush(file);
    if (fclose(file)) return false;

    return true;
}

bool fillRandomizedBuffers(unsigned char* note, VarLength* length, unsigned int size)
{
    long seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine rnd(seed);
    std::uniform_int_distribution<unsigned char> distrNote(0x24, 0x53); // [0, 11]
    std::uniform_int_distribution<unsigned short> distrLen(0, 2); // [0, 2]

    // save generated data in a file
    FILE* file;
#ifdef LINUX
    if ((file = fopen("MidiData/midiData.txt", "w")) == nullptr) return false;
#else
    if (fopen_s(&file, "midiFile.txt", "w")) return false;
#endif

    for (auto i = 0u; i < size; ++i){
        unsigned char nt = distrNote(rnd);
        unsigned short len = static_cast<unsigned short>(divUnit << distrLen(rnd));

        note[i] = nt;
        length[i] = cookVarLength(len);

        // using c++
//        unsigned short oct = ((nt - 26) / 12);
//        nt %= 12;

        // using inline asm
        unsigned short oct;
        std::tie(nt, oct) = calcNoteOct(static_cast<unsigned short>(nt));

        fprintf(file, "%d %d %d\n", nt, oct, len);
    }

    if (fclose(file)) return false;

    return true;
}

unsigned bswap_dword(unsigned a)
{
    asm (
    "bswap %0 \n\t"
    : "=r"(a)
    : "0"(a)
    //: clobbered regs
    );
    return a;
}
/*
unsigned short bswap_word(unsigned short a)
{
    asm (
    "mov %0, %%ax \n\t"
    "xchg %%al, %%ah \n\t"
    "mov %%ax, %0 \n\t"
    : "=r"(a)
    : "0"(a)
    : "ax"
    );
    return a;
}
*/
unsigned short bswap_word(unsigned short a)
{
    asm (
    "xchg %%al, %%ah \n\t"
    : "=a"(a)
    : "0"(a)
    :
    );
    return a;
}

VarLength cookVarLength(unsigned short len)
{
    VarLength vLen;
    vLen.word = bswap_word(len);
    vLen.word = static_cast<unsigned short>(vLen.word << 1);
    vLen.bytes[0] = vLen.bytes[0] >> 1;
    vLen.bytes[0] |= 0x80;
    vLen.bytes[1] &= 0x7F;
    return vLen;
}

std::pair<unsigned char, unsigned short> calcNoteOct(unsigned short note)
{
    unsigned short nt = 0, oct = 0;
    asm(
        "sub $0x24, %%ax \n\t"
        "cdq \n\t"
        "div %3 \n\t"
        "add $0x1, %%ax \n\t"
    :	"=a"(oct), "=d"(nt)
    :	"a"(note), "c"(0xC)
    :	"cc"
    );
    return std::make_pair<unsigned char, unsigned short>(static_cast<unsigned char>(nt), std::move(oct)); //RWO
}
