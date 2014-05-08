#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include "tailq.h"
#include "parser.h"

char*
mmap_file(char *infile)
{
    int f;
    char *c;
    struct stat sb;

    assert(infile);
    
    // Open the file as read only
    if ((f = open(infile, O_RDONLY)) < 0)
        printf("Error opening file '%s'\n", infile);
    
    // Read the file length, write to the stat buffer
    fstat(f, &sb);
    
    // Mem map the text to char* c
    if ((c = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, f, 0)) == MAP_FAILED) {
        return NULL;
    }
    
    return c;
}

#ifdef __cplusplus
}
#endif
