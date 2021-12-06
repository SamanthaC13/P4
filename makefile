CC=gcc
CFLAGS = -g 
SOURCES = driver.c parser.c scanner.c statsem.c
OBJECTS = driver.o parser.o scanner.o statsem.o
DEP = parser.h scanner.h token.h statsem.h
TARGET = statSem
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS)-o $@ $^
%.o : %.c $(DEP)
	$(CC) $(CFLAGS) -c -o $@ $< 
clean:
	rm -f *.o 
