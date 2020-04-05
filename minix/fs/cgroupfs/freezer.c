#include "head.h"

struct freezer_cgroup free_cgroup[NR_PID];

/* Freezer cgroup initialization */
void freezer_cgroup_init(struct inode *root)
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
    parent = add_inode(root, "freezer", FREEZER_CGROUP, &dir_stat, 1, 
        (cbdata_t) FREEZER_CGROUP);
    
    assert(parent != NULL);

    file_stat.mode = REG_RW_ALL_MODE;
    file_stat.uid = SUPER_USER;
    file_stat.gid = SUPER_USER;
    file_stat.size = 0;
    file_stat.dev = NO_DEV;

    inode = add_inode(parent, "freezer_state", NO_INDEX, &file_stat, 0,
        (cbdata_t) 0);
    
    assert(inode != NULL);

    strcpy(data[FREEZER_CGROUP], "000");

    /* Initialize freezer cgroup information */
    for(int i = 0; i < NR_PID; i++) {
        free_cgroup[i].pid = -1;
        free_cgroup[i].state = -1;
    }
}

/* Process running state control */
void freezer_ctl(char * ptr)
{
    int start = 0, mid = 0, end = 0;
    pid_t pid = 0;
    int state;
    struct minix_proc_data mpd;

    for(start = 0;; end++) {
        if(ptr[end] == ' ') {
            mid = end - 1;
            continue;
        }

        if(ptr[end] == '\n' || ptr[end] == '\0') {

            // Search and transform pid
            char tmp_pid[20], tmp_state[20];
            int len = mid -start + 1;
            if(len < 0) {
                break;
            }
            memcpy(tmp_pid, &ptr[start], len);
            tmp_pid[len] = '\0';
            pid = atoi(tmp_pid);

            // Transform vm_limit
            len = end - mid - 2;
            memcpy(tmp_state, &ptr[mid + 2], len);
            tmp_state[len] = '\0';

            if(strcmp(tmp_state, "FROZEN") == 0) {
                state = 0;
            } else {
                state = 1;
            }


            start = end + 1;

            // Judge if it is the latest vm_limit information. if yes, make the system call
            if(free_cgroup[pid].pid == -1 || (free_cgroup[pid].pid != -1 && free_cgroup[pid].state != state)) {
                free_cgroup[pid].pid = pid;
                free_cgroup[pid].state = state;

                if (get_proc_data(pid, &mpd) != OK)
		            return;
                
                if(state == 0) {
                    sys_kill(mpd.mpd_endpoint, SIGSTOP);
                } else {
                    sys_kill(mpd.mpd_endpoint, SIGCONT);
                }
                // if (sys_cgptovm(mpd.mpd_endpoint, vm_limit) != OK)
                //     return;
            }
        }

        if(ptr[end] == '\0')
            break;
    }
}