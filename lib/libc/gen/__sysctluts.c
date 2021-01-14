#include <sys/cdefs.h>
#include <lib.h>
#include <sys/sysctl.h>
#include <string.h>
#include <unistd.h>
#include "extern.h"

/* 
int __sysctluts(void * oldp, size_t * oldlenp, const void * newp, size_t newlen)
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
	
	if (oldlenp != NULL)
		*oldlenp = m.m_mib_lc_sysctl.oldlen;
	
	return r;
}

*/