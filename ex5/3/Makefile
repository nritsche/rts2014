TARGET = periodic
CFLAGS = -g -Wall -D_GNU_SOURCE
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

