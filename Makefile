COMPILER=gcc
OPTIONS=-g

# The following line commented (will not be used) and left to show other gcc options
#OPTIONS=-g -std=c++17 -pedantic -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code

COMPILE=$(COMPILER) $(OPTIONS)
BUILD=build

# Update the following with your program name
PROGRAM=PMan

# Compile main by default
all: program

# $(BUILD)/*.o expands to all .o files in the $(BUILD) directory
# In this case, we'll get $(BUILD)/file1.o $(BUILD)/file2.o
#   $@ expands to the target name, i.e. target1
#   $< expands to the first dependency, i.e. dependency1
#   $^ expands to the complete list of dependencies, i.e. dependency1 dependency2


program: PMan.c $(BUILD)/linkedlist.o $(BUILD)/emalloc.o
	$(COMPILE) $< $(BUILD)/*.o -o $(PROGRAM) -lreadline


$(BUILD)/linkedlist.o: linkedlist.c linkedlist.h build
	$(COMPILE) -c $< -o $@

$(BUILD)/emalloc.o: emalloc.c emalloc.h  build
	$(COMPILE) -c $< -o $@

# Make the build directory if it doesn't exist
build:
	mkdir -p $(BUILD)

# Delete the build directory and program
clean:
	rm -rf $(BUILD) program *.bak

# These rules do not correspond to a specific file
.PHONY: build clean