CXXFLAGS= -g -O2  -D_GNU_SOURCE
PROGRAMS = feymail_smtpd
CLEAR_PROGRAMS = bin/feymail_smtpd
LIB_FLAGS = -I.  -lpthread
LIB_SRC_FILE  = feymail.c 

all: ${PROGRAMS}


feymail_smtpd: feymail_smtpd.c
	gcc ${CXXFLAGS} -o bin/feymail_smtpd  feymail_smtpd.c ${LIB_SRC_FILE} ${LIB_FLAGS}



clean:
	rm -f *.o ${CLEAR_PROGRAMS}
