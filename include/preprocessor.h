#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>

// 宏定义结构
typedef struct MacroDefinition
{
    char *name;       // 宏名称
    char *value;      // 宏值
    char **params;    // 参数列表（函数宏）
    int num_params;   // 参数数量
    int is_function;  // 是否为函数宏
    int is_variadic;  // 是否为可变参数宏
} MacroDefinition;

// 预处理器结构
typedef struct Preprocessor
{
    char *input;                  // 输入源代码
    char *output;                 // 预处理后的代码
    int output_size;              // 输出缓冲区大小
    int output_pos;               // 当前输出位置
    int line_number;              // 当前行号
    char **include_paths;         // include路径列表
    int num_include_paths;        // include路径数量
    MacroDefinition *macros;      // 宏定义列表
    int num_macros;               // 宏定义数量
    int macro_capacity;           // 宏定义容量
    const char *current_filename; // 当前文件名
} Preprocessor;

// 函数声明
Preprocessor *preprocessor_create(void);
void preprocessor_free(Preprocessor *pp);
void preprocessor_add_include_path(Preprocessor *pp, const char *path);
void preprocessor_define_macro(Preprocessor *pp, const char *name, const char *value);
void preprocessor_undef_macro(Preprocessor *pp, const char *name);
int preprocessor_is_defined(Preprocessor *pp, const char *name);
const char *preprocessor_get_macro_value(Preprocessor *pp, const char *name);
char *preprocessor_process(Preprocessor *pp, const char *input, const char *filename);
char *read_file_content(const char *filename);

#endif // PREPROCESSOR_H
