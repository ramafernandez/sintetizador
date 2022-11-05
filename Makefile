PROGRAM = sintetizador
CC = gcc
CFLGAS = -Wall -pedantic -std=c99 -g
LDFLAGS = -lm

all: $(PROGRAM)

$(PROGRAM): decodificarmidi.o nota.o notas.o readmidi.o mainnn.o readsintetiza.o fmodulacion.o tdasintetiza.o tramo.o writewave.o
	$(CC) $^ -o $(PROGRAM) $(LDFLAGS)

notas.o: notas.c notas.h
	$(CC) $(CFLGAS) notas.c -c

nota.o: nota.c nota.h
	$(CC) $(CFLGAS) nota.c -c

decodificarmidi.o: decodificarmidi.c decodificarmidi.h
	$(CC) $(CFLGAS) decodificarmidi.c -c

readmidi.o: readmidi.c
	$(CC) $(CFLGAS) readmidi.c -c

readsintetiza.o: readsintetiza.c
	$(CC) $(CFLGAS) readsintetiza.c -c

fmodulacion.o: fmodulacion.c
	$(CC) $(CFLGAS) fmodulacion.c -c

tdasintetiza.o: tdasintetiza.c
	$(CC) $(CFLGAS) tdasintetiza.c -c

tramo.o: tramo.c
	$(CC) $(CFLGAS) tramo.c -c

writewave.o: writewave.c
	$(CC) $(CFLGAS) writewave.c -c

mainnn.o: mainnn.c
	$(CC) $(CFLGAS) mainnn.c -c

clean:
	rm -fv *.o $(PROGRAM)
