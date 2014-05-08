// parser.h
#ifndef _PARSER_H_
#define _PARSER_H_

#define MAX_FILENAME 256
#define MAX_LINE 1024

#include "tailq.h"

int
populate_ref(struct tailq *queue, struct tailq_node *node);

#endif // _PARSER_H_