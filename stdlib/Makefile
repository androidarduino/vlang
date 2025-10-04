# 简化标准库 Makefile

CC = gcc
AR = ar
CFLAGS = -Wall -O2 -fno-builtin -nostdlib -fPIC

# 目标文件
OBJS = stdio.o string.o stdlib.o

# 库文件
LIB = libmini.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rcs $@ $(OBJS)
	@echo "✓ 标准库已生成: $(LIB)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)
	@echo "✓ 清理完成"

.PHONY: all clean

