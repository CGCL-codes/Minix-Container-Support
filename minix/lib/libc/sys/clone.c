#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <sys/sched.h>
#include <signal.h>

#include <string.h>
#include <unistd.h>

#include <sys/sysctl.h>
#include <stdio.h>
#include "extern.h"

#ifdef __weak_alias
__weak_alias(clone, _clone)
#endif

int mysysctl_uts_clone(int cpid, int utstype);

int clone(int (*fn)(void *), void *stack, int flags, void *arg)
{
    message m;

    memset(&m, 0, sizeof(m));

    m.m_lc_pm_clone.type = TYPE_CLONE;
    m.m_lc_pm_clone.clone_flags = flags;
    m.m_lc_pm_clone.stack_addr = stack;

    pid_t pid = _syscall(PM_PROC_NR, PM_FORK, &m);
    if(pid != 0) {
        if((flags & CLONE_NEWUTS) == CLONE_NEWUTS){
		 mysysctl_uts_clone(pid);
			// __sysctluts(NULL, NULL, NULL, 0, pid);
		}
		
        return pid;
    } else {
        (*fn)(arg);
        _exit(0);
    }
}

int mysysctl_uts_clone(int cpid)
{
	message m;
	int r;

	memset(&m, 0, sizeof(m));
	m.m_lc_mib_sysctl.oldp = 0;
	m.m_lc_mib_sysctl.oldlen = 0;
	m.m_lc_mib_sysctl.newp = 0;
	m.m_lc_mib_sysctl.newlen = 0;
	
	int mib[2];
	mib[0] = CTL_KERN;
	mib[1] = KERN_HOSTNAME;
	m.m_lc_mib_sysctl.namep = (vir_bytes)mib;
	m.m_lc_mib_sysctl.namelen = 2;
	memcpy(m.m_lc_mib_sysctl.name, mib, sizeof(*mib) * 2);
	
	m.m_lc_mib_sysctl.uts_pendpt = getppid();     
	// printf("clone father process ID:%d\n", m.m_lc_mib_sysctl.uts_pendpt);
	m.m_lc_mib_sysctl.uts_cendpt = cpid;			
	// printf("clone child process ID:%d\n", m.m_lc_mib_sysctl.uts_cendpt);
	m.m_lc_mib_sysctl.uts_type = 2;
	r = _syscall(MIB_PROC_NR, MIB_SYSCTL, &m);	
	
	return r;
}
