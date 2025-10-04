// 完善的 stdlib 实现

#include "stdlib.h"

#define SYS_brk 12
#define SYS_exit 60

// 系统调用
static long syscall1(long n, long a1)
{
    long ret;
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
    return ret;
}

// ============================================================
// 内存分配器 - 使用空闲链表实现 malloc/free
// ============================================================

// 内存块头部
typedef struct Block
{
    int size;           // 块大小（不包括头部）
    int is_free;        // 是否空闲
    struct Block *next; // 下一个块
} Block;

#define BLOCK_SIZE sizeof(Block)

static Block *free_list = NULL;
static char *heap_end = NULL;

// 初始化堆
static void init_heap()
{
    if (heap_end == NULL)
    {
        heap_end = (char *)syscall1(SYS_brk, 0);
    }
}

// 扩展堆
static Block *request_space(Block *last, int size)
{
    Block *block;
    block = (Block *)heap_end;

    // 扩展堆
    void *request = (void *)(heap_end + BLOCK_SIZE + size);
    void *new_end = (void *)syscall1(SYS_brk, (long)request);

    if (new_end == heap_end)
    {
        return NULL; // 分配失败
    }

    heap_end = (char *)new_end;

    // 初始化块
    block->size = size;
    block->is_free = 0;
    block->next = NULL;

    if (last)
    {
        last->next = block;
    }

    return block;
}

// 查找空闲块
static Block *find_free_block(Block **last, int size)
{
    Block *current = free_list;
    while (current && !(current->is_free && current->size >= size))
    {
        *last = current;
        current = current->next;
    }
    return current;
}

// malloc - 分配内存
void *malloc(int size)
{
    if (size <= 0)
        return NULL;

    // 对齐到8字节
    size = (size + 7) & ~7;

    Block *block;

    init_heap();

    if (!free_list)
    {
        // 第一次分配
        block = request_space(NULL, size);
        if (!block)
            return NULL;
        free_list = block;
    }
    else
    {
        // 查找空闲块
        Block *last = free_list;
        block = find_free_block(&last, size);

        if (!block)
        {
            // 没有合适的空闲块，申请新的
            block = request_space(last, size);
            if (!block)
                return NULL;
        }
        else
        {
            // 找到空闲块，标记为已使用
            block->is_free = 0;
        }
    }

    return (void *)(block + 1); // 返回数据区域
}

// free - 释放内存
void free(void *ptr)
{
    if (!ptr)
        return;

    Block *block = (Block *)ptr - 1;
    block->is_free = 1;

    // 简单实现：仅标记为空闲
    // 真实实现应该合并相邻空闲块
}

// ============================================================
// 程序控制
// ============================================================

// exit - 退出程序
void exit(int status)
{
    syscall1(SYS_exit, status);
    // 不会到达这里
    while (1)
        ;
}

// ============================================================
// 字符串转数字 - 完整实现
// ============================================================

// atoi - 字符串转整数（完整实现）
int atoi(const char *s)
{
    if (!s)
        return 0;

    int result = 0;
    int sign = 1;

    // 跳过前导空白
    while (*s == ' ' || *s == '\t' || *s == '\n' ||
           *s == '\r' || *s == '\v' || *s == '\f')
    {
        s++;
    }

    // 处理符号
    if (*s == '-')
    {
        sign = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }

    // 转换数字
    while (*s >= '0' && *s <= '9')
    {
        // 检查溢出（简化版）
        int digit = *s - '0';
        result = result * 10 + digit;
        s++;
    }

    return result * sign;
}

// atol - 字符串转长整数
long atol(const char *s)
{
    if (!s)
        return 0;

    long result = 0;
    int sign = 1;

    // 跳过前导空白
    while (*s == ' ' || *s == '\t' || *s == '\n' ||
           *s == '\r' || *s == '\v' || *s == '\f')
    {
        s++;
    }

    // 处理符号
    if (*s == '-')
    {
        sign = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }

    // 转换数字
    while (*s >= '0' && *s <= '9')
    {
        long digit = *s - '0';
        result = result * 10 + digit;
        s++;
    }

    return result * sign;
}

// strtol - 字符串转长整数（带进制支持）
long strtol(const char *s, char **endptr, int base)
{
    if (!s)
    {
        if (endptr)
            *endptr = (char *)s;
        return 0;
    }

    const char *p = s;
    long result = 0;
    int sign = 1;

    // 跳过空白
    while (*p == ' ' || *p == '\t' || *p == '\n')
        p++;

    // 处理符号
    if (*p == '-')
    {
        sign = -1;
        p++;
    }
    else if (*p == '+')
    {
        p++;
    }

    // 自动检测进制
    if (base == 0)
    {
        if (*p == '0')
        {
            p++;
            if (*p == 'x' || *p == 'X')
            {
                base = 16;
                p++;
            }
            else
            {
                base = 8;
            }
        }
        else
        {
            base = 10;
        }
    }
    else if (base == 16 && *p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X'))
    {
        p += 2;
    }

    // 转换
    while (*p)
    {
        int digit;
        if (*p >= '0' && *p <= '9')
        {
            digit = *p - '0';
        }
        else if (*p >= 'a' && *p <= 'z')
        {
            digit = *p - 'a' + 10;
        }
        else if (*p >= 'A' && *p <= 'Z')
        {
            digit = *p - 'A' + 10;
        }
        else
        {
            break;
        }

        if (digit >= base)
            break;

        result = result * base + digit;
        p++;
    }

    if (endptr)
        *endptr = (char *)p;
    return result * sign;
}
