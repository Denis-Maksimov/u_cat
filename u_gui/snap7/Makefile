

CC:=C:/MinGW/mingw64/bin/gcc.exe
GPP=C:\MinGW\mingw64\bin\g++.exe
AR :=C:/MinGW/mingw64/bin/ar.exe
CXX=C:\MinGW\mingw64\bin\g++.exe
CPPFLAGS= -m64 -Wall -pedantic -Icore -Ilib -Isys -O2

OBJS= \
core/s7_client.o \
core/s7_isotcp.o \
core/s7_micro_client.o \
core/s7_partner.o \
core/s7_peer.o \
core/s7_server.o \
core/s7_text.o \
lib/snap7_libmain.o \
snap7.o \
sys/snap_msgsock.o \
sys/snap_sysutils.o \
sys/snap_tcpsrvr.o \
sys/snap_threads.o \



libsnap7.a: $(OBJS)
	# $(GPP) -c $^ -Wall -pedantic -Icore -Ilib -Isys -Ofast
	$(AR) -rvs $@ $(OBJS) 


core/s7_client.o:
core/s7_isotcp.o:
core/s7_micro_client.o:
core/s7_partner.o:
core/s7_peer.o:
core/s7_server.o:
core/s7_text.o:
lib/snap7_libmain.o:
snap7.o:
sys/snap_msgsock.o:
sys/snap_sysutils.o:
sys/snap_tcpsrvr.o:
sys/snap_threads.o:




clean:
	rm -f $(OBJS) libsnap7.a
