#ifndef __VFS_NAMESPACE_H__
#define __VFS_NAMESPACE_H__

EXTERN struct mnt_namespace {
    struct vmnt *vmnt_array_ptr;
} mnt_ns;

#endif