XENO_DESTDIR:=
XENO_CONFIG:=$(XENO_DESTDIR)/usr/bin/xeno-config
XENO_CFLAGS:=$(shell DESTDIR=$(XENO_DESTDIR) $(XENO_CONFIG) --skin native --cflags)
XENO_LIBS:=$(shell DESTDIR=$(XENO_DESTDIR) $(XENO_CONFIG) --skin native --ldflags)


TARGET = b
CFLAGS = -g -Wall -D_GNU_SOURCE $(XENO_CFLAGS)
LDFLAGS = -g -lcomedi -lrt -lm -pthread $(XENO_LIBS)
SOURCES = $(shell find -name "*.c")
all: $(TARGET)
OBJECTS = $(SOURCES:.c=.o)
$(TARGET): $(OBJECTS)
	gcc -o $@ $^ $(LDFLAGS)
%.o : %.c
	gcc $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(TARGET) $(OBJECTS)
