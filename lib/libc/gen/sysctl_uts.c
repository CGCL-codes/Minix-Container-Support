#include <lib.h>	// message
#include <sys/sysctl.h>	// CTL_KERN,KERN_HOSTNAME

#include <unistd.h>	// sleep, getpid
#include <string.h>	// memset, memcpy
//#include <stdio.h>	// printf


int sysctl_uts(void * oldp, size_t * oldlenp, const void * newp, size_t newlen)
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
	
	m.m_lc_mib_sysctl.uts_pendpt = getppid();     // 父进程此处获取父进程，子进程此处应获取祖父进程
	m.m_lc_mib_sysctl.uts_cendpt = 0;	
	
	r = _syscall(MIB_PROC_NR, MIB_SYSCTL, &m);	// 测试：MIB系统调用可行
	
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