/* memory.c - Handle memory cgroup information */

#include "head.h"

/* Memory cgroup initialization */
void mem_cgroup_init(struct inode *root)
{
    struct inode_stat dir_stat, file_stat;
    struct inode *inode;
    struct inode *parent;

    dir_stat.mode = DIR_ALL_MODE;
    dir_stat.uid = SUPER_USER;
    dir_stat.gid = SUPER_USER;
    dir_stat.size = 0;
    dir_stat.dev = NO_DEV;

    /* Create the memory directory and files included in memory directory */
    parent = add_inode(root, "memory", MEMORY_CGROUP, &dir_stat, 1, 
        (cbdata_t) MEMORY_CGROUP);
    
    assert(parent != NULL);

    file_stat.mode = REG_RW_ALL_MODE;
    file_stat.uid = SUPER_USER;
    file_stat.gid = SUPER_USER;
    file_stat.size = 0;
    file_stat.dev = NO_DEV;

    inode = add_inode(parent, "vm_limit_in_bytes", NO_INDEX, &file_stat, 0,
        (cbdata_t) 0);
    
    assert(inode != NULL);

    /* Initialize the centent in file(vm_limit_in_bytes) */
    data[MEMORY_CGROUP] = "0";

}