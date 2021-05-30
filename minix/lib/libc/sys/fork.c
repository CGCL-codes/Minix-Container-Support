#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>

#include <string.h>
#include <unistd.h>

#ifdef __weak_alias
__weak_alias(fork, _fork)
#endif

int mysysctl_uts_clone(int cpid);

pid_t fork(void)
{
  message m;

  memset(&m, 0, sizeof(m));
  m.m_lc_pm_clone.type = TYPE_FORK;
  m.m_lc_pm_clone.clone_flags = -1;
  m.m_lc_pm_clone.stack_addr = -1;
  return(_syscall(PM_PROC_NR, PM_FORK, &m));
}
