#ifndef _CGROUPFS_TYPE_H
#define _CGROUPFS_TYPE_H

typedef void *data_t;

struct memory_cgroup
{
    pid_t pid;
    vir_bytes vm_limit;
};

struct freezer_cgroup
{
    pid_t pid;
    int state;     //0: FROZEN  1: THAWED
};

#endif /* _CGROUPFS_TYPE_H */
