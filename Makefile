# Rules
COMP = g++
CC = main.cc HorarisParser.cc Horari.cc
OBJ = main.o HorarisParser.o Horari.o
CFLAGS = -Wall -O3 -Wno-sign-compare

# DEBUG  -g -O0 -fno-inline -D_GLIBCXX_DEBUG
# RUN -O3

main.exe:${OBJ}
	${COMP} ${CFLAGS} -o $@ ${OBJ}

clean: 
	rm *.o *.exe

.cc.o:
	${COMP} ${CFLAGS} -c $<