// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/mm.h>	/* offset_in_page */
#include "kern_feature.h"


#if !HAVE_SYSFS_EMIT && defined(CONFIG_SYSFS)
int sysfs_emit(char *buf, const char *fmt, ...)
{
	va_list args;
	int len;

	if (WARN(!buf || offset_in_page(buf),
		 "invalid sysfs_emit: buf:%p\n", buf))
		return 0;

	va_start(args, fmt);
	len = vscnprintf(buf, PAGE_SIZE, fmt, args);
	va_end(args);

	return len;
}
#endif
