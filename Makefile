
#Makefile

CC     = gcc
INCL   = -I$(shell $(WASMER_DIR)/bin/wasmer config --includedir)
CFLAGS = -g -c -Wall $(INCL)
LDFLAGS = -Wl,-rpath,$(shell $(WASMER_DIR)/bin/wasmer config --libdir)
LDLIBS = -lm $(shell $(WASMER_DIR)/bin/wasmer config --libs)


CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)

TARGET = project

	
$(TARGET): $(OFILES)
	$(CC) $(OFILES) -o $(TARGET) $(LDFLAGS) $(LDLIBS)


# Separate compilation of object modules
main.o: main.c
	$(CC) -c main.c $(CFLAGS)


# "make depend" : Updates list of dependecies
# "make clean"  : Cleans object files and executable 
#################################################

depend:
	@echo "Updating dependencies..."
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CC) -MM $(CFLAGS) $(CFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo "Cleaning object files and executable..."
	@/bin/rm -f *.o project Makefile.new

#Depency rules
#####################################################
# DO NOT DELETE THIS LINE
main.o: main.c /home/lolo/.wasmer/include/wasmer.h
