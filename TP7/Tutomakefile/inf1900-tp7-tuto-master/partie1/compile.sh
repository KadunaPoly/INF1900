#!/bin/bash
avr-g++ -c lib/*.cpp # compile les .cpp pour donner des .o
avr-ar -rcs lib.a *.o # assemble les .o en une archive
avr-g++ -c exec/*.cpp # compile main.cpp
avr-g++ main.o lib.a # assemble les objets et la librairie 
# donner un exec