/* memory.c - Handle memory cgroup information */

#include "head.h"

struct memory_cgroup mem_cgroup[NR_PID];

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
    strcpy(data[MEMORY_CGROUP], "000");

    /* Initialize memory_cgroup information */
    for(int i = 0; i < NR_PID; i++) {
        mem_cgroup[i].pid = -1;
        mem_cgroup[i].vm_limit = -1;
    }

}

/* Passing memory cgroup information to vm through system call */
void mem_ctl(char * ptr)
{
    int start = 0, mid = 0, end = 0;
    pid_t pid = 0;
    vir_bytes vm_limit = 0;
    struct minix_proc_data mpd;

    for(start = 0;; end++) {
        if(ptr[end] == ' ') {
            mid = end - 1;
            continue;
        }

        if(ptr[end] == '\n' || ptr[end] == '\0') {

            // Search and transform pid
            char tmp_pid[20], tmp_vm[20];
            int len = mid -start + 1;
            if(len < 0) {
                break;
            }
            memcpy(tmp_pid, &ptr[start], len);
            tmp_pid[len] = '\0';
            pid = atoi(tmp_pid);

            // Transform vm_limit
            len = end - mid - 2;
            memcpy(tmp_vm, &ptr[mid + 2], len);
            tmp_vm[len] = '\0';
            vm_limit = strtoul(tmp_vm, NULL, 0);


            start = end + 1;

            // Judge if it is the latest vm_limit information. if yes, make the system call
            if(mem_cgroup[pid].pid != -1 && mem_cgroup[pid].vm_limit != vm_limit) {
                mem_cgroup[pid].pid = pid;
                mem_cgroup[pid].vm_limit = vm_limit;

                if (get_proc_data(pid, &mpd) != OK)
		            return;
                if (sys_cgptovm(mpd.mpd_endpoint, vm_limit) != OK)
                    return;
            }
        }

        if(ptr[end] == '\0')
            break;
    }
}

/*
 * Get MINIX3-specific process data for the process identified by the given
 * kernel slot.  Return OK or a negative error code.
 */
int
get_proc_data(pid_t pid, struct minix_proc_data * mpd)
{
	int mib[4] = { CTL_MINIX, MINIX_PROC, PROC_DATA, pid };
	size_t oldlen;

	oldlen = sizeof(*mpd);
	if (__sysctl(mib, __arraycount(mib), mpd, &oldlen, NULL, 0) != 0)
		return -errno;

	return OK;
}