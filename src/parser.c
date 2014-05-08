#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tailq.h"
#include "parser.h"
#include "textmmap.h"

void
populate_ref(struct tailq *queue, struct tailq_node *node)
{
    struct file* f = node->data;
    char* buff = mmap_file(f->fname);
    char ref[MAX_LINE];
    for (int i = 0; buff[i]; i++) {
        if (strncmp(buff+i, "<a href=", 8) == 0)
        {
            i += 8 + 1;
            int length, j = i;
            while (buff[j] && buff[j] != '\'' && buff[j] != '"')
                j++;
            if ((length = j-i) >= MAX_LINE)
                continue; // skip long links
            strncpy(ref, buff+i, length);
            ref[length] = '\0';
            link_add(f->ref_to, ref);
            attempt_link_add(queue, ref, f->fname);
        }
    }
}

#ifdef __cplusplus
}
#endif