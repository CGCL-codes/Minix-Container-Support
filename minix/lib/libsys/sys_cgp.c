#include "syslib.h"


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