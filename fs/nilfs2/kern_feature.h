/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Kernel dependent features
 */

#ifndef NILFS_KERN_FEATURE_H
#define NILFS_KERN_FEATURE_H

#include <linux/version.h>
#ifndef LINUX_VERSION_CODE
# include <generated/uapi/linux/version.h>
#endif

/*
 * Please define as 0/1 here if you want to override
 */

/*
 * for Red Hat Enterprise Linux 8.x clones
 */
#if defined(RHEL_MAJOR) && (RHEL_MAJOR == 8)
# if (RHEL_MINOR > 3)
#  define	HAVE_VFS_IOC_SETFLAGS_PREPARE			1
#  define	HAVE_BLKDEV_ISSUE_FLUSH_ERROR_SECTOR_ARG	0
#  define	HAVE_FALLTHROUGH_KEYWORD			1
# endif
# if (RHEL_MINOR > 4)
#  define	HAVE_SYSFS_EMIT			1
# endif
# if (RHEL_MINOR > 5)
#  define	HAVE_BLKDEV_ISSUE_FLUSH_GFP_ARG	0
# endif
# if (RHEL_MINOR > 6)
#  define	HAVE_AOPS_READAHEAD	1
# endif
#endif

/*
 * defaults
 */

/*
 * defaults dependent to kernel versions
 */
#ifdef LINUX_VERSION_CODE
/*
 * vfs_ioc_setflags_prepare(), a generic checking and prep function
 * for FS_IOC_SETFLAGS, was added in kernel 5.2.
 */
#ifndef HAVE_VFS_IOC_SETFLAGS_PREPARE
# define HAVE_VFS_IOC_SETFLAGS_PREPARE \
	(LINUX_VERSION_CODE >= KERNEL_VERSION(5, 2, 0))
#endif
/*
 * The fallthrough pseudo-keyword was introduced in kernel 5.7 to
 * replace existing "fall through" comments.
 */
#ifndef HAVE_FALLTHROUGH_KEYWORD
# define HAVE_FALLTHROUGH_KEYWORD \
	(LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
#endif
/*
 * The error sector argument was removed from blkdev_issue_flush() in
 * kernel 5.7.
 */
#ifndef HAVE_BLKDEV_ISSUE_FLUSH_ERROR_SECTOR_ARG
# define HAVE_BLKDEV_ISSUE_FLUSH_ERROR_SECTOR_ARG \
	(LINUX_VERSION_CODE < KERNEL_VERSION(5, 7, 0))
#endif
/*
 * mpage_readpages was converted to mpage_readahead in kernel 5.8.
 */
#ifndef HAVE_AOPS_READAHEAD
# define HAVE_AOPS_READAHEAD \
	(LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0))
#endif
/*
 * In kernel 5.9, two sysfs output functions sysfs_emit() and
 * sysfs_emit_at() were added to ensure that no overrun is on the
 * temporary buffer.
 */
#ifndef HAVE_SYSFS_EMIT
# define HAVE_SYSFS_EMIT \
	(LINUX_VERSION_CODE >= KERNEL_VERSION(5, 9, 0))
#endif
/*
 * The memory allocation flag was removed from the arguments of
 * blkdev_issue_flush() in kernel 5.11.
 */
#ifndef HAVE_BLKDEV_ISSUE_FLUSH_GFP_ARG
# define HAVE_BLKDEV_ISSUE_FLUSH_GFP_ARG \
	(LINUX_VERSION_CODE < KERNEL_VERSION(5, 11, 0))
#endif
#endif /* LINUX_VERSION_CODE */


#include <linux/fs.h>

/*
 * definitions dependent to above macros
 */
#if !HAVE_VFS_IOC_SETFLAGS_PREPARE
static inline int
vfs_ioc_setflags_prepare(struct inode *inode, unsigned int oldflags,
			 unsigned int flags)
{
        /*
	 * The IMMUTABLE and APPEND_ONLY flags can only be changed by
         * the relevant capability.
         *
         * This test looks nicer. Thanks to Pauline Middelink
         */
        if ((flags ^ oldflags) & (FS_APPEND_FL | FS_IMMUTABLE_FL) &&
            !capable(CAP_LINUX_IMMUTABLE))
                return -EPERM;

        return 0;
}
#endif

#if !HAVE_FALLTHROUGH_KEYWORD
#if __has_attribute(__fallthrough__)
# define fallthrough			__attribute__((__fallthrough__))
#else
# define fallthrough			do {} while (0)  /* fallthrough */
#endif
#endif

#if !HAVE_SYSFS_EMIT
#ifdef CONFIG_SYSFS
__printf(2, 3)
int sysfs_emit(char *buf, const char *fmt, ...);
#else  /* CONFIG_SYSFS */
__printf(2, 3)
static inline int sysfs_emit(char *buf, const char *fmt, ...)
{
	return 0;
}
#endif /* CONFIG_SYSFS */
#endif /* !HAVE_SYSFS_EMIT */

#endif /* NILFS_KERN_FEATURE_H */
