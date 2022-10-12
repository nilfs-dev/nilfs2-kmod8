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
 * mpage_readpages was converted to mpage_readahead in kernel 5.8.
 */
#ifndef HAVE_AOPS_READAHEAD
# define HAVE_AOPS_READAHEAD \
	(LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0))
#endif
#endif /* LINUX_VERSION_CODE */

#endif /* NILFS_KERN_FEATURE_H */
