#include "head.h"

struct cpu_cgroup cpu_cgroup[NR_PID];

/* Cpu cgroup initialization */
void cpu_cgroup_init(struct inode *root)
{
    struct inode_stat dir_stat, file_stat;
    struct inode *inode;
    struct inode *parent;

    dir_stat.mode = DIR_ALL_MODE;
    dir_stat.uid = SUPER_USER;
    dir_stat.gid = SUPER_USER;
    dir_stat.size = 0;
    dir_stat.dev = NO_DEV;

    /* Create freezer directory */
    parent = add_inode(root, "cpu", CPU_CGROUP, &dir_stat, 1, 
        (cbdata_t) CPU_CGROUP);
    
    assert(parent != NULL);

    file_stat.mode = REG_RW_ALL_MODE;
    file_stat.uid = SUPER_USER;
    file_stat.gid = SUPER_USER;
    file_stat.size = 0;
    file_stat.dev = NO_DEV;

    inode = add_inode(parent, "cpu_shares", NO_INDEX, &file_stat, 0,
        (cbdata_t) 0);
    
    assert(inode != NULL);

    strcpy(data[CPU_CGROUP], "000");

    /* Initialize freezer cgroup information */
    for(int i = 0; i < NR_PID; i++) {
        cpu_cgroup[i].pid = -1;
        cpu_cgroup[i].cpu_shares = CPU_SHARES_DEFAULT;
        //cpu_cgroup[i].cpus = (1 << 32) - 1;
    }
}

/* Cpu time slice control */
void cpu_ctl(char * ptr)
{
    int start = 0, mid = 0, end = 0;
    pid_t pid = 0;
    int cpu_shares;
    struct minix_proc_data mpd;

    for(start = 0;; end++) {
        if(ptr[end] == ' ') {
            mid = end - 1;
            continue;
        }

        if(ptr[end] == '\n' || ptr[end] == '\0') {

            // Search and transform pid
            char tmp_pid[20], tmp_cpu_shares[20];
            int len = mid -start + 1;
            if(len < 0) {
                break;
            }
            memcpy(tmp_pid, &ptr[start], len);
            tmp_pid[len] = '\0';
            pid = atoi(tmp_pid);

            // Transform cpu_shares
            len = end - mid - 2;
            memcpy(tmp_cpu_shares, &ptr[mid + 2], len);
            tmp_cpu_shares[len] = '\0';
            cpu_shares = atoi(tmp_cpu_shares);

            // limit the upper bound and lower bound of cpu_shares
            if(cpu_shares > CPU_SHARES_UPPER_BOUND) {
                cpu_shares = CPU_SHARES_UPPER_BOUND;
            } else if(cpu_shares < CPU_SHARES_LOWER_BOUND) {
                cpu_shares = CPU_SHARES_LOWER_BOUND;
            }

            start = end + 1;

            // Judge if it is the latest cpu limit information. if yes, make the system call
            if(cpu_cgroup[pid].pid == -1 || (cpu_cgroup[pid].pid != -1 && cpu_cgroup[pid].cpu_shares != cpu_shares)) {
                cpu_cgroup[pid].pid = pid;
                cpu_cgroup[pid].cpu_shares = cpu_shares;

                if (get_proc_data(pid, &mpd) != OK)
		            return;
                
                if (sys_cgptosched(mpd.mpd_endpoint, cpu_shares, 0) != OK) {
                    return;
                }
                
            }
        }

        if(ptr[end] == '\0')
            break;
    }
}