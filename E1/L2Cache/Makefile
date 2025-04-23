CC = gcc
CFLAGS=-Wall -Wextra
TARGET=L2Cache
TARGET2=TestL2Cache

all:
	$(CC) $(CFLAGS) L2Cache.c L2Cache.h SimpleProgram2.c Cache.h -o $(TARGET)
	$(CC) $(CFLAGS) L2Cache.c L2Cache.h TestL2.c Cache.h -o $(TARGET2)

run: $(TARGET)
	./$(TARGET)

run1: $(TARGET)
	./$(TARGET)
	
run2: $(TARGET2)
	./$(TARGET2)
	
clean:
	rm $(TARGET)