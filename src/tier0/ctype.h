#ifndef __CTYPE_H_INCLUDED
#define __CTYPE_H_INCLUDED

int isspace(int c) {
    return c == ' ' || (unsigned)c-'\t' < 5;
}

int isdigit(int c) {
    return (unsigned)c-'0' < 10;
}

int isalpha(int c)
{
	return ((unsigned)c|32)-'a' < 26;
}

#endif //__CTYPE_H_INCLUDED