#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"
#include "codegen.h"

extern int yyparse();
extern ASTNode *ast_root;

void print_usage(const char *program_name) {
    printf("Usage: %s [options] <input.c>\n", program_name);
    printf("\nOptions:\n");
    printf("  -o <file>    Output assembly file (default: output.s)\n");
    printf("  --debug      Enable debug output (AST and symbol table)\n");
    printf("  -h, --help   Show this help message\n");
    printf("\nExamples:\n");
    printf("  %s program.c              # Compile to output.s and ./output\n", program_name);
    printf("  %s -o test.s test.c       # Compile to test.s\n", program_name);
    printf("  %s --debug program.c      # Compile with debug info\n", program_name);
}

int main(int argc, char **argv) {
    char *output_file = "output.s";
    char *input_file = NULL;
    int debug_mode = 0;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "--debug") == 0) {
            debug_mode = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (argv[i][0] != '-') {
            input_file = argv[i];
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }
    
    // Redirect input if file specified
    if (input_file) {
        FILE *file = freopen(input_file, "r", stdin);
        if (!file) {
            fprintf(stderr, "Error: Cannot open input file: %s\n", input_file);
            return 1;
        }
    }
    
    printf("=== C Compiler - Full Pipeline ===\n\n");
    
    // ========== Phase 1: Lexical and Syntax Analysis ==========
    printf("[Phase 1/4] Lexical and Syntax Analysis...\n");
    if (yyparse() != 0) {
        fprintf(stderr, "✗ Parsing failed.\n");
        return 1;
    }
    printf("✓ Parsing successful!\n\n");
    
    if (!ast_root) {
        fprintf(stderr, "✗ No AST generated.\n");
        return 1;
    }
    
    // Print AST (debug mode)
    if (debug_mode) {
        printf("[Debug] Abstract Syntax Tree:\n");
        print_ast(ast_root, 0);
        printf("\n");
    }
    
    // ========== Phase 2: Semantic Analysis ==========
    printf("[Phase 2/4] Semantic Analysis...\n");
    SemanticAnalyzer *analyzer = semantic_analyzer_create();
    
    int semantic_ok = analyze_program(analyzer, ast_root);
    
    if (debug_mode) {
        printf("\n[Debug] Symbol Table:\n");
        print_symbol_table(analyzer->symbol_table);
    }
    
    if (analyzer->error_count > 0) {
        fprintf(stderr, "✗ Semantic analysis failed with %d error(s).\n", 
                analyzer->error_count);
        semantic_analyzer_destroy(analyzer);
        free_ast(ast_root);
        return 1;
    }
    
    printf("✓ Semantic analysis successful!\n");
    if (analyzer->warning_count > 0) {
        printf("  ⚠ %d warning(s)\n", analyzer->warning_count);
    }
    printf("\n");
    
    // ========== Phase 3: Code Generation ==========
    printf("[Phase 3/4] Code Generation (x86-64 assembly)...\n");
    
    FILE *out = fopen(output_file, "w");
    if (!out) {
        fprintf(stderr, "✗ Failed to open output file: %s\n", output_file);
        semantic_analyzer_destroy(analyzer);
        free_ast(ast_root);
        return 1;
    }
    
    CodeGenerator *gen = codegen_create(out, analyzer);
    generate_code(gen, ast_root);
    
    fclose(out);
    codegen_destroy(gen);
    
    printf("✓ Code generation successful!\n");
    printf("  → Generated: %s\n\n", output_file);
    
    // ========== Phase 4: Assembling and Linking ==========
    printf("[Phase 4/4] Assembling and Linking...\n");
    
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "gcc -no-pie %s -o output 2>&1", output_file);
    
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        fprintf(stderr, "✗ Failed to run assembler.\n");
        semantic_analyzer_destroy(analyzer);
        free_ast(ast_root);
        return 1;
    }
    
    char buffer[256];
    int has_errors = 0;
    while (fgets(buffer, sizeof(buffer), pipe)) {
        fprintf(stderr, "%s", buffer);
        has_errors = 1;
    }
    int status = pclose(pipe);
    
    if (status != 0 || has_errors) {
        fprintf(stderr, "✗ Assembly/linking failed.\n");
        semantic_analyzer_destroy(analyzer);
        free_ast(ast_root);
        return 1;
    }
    
    printf("✓ Assembly and linking successful!\n");
    printf("  → Generated: output (executable)\n\n");
    
    // ========== Complete ==========
    printf("🎉 Compilation successful!\n\n");
    printf("Run your program:\n");
    printf("  ./output\n");
    printf("  echo $?    # Check exit code\n");
    
    // Cleanup
    semantic_analyzer_destroy(analyzer);
    free_ast(ast_root);
    
    return 0;
}
