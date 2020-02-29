/* Cgroupfs - main.c - main function of this file system */

#include "head.h"

/* The hook functions */
static struct fs_hooks hooks =
{
    .init_hook = init_hook,
    .read_hook = read_hook,
    .write_hook = write_hook,
};

/* Initialization hook */
static void init_hook(void)
{
    int first_time = TRUE;
    struct inode *root;

    if(first_time) {
        root = get_root_inode();

        mem_cgroup_init(root);

        first_time = FALSE;
    }
}

/* Main function, get into the VTreeFS loop */
int main(void)
{
    static struct inode_stat stat;

	/* Properties of the root directory. */
	stat.mode 	= DIR_ALL_MODE;
	stat.uid 	= SUPER_USER;
	stat.gid 	= SUPER_USER;
	stat.size 	= 0;
	stat.dev 	= NO_DEV;

	/* Run VTreeFS. */
	run_vtreefs(&hooks, NR_INODES, 0, &stat, NR_SUBSYSTEMS,
	    BUF_SIZE);

	return 0;
}
