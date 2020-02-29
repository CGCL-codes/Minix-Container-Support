# Makefile for cgroupfs
# .include <bsd.own.mk>

PROG= cgroupfs
SRCS= buf.c file.c main.c memory.c

# CPPFLAGS+= -I${NETBSDSRCDIR}/minix
# CPPFLAGS+= -I${NETBSDSRCDIR}/minix/fs
# CPPFLAGS+= -I${NETBSDSRCDIR}/minix/servers

DPADD+=	${LIBVTREEFS} ${LIBFSDRIVER} ${LIBSYS}
LDADD+=	-lvtreefs -lfsdriver -lsys

CFLAGS+= -D_NETBSD_SOURCE=1

MAN=

BINDIR?= /service

.include <bsd.prog.mk>