#include "syslib.h"

#include <string.h>


/*===========================================================================*
 *                                sys_cgptovm				     *
 *===========================================================================*/
int sys_cgptovm(endpoint_t proc, vir_bytes vm_limit) {
    message m;

    memset(&m, 0, sizeof(m));
    m.m_lsys_cgp_vm_info.proc = proc;
    m.m_lsys_cgp_vm_info.vm_limit = vm_limit;

    return _taskcall(VM_PROC_NR, VM_CGROUP_INFO, &m);
}

/*===========================================================================*
 *                                sys_cgptopm				     *
 *===========================================================================*/
int sys_cgptopm(endpoint_t proc, int state) {
    message m;

    memset(&m, 0, sizeof(m));
    m.m_lsys_cgp_pm_info.proc = proc;
    m.m_lsys_cgp_pm_info.state = state;

    return _taskcall(PM_PROC_NR, PM_CGROUP_INFO, &m);
}

/*===========================================================================*
 *                                sys_cgptosched				     *
 *===========================================================================*/
int sys_cgptosched(endpoint_t proc, int cpu_shares, unsigned cpus) {
    message m;

    memset(&m, 0, sizeof(m));
    m.m_lsys_cgp_sched_info.proc = proc;
    m.m_lsys_cgp_sched_info.cpu_shares = cpu_shares;
    m.m_lsys_cgp_sched_info.cpus = cpus;

    if(cpu_shares == -1) {
        return _taskcall(SCHED_PROC_NR, SCHEDULING_CGROUP_CPUSET_INFO, &m);
    } else {
        return _taskcall(SCHED_PROC_NR, SCHEDULING_CGROUP_CPU_INFO, &m);
    }
}