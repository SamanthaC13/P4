CC=gcc
CFLAGS = -g 
SOURCES = driver.c parser.c scanner.c codegen.c 
OBJECTS = driver.o parser.o scanner.o codegen. o
DEP = parser.h scanner.h token.h codegen.h
TARGET = compfs
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS)-o $@ $^
%.o : %.c $(DEP)
	$(CC) $(CFLAGS) -c -o $@ $< 
clean:
	rm -f *.o 
