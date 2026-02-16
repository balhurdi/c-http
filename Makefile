CC=gcc

C_FILES= src/main.c
BUILD_DIR=build

default: $(BUILD_DIR) $(BUILD_DIR)/server

$(BUILD_DIR)/server.o: $(C_FILES)
	$(CC) -c $(C_FILES) -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/server: $(BUILD_DIR)/server.o
	$(CC) $(BUILD_DIR)/server.o -o $(BUILD_DIR)/server

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	-rm -rf $(BUILD_DIR)
	
