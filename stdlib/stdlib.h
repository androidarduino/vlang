#ifndef STDLIB_H
#define STDLIB_H

#define NULL ((void *)0)

// 内存分配
void *malloc(int size);
void free(void *ptr);

// 程序控制
void exit(int status);

// 数值转换
int atoi(const char *s);
long atol(const char *s);
long strtol(const char *s, char **endptr, int base);

#endif
