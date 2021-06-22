
CC=gcc
CFLAGS= -I./inc -g -O2 

.PHONY: all clean



U_CAT_OBJ=\
./src/u_algorithm/u_sort.o \
./src/u_application/u_csv.o \
./src/u_application/s5_timers.o \
./src/u_application/u_pid.o \
./src/u_application/u_fsm.o \
./src/u_application/u_ema.o \
./src/u_application/u_fuzzy.o \
./src/u_application/u_dbg.o \
./src/u_math/u_math.o \
./src/u_math/u_fixed.o \
./src/u_math/table.o \
./src/u_stdlib/u_mallocat.o \
./src/u_stdlib/u_string.o \
./src/u_stdlib/u_mallocat.o \
./src/u_stdlib/u_string.o \
./src/u_types/u_hash.o \
./src/u_types/u_vector.o \
./src/u_types/u_cstring.o \
./src/u_types/u_ring.o \
./src/u_crypto/u_kuznechik.o 
# ./src/u_network/u_network.o \
# ./src/u_network/u_server.o 

MAIN_OBJ=\
./ug-plot.o \
./main3.o \
./libu_cat.a

MAIN_CFLAGS=`pkg-config --cflags gtk+-3.0` -I./inc -g -c
MAIN_LIBS=`pkg-config --libs gtk+-3.0` -lm 
# ./src/u_sellect/u_sellect.o

CC=gcc
# CFLAGS = -Wall -O2 -I./inc -I. -g 


#===============================
# main: $(MAIN_OBJ)
# 	$(CC) $^ $(MAIN_LIBS)

# main3.o: main3.c
# 	$(CC) $^ $(MAIN_CFLAGS)

# ug-plot.o: ug-plot.c
# 	$(CC) $^ $(MAIN_CFLAGS)


libu_cat.a: $(U_CAT_OBJ)
	ar crs $@ $^

#--------------------------
./src/u_algorithm/u_sort.o:
#--------------------------
./src/u_application/u_csv.o:
./src/u_application/s5_timers.o:
./src/u_application/u_fsm.o:
./src/u_application/u_ema.o:
./src/u_application/u_fuzzy.o:
./src/u_application/u_dbg.o:
#--------------------------
./src/u_math/u_math.o:
./src/u_math/u_fixed.o:
./src/u_math/table.o:
#--------------------------
./src/u_sellect/u_sellect.o:
#--------------------------
./src/u_stdlib/u_mallocat.o:
./src/u_stdlib/u_string.o:
#--------------------------
./src/u_stdlib/u_mallocat.o:
./src/u_stdlib/u_string.o:
#--------------------------
./src/u_types/u_hash.o:
./src/u_types/u_vector.o:
./src/u_types/u_cstring.o:
./src/u_types/u_ring.o:
#--------------------------
./src/u_network/u_network.o:
./src/u_network/u_server.o:
#--------------------------
./src/u_crypto/u_kuznechik.o:
#--------------------------

clean:
	rm -f $(U_CAT_OBJ) $(MAIN_OBJ) ./a.out

