CC = gcc
CFLAGS=-Wall -Wextra
TARGET=L1Cache
TARGET2=TestL1Cache

all:
	$(CC) $(CFLAGS) L1Cache.c L1Cache.h SimpleProgram1.c Cache.h -o $(TARGET)
	$(CC) $(CFLAGS) L1Cache.c L1Cache.h TestL1.c Cache.h -o $(TARGET2)

run: $(TARGET)
	./$(TARGET)
	
run1: $(TARGET)
	./$(TARGET)
	
run2: $(TARGET2)
	./$(TARGET2)
	
clean:
	rm $(TARGET)