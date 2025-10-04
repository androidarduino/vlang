#include "preprocessor.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_OUTPUT_SIZE 4096
#define MAX_INCLUDE_DEPTH 10

// 全局变量：当前include深度
static int include_depth = 0;

// 创建预处理器
Preprocessor *preprocessor_create(void)
{
    Preprocessor *pp = (Preprocessor *)malloc(sizeof(Preprocessor));
    if (!pp)
        return NULL;

    pp->input = NULL;
    pp->output = (char *)malloc(INITIAL_OUTPUT_SIZE);
    if (!pp->output)
    {
        free(pp);
        return NULL;
    }

    pp->output_size = INITIAL_OUTPUT_SIZE;
    pp->output_pos = 0;
    pp->line_number = 1;
    pp->include_paths = NULL;
    pp->num_include_paths = 0;
    pp->macros = NULL;
    pp->num_macros = 0;
    pp->macro_capacity = 0;
    pp->current_filename = NULL;

    // 添加默认include路径
    preprocessor_add_include_path(pp, ".");
    preprocessor_add_include_path(pp, "/usr/include");

    // 添加预定义宏
    preprocessor_define_macro(pp, "__STDC__", "1");
    preprocessor_define_macro(pp, "__STDC_VERSION__", "199901L");
    preprocessor_define_macro(pp, "__LINE__", "0");             // 动态更新
    preprocessor_define_macro(pp, "__FILE__", "\"<unknown>\""); // 动态更新

    return pp;
}

// 释放预处理器
void preprocessor_free(Preprocessor *pp)
{
    if (!pp)
        return;

    if (pp->output)
        free(pp->output);

    if (pp->include_paths)
    {
        for (int i = 0; i < pp->num_include_paths; i++)
        {
            if (pp->include_paths[i])
                free(pp->include_paths[i]);
        }
        free(pp->include_paths);
    }

    if (pp->macros)
    {
        for (int i = 0; i < pp->num_macros; i++)
        {
            if (pp->macros[i].name)
                free(pp->macros[i].name);
            if (pp->macros[i].value)
                free(pp->macros[i].value);
            if (pp->macros[i].params)
            {
                for (int j = 0; j < pp->macros[i].num_params; j++)
                {
                    if (pp->macros[i].params[j])
                        free(pp->macros[i].params[j]);
                }
                free(pp->macros[i].params);
            }
        }
        free(pp->macros);
    }

    free(pp);
}

// 添加include路径
void preprocessor_add_include_path(Preprocessor *pp, const char *path)
{
    if (!pp || !path)
        return;

    pp->include_paths = (char **)realloc(pp->include_paths,
                                         (pp->num_include_paths + 1) * sizeof(char *));
    pp->include_paths[pp->num_include_paths] = strdup(path);
    pp->num_include_paths++;
}

// 定义宏
void preprocessor_define_macro(Preprocessor *pp, const char *name, const char *value)
{
    if (!pp || !name)
        return;

    // 检查是否已存在，如果存在则更新
    for (int i = 0; i < pp->num_macros; i++)
    {
        if (strcmp(pp->macros[i].name, name) == 0)
        {
            if (pp->macros[i].value)
                free(pp->macros[i].value);
            pp->macros[i].value = value ? strdup(value) : strdup("");
            return;
        }
    }

    // 扩容
    if (pp->num_macros >= pp->macro_capacity)
    {
        pp->macro_capacity = pp->macro_capacity == 0 ? 16 : pp->macro_capacity * 2;
        pp->macros = (MacroDefinition *)realloc(pp->macros,
                                                pp->macro_capacity * sizeof(MacroDefinition));
    }

    // 添加新宏
    pp->macros[pp->num_macros].name = strdup(name);
    pp->macros[pp->num_macros].value = value ? strdup(value) : strdup("");
    pp->macros[pp->num_macros].params = NULL;
    pp->macros[pp->num_macros].num_params = 0;
    pp->macros[pp->num_macros].is_function = 0;
    pp->macros[pp->num_macros].is_variadic = 0;
    pp->num_macros++;
}

// 取消宏定义
void preprocessor_undef_macro(Preprocessor *pp, const char *name)
{
    if (!pp || !name)
        return;

    for (int i = 0; i < pp->num_macros; i++)
    {
        if (strcmp(pp->macros[i].name, name) == 0)
        {
            // 释放内存
            if (pp->macros[i].name)
                free(pp->macros[i].name);
            if (pp->macros[i].value)
                free(pp->macros[i].value);

            // 移动后面的元素
            for (int j = i; j < pp->num_macros - 1; j++)
            {
                pp->macros[j] = pp->macros[j + 1];
            }
            pp->num_macros--;
            return;
        }
    }
}

// 检查宏是否已定义
int preprocessor_is_defined(Preprocessor *pp, const char *name)
{
    if (!pp || !name)
        return 0;

    for (int i = 0; i < pp->num_macros; i++)
    {
        if (strcmp(pp->macros[i].name, name) == 0)
            return 1;
    }
    return 0;
}

// 获取宏的值
const char *preprocessor_get_macro_value(Preprocessor *pp, const char *name)
{
    if (!pp || !name)
        return NULL;

    for (int i = 0; i < pp->num_macros; i++)
    {
        if (strcmp(pp->macros[i].name, name) == 0)
            return pp->macros[i].value;
    }
    return NULL;
}

// 输出字符
static void output_char(Preprocessor *pp, char c)
{
    if (pp->output_pos >= pp->output_size - 1)
    {
        pp->output_size *= 2;
        pp->output = (char *)realloc(pp->output, pp->output_size);
    }
    pp->output[pp->output_pos++] = c;
}

// 输出字符串
static void output_string(Preprocessor *pp, const char *str)
{
    while (*str)
    {
        output_char(pp, *str++);
    }
}

// 跳过空白字符
static const char *skip_whitespace(const char *p)
{
    while (*p && isspace(*p) && *p != '\n')
        p++;
    return p;
}

// 读取文件内容
char *read_file_content(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *content = (char *)malloc(size + 1);
    if (!content)
    {
        fclose(fp);
        return NULL;
    }

    fread(content, 1, size, fp);
    content[size] = '\0';
    fclose(fp);

    return content;
}

// 查找include文件
static char *find_include_file(Preprocessor *pp, const char *filename, int use_quotes)
{
    char path[1024];

    // 如果是 "file.h"，先在当前目录查找
    if (use_quotes)
    {
        snprintf(path, sizeof(path), "./%s", filename);
        FILE *fp = fopen(path, "r");
        if (fp)
        {
            fclose(fp);
            return strdup(path);
        }
    }

    // 在include路径中查找
    for (int i = 0; i < pp->num_include_paths; i++)
    {
        snprintf(path, sizeof(path), "%s/%s", pp->include_paths[i], filename);
        FILE *fp = fopen(path, "r");
        if (fp)
        {
            fclose(fp);
            return strdup(path);
        }
    }

    return NULL;
}

// 处理 #include 指令
static const char *process_include(Preprocessor *pp, const char *line)
{
    const char *p = line + 8; // 跳过 "#include"
    p = skip_whitespace(p);

    if (*p != '"' && *p != '<')
    {
        fprintf(stderr, "Preprocessor error: Invalid #include directive\n");
        return NULL;
    }

    char filename[256];
    int use_quotes = (*p == '"');
    char end_char = use_quotes ? '"' : '>';
    p++; // 跳过开始引号

    int i = 0;
    while (*p && *p != end_char && i < 255)
    {
        filename[i++] = *p++;
    }
    filename[i] = '\0';

    if (*p != end_char)
    {
        fprintf(stderr, "Preprocessor error: Unclosed #include directive\n");
        return NULL;
    }

    // 检查include深度
    if (include_depth >= MAX_INCLUDE_DEPTH)
    {
        fprintf(stderr, "Preprocessor error: #include nested too deeply\n");
        return NULL;
    }

    // 查找并读取文件
    char *filepath = find_include_file(pp, filename, use_quotes);
    if (!filepath)
    {
        fprintf(stderr, "Preprocessor error: Cannot find file '%s'\n", filename);
        return NULL;
    }

    char *content = read_file_content(filepath);
    free(filepath);

    if (!content)
    {
        fprintf(stderr, "Preprocessor error: Cannot read file '%s'\n", filename);
        return NULL;
    }

    // 递归处理include文件
    include_depth++;
    char *processed = preprocessor_process(pp, content, filename);
    include_depth--;
    free(content);

    if (processed)
    {
        output_string(pp, processed);
        free(processed);
    }

    // 跳到行尾
    while (*p && *p != '\n')
        p++;

    return p;
}

// 计算条件表达式
static int evaluate_condition(Preprocessor *pp, const char *expr)
{
    const char *p = skip_whitespace(expr);

    // 处理 defined() 运算符
    if (strncmp(p, "defined", 7) == 0)
    {
        p += 7;
        p = skip_whitespace(p);

        int has_paren = 0;
        if (*p == '(')
        {
            has_paren = 1;
            p++;
            p = skip_whitespace(p);
        }

        // 提取宏名
        char macro_name[256];
        int i = 0;
        while (*p && (isalnum(*p) || *p == '_') && i < 255)
        {
            macro_name[i++] = *p++;
        }
        macro_name[i] = '\0';

        if (has_paren)
        {
            p = skip_whitespace(p);
            if (*p == ')')
                p++;
        }

        return preprocessor_is_defined(pp, macro_name);
    }

    // 尝试展开宏
    char macro_name[256];
    int i = 0;
    while (*p && (isalnum(*p) || *p == '_') && i < 255)
    {
        macro_name[i++] = *p++;
    }
    macro_name[i] = '\0';

    if (i > 0)
    {
        const char *value = preprocessor_get_macro_value(pp, macro_name);
        if (value)
        {
            // 递归求值宏的值
            return evaluate_condition(pp, value);
        }
    }

    // 数字字面量
    p = skip_whitespace(expr);
    if (*p == '0')
        return 0;
    if (*p == '1')
        return 1;
    if (isdigit(*p))
        return atoi(p) != 0;

    // 默认为真
    return 1;
}

// 处理条件编译
static const char *process_conditional(Preprocessor *pp, const char *p, int *skip_block, int *condition_stack, int *stack_depth)
{
    // #if, #ifdef, #ifndef, #elif, #else, #endif
    if (strncmp(p, "#if ", 4) == 0)
    {
        const char *expr = p + 4;
        int condition = evaluate_condition(pp, expr);

        // 保存当前条件到栈
        if (*stack_depth < 10)
        {
            condition_stack[*stack_depth] = condition;
            (*stack_depth)++;
        }

        *skip_block = !condition;

        // 跳到行尾
        while (*p && *p != '\n')
            p++;
        return p;
    }
    else if (strncmp(p, "#ifdef ", 7) == 0)
    {
        const char *macro_start = p + 7;
        macro_start = skip_whitespace(macro_start);

        char macro_name[256];
        int i = 0;
        while (*macro_start && !isspace(*macro_start) && i < 255)
        {
            macro_name[i++] = *macro_start++;
        }
        macro_name[i] = '\0';

        int condition = preprocessor_is_defined(pp, macro_name);

        if (*stack_depth < 10)
        {
            condition_stack[*stack_depth] = condition;
            (*stack_depth)++;
        }

        *skip_block = !condition;
        while (*p && *p != '\n')
            p++;
        return p;
    }
    else if (strncmp(p, "#ifndef ", 8) == 0)
    {
        const char *macro_start = p + 8;
        macro_start = skip_whitespace(macro_start);

        char macro_name[256];
        int i = 0;
        while (*macro_start && !isspace(*macro_start) && i < 255)
        {
            macro_name[i++] = *macro_start++;
        }
        macro_name[i] = '\0';

        int condition = !preprocessor_is_defined(pp, macro_name);

        if (*stack_depth < 10)
        {
            condition_stack[*stack_depth] = condition;
            (*stack_depth)++;
        }

        *skip_block = !condition;
        while (*p && *p != '\n')
            p++;
        return p;
    }
    else if (strncmp(p, "#elif ", 6) == 0)
    {
        // #elif: 如果前面的条件为假且当前条件为真，则执行
        const char *expr = p + 6;
        int condition = evaluate_condition(pp, expr);

        // 如果前面的条件已经为真，则跳过
        if (*stack_depth > 0 && condition_stack[*stack_depth - 1])
        {
            *skip_block = 1;
        }
        else
        {
            *skip_block = !condition;
            if (*stack_depth > 0)
                condition_stack[*stack_depth - 1] = condition;
        }

        while (*p && *p != '\n')
            p++;
        return p;
    }
    else if (strncmp(p, "#else", 5) == 0)
    {
        // 如果栈中有条件且为真，则跳过else块
        if (*stack_depth > 0 && condition_stack[*stack_depth - 1])
        {
            *skip_block = 1;
        }
        else
        {
            *skip_block = !(*skip_block);
        }

        while (*p && *p != '\n')
            p++;
        return p;
    }
    else if (strncmp(p, "#endif", 6) == 0)
    {
        *skip_block = 0;
        if (*stack_depth > 0)
            (*stack_depth)--;

        while (*p && *p != '\n')
            p++;
        return p;
    }

    return NULL;
}

// 字符串化运算符 # - 将参数转换为字符串
static char *stringify(const char *text)
{
    static char buffer[2048];
    char *out = buffer;
    *out++ = '"';

    const char *p = text;
    while (*p && out - buffer < 2040)
    {
        // 跳过前导和尾随空白
        if (p == text)
        {
            while (*p && isspace(*p))
                p++;
        }

        // 转义特殊字符
        if (*p == '"' || *p == '\\')
        {
            *out++ = '\\';
        }

        if (*p)
        {
            *out++ = *p++;
        }
    }

    // 去除尾部空白
    while (out > buffer + 1 && isspace(*(out - 1)))
    {
        out--;
    }

    *out++ = '"';
    *out = '\0';
    return strdup(buffer);
}

// 连接运算符 ## - 连接两个token
static char *concat_tokens(const char *left, const char *right)
{
    static char buffer[1024];
    char *out = buffer;

    // 复制左侧（去除尾部空白）
    const char *p = left;
    while (*p && out - buffer < 1020)
    {
        *out++ = *p++;
    }
    while (out > buffer && isspace(*(out - 1)))
    {
        out--;
    }

    // 复制右侧（跳过前导空白）
    p = right;
    while (*p && isspace(*p))
        p++;
    while (*p && out - buffer < 1020)
    {
        *out++ = *p++;
    }

    *out = '\0';
    return strdup(buffer);
}

// 处理宏值中的 # 和 ## 运算符
static char *process_macro_operators(const char *value, char **param_names, char **param_values, int num_params, int is_variadic, const char *va_args)
{
    static char buffer[4096];
    char *out = buffer;
    const char *p = value;

    while (*p && out - buffer < 4090)
    {
        // 处理 ## 连接运算符
        if (*p == '#' && *(p + 1) == '#')
        {
            // 跳过 ##
            p += 2;
            p = skip_whitespace(p);

            // 回退到上一个token的末尾
            while (out > buffer && isspace(*(out - 1)))
            {
                out--;
            }

            // 提取右侧token
            char right_token[256];
            int i = 0;

            if (isalpha(*p) || *p == '_')
            {
                while (*p && (isalnum(*p) || *p == '_') && i < 255)
                {
                    right_token[i++] = *p++;
                }
                right_token[i] = '\0';

                // 查找是否是参数
                const char *right_value = right_token;
                for (int j = 0; j < num_params; j++)
                {
                    if (strcmp(param_names[j], right_token) == 0)
                    {
                        right_value = param_values[j];
                        break;
                    }
                }

                // 连接（不添加空格）
                while (*right_value && out - buffer < 4090)
                {
                    *out++ = *right_value++;
                }
            }
            continue;
        }

        // 处理 # 字符串化运算符
        if (*p == '#' && *(p + 1) != '#')
        {
            p++;
            p = skip_whitespace(p);

            // 提取参数名
            char param_name[256];
            int i = 0;
            while (*p && (isalnum(*p) || *p == '_') && i < 255)
            {
                param_name[i++] = *p++;
            }
            param_name[i] = '\0';

            // 查找参数值
            for (int j = 0; j < num_params; j++)
            {
                if (strcmp(param_names[j], param_name) == 0)
                {
                    char *str = stringify(param_values[j]);
                    while (*str && out - buffer < 4090)
                    {
                        *out++ = *str++;
                    }
                    break;
                }
            }
            continue;
        }

        // 处理普通标识符（可能是参数或__VA_ARGS__）
        if (isalpha(*p) || *p == '_')
        {
            char name[256];
            int i = 0;
            const char *start = p;
            while (*p && (isalnum(*p) || *p == '_') && i < 255)
            {
                name[i++] = *p++;
            }
            name[i] = '\0';

            // 检查是否是__VA_ARGS__
            if (is_variadic && strcmp(name, "__VA_ARGS__") == 0)
            {
                const char *va = va_args ? va_args : "";
                while (*va && out - buffer < 4090)
                {
                    *out++ = *va++;
                }
                continue;
            }

            // 检查是否是参数
            int found = 0;
            for (int j = 0; j < num_params; j++)
            {
                if (strcmp(param_names[j], name) == 0)
                {
                    const char *val = param_values[j];
                    while (*val && out - buffer < 4090)
                    {
                        *out++ = *val++;
                    }
                    found = 1;
                    break;
                }
            }

            if (!found)
            {
                // 不是参数，原样输出
                while (start < p && out - buffer < 4090)
                {
                    *out++ = *start++;
                }
            }
        }
        else
        {
            *out++ = *p++;
        }
    }

    *out = '\0';
    return strdup(buffer);
}

// 展开宏
static char *expand_macros(Preprocessor *pp, const char *text)
{
    static char buffer[4096];
    char *out = buffer;
    const char *p = text;

    while (*p && out - buffer < 4090)
    {
        // 查找标识符
        if (isalpha(*p) || *p == '_')
        {
            char name[256];
            int i = 0;
            while (*p && (isalnum(*p) || *p == '_') && i < 255)
            {
                name[i++] = *p++;
            }
            name[i] = '\0';

            // 查找宏定义
            const char *value = preprocessor_get_macro_value(pp, name);
            if (value)
            {
                // 展开宏
                while (*value && out - buffer < 4090)
                {
                    *out++ = *value++;
                }
            }
            else
            {
                // 不是宏，原样输出
                const char *n = name;
                while (*n && out - buffer < 4090)
                {
                    *out++ = *n++;
                }
            }
        }
        else
        {
            *out++ = *p++;
        }
    }

    *out = '\0';
    return strdup(buffer);
}

// 预处理主函数
char *preprocessor_process(Preprocessor *pp, const char *input, const char *filename)
{
    if (!pp || !input)
        return NULL;

    // 重置输出缓冲区
    pp->output_pos = 0;
    pp->line_number = 1;
    pp->current_filename = filename;

    // 更新预定义宏
    char line_str[32];
    snprintf(line_str, sizeof(line_str), "%d", pp->line_number);
    preprocessor_define_macro(pp, "__LINE__", line_str);

    char filename_str[512];
    snprintf(filename_str, sizeof(filename_str), "\"%s\"", filename ? filename : "<unknown>");
    preprocessor_define_macro(pp, "__FILE__", filename_str);

    const char *p = input;
    int skip_block = 0;            // 是否跳过当前代码块
    int condition_stack[10] = {0}; // 条件栈
    int stack_depth = 0;

    while (*p)
    {
        // 处理单行注释 //
        if (*p == '/' && *(p + 1) == '/')
        {
            p += 2;
            while (*p && *p != '\n')
                p++;
            if (*p == '\n')
            {
                if (!skip_block)
                    output_char(pp, '\n');
                p++;
                pp->line_number++;
            }
            continue;
        }

        // 处理多行注释 /* */
        if (*p == '/' && *(p + 1) == '*')
        {
            p += 2;
            while (*p && !(*p == '*' && *(p + 1) == '/'))
            {
                if (*p == '\n')
                {
                    if (!skip_block)
                        output_char(pp, '\n');
                    pp->line_number++;
                }
                p++;
            }
            if (*p == '*' && *(p + 1) == '/')
                p += 2;
            continue;
        }

        // 处理预处理指令
        if (*p == '#')
        {
            const char *directive_start = p;
            p++;
            p = skip_whitespace(p);

            // #define
            if (strncmp(p, "define ", 7) == 0)
            {
                if (!skip_block)
                {
                    p += 7;
                    p = skip_whitespace(p);

                    // 提取宏名
                    char macro_name[256];
                    int i = 0;
                    while (*p && (isalnum(*p) || *p == '_') && i < 255)
                    {
                        macro_name[i++] = *p++;
                    }
                    macro_name[i] = '\0';

                    // 检查是否是函数宏（紧跟'('）
                    int is_function_macro = 0;
                    int is_variadic = 0;
                    char **params = NULL;
                    int num_params = 0;

                    if (*p == '(')
                    {
                        is_function_macro = 1;
                        p++; // 跳过 '('

                        // 解析参数列表
                        params = (char **)malloc(16 * sizeof(char *));
                        while (*p && *p != ')' && num_params < 16)
                        {
                            p = skip_whitespace(p);

                            // 检查是否是 ...
                            if (*p == '.' && *(p + 1) == '.' && *(p + 2) == '.')
                            {
                                is_variadic = 1;
                                p += 3;
                                p = skip_whitespace(p);
                                break;
                            }

                            // 提取参数名
                            char param[64];
                            int j = 0;
                            while (*p && (isalnum(*p) || *p == '_') && j < 63)
                            {
                                param[j++] = *p++;
                            }
                            param[j] = '\0';

                            if (j > 0)
                            {
                                params[num_params++] = strdup(param);
                            }

                            p = skip_whitespace(p);
                            if (*p == ',')
                            {
                                p++;
                            }
                        }

                        if (*p == ')')
                        {
                            p++;
                        }
                    }

                    p = skip_whitespace(p);

                    // 提取宏值
                    char macro_value[1024];
                    i = 0;
                    while (*p && *p != '\n' && i < 1023)
                    {
                        macro_value[i++] = *p++;
                    }
                    macro_value[i] = '\0';

                    // 去除尾部空白
                    while (i > 0 && isspace(macro_value[i - 1]))
                    {
                        macro_value[--i] = '\0';
                    }

                    // 保存宏定义
                    if (is_function_macro)
                    {
                        // 扩容
                        if (pp->num_macros >= pp->macro_capacity)
                        {
                            pp->macro_capacity = pp->macro_capacity == 0 ? 16 : pp->macro_capacity * 2;
                            pp->macros = (MacroDefinition *)realloc(pp->macros,
                                                                    pp->macro_capacity * sizeof(MacroDefinition));
                        }

                        pp->macros[pp->num_macros].name = strdup(macro_name);
                        pp->macros[pp->num_macros].value = strdup(macro_value);
                        pp->macros[pp->num_macros].params = params;
                        pp->macros[pp->num_macros].num_params = num_params;
                        pp->macros[pp->num_macros].is_function = 1;
                        pp->macros[pp->num_macros].is_variadic = is_variadic;
                        pp->num_macros++;
                    }
                    else
                    {
                        preprocessor_define_macro(pp, macro_name, macro_value);
                    }
                }

                while (*p && *p != '\n')
                    p++;
            }
            // #undef
            else if (strncmp(p, "undef ", 6) == 0)
            {
                if (!skip_block)
                {
                    p += 6;
                    p = skip_whitespace(p);

                    char macro_name[256];
                    int i = 0;
                    while (*p && (isalnum(*p) || *p == '_') && i < 255)
                    {
                        macro_name[i++] = *p++;
                    }
                    macro_name[i] = '\0';

                    preprocessor_undef_macro(pp, macro_name);
                }

                while (*p && *p != '\n')
                    p++;
            }
            // #error
            else if (strncmp(p, "error", 5) == 0)
            {
                if (!skip_block)
                {
                    p += 5;
                    p = skip_whitespace(p);

                    fprintf(stderr, "#error: %s\n", p);
                    return NULL; // 编译错误
                }

                while (*p && *p != '\n')
                    p++;
            }
            // #pragma
            else if (strncmp(p, "pragma", 6) == 0)
            {
                // 忽略 #pragma，不做处理
                while (*p && *p != '\n')
                    p++;
            }
            // #include
            else if (strncmp(p, "include", 7) == 0)
            {
                if (!skip_block)
                {
                    const char *next = process_include(pp, directive_start);
                    if (next)
                        p = next;
                    else
                        return NULL; // 错误
                }
                else
                {
                    // 跳过此行
                    while (*p && *p != '\n')
                        p++;
                }
            }
            // 条件编译
            else if (strncmp(p, "if", 2) == 0 ||
                     strncmp(p, "ifdef", 5) == 0 ||
                     strncmp(p, "ifndef", 6) == 0 ||
                     strncmp(p, "elif", 4) == 0 ||
                     strncmp(p, "else", 4) == 0 ||
                     strncmp(p, "endif", 5) == 0)
            {
                const char *next = process_conditional(pp, directive_start, &skip_block, condition_stack, &stack_depth);
                if (next)
                    p = next;
                else
                    return NULL;
            }
            else
            {
                // 其他预处理指令，暂时忽略
                while (*p && *p != '\n')
                {
                    if (!skip_block)
                        output_char(pp, *p);
                    p++;
                }
            }

            if (*p == '\n')
            {
                if (!skip_block)
                    output_char(pp, '\n');
                p++;
                pp->line_number++;
            }
        }
        else
        {
            // 普通代码 - 尝试展开宏
            if (!skip_block)
            {
                // 查找标识符进行宏展开
                if (isalpha(*p) || *p == '_')
                {
                    char name[256];
                    int i = 0;
                    const char *start = p;
                    while (*p && (isalnum(*p) || *p == '_') && i < 255)
                    {
                        name[i++] = *p++;
                    }
                    name[i] = '\0';

                    // 查找宏定义
                    MacroDefinition *macro = NULL;
                    for (int j = 0; j < pp->num_macros; j++)
                    {
                        if (strcmp(pp->macros[j].name, name) == 0)
                        {
                            macro = &pp->macros[j];
                            break;
                        }
                    }

                    if (macro)
                    {
                        // 检查是否是函数宏
                        if (macro->is_function)
                        {
                            // 跳过空白
                            const char *saved_p = p;
                            p = skip_whitespace(p);

                            if (*p == '(')
                            {
                                p++; // 跳过 '('

                                // 解析实参
                                char **arg_values = (char **)malloc(16 * sizeof(char *));
                                int num_args = 0;
                                char *va_args_str = NULL;

                                while (*p && *p != ')' && num_args < 16)
                                {
                                    p = skip_whitespace(p);

                                    // 提取实参值
                                    char arg[256];
                                    int k = 0;
                                    int paren_depth = 0;

                                    while (*p && (paren_depth > 0 || (*p != ',' && *p != ')')) && k < 255)
                                    {
                                        if (*p == '(')
                                            paren_depth++;
                                        if (*p == ')')
                                            paren_depth--;
                                        if (paren_depth >= 0)
                                        {
                                            arg[k++] = *p++;
                                        }
                                    }
                                    arg[k] = '\0';

                                    // 去除尾部空白
                                    while (k > 0 && isspace(arg[k - 1]))
                                    {
                                        arg[--k] = '\0';
                                    }

                                    if (k > 0)
                                    {
                                        if (macro->is_variadic && num_args >= macro->num_params)
                                        {
                                            // 可变参数，收集到va_args
                                            if (va_args_str == NULL)
                                            {
                                                va_args_str = strdup(arg);
                                            }
                                            else
                                            {
                                                char *tmp = (char *)malloc(strlen(va_args_str) + strlen(arg) + 3);
                                                sprintf(tmp, "%s, %s", va_args_str, arg);
                                                free(va_args_str);
                                                va_args_str = tmp;
                                            }
                                        }
                                        else
                                        {
                                            arg_values[num_args++] = strdup(arg);
                                        }
                                    }

                                    p = skip_whitespace(p);
                                    if (*p == ',')
                                    {
                                        p++;
                                    }
                                }

                                if (*p == ')')
                                {
                                    p++;
                                }

                                // 展开宏，应用参数替换和运算符处理
                                char *expanded = process_macro_operators(
                                    macro->value,
                                    macro->params,
                                    arg_values,
                                    macro->num_params,
                                    macro->is_variadic,
                                    va_args_str);

                                output_string(pp, expanded);
                                free(expanded);

                                // 清理
                                for (int k = 0; k < num_args; k++)
                                {
                                    free(arg_values[k]);
                                }
                                free(arg_values);
                                if (va_args_str)
                                {
                                    free(va_args_str);
                                }
                            }
                            else
                            {
                                // 没有'('，不展开函数宏，原样输出
                                p = saved_p;
                                while (start < p)
                                {
                                    output_char(pp, *start++);
                                }
                            }
                        }
                        else
                        {
                            // 对象宏，直接展开
                            output_string(pp, macro->value);
                        }
                    }
                    else
                    {
                        // 不是宏，原样输出
                        while (start < p)
                        {
                            output_char(pp, *start++);
                        }
                    }
                }
                else
                {
                    output_char(pp, *p);
                    p++;
                }
            }
            else
            {
                p++;
            }

            if (*(p - 1) == '\n')
            {
                pp->line_number++;
                // 更新 __LINE__ 宏
                char line_str[32];
                snprintf(line_str, sizeof(line_str), "%d", pp->line_number);
                preprocessor_define_macro(pp, "__LINE__", line_str);
            }
        }
    }

    // 添加字符串结束符
    output_char(pp, '\0');

    // 返回处理后的代码
    return strdup(pp->output);
}
