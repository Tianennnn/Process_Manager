COMPILER=gcc
OPTIONS=-g

COMPILE=$(COMPILER) $(OPTIONS)
BUILD=build

PROGRAM=PMan

all: program

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

.PHONY: build clean
