CC = gcc
CFLAGS = -c -Wall -pedantic -O2
CFLAGSDB = -c -Wall -pedantic -ggdb

all: ep4 ep4db

ep4: ep4.o 
	$(CC) ep4.o -o ep4 

ep4db: ep4db.o 
	$(CC) ep4db.o -o ep4db 

ep4.o: ep4.c
	$(CC) $(CFLAGS) ep4.c 

ep4db.o: ep4.c
	$(CC) $(CFLAGSDB) ep4.c -o ep4db.o

clean:
	rm -rf *.o ep4 ep4db
