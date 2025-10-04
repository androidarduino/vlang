#include "codegen.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// 创建代码生成器
CodeGenerator *codegen_create(FILE *output, SemanticAnalyzer *analyzer)
{
    CodeGenerator *gen = (CodeGenerator *)malloc(sizeof(CodeGenerator));
    if (!gen)
    {
        fprintf(stderr, "Error: Failed to allocate code generator\n");
        exit(1);
    }
    gen->output = output;
    gen->analyzer = analyzer;
    gen->label_counter = 0;
    gen->current_stack_offset = 0;
    gen->max_stack_size = 0;
    gen->loop_context = NULL;
    gen->return_label = 0;
    return gen;
}

// 销毁代码生成器
void codegen_destroy(CodeGenerator *gen)
{
    if (gen)
    {
        free(gen);
    }
}

// 生成新标签
int new_label(CodeGenerator *gen)
{
    return gen->label_counter++;
}

// 输出汇编指令
void emit(CodeGenerator *gen, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(gen->output, format, args);
    va_end(args);
    fprintf(gen->output, "\n");
}

// 压栈
void push_reg(CodeGenerator *gen, const char *reg)
{
    emit(gen, "    pushq %%%s", reg);
    gen->current_stack_offset += 8;
    if (gen->current_stack_offset > gen->max_stack_size)
    {
        gen->max_stack_size = gen->current_stack_offset;
    }
}

// 出栈
void pop_reg(CodeGenerator *gen, const char *reg)
{
    emit(gen, "    popq %%%s", reg);
    gen->current_stack_offset -= 8;
}

// 生成函数序言
void gen_prologue(CodeGenerator *gen, const char *func_name)
{
    emit(gen, "");
    emit(gen, "    .globl %s", func_name);
    emit(gen, "    .type %s, @function", func_name);
    emit(gen, "%s:", func_name);
    emit(gen, "    pushq %%rbp");
    emit(gen, "    movq %%rsp, %%rbp");
    // 为局部变量预留空间（后面会调整）
    emit(gen, "    subq $64, %%rsp  # Reserve space for local variables");
}

// 生成函数尾声
void gen_epilogue(CodeGenerator *gen)
{
    emit(gen, ".L%d:  # Function return", gen->return_label);
    emit(gen, "    movq %%rbp, %%rsp");
    emit(gen, "    popq %%rbp");
    emit(gen, "    ret");
}

// 生成表达式代码（结果放在 %rax）
void gen_expression(CodeGenerator *gen, ASTNode *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_INT_LITERAL:
        emit(gen, "    movq $%d, %%rax  # Load integer constant", node->value.int_val);
        break;

    case AST_FLOAT_LITERAL:
        // 简化：将 float 转为 int
        emit(gen, "    movq $%d, %%rax  # Load float as int", (int)node->value.float_val);
        break;

    case AST_IDENTIFIER:
    {
        // 使用存储的符号信息
        const char *name = node->value.string_val;
        Symbol *symbol = (Symbol *)node->semantic_info;
        if (symbol)
        {
            int offset = -(symbol->offset + 8); // 相对于 rbp 的偏移
            emit(gen, "    movq %d(%%rbp), %%rax  # Load variable '%s'", offset, name);
        }
        else
        {
            emit(gen, "    movq $0, %%rax  # ERROR: Unknown variable '%s'", name);
        }
        break;
    }

    case AST_BINARY_EXPR:
    {
        if (node->num_children < 2)
            break;

        // 先计算右操作数
        gen_expression(gen, node->children[1]);
        push_reg(gen, "rax"); // 保存右操作数

        // 计算左操作数
        gen_expression(gen, node->children[0]);

        // 恢复右操作数到 rbx
        pop_reg(gen, "rbx");

        // 执行操作（结果在 rax）
        switch (node->value.op_type)
        {
        case OP_ADD:
            emit(gen, "    addq %%rbx, %%rax  # Add");
            break;
        case OP_SUB:
            emit(gen, "    subq %%rbx, %%rax  # Subtract");
            break;
        case OP_MUL:
            emit(gen, "    imulq %%rbx, %%rax  # Multiply");
            break;
        case OP_DIV:
            emit(gen, "    cqto  # Sign extend");
            emit(gen, "    idivq %%rbx  # Divide");
            break;
        case OP_MOD:
            emit(gen, "    cqto  # Sign extend");
            emit(gen, "    idivq %%rbx  # Modulo");
            emit(gen, "    movq %%rdx, %%rax  # Result in rax");
            break;
        case OP_LT:
            emit(gen, "    cmpq %%rbx, %%rax  # Compare <");
            emit(gen, "    setl %%al");
            emit(gen, "    movzbq %%al, %%rax");
            break;
        case OP_GT:
            emit(gen, "    cmpq %%rbx, %%rax  # Compare >");
            emit(gen, "    setg %%al");
            emit(gen, "    movzbq %%al, %%rax");
            break;
        case OP_LE:
            emit(gen, "    cmpq %%rbx, %%rax  # Compare <=");
            emit(gen, "    setle %%al");
            emit(gen, "    movzbq %%al, %%rax");
            break;
        case OP_GE:
            emit(gen, "    cmpq %%rbx, %%rax  # Compare >=");
            emit(gen, "    setge %%al");
            emit(gen, "    movzbq %%al, %%rax");
            break;
        case OP_EQ:
            emit(gen, "    cmpq %%rbx, %%rax  # Compare ==");
            emit(gen, "    sete %%al");
            emit(gen, "    movzbq %%al, %%rax");
            break;
        case OP_NE:
            emit(gen, "    cmpq %%rbx, %%rax  # Compare !=");
            emit(gen, "    setne %%al");
            emit(gen, "    movzbq %%al, %%rax");
            break;
        case OP_AND:
            // 逻辑与: 两个都非零则为1，否则为0
            emit(gen, "    testq %%rax, %%rax  # Test left operand");
            emit(gen, "    setne %%al");
            emit(gen, "    testq %%rbx, %%rbx  # Test right operand");
            emit(gen, "    setne %%bl");
            emit(gen, "    andb %%bl, %%al    # AND both");
            emit(gen, "    movzbq %%al, %%rax");
            break;
        case OP_OR:
            // 逻辑或: 至少一个非零则为1，否则为0
            emit(gen, "    testq %%rax, %%rax  # Test left operand");
            emit(gen, "    setne %%al");
            emit(gen, "    testq %%rbx, %%rbx  # Test right operand");
            emit(gen, "    setne %%bl");
            emit(gen, "    orb %%bl, %%al     # OR both");
            emit(gen, "    movzbq %%al, %%rax");
            break;
        default:
            break;
        }
        break;
    }

    case AST_ASSIGN_EXPR:
    {
        if (node->num_children < 2)
            break;

        ASTNode *lhs = node->children[0];

        // 检查左侧是否是数组下标 (arr[i] = value)
        if (lhs->type == AST_ARRAY_SUBSCRIPT)
        {
            if (lhs->num_children < 2)
                break;

            ASTNode *array_node = lhs->children[0];
            ASTNode *index_node = lhs->children[1];

            // 计算索引
            gen_expression(gen, index_node);
            push_reg(gen, "rax"); // 保存索引

            // 获取数组基地址
            if (array_node->type == AST_IDENTIFIER)
            {
                Symbol *symbol = (Symbol *)array_node->semantic_info;
                if (symbol)
                {
                    // 数组在栈上，使用负偏移
                    int offset = -(symbol->offset + 8);
                    emit(gen, "    leaq %d(%%rbp), %%rbx  # Load array base address (assign)", offset);
                }
            }
            else
            {
                gen_expression(gen, array_node);
                emit(gen, "    movq %%rax, %%rbx  # Array base address");
            }

            // 弹出索引
            pop_reg(gen, "rax");

            // 计算元素地址
            emit(gen, "    imulq $8, %%rax  # Calculate offset (index * 8)");
            emit(gen, "    addq %%rbx, %%rax  # Add base address");
            push_reg(gen, "rax"); // 保存元素地址

            // 计算右侧表达式的值
            gen_expression(gen, node->children[1]);

            // 恢复元素地址
            pop_reg(gen, "rbx");

            // 存储值到数组元素
            emit(gen, "    movq %%rax, (%%rbx)  # Store to array element");
        }
        // 检查左侧是否是解引用表达式 (*ptr = value)
        else if (lhs->type == AST_UNARY_EXPR && lhs->value.op_type == OP_DEREF)
        {
            // 先计算指针地址
            gen_expression(gen, lhs->children[0]);
            push_reg(gen, "rax"); // 保存指针地址

            // 计算右侧表达式的值
            gen_expression(gen, node->children[1]);

            // 恢复指针地址到 rbx
            pop_reg(gen, "rbx");

            // 存储值到指针指向的位置
            emit(gen, "    movq %%rax, (%%rbx)  # Store to dereferenced pointer");
        }
        else if (lhs->type == AST_IDENTIFIER || lhs->type == AST_DECLARATOR)
        {
            // 计算右侧表达式
            gen_expression(gen, node->children[1]);

            // 存储到左侧变量
            const char *name = lhs->value.string_val;
            Symbol *symbol = (Symbol *)lhs->semantic_info;
            if (symbol)
            {
                int offset = -(symbol->offset + 8);
                emit(gen, "    movq %%rax, %d(%%rbp)  # Store to variable '%s'", offset, name);
            }
        }
        break;
    }

    case AST_UNARY_EXPR:
    {
        if (node->num_children < 1)
            break;

        // 应用一元运算符
        switch (node->value.op_type)
        {
        case OP_ADDR:
            // 取地址：不计算操作数的值，而是返回其地址
            if (node->children[0]->type == AST_IDENTIFIER)
            {
                ASTNode *id = node->children[0];
                Symbol *symbol = (Symbol *)id->semantic_info;
                if (symbol)
                {
                    int offset = -(symbol->offset + 8);
                    emit(gen, "    leaq %d(%%rbp), %%rax  # Load address of '%s'",
                         offset, id->value.string_val);
                }
            }
            break;
        case OP_DEREF:
        case OP_NOT:
        case OP_NEG:
        default:
            // 对于其他运算符，先计算操作数
            gen_expression(gen, node->children[0]);

            if (node->value.op_type == OP_NOT)
            {
                // 逻辑非: 0变1，非0变0
                emit(gen, "    testq %%rax, %%rax  # Test operand");
                emit(gen, "    sete %%al           # Set if zero");
                emit(gen, "    movzbq %%al, %%rax  # Extend to 64-bit");
            }
            else if (node->value.op_type == OP_NEG)
            {
                // 取负
                emit(gen, "    negq %%rax  # Negate");
            }
            else if (node->value.op_type == OP_DEREF)
            {
                // 解引用：rax 中是指针（地址），加载该地址处的值
                emit(gen, "    movq (%%rax), %%rax  # Dereference pointer");
            }
            break;
        }
        break;
    }

    case AST_CALL_EXPR:
    {
        if (node->num_children < 1)
            break;

        // 获取函数名
        ASTNode *func_node = node->children[0];
        if (func_node->type != AST_IDENTIFIER)
            break;

        const char *func_name = func_node->value.string_val;

        // System V AMD64 ABI: 参数寄存器
        const char *param_regs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

        // 准备参数
        int num_args = 0;
        if (node->num_children > 1 && node->children[1]->type == AST_ARG_LIST)
        {
            ASTNode *arg_list = node->children[1];
            num_args = arg_list->num_children;

            // 将参数压栈（从右到左）
            for (int i = num_args - 1; i >= 0; i--)
            {
                gen_expression(gen, arg_list->children[i]);
                push_reg(gen, "rax");
            }

            // 从栈弹出到参数寄存器（从左到右）
            int num_reg_args = num_args < 6 ? num_args : 6;
            for (int i = 0; i < num_reg_args; i++)
            {
                pop_reg(gen, param_regs[i]);
            }
        }

        // 调用函数前保证栈对齐到16字节
        // System V AMD64 ABI要求 call 前栈是16字节对齐的
        emit(gen, "    call %s", func_name);

        // rax 现在包含返回值
        break;
    }

    case AST_ARRAY_SUBSCRIPT:
    {
        // 数组下标访问: array[index]
        if (node->num_children < 2)
            break;

        ASTNode *array_node = node->children[0];
        ASTNode *index_node = node->children[1];

        // 计算索引
        gen_expression(gen, index_node);
        push_reg(gen, "rax"); // 保存索引

        // 获取数组地址
        if (array_node->type == AST_IDENTIFIER)
        {
            Symbol *symbol = (Symbol *)array_node->semantic_info;
            if (symbol)
            {
                // 数组在栈上，计算基地址
                // symbol->offset 是第一个元素的偏移量
                int offset = -(symbol->offset + 8);
                emit(gen, "    leaq %d(%%rbp), %%rbx  # Load array base address", offset);
            }
        }
        else
        {
            // 更复杂的数组表达式
            gen_expression(gen, array_node);
            emit(gen, "    movq %%rax, %%rbx  # Array base address");
        }

        // 弹出索引
        pop_reg(gen, "rax");

        // 计算偏移：index * element_size
        // 假设元素大小为 8 字节（int/指针）
        emit(gen, "    imulq $8, %%rax  # Calculate offset (index * 8)");

        // 计算元素地址: base + offset
        emit(gen, "    addq %%rbx, %%rax  # Add base address");

        // 加载元素值
        emit(gen, "    movq (%%rax), %%rax  # Load array element");
        break;
    }

    case AST_INIT_LIST:
    {
        // 初始化列表在声明中处理，这里不生成代码
        break;
    }

    case AST_MEMBER_ACCESS:
    {
        // 结构体成员访问
        if (node->num_children < 2)
            break;

        // TODO: 实现结构体成员访问的代码生成
        emit(gen, "    # Member access not fully implemented yet");
        emit(gen, "    movq $0, %%rax");
        break;
    }

    default:
        break;
    }
}

// 生成语句代码
void gen_statement(CodeGenerator *gen, ASTNode *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_DECLARATION:
    {
        // 变量声明，可能有初始化
        if (node->num_children >= 2)
        {
            ASTNode *type_spec = node->children[0];
            ASTNode *declarator = node->children[1];

            // 如果是赋值表达式（带初始化）
            if (declarator->type == AST_ASSIGN_EXPR)
            {
                if (declarator->num_children >= 2)
                {
                    ASTNode *init_expr = declarator->children[1];

                    // 处理数组初始化列表
                    if (init_expr->type == AST_INIT_LIST)
                    {
                        // 获取数组符号信息
                        Symbol *symbol = NULL;
                        ASTNode *id_node = declarator->children[0];

                        // 遍历查找标识符节点
                        while (id_node && id_node->type == AST_DECLARATOR && id_node->num_children > 0)
                        {
                            id_node = id_node->children[0];
                        }

                        if (id_node && id_node->type == AST_IDENTIFIER)
                        {
                            symbol = (Symbol *)id_node->semantic_info;
                        }

                        if (symbol)
                        {
                            // 初始化每个数组元素
                            for (int i = 0; i < init_expr->num_children; i++)
                            {
                                // 计算初始化表达式的值
                                gen_expression(gen, init_expr->children[i]);

                                // 存储到数组元素位置：offset + i * 8
                                int elem_offset = symbol->offset + (i * 8);
                                emit(gen, "    movq %%rax, %d(%%rbp)  # Initialize array[%d]",
                                     elem_offset, i);
                            }
                        }
                    }
                    else
                    {
                        // 普通初始化表达式
                        gen_expression(gen, declarator);
                    }
                }
            }
        }
        break;
    }

    case AST_EXPR_STMT:
        if (node->num_children > 0)
        {
            gen_expression(gen, node->children[0]);
        }
        break;

    case AST_COMPOUND_STMT:
        for (int i = 0; i < node->num_children; i++)
        {
            gen_statement(gen, node->children[i]);
        }
        break;

    case AST_IF_STMT:
    {
        int else_label = new_label(gen);
        int end_label = new_label(gen);

        // 计算条件
        if (node->num_children > 0)
        {
            gen_expression(gen, node->children[0]);
            emit(gen, "    testq %%rax, %%rax  # Test condition");
            emit(gen, "    je .L%d  # Jump if false", else_label);
        }

        // then 分支
        if (node->num_children > 1)
        {
            gen_statement(gen, node->children[1]);
        }
        emit(gen, "    jmp .L%d  # Jump to end", end_label);

        // else 分支
        emit(gen, ".L%d:", else_label);
        if (node->num_children > 2)
        {
            gen_statement(gen, node->children[2]);
        }

        emit(gen, ".L%d:", end_label);
        break;
    }

    case AST_WHILE_STMT:
    {
        int start_label = new_label(gen);
        int end_label = new_label(gen);

        // 设置循环上下文
        LoopContext loop_ctx;
        loop_ctx.start_label = start_label;
        loop_ctx.end_label = end_label;
        loop_ctx.continue_label = start_label; // continue 跳到循环开始
        loop_ctx.parent = gen->loop_context;
        gen->loop_context = &loop_ctx;

        emit(gen, ".L%d:  # While loop start", start_label);

        // 计算条件
        if (node->num_children > 0)
        {
            gen_expression(gen, node->children[0]);
            emit(gen, "    testq %%rax, %%rax  # Test condition");
            emit(gen, "    je .L%d  # Jump if false", end_label);
        }

        // 循环体
        if (node->num_children > 1)
        {
            gen_statement(gen, node->children[1]);
        }

        emit(gen, "    jmp .L%d  # Loop back", start_label);
        emit(gen, ".L%d:  # While loop end", end_label);

        // 恢复循环上下文
        gen->loop_context = loop_ctx.parent;
        break;
    }

    case AST_FOR_STMT:
    {
        int start_label = new_label(gen);
        int end_label = new_label(gen);
        int inc_label = new_label(gen);

        // 初始化部分 (expression_statement)
        if (node->num_children > 0 && node->children[0])
        {
            gen_statement(gen, node->children[0]);
        }

        // 设置循环上下文
        LoopContext loop_ctx;
        loop_ctx.start_label = start_label;
        loop_ctx.end_label = end_label;
        loop_ctx.continue_label = inc_label; // continue 跳到增量部分
        loop_ctx.parent = gen->loop_context;
        gen->loop_context = &loop_ctx;

        emit(gen, ".L%d:  # For loop start", start_label);

        // 条件部分 (expression_statement)
        if (node->num_children > 1 && node->children[1])
        {
            // expression_statement 可能包含表达式
            if (node->children[1]->num_children > 0)
            {
                gen_expression(gen, node->children[1]->children[0]);
                emit(gen, "    testq %%rax, %%rax  # Test condition");
                emit(gen, "    je .L%d  # Jump if false", end_label);
            }
        }

        // 循环体
        int body_index = (node->num_children == 3) ? 2 : 3;
        if (node->num_children >= body_index + 1)
        {
            gen_statement(gen, node->children[body_index]);
        }

        emit(gen, ".L%d:  # For loop increment", inc_label);

        // 增量部分 (如果有4个子节点，第3个是增量表达式)
        if (node->num_children == 4 && node->children[2])
        {
            gen_expression(gen, node->children[2]);
        }

        emit(gen, "    jmp .L%d  # Loop back", start_label);
        emit(gen, ".L%d:  # For loop end", end_label);

        // 恢复循环上下文
        gen->loop_context = loop_ctx.parent;
        break;
    }

    case AST_RETURN_STMT:
        if (node->num_children > 0)
        {
            gen_expression(gen, node->children[0]);
        }
        else
        {
            emit(gen, "    movq $0, %%rax  # Return 0");
        }
        emit(gen, "    jmp .L%d  # Return", gen->return_label);
        break;

    case AST_BREAK_STMT:
        if (gen->loop_context)
        {
            emit(gen, "    jmp .L%d  # Break", gen->loop_context->end_label);
        }
        else
        {
            emit(gen, "    # ERROR: break outside loop");
        }
        break;

    case AST_CONTINUE_STMT:
        if (gen->loop_context)
        {
            emit(gen, "    jmp .L%d  # Continue", gen->loop_context->continue_label);
        }
        else
        {
            emit(gen, "    # ERROR: continue outside loop");
        }
        break;

    default:
        break;
    }
}

// 生成函数代码
void gen_function(CodeGenerator *gen, ASTNode *node)
{
    if (!node || node->type != AST_FUNCTION_DEF)
        return;
    if (node->num_children < 3)
        return;

    // 获取函数名
    ASTNode *declarator = node->children[1];
    const char *func_name = declarator->value.string_val;

    // 为这个函数分配唯一的返回标签
    gen->return_label = new_label(gen);

    // 生成序言
    gen_prologue(gen, func_name);

    // 处理函数参数（从寄存器保存到栈）
    // System V AMD64 ABI: rdi, rsi, rdx, rcx, r8, r9
    const char *param_regs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
    if (declarator->num_children > 0 && declarator->children[0]->type == AST_PARAM_LIST)
    {
        ASTNode *param_list = declarator->children[0];
        int num_params = param_list->num_children < 6 ? param_list->num_children : 6;

        for (int i = 0; i < num_params; i++)
        {
            ASTNode *param = param_list->children[i];
            if (param->type == AST_DECLARATION && param->num_children >= 2)
            {
                ASTNode *param_declarator = param->children[1];
                const char *param_name = param_declarator->value.string_val;
                Symbol *param_symbol = (Symbol *)param_declarator->semantic_info;

                if (param_symbol)
                {
                    int offset = -(param_symbol->offset + 8);
                    emit(gen, "    movq %%%s, %d(%%rbp)  # Save parameter '%s'",
                         param_regs[i], offset, param_name);
                }
            }
        }
    }

    // 生成函数体
    gen_statement(gen, node->children[2]);

    // 生成尾声
    gen_epilogue(gen);
}

// 生成完整程序代码
void generate_code(CodeGenerator *gen, ASTNode *root)
{
    if (!root || root->type != AST_PROGRAM)
        return;

    // 输出文件头
    emit(gen, "    .file \"output.c\"");
    emit(gen, "    .text");

    // 遍历所有函数
    for (int i = 0; i < root->num_children; i++)
    {
        if (root->children[i]->type == AST_FUNCTION_DEF)
        {
            gen_function(gen, root->children[i]);
        }
    }

    // 输出文件尾
    emit(gen, "");
    emit(gen, "    .section .note.GNU-stack,\"\",@progbits");
}
