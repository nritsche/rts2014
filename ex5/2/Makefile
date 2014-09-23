TARGET = busy-wait
CFLAGS = -g -Wall
LDFLAGS = -g -lcomedi -lrt -lm -pthread
SOURCES = $(shell find -name "*.c")
all: $(TARGET)
OBJECTS = $(SOURCES:.c=.o)
$(TARGET): $(OBJECTS)
	gcc -o $@ $^ $(LDFLAGS)
%.o : %.c
	gcc $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(TARGET) $(OBJECTS)

