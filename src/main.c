/* HTML Cross Referencer
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "tailq.h"

void extract_files(struct tailq *queue, char *infile);
void take_input(const char *query, char *infile);
char* mmap_file(char *infile);

int
main(int argc, char **argv)
{
    char infile[MAX_FILENAME];
    struct tailq        queue;
    struct tailq_node   *node;
    
    take_input("Input file: ", infile);
    extract_files(&queue, infile);
    
    node = queue.head;
    for (int i=0; node; i++) {
        populate_ref(&queue, node);
        node = node->next;
    }
    print_results(&queue);
    
    return 0;
}

void
take_input(const char *query, char *infile)
{
    unsigned long infileLen;
    printf("%s", query);
    
    if (!fgets(infile, MAX_FILENAME, stdin)) {
        printf("Error taking input\n");
        exit(-1);
    }
     
    infileLen = strlen(infile);
    if (infile[infileLen-1] == '\n')
        infile[infileLen-1] = '\0';
}

void
extract_files(struct tailq *queue, char *infile)
{
    char html_fname[MAX_FILENAME];
    char *file = mmap_file(infile);
    int newLineIndex = 0, length = 0;
    unsigned long fileLen = strlen(file);
    
    // Read each line into the queue
    for (int i = 0; i<fileLen+1; i++) {
        if (file[i] == '\n' || file[i] == '\0') {
            length = i-newLineIndex;
            strncpy(html_fname, file+newLineIndex, length);
            html_fname[length] = '\0';
            file_add(queue, html_fname);
            newLineIndex = i+1;
        }
    }
}