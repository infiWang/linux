// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2009 Lemote Inc.
 * Author: Wu Zhangjin, wuzhangjin@gmail.com
 */

#include <linux/err.h>
#include <linux/smp.h>
#include <linux/platform_device.h>

#include <loongson-pch.h>

int msi_groups;

bool cpu_support_msi(void)
{
	struct cpuinfo_mips *c = &boot_cpu_data;

	if ((c->processor_id & PRID_IMP_MASK) == PRID_IMP_LOONGSON_64G) {
		msi_groups = 8;
		return true;
	}

	if ((c->processor_id & PRID_REV_MASK) >= PRID_REV_LOONGSON3A_R2_1) {
		msi_groups = 4;
		return true;
	}

	return false;
}

static struct platform_device loongson2_cpufreq_device = {
	.name = "loongson2_cpufreq",
	.id = -1,
};

static struct platform_device loongson3_cpufreq_device = {
	.name = "loongson3_cpufreq",
	.id = -1,
};

static int __init loongson_cpufreq_init(void)
{
	struct cpuinfo_mips *c = &boot_cpu_data;

	/* Only 2F revision and it's successors support CPUFreq */
	if ((c->processor_id & PRID_IMP_MASK) == PRID_IMP_LOONGSON_64G)
		return platform_device_register(&loongson3_cpufreq_device);
	if ((c->processor_id & PRID_REV_MASK) == PRID_REV_LOONGSON2F)
		return platform_device_register(&loongson2_cpufreq_device);
	if ((c->processor_id & PRID_REV_MASK) >= PRID_REV_LOONGSON3A_R1)
		return platform_device_register(&loongson3_cpufreq_device);

	return -ENODEV;
}

arch_initcall(loongson_cpufreq_init);
