#ifdef __cplusplus
//extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tailq.h"

void
tailq_insert(struct tailq *queue, struct tailq_node *node)
{
    assert(queue);
    assert(node);
    
    if (queue->tail == NULL) {
        queue->head = queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
}
    
void
tailq_remove(struct tailq *queue, struct tailq_node *node)
{
    struct tailq_node *prev_node = tailq_prev(queue, node);
    
    if (node == queue->head)
        queue->head = node->next;
    
    if (node == queue->tail)
        queue->tail = prev_node;
    
    if (prev_node)
        prev_node->next = node->next;
    
    free(node);
}

struct tailq_node*
tailq_prev(struct tailq *queue, struct tailq_node *node)
{
    struct tailq_node *nodefind = queue->head;
    
    for (; nodefind; nodefind = nodefind->next) {
        if (nodefind->next && node == nodefind->next)
            return nodefind;
    }
    return NULL;
}

struct tailq_node*
tailq_search_str(struct tailq *queue, char *s)
{
    struct tailq_node *node = queue->head;
    
    for (; node; node = node->next) {
        struct file *f = node->data;
        unsigned long len = strlen(f->fname);
        if (strncmp(f->fname, s, len) == 0)
            return node;
    }
    return NULL;
}

void
print_results(struct tailq *queue)
{
    struct tailq_node *to_node, *from_node, *node = queue->head;
    struct file *f;
    for (; node; node = node->next) {
        f = node->data;
        to_node = f->ref_to->head;
        from_node = f->ref_from->head;
        
        printf("%s refers to: \n", f->fname);
        for (; to_node; to_node = to_node->next) {
            char* s = to_node->data;
            printf("    %s\n", s);
        }
        
        printf("%s is referred to by: \n", f->fname);
        for (; from_node; from_node = from_node->next) {
            char* s = from_node->data;
            printf("    %s\n", s);
        }
        printf("\n");
    }
}

void
oom_exit(const char *file, int line)
{
    fprintf(stderr, "Out of memory at %s:%d\n", file, line);
    exit(-1);
}

int
attempt_link_add(struct tailq *queue, char *to, char *from)
{
    struct tailq_node* node = tailq_search_str(queue, to);
    if (!node)
        node = file_add(queue, to);
    struct file *f = node->data;
    link_add(f->ref_from, from);
    return 1;
}

struct tailq_node*
link_add(struct tailq *queue, char *linkname)
{
    struct tailq_node *node;
    
    assert(queue);
    assert(linkname);
    
    node = calloc(1, sizeof(*node));
    if (node == NULL) {
        oom_exit(__FILE__, __LINE__);
    }
    
    node->data = strdup(linkname);
    tailq_insert(queue, node);
    return node;
}


struct tailq_node*
file_add(struct tailq *queue, char *filename)
{
    struct tailq_node *node;
    struct tailq *ref_from, *ref_to;
    struct file *f;
    
    assert(queue);
    assert(filename);
    
    node = calloc(1, sizeof(*node));
    if (node == NULL) {
        oom_exit(__FILE__, __LINE__);
    }
    
    ref_from = calloc(1, sizeof(*ref_from));
    if (ref_from == NULL) {
        oom_exit(__FILE__, __LINE__);
    }
    
    ref_to = calloc(1, sizeof(*ref_to));
    if (ref_to == NULL) {
        oom_exit(__FILE__, __LINE__);
    }
    
    f = calloc(1, sizeof(*f));
    if (f == NULL) {
        oom_exit(__FILE__, __LINE__);
    }
    
    f->fname = strdup(filename);
    f->ref_from = ref_from;
    f->ref_to = ref_to;
    
    node->data = f;
    tailq_insert(queue, node);
    return node;
}

#ifdef __cplusplus
//}
#endif