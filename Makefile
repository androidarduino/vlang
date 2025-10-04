# C Compiler Project Makefile
# Author: George
# Date: 2025-10-04

# Compiler and flags
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lfl

# Directories
SRC_DIR = src
BUILD_DIR = build
INC_DIR = include
BIN_DIR = .

# Source files
LEXER_SRC = $(SRC_DIR)/lexer/lexer.l
PARSER_SRC = $(SRC_DIR)/parser/parser.y
AST_SRC = $(SRC_DIR)/ast/ast.c
PREPROCESSOR_SRC = $(SRC_DIR)/preprocessor/preprocessor.c
SEMANTIC_SRC = $(SRC_DIR)/semantic/types.c \
               $(SRC_DIR)/semantic/symbol_table.c \
               $(SRC_DIR)/semantic/semantic.c
CODEGEN_SRC = $(SRC_DIR)/codegen/codegen.c
MAIN_SRC = $(SRC_DIR)/main.c

# Generated files
LEXER_GEN = $(BUILD_DIR)/lex.yy.c
PARSER_GEN = $(BUILD_DIR)/y.tab.c
PARSER_HDR = $(BUILD_DIR)/y.tab.h

# Object files
OBJS = $(BUILD_DIR)/lex.yy.o \
       $(BUILD_DIR)/y.tab.o \
       $(BUILD_DIR)/ast.o \
       $(BUILD_DIR)/preprocessor.o \
       $(BUILD_DIR)/types.o \
       $(BUILD_DIR)/symbol_table.o \
       $(BUILD_DIR)/semantic.o \
       $(BUILD_DIR)/codegen.o \
       $(BUILD_DIR)/main.o

# Target executable
TARGET = $(BIN_DIR)/vc

# Colors for output
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

.PHONY: all clean test help install

# Default target
all: $(TARGET)
	@echo "$(GREEN)✓ Build complete!$(NC)"
	@echo "$(YELLOW)Run './vc <file.c>' to compile a C program$(NC)"

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Generate parser files
$(PARSER_GEN) $(PARSER_HDR): $(PARSER_SRC) | $(BUILD_DIR)
	@echo "Generating parser..."
	$(BISON) -y -d -o $(PARSER_GEN) $(PARSER_SRC)

# Generate lexer file
$(LEXER_GEN): $(LEXER_SRC) $(PARSER_HDR) | $(BUILD_DIR)
	@echo "Generating lexer..."
	$(FLEX) -o $(LEXER_GEN) $(LEXER_SRC)

# Compile lexer
$(BUILD_DIR)/lex.yy.o: $(LEXER_GEN)
	@echo "Compiling lexer..."
	$(CC) $(CFLAGS) -I$(BUILD_DIR) -c $< -o $@

# Compile parser
$(BUILD_DIR)/y.tab.o: $(PARSER_GEN)
	@echo "Compiling parser..."
	$(CC) $(CFLAGS) -I$(BUILD_DIR) -c $< -o $@

# Compile AST
$(BUILD_DIR)/ast.o: $(AST_SRC)
	@echo "Compiling AST..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile preprocessor
$(BUILD_DIR)/preprocessor.o: $(PREPROCESSOR_SRC)
	@echo "Compiling preprocessor..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile types
$(BUILD_DIR)/types.o: $(SRC_DIR)/semantic/types.c
	@echo "Compiling types..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile symbol table
$(BUILD_DIR)/symbol_table.o: $(SRC_DIR)/semantic/symbol_table.c
	@echo "Compiling symbol table..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile semantic analyzer
$(BUILD_DIR)/semantic.o: $(SRC_DIR)/semantic/semantic.c
	@echo "Compiling semantic analyzer..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile code generator
$(BUILD_DIR)/codegen.o: $(CODEGEN_SRC)
	@echo "Compiling code generator..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main
$(BUILD_DIR)/main.o: $(MAIN_SRC)
	@echo "Compiling main..."
	$(CC) $(CFLAGS) -I$(BUILD_DIR) -c $< -o $@

# Link everything
$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	rm -f output output.s
	@echo "$(GREEN)✓ Clean complete$(NC)"

# Run tests
test: $(TARGET)
	@echo "$(YELLOW)Running tests...$(NC)"
	@echo ""
	@echo "Test 1: Simple return"
	@echo 'int main() { return 42; }' | ./vc /dev/stdin -o test1.s
	@./output; echo "Exit code: $$?"
	@echo ""
	@echo "Test 2: Variables and arithmetic"
	@echo 'int main() { int x; int y; x = 10; y = 5; return x + y; }' | ./vc /dev/stdin -o test2.s
	@./output; echo "Exit code: $$?"
	@echo ""
	@echo "$(GREEN)✓ Tests complete$(NC)"
	@rm -f test1.s test2.s output

# Show help
help:
	@echo "C Compiler - Makefile Help"
	@echo ""
	@echo "Targets:"
	@echo "  all       Build the compiler (default)"
	@echo "  clean     Remove build artifacts"
	@echo "  test      Run basic tests"
	@echo "  help      Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make           # Build the compiler"
	@echo "  make clean     # Clean build files"
	@echo "  make test      # Run tests"
	@echo ""
	@echo "Compiler usage:"
	@echo "  ./vc program.c              # Compile program.c"
	@echo "  ./vc -o out.s program.c     # Specify output file"
	@echo "  ./vc --debug program.c      # Enable debug output"

# Install (copy to system directory)
install: $(TARGET)
	@echo "Installing compiler to /usr/local/bin..."
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "$(GREEN)✓ Installation complete$(NC)"
	@echo "You can now run 'vc <file.c>' from anywhere"

# Development helpers
rebuild: clean all

.SUFFIXES:
