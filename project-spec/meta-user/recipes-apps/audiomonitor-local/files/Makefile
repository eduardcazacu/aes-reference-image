# Output file
TARGET = audiomonitor-local

# Aplication directories src for .c files, inc for .h files
SRC_DIR = src
INC_DIR = inc

# Setting compiler flags
CFLAGS = -I$(INC_DIR)
LDLIBS = -lm -lcpprest -lpthread -lboost_system -lssl -lcrypto -lstdc++
SIZE = $(ARCH)-size

# Build #
###################################################################################################

# Listing all to be compiled *.c files
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Setting buid directory paths
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
   
# Listing object files from source files
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
     
# This is the default target if the user just calls 'make'
all: build
     
# Build all the files
build: builddirs $(BUILD_DIR)/$(TARGET)
     
# Create the required directories (if not already existing)
builddirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)
	
# Link everything together
$(BUILD_DIR)/$(TARGET): $(OBJ)
	@echo Linking $@
	$(CXX) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(TARGET)

# Compile c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo Compiling $^
	@$(CC) $(CFLAGS) -c $^ -o $@
     
# Clean up
clean:
	@rm -rf $(BUILD_DIR) $(OBJ) $(TARGET) $(TARGET).* *.a *.o *~
	@echo Done
     
# Clean must be a phony target so make knows this never exists as a file
.PHONY: clean
