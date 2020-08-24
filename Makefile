CXXFLAGS= -g -O2  -D_GNU_SOURCE
PROGRAMS = feymail-smtpd feymail-queue
CLEAR_PROGRAMS = bin/feymail-smtpd
LIB_FLAGS = -I.  -lpthread
LIB_SRC_FILE  = feymail.c  feymail-fd.c feymail-string.c feymail-sys.c feymail-alloc.c

all: ${PROGRAMS}


feymail-smtpd: feymail-smtpd.c
	gcc ${CXXFLAGS} -o bin/feymail-smtpd  feymail-smtpd.c ${LIB_SRC_FILE} ${LIB_FLAGS}

feymail-queue: feymail-queue.c
	gcc ${CXXFLAGS} -o bin/feymail-queue  feymail-queue.c ${LIB_SRC_FILE} ${LIB_FLAGS}




clean:
	rm -f *.o ${CLEAR_PROGRAMS}
