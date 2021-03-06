SRC_DIR=src
SRCS=$(shell find $(SRC_DIR) -name '*.cpp')
OBJS=$(subst .cpp,.o,$(SRCS))
TARGET_DIR=lib
TARGET=$(TARGET_DIR)/libNotifcationIcon.a

INCLUDE=include
# Minimum windows requirements: Windows Vista with IE 6
CPPFLAGS=-DNTDDI_VERSION=0x06000000 -D_WIN32_WINNT=0x0600 -DWINVER=0x0600 -D_WIN32_IE=0x0600
CPPFLAGS+=$(foreach d, $(INCLUDE), -I$d)

AR=ar
CPP=g++

.PHONY: default all tidy clean maintainer-clean

default: all

all: $(TARGET)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	mkdir -p $(TARGET_DIR)
	$(AR) rcs $@ $^

tidy:
	rm -f $(OBJS)

clean: tidy
	rm -rf $(TARGET_DIR)

maintainer-clean: dist-clean
	git clean -xdf