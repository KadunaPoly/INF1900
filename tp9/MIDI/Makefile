-include common.mk

#CONFIGURATIONS
CC=avr-gcc
OBJCOPY=avr-objcopy
AVRDUDE=avrdude
OPTLEVEL=s
MCU=atmega324pa
AVRDUDE_PROGRAMMERID=usbasp
HEXFORMAT=ihex

#FLAGS
CFLAGS=-I$(INC_DIR) -MMD -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall   
CXXFLAGS=-fno-exceptions  
LDFLAGS=-Wl,-Map,$(trg).map -mmcu=$(MCU)

#SOURCE FILES
EXEC_DIR=exec
LIB_DIR=./lib
LIB=ex
INC_DIR=./include
src=$(wildcard $(EXEC_DIR)/*.cpp)
obj=$(src:.cpp=.o)
trg=main.out

HEXROMTRG=main.hex
HEXTRG=$(HEXROMTRG) main.ee.hex

REMOVE=rm -f

.PHONY:all
all:lib compile

debug: $(CXXFLAGS) += -DDEBUG -g
debug: $(CFLAGS) += -DDEBUG -g
debug: lib compile

.PHONY:compile
compile:$(trg)
$(trg): $(obj)
	$(CC) $(LDFLAGS) -o $(trg) $(obj) -L$(LIB_DIR) -l$(LIB)

$(obj):$(src)
	$(CC) $(CFLAGS) $(CXXFLAGS) $(FLAGS) -I$(INC_DIR) -c -o $@ $<


# Production des fichiers hex a partir des fichiers elf
%.hex: %.out
	$(OBJCOPY) -j .text -j .data \
		-O $(HEXFORMAT) $< $@



.PHONY:lib
lib:
	cd $(LIB_DIR) && $(MAKE)




# Verification des dependances (header dependencies)
-include *.d

# Pour plus d'information sur cette section, consulter:
# http://bit.ly/2580FU8



# Make install permet de compiler le projet puis
# d'ecrire le programme en memoire flash dans votre
# microcontroleur. Celui-ci doit etre branche par cable USB
.PHONY: install
install: $(HEXROMTRG)				
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	-p $(MCU) -P -e -U flash:w:$(HEXROMTRG)





.PHONY:vars
vars:
	@echo "src = $(src)"
	@echo "obj = $(obj)"
	@echo "trg = $(trg)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "HEXROM = $(HEXROMTRG)"

.PHONY:list
list:
	@grep '^[^#[:space:]].*:' Makefile

.PHONY:clean
clean:
	$(REMOVE) $(trg) $(trg).map $(obj) $(HEXTRG) *.d
	cd $(LIB_DIR) && $(MAKE) clean
