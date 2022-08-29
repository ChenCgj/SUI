# make -- build the static lib
# make test -- build the test program
# make runner -- build the runner game
# make demo -- build a demo

name = sui
test_name = sui_test
runner_name = runner
demo_name = Space_impact

OBJ_DIR = ./obj
BUILD_DIR = ./build
SOURCE_DIR = ./source

BASE = base
LAYOUT = layout
MAIN = main
UI = ui
UTILS = utils
WINDOW = window

INCLUDE_DIR = /usr/include/SDL2 /usr/local/include/SDL2 ./include $(SOURCE_DIR)/include
TEST_DIR = test
RUNNER_DIR = runner
DEMO_DIR = Space_Impact

INCLUDE_FLAGS = $(addprefix -I, $(INCLUDE_DIR))

LDFLAGS = -L$(BUILD_DIR) -l$(name) -lSDL2 -lSDL2_ttf -lSDL2_image
CXXFLAGS = -g -Wall $(INCLUDE_FLAGS)
ARFLAGS = rcsv

AR = ar
CXX = g++

srcfile = $(patsubst $(SOURCE_DIR)/%, %, $(wildcard $(SOURCE_DIR)/**/*.cpp))

target : init_folder $(BUILD_DIR)/lib$(name).a

$(BUILD_DIR)/lib$(name).a : $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(srcfile))
	$(AR) $(ARFLAGS) $@ $(OBJ_DIR)/**/*.o

$(OBJ_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

$(TEST_DIR)/$(test_name) : $(TEST_DIR)/*.cpp
	$(CXX) -g -Wall -Iinclude -o $@ $^ $(LDFLAGS) -no-pie

$(RUNNER_DIR)/$(runner_name) : $(RUNNER_DIR)/*.cpp
	$(CXX) -g -Wall -Iinclude -o $@ $^ $(LDFLAGS) -no-pie

$(DEMO_DIR)/$(demo_name) : $(DEMO_DIR)/source/*.cpp
	$(CXX) -g -Wall -Iinclude -I$(DEMO_DIR)/include -o $@ $^ $(LDFLAGS) -no-pie

.PHONY : init_folder clean test runner demo

test : target $(TEST_DIR)/$(test_name)

runner : target $(RUNNER_DIR)/$(runner_name)

demo : target $(DEMO_DIR)/$(demo_name)

init_folder :
	-mkdir -p $(OBJ_DIR)/$(BASE)
	-mkdir -p $(OBJ_DIR)/$(LAYOUT)
	-mkdir -p $(OBJ_DIR)/$(MAIN)
	-mkdir -p $(OBJ_DIR)/$(UI)
	-mkdir -p $(OBJ_DIR)/$(UTILS)
	-mkdir -p $(OBJ_DIR)/$(WINDOW)
	-mkdir $(BUILD_DIR)

clean:
	-rm -rf $(OBJ_DIR)
	-rm -rf $(BUILD_DIR)
	-rm $(TEST_DIR)/$(test_name)
	-rm $(RUNNER_DIR)/$(runner_name)
	-rm $(DEMO_DIR)/$(demo_name)