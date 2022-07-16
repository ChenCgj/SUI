# make -- build the static lib
# make test -- build the test program

name = sui
test_name = sui_test

OBJ_DIR = ./obj
BUILD_DIR = ./build
SOURCE_DIR = ./source
INCLUDE_DIR = /usr/include/SDL2 /usr/local/include/SDL2 ./include $(SOURCE_DIR)/include
TEST_DIR = test

INCLUDE_FLAGS = $(addprefix -I, $(INCLUDE_DIR))

LDFLAGS = -L$(BUILD_DIR) -l$(name) -lSDL2 -lSDL2_ttf -lSDL2_image
CXXFLAGS = -g -Wall $(INCLUDE_FLAGS)
ARFLAGS = rcsv

AR = ar
CXX = g++

srcfile = $(patsubst $(SOURCE_DIR)/%, %, $(wildcard $(SOURCE_DIR)/**/*.cpp))

target : init_folder $(BUILD_DIR)/lib$(name).a

$(BUILD_DIR)/lib$(name).a : $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(srcfile))
	$(AR) $(ARFLAGS) $@ $(OBJ_DIR)/*.o

$(OBJ_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $(OBJ_DIR)/$(notdir $@) $^

$(TEST_DIR)/$(test_name) : $(TEST_DIR)/*.cpp
	$(CXX) -g -Wall -Iinclude -o $@ $^ $(LDFLAGS) -no-pie

.PHONY : init_folder clean test

test : init_folder $(BUILD_DIR)/lib$(name).a $(TEST_DIR)/$(test_name)

init_folder :
	-mkdir $(OBJ_DIR)
	-mkdir $(BUILD_DIR)

clean:
	-rm -rf $(OBJ_DIR)
	-rm -rf $(BUILD_DIR)
	-rm $(TEST_DIR)/$(test_name)
