
GNU_TOOLS_PREFIX=
CXX=$(GNU_TOOLS_PREFIX)g++
AR=$(GNU_TOOLS_PREFIX)ar
GCC_MACHINE:=$(shell $(CXX) -dumpmachine)
OBJ_DIR=obj/$(GCC_MACHINE)
LIB_DIR=lib/$(GCC_MACHINE)
LIB_FILE=$(LIB_DIR)/libtenacious.a
SOURCE_FILES=$(wildcard src/*.cpp)
HEADER_FILES=$(wildcard include/*.h)
OBJ_FILES=$(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SOURCE_FILES))

# This is where it ends up if you install the libgtest-dev Debian package.
# If you're not on Debian then you may wish to override this.
GTEST_ROOT=/usr/src/gtest

all: $(LIB_DIR)/libtenacious.a

$(OBJ_DIR)/%.o: src/%.cpp $(OBJ_DIR) include/*.h
	$(CXX) -c $< -o $@

$(LIB_FILE)/: $(OBJ_FILES) $(LIB_DIR)
	rm -f $@
	$(AR) rcs $@ $(OBJ_FILES)

$(OBJ_DIR):
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	[ -d $(LIB_DIR) ] || mkdir -p $(LIB_DIR)

tests.run: tests/*.cpp $(LIB_FILE)
	@g++ -I$(GTEST_ROOT) -Iinclude tests/*.cpp $(GTEST_ROOT)/src/gtest-all.cc $(GTEST_ROOT)/src/gtest_main.cc -lpthread $(LIB_FILE) -o tests.run

test: tests/* $(LIB_FILE) tests.run
	@./tests.run

.PHONY: all test

