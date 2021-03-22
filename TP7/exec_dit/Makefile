
LIB_DIR:=$(lib_dir/)
LIB_SRC:=$(wildcard $(LIB_DIR)*.cpp)
LIB_OBJ:=$(subst lib_dir, lib_dir, $(LIB_SRC:.cpp=.o))

EXEC_SRC := $(wildcard exec_dit/*.cpp)
EXEC_OBJ := $(subst exec, exec, $(EXEC_SRC:.cpp=.o))

all: exec.out 

exec.out: lib.a $(EXEC_OBJ)
	avr-g++ $^ 

$(LIB_DIR)lib.a: $(LIB_OBJ)	
	avr-ar -rcs $@ $^

$(LIB_OBJ): %.o : %.cpp
	avr-g++ -o $@ -c $^ -I$(LIB_DIR)

$(EXEC_OBJ): %.o : %.cpp
	avr-g++ -o $@ -c $^ -I$(LIB_DIR)