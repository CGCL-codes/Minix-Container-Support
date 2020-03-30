# Makefile for cgroupfs
.include <bsd.own.mk>

PROG= cgroupfs
SRCS= buf.c file.c main.c memory.c freezer.c

# CPPFLAGS+= -I${NETBSDSRCDIR}/minix
# CPPFLAGS+= -I${NETBSDSRCDIR}/minix/fs
# CPPFLAGS+= -I${NETBSDSRCDIR}/minix/servers

DPADD+=	${LIBVTREEFS} ${LIBFSDRIVER}
LDADD+=	-lvtreefs -lfsdriver

CFLAGS+= -D_NETBSD_SOURCE=1

MAN=

BINDIR?= /service

#.include <bsd.prog.mk>
.include <minix.service.mk>