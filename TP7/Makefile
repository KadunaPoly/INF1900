#COMMENT STRUCTURE:

## Previous Code:
## New Code:
## Why:




## Previous Code:
# LIB_DIR:=$(/usagers3/thmes/inf1900-021941/commun/TP7/lib_dir/)
## New Code:
PROJ_DIR:=./
LIB_DIR:=$(PROJ_DIR)/TP3
LIB_NAME:=lib.a
## Why: Absolute path for Makefiles usually means bad compatibility across computers/systems.

## Previosu Code:
#LIB_SRC:=$(wildcard $(LIB_DIR)*.cpp)
#LIB_OBJ:=$(subst lib_dir, lib_dir, $(LIB_SRC:.cpp=.o))
## New Code:
## Why: We don't need these if they are already made into a library by the "lib" function
EXEC_SRC := $(wildcard exec_dit/*.cpp)
EXEC_OBJ := $(subst exec, exec, $(EXEC_SRC:.cpp=.o)) # subst exec, exec? :O

all: exec.out 

exec.out: lib.a $(EXEC_OBJ)
	avr-g++ $^ 

## Previous Code:
#$(LIB_DIR)lib.a: $(LIB_OBJ)	
#	avr-ar -rcs $@ $^
## New Code:
.PHONY:lib
lib:
	cd $(LIB_DIR) && $(MAKE)
## Why: This just changes directory into the lib_dir to use the makefile in that directory. Way easier to decompose problems.

$(LIB_OBJ): %.o : %.cpp
	avr-g++ -o $@ -c $^ -I$(LIB_DIR) -l$(LIB_NAME) #Added LIB_NAME so that the compiler knows to add a library -I is the directory to library, -l is the name of library

$(EXEC_OBJ): %.o : %.cpp
	avr-g++ -o $@ -c $^ -I$(LIB_DIR) -l$(LIB_NAME) #Added LIB_NAME so that the compiler knows to add a library -I is the directory to library, -l is the name of library