-include common.mk

LIB_SRC:=$(wildcard lib/*.cpp) # cree une liste de tous les fichiers source de la lib
LIB_OBJ:=$(subst lib,lib,$(LIB_SRC:.cpp=.o))

EXEC_SRC:=$(wildcard exec/*.cpp)
EXEC_OBJ:=$(subst exec,exec,$(EXEC_SRC:.cpp=.o))

all: exec.out

exec.out: lib/libex.a $(EXEC_OBJ)
	@echo "Built exec"
	avr-g++ $^ -o $@ -l ex -L lib

lib/libex.a: $(LIB_OBJ)
	@echo "Built lib"
	avr-ar -rcs $@ $^

$(LIB_OBJ): %.o : %.cpp
	avr-g++ -o $@ -c $< $(FLAGS) $(INCLUDES)

$(EXEC_OBJ): %.o : %.cpp
	avr-g++ -o $@ -c $< $(FLAGS) $(INCLUDES)

-include lib/*.d
-include exec/*.d

clean:
	@rm -f lib/*.a lib/*.o  lib/*.d
	@rm -f exec/*.o exec/*.d
	@rm -f *.out
