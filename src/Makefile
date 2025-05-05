# Makefile

# 编译器和编译选项
CXX      := g++
CXXFLAGS := -std=c++17 -O3 -fopenmp -Wall -Wextra -Iinclude -Wno-free-nonheap-object

# 源文件路径、头文件路径、输出路径
SRC_DIR  := src
INC_DIR  := include
BUILD_DIR:= build

# 源文件列表
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TARGET   := $(BUILD_DIR)/parallel

# 伪目标
.PHONY: all clean dir run

all: dir $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lgomp
	@echo "Build complete: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

dir:
	@mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)/*
	@echo "Build cleaned."

# 可选：单独运行测试
run: $(TARGET)
	@./$(TARGET)