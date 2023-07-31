CC := g++
CFLAGS := -Wall -Wextra -Iinclude -static-libstdc++

SRC_DIR := source
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := MeleeSettingsFixer

#GOOFY AHHHHHHH CHATGPT CODE
DLLS := libgcc_s_seh-1.dll libstdc++-6.dll

.PHONY: all clean

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

#HAHAHAHH WHAT??? CHATGPT CODE 
package: $(TARGET)
	powershell Compress-Archive -Path $(TARGET).exe, libgcc_s_seh-1.dll, libstdc++-6.dll -DestinationPath $(TARGET).zip

clean:
	rm -rf $(BUILD_DIR) $(TARGET)