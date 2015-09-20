PROGRAMS=test1 test2 client
LIBRARY=libcounter.so

CXXFLAGS=-fPIC -g
LDFLAGS=-g


all: ${PROGRAMS} ${LIBRARY}


${LIBRARY}: Counter.o
	${CXX} ${LDFLAGS} -shared -o $@ $^

test1: test1.o ${LIBRARY}
	${CXX} ${LDFLAGS} -L . -o $@ test1.o -lcounter

test2: test2.o ${LIBRARY}
	${CXX} ${LDFLAGS} -L . -o $@ test2.o -lcounter


clean:
	-rm -f ${PROGRAMS} ${LIBRARY} *.o core *~

