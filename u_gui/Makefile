
CC=gcc
CFLAGS= -I./inc -g -O2 

.PHONY: all clean


MAIN_OBJ=\
./src/ug-plot.o \
./src/ug-indicator.o \
./src/ug-arrow.o \
./src/ug-plc.o \
./main.o 


MAIN_CFLAGS=`pkg-config --cflags gtk+-3.0` -I./inc -I../inc -g -c
MAIN_LIBS=`pkg-config --libs gtk+-3.0` -lm -lu_cat 

CC=gcc


main: $(MAIN_OBJ)
	$(CC) $^ -L. $(MAIN_LIBS) -Wl,-rpath,.

main.o: main.c
	$(CC) $^ $(MAIN_CFLAGS) -o$@

./src/ug-plot.o: ./src/ug-plot.c
	$(CC) $^ $(MAIN_CFLAGS) -o$@

./src/ug-indicator.o: ./src/ug-indicator.c
	$(CC) $^ $(MAIN_CFLAGS) -o$@

./src/ug-arrow.o: ./src/ug-arrow.c
	$(CC) $^ $(MAIN_CFLAGS) -o$@

./src/ug-plc.o: ./src/ug-plc.c
	$(CC) $^ $(MAIN_CFLAGS) -o$@

clean:
	rm -f $(MAIN_OBJ) ./a.out

