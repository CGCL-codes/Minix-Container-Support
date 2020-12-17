#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#include <string.h>
#include <sys/statvfs.h>

int getvfsstat(struct statvfs *buf, size_t bufsize, int flags, int endpoint)
{
  message m;

  memset(&m, 0, sizeof(m));
  m.m_lc_vfs_getvfsstat.buf = (vir_bytes) buf;
  m.m_lc_vfs_getvfsstat.len = bufsize;
  m.m_lc_vfs_getvfsstat.flags = flags;
  m.m_lc_vfs_getvfsstat.endpoint = endpoint;

  return(_syscall(VFS_PROC_NR, VFS_GETVFSSTAT, &m));
}
