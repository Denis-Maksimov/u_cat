
CC=gcc
CFLAGS= -I./inc -g -O2 

.PHONY: all clean



OBJ=\
./src/u_algorithm/u_sort.o \
./src/u_application/u_csv.o \
./src/u_application/u_hex.o \
./src/u_application/s5_timers.o \
./src/u_application/u_pid.o \
./src/u_application/u_fsm.o \
./src/u_application/u_ema.o \
./src/u_application/u_fuzzy.o \
./src/u_math/u_math.o \
./src/u_math/u_fixed.o \
./src/u_math/table.o \
./src/u_stdlib/u_mallocat.o \
./src/u_stdlib/u_string.o \
./src/u_stdlib/u_mallocat.o \
./src/u_stdlib/u_string.o \
./src/u_types/u_hash.o \
./src/u_types/u_vector.o \
./src/u_types/u_cstring.o 


# ./src/u_sellect/u_sellect.o

CC=gcc
# CFLAGS = -Wall -O2 -I./inc -I. -g 

# libu_stdlib.a: $(OBJ)
# 	ar crs $@ $^

main: main.o libu_cat.a
	gcc main.o libu_cat.a -lm

main.o:




libu_cat.a: $(OBJ)
	ar crs $@ $^
#--------------------------
./src/u_algorithm/u_sort.o:
#--------------------------
./src/u_application/u_csv.o:
./src/u_application/u_hex.o:
./src/u_application/s5_timers.o:
./src/u_application/u_fsm.o:
./src/u_application/u_ema.o:
./src/u_application/u_fuzzy.o:
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
#--------------------------
#--------------------------
#--------------------------

clean:
	rm -f $(OBJ) ./a.out

