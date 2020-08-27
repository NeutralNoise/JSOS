#ifndef VA_DEFS_H_INCLUDED
#define VA_DEFS_H_INCLUDED

typedef char * va_list;

#define __CRT_VA_START(ap, l) ap = (char*)&l + sizeof(l)
#define __CRT_VA_ARG(ap, type) *(type*)((ap += sizeof(int)) - sizeof(int))
#define __CRT_VA_END(ap) ap = nullptr


#endif //VA_DEFS_H_INCLUDED