#ifndef _CGROUPFS_CONST_H
#define _CGROUPFS_CONST_H

/* Max number of inodes */
#define NR_INODES   512
/* Max number of pid */
#define NR_PID  ((NR_TASKS + NR_PROCS) * 4)

/* File modes */
#define REG_RW_ALL_MODE (S_IFREG | 0666)  /* World-readable-writable regular */
#define DIR_ALL_MODE (S_IFDIR | 0555)     /* World-accessible directory */

/* Size of the I/O buffer and the str storing file content. */
#define BUF_SIZE	4097			/* 4KB+1 (see buf.c) */
#define STR_SIZE    1024*5         /* 5KB */
#define NR_SUBSYSTEMS   2

/* Directory index */
#define MEMORY_CGROUP   0
#define FREEZER_CGROUP  1

#define FROZEN 0
#define THAWED 1


#endif /* _CGROUPFS_CONST_H */