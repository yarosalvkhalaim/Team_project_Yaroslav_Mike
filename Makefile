CC = clang

TARGET = qr_scanner

SRC_DIR = src
INC_DIR = -Iinclude

ifeq ($(OS),Windows_NT)
	TARGET := $(TARGET).exe
	# Використовуємо del у Windows
	RM = del /Q /F
else
	# Використовуємо rm -f у *nix (Linux, macOS)
	RM = rm -f
endif


SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,%.o,$(SRC))

PKG_CFLAGS = $(shell pkg-config --cflags zbar opencv)
PKG_LIBS = $(shell pkg-config --libs zbar opencv)

CFLAGS = -Wall -g -Wno-unused-function $(INC_DIR) $(PKG_CFLAGS)

LDFLAGS = $(PKG_LIBS) -Wl,-rpath,/opt/homebrew/lib -Wl,-rpath,/usr/local/lib

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Linking executable: $@"
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: $(SRC_DIR)/%.c
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning up..."
	$(RM) $(TARGET) $(OBJ)