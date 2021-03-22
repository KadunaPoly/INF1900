# Nom du microcontroleur cible
# (exemple: 'at90s8515')
MCU=atmega324pa

# Niveau d'optimization
# Utilisez s (size opt), 1, 2, 3 ou 0 (off)
OPTLEVEL=s

####### variables #######

#compilateur utilise
CC=avr-gcc
#pour copier le contenu d'un fichier objet vers un autre
OBJCOPY=avr-objcopy
#pour permettre le transfert vers le microcontroleur
AVRDUDE=avrdude
#pour supprimer les fichiers lorsque l'on appel make clean
REMOVE=rm -f
# HEXFORMAT -- format pour les fichiers produient .hex
HEXFORMAT=ihex

CFLAGS=-I. -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall                                        

# Flags pour le compilateur en C++
CXXFLAGS=-fno-exceptions     

# Linker pour lier les librairies utilisees
LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU)


HEXROMTRG=$(PROJECTNAME).hex
HEXTRG=$(HEXROMTRG) $(PROJECTNAME).ee.hex

CFILES=$(filter %.c, $(PRJSRC))
# Fichier C++
CPPFILES=$(filter %.cpp, $(PRJSRC))

INCLUDES=-I../include/

# Liste de tout les fichiers objet que nous devons creer
OBJDEPS=$(CFILES:.c=.o) \
	$(CPPFILES:.cpp=.o)


TRG=libex.a
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
	
.PHONY: all clean help

# Make all permet simplement de compiler le projet
#
$(TRG): $(OBJ)
	@echo "... making the library \n"
	$(AR) rcs $@ $^

%.o:%.cpp
	$(CC) -c $(CFLAGS) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)


# Make clean permet d'effacer tout les fichiers generes
# lors de la compilation
clean:
	$(REMOVE) $(TRG) *.o *.d
	@echo 'cleaning all the objects, .d and the lib.a ...'


help:
	
	
	@echo '  all       (=make) compile and link and create the library lib.a'
	@echo '  clean 	   clean .o , the library and .d '
	@echo '  help      print this message.'










