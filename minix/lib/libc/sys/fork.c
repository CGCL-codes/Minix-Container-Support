#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>

#include <string.h>
#include <unistd.h>

#ifdef __weak_alias
__weak_alias(fork, _fork)
#endif

pid_t fork(void)
{
  message m;

  memset(&m, 0, sizeof(m));
  m.m_lc_pm_clone.type = TYPE_FORK;
  return(_syscall(PM_PROC_NR, PM_FORK, &m));
}
