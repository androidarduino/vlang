#ifndef TYPES_H
#define TYPES_H

// 基本数据类型
typedef enum
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_VOID,
    TYPE_SHORT,    // 短整型
    TYPE_LONG,     // 长整型
    TYPE_DOUBLE,   // 双精度浮点
    TYPE_UNSIGNED, // 无符号整型
    TYPE_POINTER,
    TYPE_ARRAY,
    TYPE_FUNCTION,
    TYPE_STRUCT,
    TYPE_UNKNOWN
} DataType;

// 前向声明
struct StructMember;

// 类型信息结构
typedef struct TypeInfo
{
    DataType base_type;
    int pointer_level;             // 指针层级 (0=非指针, 1=*, 2=**, ...)
    int array_size;                // 数组大小 (-1 表示非数组)
    int *array_sizes;              // 多维数组大小数组
    int array_dimensions;          // 数组维度数
    int is_const;                  // 是否是 const 限定
    int is_volatile;               // 是否是 volatile 限定
    struct TypeInfo *return_type;  // 函数返回类型
    struct TypeInfo **param_types; // 函数参数类型
    int num_params;                // 参数数量
    char *struct_name;             // 结构体名称
    struct StructMember *members;  // 结构体成员
    int num_members;               // 成员数量
    int struct_size;               // 结构体大小（字节）
} TypeInfo;

// 结构体成员
typedef struct StructMember
{
    char *name;
    TypeInfo *type;
    int offset; // 相对于结构体起始位置的偏移
} StructMember;

// 函数声明
TypeInfo *create_type(DataType base_type);
TypeInfo *create_pointer_type(TypeInfo *base);
TypeInfo *create_array_type(TypeInfo *base, int size);
TypeInfo *create_function_type(TypeInfo *return_type);
void add_param_type(TypeInfo *func_type, TypeInfo *param_type);
int types_compatible(TypeInfo *t1, TypeInfo *t2);
const char *type_to_string(TypeInfo *type);
void free_type(TypeInfo *type);

#endif // TYPES_H
