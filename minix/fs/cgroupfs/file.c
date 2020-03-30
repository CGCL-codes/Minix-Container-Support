/* file.c - Read/Write hooks for reading/writing cgroup information into files. */

#include "head.h"

char data[NR_SUBSYSTEMS][STR_SIZE];

ssize_t read_hook(struct inode *inode, char *ptr, size_t len, off_t off,
	cbdata_t cbdata)
{
    struct inode *parent;
    int parent_index;

    buf_init(ptr, len, off);

    parent = get_parent_inode(inode);
    parent_index = get_inode_index(parent);

    // switch (parent_index)
    // {
    // case MEMORY_CGROUP:
        
    //     buf_read(data[MEMORY_CGROUP], sizeof(char) * strlen(data[MEMORY_CGROUP]));
    //     break;
    
    // default:
    //     break;
    // }

    buf_read(data[parent_index], sizeof(char) * strlen(data[parent_index]));

    return buf_result();
}


ssize_t write_hook(struct inode *inode, char *ptr, size_t max,
    off_t off, cbdata_t cbdata)
{
    
    struct inode *parent;
    int parent_index;

    parent = get_parent_inode(inode);
    parent_index = get_inode_index(parent);

    /* Initialize the file content */
    if(off == 0) {
        memset(data[parent_index], 0, sizeof(char) * STR_SIZE);
    }

    switch (parent_index)
    {
    case MEMORY_CGROUP:
        
        buf_write(ptr, data[MEMORY_CGROUP], max, off);
        mem_ctl(data[MEMORY_CGROUP]);
        break;
    
    case FREEZER_CGROUP:
        
        buf_write(ptr, data[FREEZER_CGROUP], max, off);
        freezer_ctl(data[FREEZER_CGROUP]);
        break;
    
    default:
        break;
    }

    return max;
}