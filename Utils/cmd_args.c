#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_FILE_SIZE 0x100

typedef struct Init
{
    char filename[MAX_FILE_SIZE];
    // ...
} Init;

int parse_args(int argc, char **argv, Init *init);
void help();
void default_init();

int main(int argc, char** argv)
{


    return EXIT_SUCCESS;
}

int parse_args(int argc, char **argv, Init *init)
{
    if (argc < 2) {
        default_init();
        return 0;
        // or
        // puts("Not enough arguments provided..exiting.");
        // return -1;
    }
    if (argc == 2 && strcmp(argv[1], "--help") || strcmp(argv[1], "-h")) {
        help();
        return 0;
    }
    for (uint8_t i = 1; i < argc; i+=2) {
        if (argv[i][0] != '-') break;
        if (argv[i+1][0] == '-') break;
        if (strcmp(argv[i], "-o")) {
            if (strlen(argv[i+1]) > MAX_FILE_SIZE) {
                puts("Filename exceeding maximum permitted size...exiting.");
                return -1;
            }
            strcpy(init->filename, argv[i+1]);
        }  
        // ...
        
        return 0;
    }
    
    puts("Error occured while processing arguments.. exiting.")
    return -1;
}
