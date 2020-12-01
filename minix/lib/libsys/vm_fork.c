
#include "syslib.h"

#include <string.h>
#include <minix/vm.h>

/*===========================================================================*
 *                                vm_fork				     *
 *===========================================================================*/
int vm_fork(endpoint_t ep, int slot, endpoint_t *childep, uint32_t clone_flags, uint32_t stack_addr, uint32_t type)
{
    message m;
    int result;

    memset(&m, 0, sizeof(m));
    m.VMF_ENDPOINT = ep;
    m.VMF_SLOTNO = slot;
    m.VMF_CLONE_FLAGS = clone_flags;
    m.VMF_STACK_ADDR = stack_addr;
    m.VMF_TYPE = type;

    result = _taskcall(VM_PROC_NR, VM_FORK, &m);

    *childep = m.VMF_CHILD_ENDPOINT;

    return(result);
}

