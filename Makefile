# variables like paths and files
include vars.make

# compilation variables, can be referred to by $(VARIABLENAME)
CXXFLAGS = -std=c++17
DEBUGFLAGS := -g $(CXXFLAGS) -Wall- Wextra -pedantic
VALGRINDFLAGS =  --leak-check=full --show-leak-kinds=all
# --trace-childen=yes

SFMLLIBS = $(patsubst %, -lsfml-%, graphics window system audio)

# platform specific settings
ifeq ($(OS),Windows_NT)
	RM=del /f /q
	EXECUTABLE:=$(EXECNAME).exe
else
	RM=rm -f
	EXECUTABLE:=$(EXECNAME).out
endif


# nonfile targets, avoids conflicts and increases performance
.PHONY: all clean run valgrind-run

# call "make <target[, target, ...]>" from CLI to build a target
# first target, all, is built if no targets are given 

# syntax is target:[ pattern:] dependencies 
# 	commands to run for building target
#
# 	Note that commands must be indented with a tab!
all: clean $(EXECUTABLE) run

daniel: daniel-$(EXECUTABLE) run

wsl: wslexport

$(EXECUTABLE):
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE) $(SFMLLIBS)

clean:
	$(RM) $(EXECUTABLE)

run: 
	@echo "Running \"$(EXECUTABLE)\""
	@./$(EXECUTABLE)

valgrind-run: $(EXECUTABLE)
	@valgrind $(VALGRINDFLAGS) ./$(EXECUTABLE)


# specific compliation for Daniel's enviroment
daniel-$(EXECUTABLE):
	# commands can be broken into multiple lines with backslash
	$(CXX) $(CXXFLAGS) $(SOURCES) -I$(DANIELSFMLPATH)/include -o $(EXECUTABLE) \
		-L$(DANIELSFMLPATH)/lib $(SFMLLIBS)

wslexport:
	@export DISPLAY=:0 && $(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE) $(SFMLLIBS) && ./$(EXECUTABLE)
