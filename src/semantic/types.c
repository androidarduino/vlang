#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 创建基本类型
TypeInfo *create_type(DataType base_type)
{
    TypeInfo *type = (TypeInfo *)malloc(sizeof(TypeInfo));
    if (!type)
    {
        fprintf(stderr, "Error: Failed to allocate memory for TypeInfo\n");
        exit(1);
    }
    type->base_type = base_type;
    type->pointer_level = 0;
    type->array_size = -1;
    type->array_sizes = NULL;
    type->array_dimensions = 0;
    type->is_const = 0;
    type->is_volatile = 0;
    type->return_type = NULL;
    type->param_types = NULL;
    type->num_params = 0;
    type->struct_name = NULL;
    type->members = NULL;
    type->num_members = 0;
    type->struct_size = 0;
    return type;
}

// 创建指针类型
TypeInfo *create_pointer_type(TypeInfo *base)
{
    TypeInfo *type = (TypeInfo *)malloc(sizeof(TypeInfo));
    if (!type)
    {
        fprintf(stderr, "Error: Failed to allocate memory for pointer type\n");
        exit(1);
    }
    memcpy(type, base, sizeof(TypeInfo));
    type->pointer_level = base->pointer_level + 1;
    return type;
}

// 创建数组类型
TypeInfo *create_array_type(TypeInfo *base, int size)
{
    TypeInfo *type = (TypeInfo *)malloc(sizeof(TypeInfo));
    if (!type)
    {
        fprintf(stderr, "Error: Failed to allocate memory for array type\n");
        exit(1);
    }
    memcpy(type, base, sizeof(TypeInfo));
    type->array_size = size;
    return type;
}

// 创建函数类型
TypeInfo *create_function_type(TypeInfo *return_type)
{
    TypeInfo *type = create_type(TYPE_FUNCTION);
    type->return_type = return_type;
    type->param_types = NULL;
    type->num_params = 0;
    return type;
}

// 添加函数参数类型
void add_param_type(TypeInfo *func_type, TypeInfo *param_type)
{
    if (!func_type || func_type->base_type != TYPE_FUNCTION)
    {
        return;
    }

    func_type->param_types = (TypeInfo **)realloc(
        func_type->param_types,
        (func_type->num_params + 1) * sizeof(TypeInfo *));
    if (!func_type->param_types)
    {
        fprintf(stderr, "Error: Failed to reallocate memory for param types\n");
        exit(1);
    }

    func_type->param_types[func_type->num_params++] = param_type;
}

// 检查类型兼容性
int types_compatible(TypeInfo *t1, TypeInfo *t2)
{
    if (!t1 || !t2)
        return 0;

    // 基本类型必须相同
    if (t1->base_type != t2->base_type)
    {
        // 特殊情况：数值类型之间可以隐式转换
        // int, float, double, long, short, char, unsigned 可以互相转换
        DataType numeric_types[] = {TYPE_INT, TYPE_FLOAT, TYPE_DOUBLE, 
                                     TYPE_LONG, TYPE_SHORT, TYPE_CHAR, TYPE_UNSIGNED};
        int t1_is_numeric = 0, t2_is_numeric = 0;
        
        for (int i = 0; i < 7; i++)
        {
            if (t1->base_type == numeric_types[i]) t1_is_numeric = 1;
            if (t2->base_type == numeric_types[i]) t2_is_numeric = 1;
        }
        
        if (t1_is_numeric && t2_is_numeric)
        {
            return 1; // 数值类型间可以转换
        }
        return 0;
    }

    // 指针层级必须相同
    if (t1->pointer_level != t2->pointer_level)
    {
        return 0;
    }

    // 数组大小可以不同（数组退化为指针）

    return 1;
}

// 将类型转换为字符串（用于调试）
const char *type_to_string(TypeInfo *type)
{
    static char buffer[256];

    if (!type)
    {
        return "null";
    }

    switch (type->base_type)
    {
    case TYPE_INT:
        strcpy(buffer, "int");
        break;
    case TYPE_FLOAT:
        strcpy(buffer, "float");
        break;
    case TYPE_CHAR:
        strcpy(buffer, "char");
        break;
    case TYPE_VOID:
        strcpy(buffer, "void");
        break;
    case TYPE_SHORT:
        strcpy(buffer, "short");
        break;
    case TYPE_LONG:
        strcpy(buffer, "long");
        break;
    case TYPE_DOUBLE:
        strcpy(buffer, "double");
        break;
    case TYPE_UNSIGNED:
        strcpy(buffer, "unsigned");
        break;
    case TYPE_FUNCTION:
        if (type->return_type)
        {
            snprintf(buffer, sizeof(buffer), "function()->%s",
                     type_to_string(type->return_type));
        }
        else
        {
            strcpy(buffer, "function");
        }
        return buffer;
    case TYPE_STRUCT:
        if (type->struct_name)
        {
            snprintf(buffer, sizeof(buffer), "struct %s", type->struct_name);
        }
        else
        {
            strcpy(buffer, "struct");
        }
        return buffer;
    default:
        strcpy(buffer, "unknown");
        break;
    }

    // 添加指针标记
    for (int i = 0; i < type->pointer_level; i++)
    {
        strcat(buffer, "*");
    }

    // 添加数组标记
    if (type->array_size >= 0)
    {
        char temp[32];
        snprintf(temp, sizeof(temp), "[%d]", type->array_size);
        strcat(buffer, temp);
    }

    return buffer;
}

// 释放类型信息
void free_type(TypeInfo *type)
{
    if (!type)
        return;

    if (type->return_type)
    {
        free_type(type->return_type);
    }

    if (type->param_types)
    {
        for (int i = 0; i < type->num_params; i++)
        {
            free_type(type->param_types[i]);
        }
        free(type->param_types);
    }

    if (type->array_sizes)
    {
        free(type->array_sizes);
    }

    if (type->struct_name)
    {
        free(type->struct_name);
    }

    if (type->members)
    {
        for (int i = 0; i < type->num_members; i++)
        {
            if (type->members[i].name)
            {
                free(type->members[i].name);
            }
            if (type->members[i].type)
            {
                free_type(type->members[i].type);
            }
        }
        free(type->members);
    }

    free(type);
}
