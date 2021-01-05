/*	$NetBSD: sethostname.c,v 1.13 2012/06/25 22:32:43 abs Exp $	*/

/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
#if 0
static char sccsid[] = "@(#)sethostname.c	8.1 (Berkeley) 6/4/93";
#else
__RCSID("$NetBSD: sethostname.c,v 1.13 2012/06/25 22:32:43 abs Exp $");
#endif
#endif /* LIBC_SCCS and not lint */

#include "namespace.h"
#include <sys/param.h>
#include <sys/sysctl.h>

#include <assert.h>
#include <errno.h>
#include <unistd.h>

#include <lib.h>
#include <string.h>
#include <stdio.h>

#ifdef __weak_alias
__weak_alias(sethostname,_sethostname)
#endif

int sysctl_uts(void * oldp, size_t * oldlenp, const void * newp, size_t newlen);

int
sethostname(const char *name, size_t namelen)
{
	int mib[2];

	_DIAGASSERT(name != NULL);

	mib[0] = CTL_KERN;
	mib[1] = KERN_HOSTNAME;
	if (sysctl_uts(NULL, NULL, name, namelen) == -1)
		return (-1);
	return (0);
}

int mysysctl_uts(void * oldp, size_t * oldlenp, const void * newp, size_t newlen)
{
	message m;
	int r;

	memset(&m, 0, sizeof(m));
	m.m_lc_mib_sysctl.oldp = (vir_bytes)oldp;
	m.m_lc_mib_sysctl.oldlen = (oldlenp != NULL) ? *oldlenp : 0;
	m.m_lc_mib_sysctl.newp = (vir_bytes)newp;
	m.m_lc_mib_sysctl.newlen = newlen;
	
	int mib[2];
	mib[0] = CTL_KERN;
	mib[1] = KERN_HOSTNAME;
	m.m_lc_mib_sysctl.namep = (vir_bytes)mib;
	m.m_lc_mib_sysctl.namelen = 2;
	memcpy(m.m_lc_mib_sysctl.name, mib, sizeof(*mib) * 2);
	
	m.m_lc_mib_sysctl.uts_pendpt = getppid(); 
	m.m_lc_mib_sysctl.uts_cendpt = 0;	

	r = _syscall(MIB_PROC_NR, MIB_SYSCTL, &m);

	/*
	 * We copy the NetBSD behavior of replying with the old length also if
	 * the call failed, typically with ENOMEM.  This is undocumented
	 * behavior, but unfortunately relied on by sysctl(8) and other NetBSD
	 * userland code.  If the call failed at the IPC level, the resulting
	 * value will be garbage, but it should then not be used anyway.
	 */
	if (oldlenp != NULL)
		*oldlenp = m.m_mib_lc_sysctl.oldlen;
	
	return r;
}
