# IDP Games build.
#
# Uses the downloaded model-M X compilation suite for xcc/xas/xld.
# Uses the docker container for the Partner SDK and cpmdisk.

X_SUITE_URL		= https://github.com/retro-vault/xyz/releases/latest/download/x-m-linux.zip
X_SUITE_DIR		= tools/x-m-linux
X_BIN_DIR		= $(X_SUITE_DIR)/bin
XCC				= $(X_BIN_DIR)/xcc
XLD				= $(X_BIN_DIR)/xld

IMAGE			= wischner/xcc-z80-idp:latest
DOCKER_RUN_CPMDISK	= docker run --rm \
						--user "$(shell id -u):$(shell id -g)" \
						-v "$(CURDIR):$(CURDIR)" \
						-w $(CURDIR) \
						$(IMAGE)

.PHONY: all com img clean
com:
	make --no-print-directory all

# Check if X suite exists, if not download it (only for non-clean targets)
ifneq ($(MAKECMDGOALS),clean)
ifeq ($(wildcard $(XCC)),)
$(info Downloading X compilation suite...)
_ := $(shell mkdir -p $(X_SUITE_DIR) && cd tools && \
	curl -sL -o x-m-linux.zip "$(X_SUITE_URL)" && \
	unzip -q x-m-linux.zip && \
	rm -f x-m-linux.zip)
endif
endif

BUILD_DIR	= build
BIN_DIR		= bin
SRC_DIR		= src
PLATFORM	= cpm3
CFLAGS		= -Os -I$(SRC_DIR) -I/opt/idp/include
LDFLAGS		= --platform $(PLATFORM) --oformat=binary
SDK_LIBS	= -L/opt/idp/lib -lsdk
DISK_TYPE	= fdd
DISK_IMAGE	= $(BIN_DIR)/idp-games.img

GAMES		= advent frogger invaders tetris tetrisg
GRAPHICS_GAMES	= frogger invaders tetris tetrisg

COMMON_SRCS	= $(wildcard $(SRC_DIR)/common/*.c)
COMMON_OBJS	= $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.rel,$(COMMON_SRCS))
ADVENT_SRCS	= $(wildcard $(SRC_DIR)/advent/*.c)
ADVENT_OBJS	= $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.rel,$(ADVENT_SRCS))
HEADERS		= $(wildcard $(SRC_DIR)/*/*.h)
COM_FILES	= $(addprefix $(BIN_DIR)/,$(addsuffix .com,$(GAMES)))

DATA_SRCS	= $(wildcard $(SRC_DIR)/advent/*.bin $(SRC_DIR)/tetrisg/*.bin)
DATA_FILES	= $(addprefix $(BIN_DIR)/,$(notdir $(DATA_SRCS)))

vpath %.bin $(SRC_DIR)/advent $(SRC_DIR)/tetrisg

.PHONY: all
all: $(DISK_IMAGE)

.PHONY: img
img: $(DISK_IMAGE)

$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(@D)
	$(DOCKER_RUN_CPMDISK) $(XCC) --platform $(PLATFORM) $(CFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%.bin: %.bin | $(BIN_DIR)
	cp $< $@

$(BIN_DIR)/advent.com: $(ADVENT_OBJS) | $(BIN_DIR)
	$(DOCKER_RUN_CPMDISK) $(XCC) $(LDFLAGS) $(SDK_LIBS) -o $@ $^

define GRAPHICS_GAME_template
$(BIN_DIR)/$(1).com: $(BUILD_DIR)/$(1)/$(1).rel $(COMMON_OBJS) | $(BIN_DIR)
	$(DOCKER_RUN_CPMDISK) $(XCC) $(LDFLAGS) $(SDK_LIBS) -o $$@ $$^
endef

$(foreach game,$(GRAPHICS_GAMES),$(eval $(call GRAPHICS_GAME_template,$(game))))

$(DISK_IMAGE): $(COM_FILES) $(DATA_FILES) | $(BIN_DIR)
	rm -f $@
	$(DOCKER_RUN_CPMDISK) cpmdisk create $@ $(DISK_TYPE)
	$(DOCKER_RUN_CPMDISK) cpmdisk add $@ -u 0 $(COM_FILES) $(DATA_FILES)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
