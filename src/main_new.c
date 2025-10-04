#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ast.h"
#include "semantic.h"
#include "codegen.h"
#include "preprocessor.h"

extern int yyparse();
extern ASTNode *ast_root;
extern FILE *yyin;

void print_usage(const char *program_name)
{
    printf("Usage: %s [options] <input.c> [input2.c ...]\n", program_name);
    printf("\nOptions:\n");
    printf("  -c           Compile only (generate .o files)\n");
    printf("  -o <file>    Output file name\n");
    printf("  --debug      Enable debug output (AST and symbol table)\n");
    printf("  -h, --help   Show this help message\n");
    printf("\nExamples:\n");
    printf("  %s program.c              # Compile to executable 'output'\n", program_name);
    printf("  %s -o test test.c         # Compile to executable 'test'\n", program_name);
    printf("  %s -c file1.c file2.c     # Generate file1.o and file2.o\n", program_name);
    printf("  %s file1.c file2.c        # Compile and link multiple files\n", program_name);
    printf("  %s --debug program.c      # Compile with debug info\n", program_name);
}

// 编译单个文件到汇编
int compile_to_assembly(const char *input_file, const char *output_file, int debug_mode)
{
    printf("\n[Compiling] %s → %s\n", input_file, output_file);

    // ========== Phase 0: Preprocessing ==========
    printf("  [1/4] Preprocessing...\n");

    char *source_code = read_file_content(input_file);
    if (!source_code)
    {
        fprintf(stderr, "  ✗ Cannot read input file: %s\n", input_file);
        return 1;
    }

    Preprocessor *pp = preprocessor_create();
    if (!pp)
    {
        fprintf(stderr, "  ✗ Cannot create preprocessor\n");
        free(source_code);
        return 1;
    }

    char *preprocessed_code = preprocessor_process(pp, source_code, input_file);
    free(source_code);
    preprocessor_free(pp);

    if (!preprocessed_code)
    {
        fprintf(stderr, "  ✗ Preprocessing failed\n");
        return 1;
    }

    // 写入临时文件
    char temp_file[] = "/tmp/cc_XXXXXX";
    int fd = mkstemp(temp_file);
    if (fd == -1)
    {
        fprintf(stderr, "  ✗ Cannot create temporary file\n");
        free(preprocessed_code);
        return 1;
    }

    FILE *temp_fp = fdopen(fd, "w");
    fprintf(temp_fp, "%s", preprocessed_code);
    fclose(temp_fp);
    free(preprocessed_code);

    // ========== Phase 1: Parsing ==========
    printf("  [2/4] Parsing...\n");

    yyin = fopen(temp_file, "r");
    if (!yyin)
    {
        fprintf(stderr, "  ✗ Cannot open temp file\n");
        unlink(temp_file);
        return 1;
    }

    if (yyparse() != 0)
    {
        fprintf(stderr, "  ✗ Parsing failed\n");
        fclose(yyin);
        unlink(temp_file);
        return 1;
    }

    fclose(yyin);
    unlink(temp_file);

    if (!ast_root)
    {
        fprintf(stderr, "  ✗ No AST generated\n");
        return 1;
    }

    if (debug_mode)
    {
        printf("  [Debug] AST:\n");
        print_ast(ast_root, 2);
    }

    // ========== Phase 2: Semantic Analysis ==========
    printf("  [3/4] Semantic Analysis...\n");

    SemanticAnalyzer *analyzer = semantic_analyzer_create();
    analyze_program(analyzer, ast_root);

    if (debug_mode)
    {
        printf("  [Debug] Symbol Table:\n");
        print_symbol_table(analyzer->symbol_table);
    }

    if (analyzer->error_count > 0)
    {
        fprintf(stderr, "  ✗ Semantic analysis failed with %d error(s)\n",
                analyzer->error_count);
        semantic_analyzer_destroy(analyzer);
        free_ast(ast_root);
        ast_root = NULL;
        return 1;
    }

    // ========== Phase 3: Code Generation ==========
    printf("  [4/4] Code Generation...\n");

    FILE *out = fopen(output_file, "w");
    if (!out)
    {
        fprintf(stderr, "  ✗ Failed to open output file: %s\n", output_file);
        semantic_analyzer_destroy(analyzer);
        free_ast(ast_root);
        ast_root = NULL;
        return 1;
    }

    CodeGenerator *gen = codegen_create(out, analyzer);
    generate_code(gen, ast_root);

    fclose(out);
    codegen_destroy(gen);
    semantic_analyzer_destroy(analyzer);
    free_ast(ast_root);
    ast_root = NULL;

    printf("  ✓ Generated: %s\n", output_file);
    return 0;
}

int main(int argc, char **argv)
{
    char *output_file = NULL;
    char **input_files = NULL;
    int num_input_files = 0;
    int compile_only = 0; // -c选项
    int debug_mode = 0;

    // Parse command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            compile_only = 1;
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        {
            output_file = argv[++i];
        }
        else if (strcmp(argv[i], "--debug") == 0)
        {
            debug_mode = 1;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_usage(argv[0]);
            return 0;
        }
        else if (argv[i][0] != '-')
        {
            // Input file
            if (!input_files)
            {
                input_files = (char **)malloc(sizeof(char *) * (argc - i));
            }
            input_files[num_input_files++] = argv[i];
        }
        else
        {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (num_input_files == 0)
    {
        fprintf(stderr, "Error: No input files\n");
        print_usage(argv[0]);
        return 1;
    }

    printf("════════════════════════════════════════════════════════\n");
    printf("🚀 C Compiler - Multi-File Compilation\n");
    printf("════════════════════════════════════════════════════════\n");
    printf("Mode: %s\n", compile_only ? "Compile only (-c)" : "Compile and link");
    printf("Input files: %d\n", num_input_files);

    // Array to store object files
    char **object_files = (char **)malloc(sizeof(char *) * num_input_files);

    // Compile each file to assembly
    for (int i = 0; i < num_input_files; i++)
    {
        char *input = input_files[i];
        char *asm_file = (char *)malloc(strlen(input) + 10);

        // Generate assembly file name: file.c -> file.s
        strcpy(asm_file, input);
        char *dot = strrchr(asm_file, '.');
        if (dot)
        {
            strcpy(dot, ".s");
        }
        else
        {
            strcat(asm_file, ".s");
        }

        if (compile_to_assembly(input, asm_file, debug_mode) != 0)
        {
            fprintf(stderr, "\n✗ Compilation failed for %s\n", input);
            free(asm_file);
            for (int j = 0; j < i; j++)
                free(object_files[j]);
            free(object_files);
            free(input_files);
            return 1;
        }

        object_files[i] = asm_file;
    }

    printf("\n");
    printf("════════════════════════════════════════════════════════\n");
    printf("✓ All files compiled successfully!\n");
    printf("════════════════════════════════════════════════════════\n\n");

    if (compile_only)
    {
        // -c mode: assemble to .o files
        printf("[Assembling to object files]\n");
        for (int i = 0; i < num_input_files; i++)
        {
            char *obj_file = (char *)malloc(strlen(object_files[i]) + 10);
            strcpy(obj_file, object_files[i]);
            char *dot = strrchr(obj_file, '.');
            if (dot)
            {
                strcpy(dot, ".o");
            }

            char cmd[512];
            snprintf(cmd, sizeof(cmd), "gcc -c %s -o %s 2>&1", object_files[i], obj_file);

            printf("  %s → %s\n", object_files[i], obj_file);

            FILE *pipe = popen(cmd, "r");
            if (!pipe)
            {
                fprintf(stderr, "  ✗ Failed to run assembler\n");
                free(obj_file);
                continue;
            }

            char buffer[256];
            int has_errors = 0;
            while (fgets(buffer, sizeof(buffer), pipe))
            {
                fprintf(stderr, "    %s", buffer);
                has_errors = 1;
            }
            int status = pclose(pipe);

            if (status == 0 && !has_errors)
            {
                printf("  ✓ Generated: %s\n", obj_file);
            }
            else
            {
                fprintf(stderr, "  ✗ Assembly failed for %s\n", object_files[i]);
            }

            free(obj_file);
        }
    }
    else
    {
        // Link mode: assemble and link
        printf("[Assembling and Linking]\n");

        if (!output_file)
        {
            output_file = "output";
        }

        // Build gcc command
        char cmd[4096] = "gcc -no-pie ";
        for (int i = 0; i < num_input_files; i++)
        {
            strcat(cmd, object_files[i]);
            strcat(cmd, " ");
        }
        strcat(cmd, "-o ");
        strcat(cmd, output_file);
        strcat(cmd, " 2>&1");

        printf("  Linking %d file(s)...\n", num_input_files);

        FILE *pipe = popen(cmd, "r");
        if (!pipe)
        {
            fprintf(stderr, "  ✗ Failed to run linker\n");
            for (int i = 0; i < num_input_files; i++)
                free(object_files[i]);
            free(object_files);
            free(input_files);
            return 1;
        }

        char buffer[256];
        int has_errors = 0;
        while (fgets(buffer, sizeof(buffer), pipe))
        {
            fprintf(stderr, "  %s", buffer);
            has_errors = 1;
        }
        int status = pclose(pipe);

        if (status != 0 || has_errors)
        {
            fprintf(stderr, "\n✗ Linking failed\n");
            for (int i = 0; i < num_input_files; i++)
                free(object_files[i]);
            free(object_files);
            free(input_files);
            return 1;
        }

        printf("  ✓ Generated executable: %s\n", output_file);
    }

    // Cleanup assembly files
    for (int i = 0; i < num_input_files; i++)
    {
        unlink(object_files[i]); // Delete .s files
        free(object_files[i]);
    }
    free(object_files);
    free(input_files);

    printf("\n");
    printf("════════════════════════════════════════════════════════\n");
    printf("🎉 Compilation successful!\n");
    printf("════════════════════════════════════════════════════════\n");

    if (!compile_only)
    {
        printf("\nRun your program:\n");
        printf("  ./%s\n", output_file);
        printf("  echo $?    # Check exit code\n");
    }

    return 0;
}
