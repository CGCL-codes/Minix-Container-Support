#include "head.h"

struct cpuset_cgroup cpuset_cgroup[NR_PID];

/* Cpuset cgroup initialization */
void cpuset_cgroup_init(struct inode *root)
{
    struct inode_stat dir_stat, file_stat;
    struct inode *inode;
    struct inode *parent;

    dir_stat.mode = DIR_ALL_MODE;
    dir_stat.uid = SUPER_USER;
    dir_stat.gid = SUPER_USER;
    dir_stat.size = 0;
    dir_stat.dev = NO_DEV;

    /* Create cpuset directory */
    parent = add_inode(root, "cpuset", CPUSET_CGROUP, &dir_stat, 1, 
        (cbdata_t) CPUSET_CGROUP);
    
    assert(parent != NULL);

    file_stat.mode = REG_RW_ALL_MODE;
    file_stat.uid = SUPER_USER;
    file_stat.gid = SUPER_USER;
    file_stat.size = 0;
    file_stat.dev = NO_DEV;

    inode = add_inode(parent, "cpus", NO_INDEX, &file_stat, 0,
        (cbdata_t) 0);
    
    assert(inode != NULL);

    strcpy(data[CPUSET_CGROUP], "000");

    /* Initialize freezer cgroup information */
    for(int i = 0; i < NR_PID; i++) {
        cpuset_cgroup[i].pid = -1;
        cpuset_cgroup[i].cpus = CPUS_DEFAULT;
    }
}

/* Cpuset control */
void cpuset_ctl(char * ptr)
{
    int start = 0, mid = 0, end = 0;
    pid_t pid = 0;
    unsigned cpus = 0;
    struct minix_proc_data mpd;

    for(start = 0;; end++) {
        if(ptr[end] == ' ') {
            mid = end - 1;
            continue;
        }

        if(ptr[end] == '\n' || ptr[end] == '\0') {

            // Search and transform pid
            char tmp_pid[20], tmp_cpus[30];
            int len = mid -start + 1;
            if(len < 0) {
                break;
            }
            memcpy(tmp_pid, &ptr[start], len);
            tmp_pid[len] = '\0';
            pid = atoi(tmp_pid);

            // Transform cpus
            len = end - mid - 2;
            memcpy(tmp_cpus, &ptr[mid + 2], len);
            tmp_cpus[len] = '\0';
            
            int i = 0, j = 0;
            while(1) {
                if(tmp_cpus[j] != ',' && tmp_cpus[j] != '\0') {
                    j++;
                    continue;
                }
                char tmp_cpu[5];
                int l = j - i;
                memcpy(tmp_cpu, &tmp_cpus[i], l);
                tmp_cpu[l] = '\0';
                int cpu = atoi(tmp_cpu);
                if(cpu >= 16) {
                    continue;
                }
                cpus = cpus | (1 << cpu);

                if(tmp_cpus[j] == '\0') {
                    break;
                }

                j++;
                i = j;
            }

            start = end + 1;

            // Judge if it is the latest cpuset limit information
            if(cpuset_cgroup[pid].pid == -1 || (cpuset_cgroup[pid].pid != -1 && cpuset_cgroup[pid].cpus != cpus)) {
                cpuset_cgroup[pid].pid = pid;
                cpuset_cgroup[pid].cpus = cpus;

                if (get_proc_data(pid, &mpd) != OK)
		            return;

                if (sys_cgptosched(mpd.mpd_endpoint, -1, cpus) != OK) {
                    return;
                }
                
            }
        }

        if(ptr[end] == '\0')
            break;
    }
}