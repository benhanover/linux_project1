# MAkefile that create will run all of our programs
CC = g++
LIBS = libutility.so

# SRCS will hold an array of all the .c files
SRCS :=$(subst ./,,$(shell find . -name "*.c"))

# OBJS will hold an array of the corresponding .out to the .c files.
OBJS :=$(patsubst %.c,%.out,$(SRCS))

# all will be the default TARGET and will create all the object files
all: $(OBJS)

#  This rule will be executed for every .out file present in the target "all"
%.out: %.c
	$(CC) $^ -o $@ $(LIBS)

clean:
#need to remove all the dirs.
	rm arrivels.out full_schedule.out airplane.out regenerate.out

.PHONYL: all clean
