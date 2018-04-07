SRC_DIR=.
SRCS=$(shell find $(SRC_DIR) -name '*.cpp')
OBJS=$(subst .cpp,.o,$(SRCS))
TARGET=NotifcationIcon.exe

INCLUDE=../include
CPPFLAGS=-DNTDDI_VERSION=0x06020000 -D_WIN32_WINNT=0x0602 -DWINVER=0x0602 -D_WIN32_IE=0x0A00
CPPFLAGS+=$(foreach d, $(INCLUDE), -I$d)
LIBDIR=../lib
LIBS=-lNotifcationIcon -lole32
LDFLAGS=$(foreach d, $(LIBDIR), -L$d) $(LIBS) -Wl,--subsystem,windows

CPP=g++

.PHONY: default all tidy clean

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