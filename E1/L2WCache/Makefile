CC = gcc
CFLAGS=-Wall -Wextra
TARGET=L2WCache
TARGET2=TestL2WCache

all:
	$(CC) $(CFLAGS) L2WCache.c L2WCache.h SimpleProgram2W.c Cache.h -o $(TARGET)
	$(CC) $(CFLAGS) L2WCache.c L2WCache.h TestL2W.c Cache.h -o $(TARGET2)

run: $(TARGET)
	./$(TARGET)

run1: $(TARGET)
	./$(TARGET)
	
run2: $(TARGET2)
	./$(TARGET2)
		
clean:
	rm $(TARGET)