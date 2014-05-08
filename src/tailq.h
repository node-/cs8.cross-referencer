// tailq.h
#ifndef _TAILQ_H_
#define _TAILQ_H_

struct tailq_node {
    void *data;
    struct tailq_node *next;
};

struct tailq {
    struct tailq_node *head;
    struct tailq_node *tail;
};

struct file {
    char *fname;
    struct tailq *ref_from;
    struct tailq *ref_to;
};

void tailq_insert(struct tailq *queue, struct tailq_node *node);
struct tailq_node* tailq_search(struct tailq *queue, char *s);

void print_results(struct tailq *queue);
void oom_exit(const char *file, int line);
void attempt_link_add(struct tailq *queue, char *from, char *to);
void link_add(struct tailq *queue, char *linkname);
void file_add(struct tailq *queue, char *filename);

#endif // _TAILQ_H_