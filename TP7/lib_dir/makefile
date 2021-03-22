EXEC=lib.a
all : $(EXEC)
$(EXEC) : main.o
	avr-ar -crs lib.a *.o
main.o : main.cpp fonction1.h fonction1.cpp
	avr-g++ -o main.o -c main.cpp fonction1.cpp