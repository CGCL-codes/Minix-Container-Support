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

    m.m_lsys_krn_sys_fork.type = TYPE_CLONE;
    m.m_lsys_krn_sys_fork.clone_flags = flags;
    m.m_lsys_krn_sys_fork.stack_addr = stack;

    pid_t pid = _syscall(PM_PROC_NR, PM_FORK, &m);
    if(pid != 0) {
        return pid;
    } else {
        (*fn)(arg);
        _exit(0);
    }
}