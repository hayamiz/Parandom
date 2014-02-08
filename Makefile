
LDFLAGS := -pthread
CFLAGS := -g -Wall -O3 -I./SFMT -DMEXP=19937 -msse2 -DHAVE_SSE2

parandom: parandom.o SFMT.o

SFMT.c: SFMT/SFMT.c
	cp $< $@
