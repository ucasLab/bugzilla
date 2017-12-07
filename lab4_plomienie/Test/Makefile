NAME = test
ARCH = arm

OBJECTS=test.o

CROSS_COMPILE = arm-none-eabi-
CC	= $(CROSS_COMPILE)gcc
LD	= $(CROSS_COMPILE)ld -v
AR	= $(CROSS_COMPILE)ar
AS	= $(CROSS_COMPILE)as
CP	= $(CROSS_COMPILE)objcopy
OD	= $(CROSS_COMPILE)objdump

LIBS =
CFLAGS	= -mthumb-interwork -mthumb -marm -march=armv5te -mcpu=arm926ej-s -fno-strict-aliasing -mtune=arm926ej-s -msoft-float -g3 -Wall
AFLAGS	= -march=armv5te -mcpu=arm926ej-s -g3
LFLAGS	= -T test.ld 
CPFLAGS	= -O binary
ODFLAGS	=

all: test
	@echo "All done"

clean:
	-rm $(OBJECTS)
	@echo "Clean done"
test: $(OBJECTS)
	$(LD) $(LFLAGS) $(OBJECTS) $(LIBS) -o test
	$(CP) $(CPFLAGS) test test.bin	

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.S.o:
	$(AS) $(AFLAGS) $< -o $@

	
