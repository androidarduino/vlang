// 完善的 stdio 实现
// 使用 GCC 内置的 stdarg 支持

#include "stdio.h"
#include "string.h"

// 使用 GCC 内置的 va_list
typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

// 系统调用封装 (x86-64 Linux)
static long syscall3(long n, long a1, long a2, long a3)
{
    long ret;
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory");
    return ret;
}

// putchar - 输出单个字符
int putchar(int c)
{
    char ch = (char)c;
    syscall3(SYS_write, STDOUT_FILENO, (long)&ch, 1);
    return c;
}

// puts - 输出字符串
int puts(const char *s)
{
    if (!s)
        return EOF;
    int len = strlen(s);
    syscall3(SYS_write, STDOUT_FILENO, (long)s, len);
    putchar('\n');
    return len + 1;
}

// ============================================================
// printf 辅助函数
// ============================================================

// 整数转字符串（支持不同进制）
static int int_to_str(long long num, char *buf, int base, int is_unsigned)
{
    char digits[] = "0123456789abcdef";
    int i = 0;
    int negative = 0;

    if (num == 0)
    {
        buf[i++] = '0';
        return i;
    }

    if (!is_unsigned && num < 0)
    {
        negative = 1;
        num = -num;
    }

    // 转换为字符（逆序）
    unsigned long long unum = (unsigned long long)num;
    while (unum > 0)
    {
        buf[i++] = digits[unum % base];
        unum = unum / base;
    }

    if (negative)
    {
        buf[i++] = '-';
    }

    // 反转
    for (int j = 0; j < i / 2; j++)
    {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }

    return i;
}

// 浮点数转字符串（简化版）
static int float_to_str(double num, char *buf, int precision)
{
    int i = 0;

    // 处理符号
    if (num < 0)
    {
        buf[i++] = '-';
        num = -num;
    }

    // 整数部分
    long long int_part = (long long)num;
    char tmp[32];
    int len = int_to_str(int_part, tmp, 10, 0);
    for (int j = 0; j < len; j++)
    {
        buf[i++] = tmp[j];
    }

    // 小数点
    buf[i++] = '.';

    // 小数部分
    double frac = num - (double)int_part;
    for (int j = 0; j < precision; j++)
    {
        frac = frac * 10;
        int digit = (int)frac;
        buf[i++] = '0' + digit;
        frac = frac - digit;
    }

    return i;
}

// ============================================================
// 完整的 printf 实现（使用 stdarg）
// 支持: %d, %i, %u, %x, %X, %o, %c, %s, %p, %f, %ld, %lu, %lld, %%
// ============================================================

int printf(const char *format, ...)
{
    if (!format)
        return 0;

    va_list args;
    va_start(args, format);

    int count = 0;
    const char *p = format;
    char buf[128];

    while (*p)
    {
        if (*p == '%' && *(p + 1))
        {
            p++;

            // 解析标志
            int width = 0;
            int precision = 6; // 默认精度
            int is_long = 0;
            int is_long_long = 0;

            // 解析宽度
            while (*p >= '0' && *p <= '9')
            {
                width = width * 10 + (*p - '0');
                p++;
            }

            // 解析精度
            if (*p == '.')
            {
                p++;
                precision = 0;
                while (*p >= '0' && *p <= '9')
                {
                    precision = precision * 10 + (*p - '0');
                    p++;
                }
            }

            // 解析长度修饰符
            if (*p == 'l')
            {
                p++;
                is_long = 1;
                if (*p == 'l')
                {
                    p++;
                    is_long_long = 1;
                }
            }

            // 格式化类型
            if (*p == 'd' || *p == 'i')
            {
                // 有符号整数
                long long num;
                if (is_long_long)
                {
                    num = va_arg(args, long long);
                }
                else if (is_long)
                {
                    num = va_arg(args, long);
                }
                else
                {
                    num = va_arg(args, int);
                }

                int len = int_to_str(num, buf, 10, 0);
                for (int i = 0; i < len; i++)
                {
                    putchar(buf[i]);
                    count++;
                }
            }
            else if (*p == 'u')
            {
                // 无符号整数
                unsigned long long num;
                if (is_long_long)
                {
                    num = va_arg(args, unsigned long long);
                }
                else if (is_long)
                {
                    num = va_arg(args, unsigned long);
                }
                else
                {
                    num = va_arg(args, unsigned int);
                }

                int len = int_to_str(num, buf, 10, 1);
                for (int i = 0; i < len; i++)
                {
                    putchar(buf[i]);
                    count++;
                }
            }
            else if (*p == 'x' || *p == 'X')
            {
                // 十六进制
                unsigned long num = va_arg(args, unsigned long);
                int len = int_to_str(num, buf, 16, 1);
                for (int i = 0; i < len; i++)
                {
                    putchar(buf[i]);
                    count++;
                }
            }
            else if (*p == 'o')
            {
                // 八进制
                unsigned long num = va_arg(args, unsigned long);
                int len = int_to_str(num, buf, 8, 1);
                for (int i = 0; i < len; i++)
                {
                    putchar(buf[i]);
                    count++;
                }
            }
            else if (*p == 'c')
            {
                // 字符
                char c = (char)va_arg(args, int);
                putchar(c);
                count++;
            }
            else if (*p == 's')
            {
                // 字符串
                const char *s = va_arg(args, const char *);
                if (s)
                {
                    while (*s)
                    {
                        putchar(*s++);
                        count++;
                    }
                }
                else
                {
                    const char *null_str = "(null)";
                    while (*null_str)
                    {
                        putchar(*null_str++);
                        count++;
                    }
                }
            }
            else if (*p == 'p')
            {
                // 指针
                unsigned long ptr = (unsigned long)va_arg(args, void *);
                putchar('0');
                putchar('x');
                count += 2;
                int len = int_to_str(ptr, buf, 16, 1);
                for (int i = 0; i < len; i++)
                {
                    putchar(buf[i]);
                    count++;
                }
            }
            else if (*p == 'f')
            {
                // 浮点数
                double num = va_arg(args, double);

                int len = float_to_str(num, buf, precision);
                for (int i = 0; i < len; i++)
                {
                    putchar(buf[i]);
                    count++;
                }
            }
            else if (*p == '%')
            {
                // %%
                putchar('%');
                count++;
            }
            else
            {
                // 未知格式，原样输出
                putchar('%');
                putchar(*p);
                count += 2;
            }

            p++;
        }
        else
        {
            putchar(*p++);
            count++;
        }
    }

    va_end(args);
    return count;
}
