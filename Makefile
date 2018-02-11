SRC_DIR=.
SRCS=$(shell find $(SRC_DIR) -name '*.cpp')
OBJS=$(subst .cpp,.o,$(SRCS))
TARGET=NotifcationIcon.exe

CPPFLAGS=-DNTDDI_VERSION=0x06020000 -D_WIN32_WINNT=0x0602 -DWINVER=0x0602 -D_WIN32_IE=0x0A00
CPP=g++
LIBS=-lole32
LDFLAGS=$(LIBS)

.PHONY: default all tidy clean maintainer-clean

default: all

all: $(TARGET)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CPP) $^ $(LDFLAGS) -o $@

tidy:
	rm -f $(OBJS)

clean: tidy
	rm -f $(TARGET)

maintainer-clean: dist-clean
	git clean -xdf