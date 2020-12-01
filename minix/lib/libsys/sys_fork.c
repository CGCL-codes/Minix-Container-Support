#include "syslib.h"

int sys_fork(parent, child, child_endpoint, flags, msgaddr, clone_flags, stack_addr, type)
endpoint_t parent;		/* process doing the fork */
endpoint_t child;		/* which proc has been created by the fork */
endpoint_t *child_endpoint;
u32_t flags;
vir_bytes *msgaddr;
uint32_t clone_flags;
uint32_t stack_addr;
uint32_t type;
{
/* A process has forked.  Tell the kernel. */

  message m;
  int r;

  m.m_lsys_krn_sys_fork.endpt = parent;
  m.m_lsys_krn_sys_fork.slot = child;
  m.m_lsys_krn_sys_fork.flags = flags;
  m.m_lsys_krn_sys_fork.clone_flags = clone_flags;
  m.m_lsys_krn_sys_fork.stack_addr = stack_addr;
  m.m_lsys_krn_sys_fork.type = type;
  r = _kernel_call(SYS_FORK, &m);
  *child_endpoint = m.m_krn_lsys_sys_fork.endpt;
  *msgaddr = m.m_krn_lsys_sys_fork.msgaddr;
  return r;
}
