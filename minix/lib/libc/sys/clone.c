#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>

#include <string.h>
#include <unistd.h>

#ifdef __weak_alias
__weak_alias(clone, _clone)
#endif

int clone(int (*fn)(void *), void *stack, int flags, void *arg)
{
    message m;

    memset(&m, 0, sizeof(m));

    m.m_lc_pm_clone.type = TYPE_CLONE;
    m.m_lc_pm_clone.clone_flags = flags;
    m.m_lc_pm_clone.stack_addr = stack;

    pid_t pid = _syscall(PM_PROC_NR, PM_FORK, &m);
    if(pid != 0) {

        if(flags == CLONE_NEWUTS){

            	int mib[2];

                mib[0] = CTL_KERN;
                mib[1] = KERN_HOSTNAME;

                message m;
                int r;

                memset(&m, 0, sizeof(m));

                m.m_lc_mib_sysctl.oldp = 0;
                m.m_lc_mib_sysctl.oldlen = 0;
                m.m_lc_mib_sysctl.newp = 0;
                m.m_lc_mib_sysctl.newlen = 0;
                m.m_lc_mib_sysctl.namelen = 2;
                m.m_lc_mib_sysctl.namep = (vir_bytes)mib;
                
                m.m_lc_mib_sysctl.uts_pendpt = getppid();
                m.m_lc_mib_sysctl.uts_cendpt = pid;

                r = _syscall(MIB_PROC_NR, MIB_SYSCTL, &m);

        }

        return pid;
    } else {
        (*fn)(arg);
        _exit(0);
    }
}