IDIR =src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=src

LIBS=-lm

_DEPS = parser.h tailq.h textmmap.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.c parser.c tailq.c textmmap.c
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
cr: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
