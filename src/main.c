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
char* mmap_file(char *infile);

int
main(int argc, char **argv)
{
    char infile[MAX_FILENAME];
    struct tailq        queue;
    struct tailq_node   *node;
    
    if (argc == 1) {
        file_add(&queue, "index.html");
    } else if (argc == 2) {
        strncpy(infile, argv[1], MAX_FILENAME-1);
        extract_files(&queue, infile);
    } else {
        printf("usage: ./cr [in file]\n");
        exit(-1);
    }
    
    node = queue.head;
    for (int i = 0; node; i++) {
        if (!populate_ref(&queue, node))
            tailq_remove(&queue, node);
        node = node->next;
    }
    print_results(&queue);
    
    getchar();
    printf("Bye!\n");
    
    return 0;
}

void
extract_files(struct tailq *queue, char *infile)
{
    char html_fname[MAX_FILENAME];
    char *file;
    int newLineIndex = 0, length = 0;
    unsigned long fileLen;
    
    file = mmap_file(infile);
    
    if (!file)
        return;
    
    fileLen = strlen(file);
    
    // Read each line into the queue
    for (int i = 0; i<fileLen+1; i++) {
        if (file[i] == '\n' || (i > 0 && file[i] == '\0' && file[i-1] != '\n')) {
            length = i-newLineIndex;
            strncpy(html_fname, file+newLineIndex, length);
            html_fname[length] = '\0';
            file_add(queue, html_fname);
            newLineIndex = i+1;
        }
    }
}
