V=1
SOURCE_DIR=src
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

all: glxgears.z64
.PHONY: all

OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/gears.o

glxgears.z64: N64_ROM_TITLE="GLXGears"

$(BUILD_DIR)/glxgears.elf: $(OBJS)

clean:
	rm -f $(BUILD_DIR)/* *.z64
.PHONY: clean

bear: clean
	bear make

-include $(wildcard $(BUILD_DIR)/*.d)