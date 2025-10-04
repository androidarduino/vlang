#ifndef STDIO_H
#define STDIO_H

// 简化的 stdio.h 实现
// 使用系统调用直接实现，避免依赖 glibc

#define NULL ((void *)0)
#define EOF (-1)

// 系统调用号 (x86-64 Linux)
#define SYS_read 0
#define SYS_write 1
#define SYS_open 2
#define SYS_close 3
#define SYS_exit 60

// 标准文件描述符
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// 基础 I/O 函数
int putchar(int c);
int puts(const char *s);

// 完整 printf 实现
// 支持格式: %d, %i, %u, %x, %X, %o, %c, %s, %p, %f, %ld, %lu, %lld, %%
int printf(const char *format, ...);

// sprintf (简化版)
int sprintf(char *str, const char *format, ...);

#endif
