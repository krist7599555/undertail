OBJS = ./main.cpp
CC = g++

INCLUDE_PATHS = -I/usr/local/include
LIBRARY_PATHS = -L/usr/local/lib
COMPILER_FLAGS = -std=c++17 -Wall -O2
LINKER_FLAGS = -lSDL2 -lSDL2_image
OBJ_NAME = exe_name

all:
	$(CC) -o $(OBJ_NAME) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(OBJS)
	./$(OBJ_NAME)

intel:
	g++ -masm=intel -S ./main.c