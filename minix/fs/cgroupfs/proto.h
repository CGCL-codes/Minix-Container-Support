#ifndef _CGROUPFS_PROTO_H
#define _CGROUPFS_PROTO_H

/* buf.c */
void buf_init(char * ptr, size_t len, off_t start);
void buf_write(char * data, char * ptr, size_t len, off_t start);
void buf_read(char * data, size_t len);
ssize_t buf_result(void);

/* file.c */
ssize_t read_hook(struct inode *inode, char *ptr, size_t len, 
    off_t off, cbdata_t cbdata);
ssize_t write_hook(struct inode *inode, char *ptr, size_t max,
    off_t off, cbdata_t cbdata);

/* memory.c */
void mem_cgroup_init(struct inode *root);



#endif