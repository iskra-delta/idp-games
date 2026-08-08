# IDP Games build.
#
# The host make starts one container; make inside the container performs the
# complete build with xcc and cpmdisk.

IMAGE		= wischner/xcc-z80-idp:latest
LOCAL_XCC	?= $(abspath ../../retro-vault/xyz/bin/x/bin/xcc)

ifeq ($(IN_CONTAINER),)

CONTAINER_WORKDIR	= /work
DOCKER_RUN		= docker run --rm \
					--user "$(shell id -u):$(shell id -g)" \
					-v "$(CURDIR):$(CONTAINER_WORKDIR)" \
					-v "$(LOCAL_XCC):/opt/x/bin/xcc:ro" \
					-w $(CONTAINER_WORKDIR) \
					$(IMAGE)

.PHONY: all com img clean
all com img:
	test -x "$(LOCAL_XCC)"
	$(DOCKER_RUN) make --no-print-directory IN_CONTAINER=1 $@

clean:
	rm -rf build bin

else

BUILD_DIR	= build
BIN_DIR		= bin
SRC_DIR		= src
PLATFORM	= cpm3
CFLAGS		= -Os -I$(SRC_DIR)
LDFLAGS		= --platform $(PLATFORM) --oformat=binary
SDK_LIBS	= -lsdk
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

.PHONY: com
com: $(COM_FILES) $(DATA_FILES)

.PHONY: img
img: $(DISK_IMAGE)

$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(@D)
	xcc --platform $(PLATFORM) $(CFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%.bin: %.bin | $(BIN_DIR)
	cp $< $@

$(BIN_DIR)/advent.com: $(ADVENT_OBJS) | $(BIN_DIR)
	xcc $(LDFLAGS) $(SDK_LIBS) -o $@ $^

define GRAPHICS_GAME_template
$(BIN_DIR)/$(1).com: $(BUILD_DIR)/$(1)/$(1).rel $(COMMON_OBJS) | $(BIN_DIR)
	xcc $(LDFLAGS) $(SDK_LIBS) -o $$@ $$^
endef

$(foreach game,$(GRAPHICS_GAMES),$(eval $(call GRAPHICS_GAME_template,$(game))))

$(DISK_IMAGE): $(COM_FILES) $(DATA_FILES) | $(BIN_DIR)
	rm -f $@
	cpmdisk create $@ $(DISK_TYPE)
	cpmdisk add $@ -u 0 $(COM_FILES) $(DATA_FILES)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

endif
