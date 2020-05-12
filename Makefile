#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
#
CC = gcc
CFLAGS   = -O3 -march=native -mtune=native -MMD -MP
INCLUDES = -I/usr/local/include -I/usr/local/include/plibsys
LFLAGS   = -L/usr/local/lib 
LIBS     = -lm -lrt -lpthread -lplibsysstatic -lsofa_c 
SRCS     = acs_log.c acs_ini.c acs_net.c acs_ast.c acs_jsn.c acs_chk.c acs_cmd.c acs_utl.c 
OBJS     = $(SRCS:.c=.o)
DEPS     = $(OBJS:.o=.d)
ACS      = acs 
TXCMD    = txcmd 

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: clean

all:    $(ACS) $(TXCMD) 
	@echo Done  

$(ACS): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) acs.c -o $(ACS) $(OBJS) $(LFLAGS) $(LIBS)

$(TXCMD): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) txcmd.c -o $(TXCMD) acs_utl.o $(LFLAGS) $(LIBS)

-include $(DEPS) 

# Compile sources  
#.c.o:
%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@  


# Cleanup 
clean:
	$(RM) *.o *.d $(ACS) $(TXCMD)

# Don't build dependencies
#depend: $(SRCS)
#	makedepend $(INCLUDES) $^

