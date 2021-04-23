
#Makefile

CC     = gcc
INCL   += -I$(WAMR_DIR)/core/iwasm/include
INCL   += -I$(WAMR_DIR)/core/iwasm/common
CFLAGS = -g -c -Wall $(INCL)
LDFLAGS = -Wl,-rpath,$(WAMR_DIR)/product-mini/platforms/linux/build
LDLIBS = -lm -L$(WAMR_DIR)/product-mini/platforms/linux/build -liwasm 

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

TARGET = wafle

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

	
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)


.PHONY: clean

clean:
	@echo "Cleaning object files and executable..."
	@/bin/rm -f *.o *.d $(TARGET) Makefile.new

-include $(DEPS)
