// 简化的 string 实现

#include "string.h"

// strlen - 计算字符串长度
int strlen(const char *s) {
    if (!s) return 0;
    int len = 0;
    while (*s++) len++;
    return len;
}

// strcmp - 比较字符串
int strcmp(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

// strcpy - 复制字符串
char *strcpy(char *dest, const char *src) {
    if (!dest || !src) return dest;
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

// strcat - 连接字符串
char *strcat(char *dest, const char *src) {
    if (!dest || !src) return dest;
    char *d = dest;
    while (*d) d++;
    while ((*d++ = *src++));
    return dest;
}

// memset - 设置内存
void *memset(void *s, int c, int n) {
    if (!s) return s;
    unsigned char *p = (unsigned char*)s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}

// memcpy - 复制内存
void *memcpy(void *dest, const void *src, int n) {
    if (!dest || !src) return dest;
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;
    while (n--) *d++ = *s++;
    return dest;
}

