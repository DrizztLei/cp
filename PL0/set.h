//
// Created by elvis on 11/28/17.
//

#ifndef PL0_SET_H
#define PL0_SET_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

typedef struct snode
{
    int elem;
    struct snode *next;
} snode, *symset;

symset phi, declbegsys, statbegsys, facbegsys, relset;

symset createset(int data, .../* SYM_NULL */);

void destroyset(symset s);
symset uniteset(symset s1, symset s2);
int inset(int elem, symset s);

#endif //PL0_SET_H