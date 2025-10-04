#ifndef STRING_H
#define STRING_H

#define NULL ((void*)0)

// 字符串操作
int strlen(const char *s);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);

// 内存操作
void *memset(void *s, int c, int n);
void *memcpy(void *dest, const void *src, int n);

#endif

