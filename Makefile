CC = gcc
CFLAGS = -W -Wall -pedantic #-I/usr/include/ -I.
LDFLAGS = -lncurses
DEPS = # h. files needed
OBJ = main.o # main and support functions

%.o: %.c $(DEPS) 					# Turn .c files into .o files
	$(CC) $(CFLAGS) -c -o $@ $<

conway: $(OBJ)						# Compile OBJ files, the executable is 'conway'
	$(CC)  -o $@ $^  $(LDFLAGS) $(CFLAGS)

clean:								# Remove
	rm -f $(OBJ) conway 
