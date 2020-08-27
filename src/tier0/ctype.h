#ifndef __CTYPE_H_INCLUDED
#define __CTYPE_H_INCLUDED

int isspace(int c) {
    return c == ' ' || (unsigned)c-'\t' < 5;
}

int isdigit(int c) {
    return (unsigned)c-'0' < 10;
}

#endif //__CTYPE_H_INCLUDED