TARGET = test
CFLAGS = -g -Wall
LDFLAGS = -g -lm -pthread
SOURCES = $(shell find -name "*.c")
all: $(TARGET)
OBJECTS = $(SOURCES:.c=.o)
$(TARGET): $(OBJECTS)
	/home/student/p/project/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc -o $@ $^ $(LDFLAGS)
	cp test /export/nfs/root/projekt/
%.o : %.c
	/home/student/p/project/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(TARGET) $(OBJECTS)

