TARGET=ascontrol

CFILES=$(wildcard src/*.c)
OFILES=$(patsubst %.c, %.o, $(CFILES))

PCLIBS=gtk+-2.0 webkit-1.0
LIBS=

CFLAGS=-std=c99 -g -Wall -Werror \
       $(shell pkg-config --cflags $(PCLIBS))

LDFLAGS=$(shell pkg-config --libs $(PCLIBS)) \
        $(addprefix -l,$(LIBS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OFILES)
	$(CC) -o $@ $(OFILES) $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OFILES)

