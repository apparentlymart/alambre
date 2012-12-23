
GNU_TOOLS_PREFIX=
CXX=$(GNU_TOOLS_PREFIX)g++
AR=$(GNU_TOOLS_PREFIX)ar
GCC_MACHINE:=$(shell $(CXX) -dumpmachine)
OBJ_DIR=obj/$(GCC_MACHINE)
LIB_DIR=lib/$(GCC_MACHINE)
INCLUDE_DIR=include/
LIB_FILE=$(LIB_DIR)/libtenacious.a
SOURCE_FILES=$(shell find src -type f -name '*.cpp')
HEADER_FILES=$(shell find $(INCLUDE_DIR) -type f -name '*.h')
OBJ_FILES=$(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SOURCE_FILES))
ifeq ($(GCC_MACHINE),avr)
ifneq ($(MCU),)
	CXX_OPTS=-mmcu=$(MCU)
	GCC_MACHINE:=$(shell $(CXX) -dumpmachine)-$(MCU)
else
	ERR:=$(error Must set MCU when building for avr targets)
endif
endif
DOCS_PYTHON_ENV=docs/pythonenv

# This is where it ends up if you install the libgtest-dev Debian package.
# If you're not on Debian then you may wish to override this.
GTEST_ROOT=/usr/src/gtest

all: $(LIB_DIR)/libtenacious.a

$(OBJ_DIR)/%.o: src/%.cpp $(OBJ_DIR) $(HEADER_FILES)
	@mkdir -p $(@D)
	$(CXX) -I$(INCLUDE_DIR) -O3 $(CXX_OPTS) -c $< -o $@

$(LIB_FILE)/: $(OBJ_FILES) $(LIB_DIR)
	rm -f $@
	$(AR) rcs $@ $(OBJ_FILES)

$(OBJ_DIR):
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	[ -d $(LIB_DIR) ] || mkdir -p $(LIB_DIR)

tests.run: tests/*.cpp $(LIB_FILE)
	@g++ $(CXX_OPTS) -I$(GTEST_ROOT) -Itests -Iinclude tests/*.cpp $(GTEST_ROOT)/src/gtest-all.cc $(GTEST_ROOT)/src/gtest_main.cc -lpthread -lgmock $(LIB_FILE) -o tests.run

test: tests/* $(LIB_FILE) tests.run
	@./tests.run

show_config:
	@echo Building for $(GCC_MACHINE)
	@echo Header files are $(HEADER_FILES)
	@echo Source files are $(SOURCE_FILES)
	@echo Object files are $(OBJ_FILES)
	@echo Building with $(CXX) $(CXX_OPTS)
	@echo Will generate library at $(LIB_FILE)

docs: $(DOCS_PYTHON_ENV)/bin/sphinx-build
	doxygen
	$(DOCS_PYTHON_ENV)/bin/sphinx-build docs/source docs/build

$(DOCS_PYTHON_ENV)/bin/sphinx-build: $(DOCS_PYTHON_ENV)
	$(DOCS_PYTHON_ENV)/bin/pip install docutils sphinx breathe

$(DOCS_PYTHON_ENV):
	virtualenv $(DOCS_PYTHON_ENV)

# This is here primarily for the benefit of the Travis-CI build,
# to force a build of some AVR variants of the library as well
# as run the tests on a native build.
build_and_test:
	make GNU_TOOLS_PREFIX=avr- MCU=atmega328p
	make test

.PHONY: all test show_config docs build_and_test
