/* linux/arch/unicore/mach-sep0611/clock.c
 *
 * Copyright (c) 2009-2011 SEUIC
 *
 * Southeast University ASIC SoC support
 *
 * SEP0611 board devices definition
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  07-31-2011	zy initial version
 *
 */     
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/suspend.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/consumer.h>

#include <asm/io.h>

#include <mach/irqs.h>
#include <mach/hardware.h>

#define	MHZ 1000000UL
#define	KHZ 1000UL

#define SEP_TRANSITION_LATENCY	10000
#define SEP_FREQ_USE_LOWER_CLK	0

#if defined(CONFIG_SEP_CLK_800)
	#define SEP_CPU_FREQ_ONDEMAND_MAX	804
#elif defined(CONFIG_SEP_CLK_600)
	#define SEP_CPU_FREQ_ONDEMAND_MAX	600
#else
	#define SEP_CPU_FREQ_ONDEMAND_MAX	504
#endif

#if defined(CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE)
static struct sep_freq_table_t sep_freq_performance_table = {
	/* apll/ mpll/ dpll */
#if (SEP_CPU_FREQ_ONDEMAND_MAX >= 800)
	804*MHZ, 640*MHZ, 402*MHZ   
#elif (SEP_CPU_FREQ_ONDEMAND_MAX >= 600)
	600*MHZ, 480*MHZ, 402*MHZ  
#else
	504*MHZ, 480*MHZ, 402*MHZ
#endif
};
#endif
//#define FREQ_DBG
#ifdef FREQ_DBG
	#define	freq_dbg(fmt, args...)	printk(fmt, ##args)
#else
	#define	freq_dbg(fmt, args...)
#endif

static spinlock_t sep0611_cpufreq_lock;

static struct sep_freq_table_t sep_freq_table[] = {
	/*unit is kHz*/
//	{100*KHZ, 180*KHZ, 400*KHZ},
//	{150*KHZ, 180*KHZ, 400*KHZ},
	{175*KHZ, 180*KHZ, 400*KHZ},
//	{200*KHZ, 180*KHZ, 400*KHZ},
//    {250*KHZ, 180*KHZ, 400*KHZ},
//	{275*KHZ, 180*KHZ, 400*KHZ},
	{300*KHZ, 180*KHZ, 400*KHZ},
//	{350*KHZ, 180*KHZ, 400*KHZ},
//	{400*KHZ, 300*KHZ, 400*KHZ},
//	{450*KHZ, 300*KHZ, 400*KHZ},
	{500*KHZ, 300*KHZ, 400*KHZ},
//	{550*KHZ, 480*KHZ, 400*KHZ},
//	{600*KHZ, 480*KHZ, 400*KHZ},
	{650*KHZ, 480*KHZ, 400*KHZ},
//	{700*KHZ, 640*KHZ, 400*KHZ},
//	{750*KHZ, 640*KHZ, 400*KHZ},
	{800*KHZ, 640*KHZ, 400*KHZ},
};
#define NUM_FREQS	ARRAY_SIZE(sep_freq_table)

struct sep_freq_limit_table_t {
	struct sep_freq_table_t freq;
	unsigned int usecount;
};
/*
static struct sep_freq_limit_table_t sep_freq_limit_table[SEP_FREQ_LIMIT_MAX] = {
};
*/
static struct sep_freq_table_t sep_freq_curr_limit_table;

static struct cpufreq_frequency_table sep_cpufreq_table[NUM_FREQS + 1];

static struct clk *apll_clk;
static struct clk *mpll_clk;
static struct clk *dpll_clk;
static struct clk *bus5_clk;

// Get current CPU frequency in kHz, by calling clk_get_rate
static unsigned int sep_cpufreq_get(unsigned int cpu)
{
	int	ret;
	freq_dbg("[%s]%d\n",__func__,__LINE__);

	ret = clk_get_rate(apll_clk) / 1000; //Hz -> KHz for the up layer
	return ret;
}

static unsigned int sep_cpufreq_get_pll_table(unsigned int cpu_clk)
{
	int i;

	for (i=0 ; i<NUM_FREQS ; i++) {
		if (sep_freq_table[i].apll_freq >= cpu_clk) {
			i++;
			freq_dbg("index in the freq table is 0d%d\n",i);
			break;
		}
	}
	freq_dbg("the selected freq is 0d%d\n",sep_freq_table[i-1].apll_freq);

	return sep_freq_table[i-1].apll_freq;
}

static int sep0611_freq_to_index(unsigned int khz)
{
	int i;
	for(i = 0; i < NUM_FREQS; i++)
		if(sep_freq_table[i].apll_freq >= khz)
		break;

	return i;
}

static unsigned int sep0611_index_to_freq(int index)
{
	unsigned int freq = 0;
	if(index < NUM_FREQS)
		freq = sep_freq_table[index].apll_freq;

	return freq;
}
#if 0
void set_i2s_clk(int bus5_rate){
	unsigned int n1, n2;
	unsigned int scr;
	unsigned int mclk = bus5_rate; 
	unsigned int base;
	unsigned int i;
	unsigned int mclk_tab[] = {
		100000000,
		180000000,
		300000000,
		480000000,
		640000000
		};
	int rate = 44100;
	int bits = 16;

	if(mclk == 0){
		printk("ERR:not find i2s clock.\n");
		return 0;
	}
	for (i = 0; i < ARRAY_SIZE(mclk_tab); i++){
		mclk = mclk_tab[i] / 8;
		freq_dbg("mclk ==================== %d\n",mclk_tab[i]);
		n1 = mclk/rate/bits/2;

		/* n1 must to be even */
		if(n1%2 == 1){
			n1 -= 1;
		}

		n2 = mclk/rate/2 - ((bits-1) * n1);

		scr = n2<<12 | n1<<4 | 1;
		freq_dbg("scr ===================== 0x%x\n", scr);
	}
	base = ioremap(0xB100A000, 0x200);
	writel(scr, base + 0x118);
}
#endif 


static int sep_cpufreq_set_clock_table(struct sep_freq_table_t *curr_clk_tbl)
{
	int ret = 0;
#if 0
	set_i2s_clk(curr_clk_tbl->mpll_freq * 1000 / 8);
#endif

	//freq_dbg("%s,%d,%d,%d\n",__func__,curr_clk_tbl->apll_freq,curr_clk_tbl->mpll_freq,curr_clk_tbl->dpll_freq);

	clk_set_rate(apll_clk, curr_clk_tbl->apll_freq * 1000); //kHz -> Hz for the driver layrer

	if (mpll_clk->rate == curr_clk_tbl->mpll_freq * 1000){
		freq_dbg("mpll rate not change: mpll = %d\n", mpll_clk->rate);
		goto dpll_set;
	}

	clk_set_rate(mpll_clk, curr_clk_tbl->mpll_freq * 1000);

	dpll_set:
	if (dpll_clk->rate == curr_clk_tbl->dpll_freq * 1000){
		freq_dbg("dpll rate not change: dpll = %d\n", dpll_clk->rate);
		return ret;
	}

	clk_set_rate(dpll_clk, curr_clk_tbl->dpll_freq * 1000);

	return ret;
}


static void sep0611_systimer_reconfig(unsigned int freq)
{
	unsigned int latch;
	unsigned long flags;
	
	latch = (freq + HZ/2)/HZ;

	local_irq_save(flags);
	writel(latch, TIMER_T2LCR_V);
	local_irq_restore(flags);
}

static int sep_cpufreq_target(struct cpufreq_policy *policy, unsigned int target_freq, unsigned int relation)
{
	struct cpufreq_freqs freqs;
	struct clk *bus4_clk;
	int ret = 0;
	int bus4_rate;
	unsigned int flags, index;

	cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq, policy->cpuinfo.max_freq);
	
	if (target_freq < policy->cpuinfo.min_freq)
		target_freq = policy->cpuinfo.min_freq;

	if (target_freq < policy->min)
		target_freq = policy->min;
/*for dbg*/
//	freq_dbg("before execute target\n");
//	freq_dbg("hard apll is 0x%lx\n",*(volatile unsigned long*)PMU_APLL_CFG_V);
//	freq_dbg("hard mpll is 0x%lx\n",*(volatile unsigned long*)PMU_MPLL_CFG_V);
//	freq_dbg("hard dpll is 0x%lx\n",*(volatile unsigned long*)PMU_DPLL_CFG_V);
/*for dbg*/

#if SEP_FREQ_USE_LOWER_CLK//using low freq,and change the freq (just conservative)
	freq_dbg("in SEP_FREQ_USE_LOWER_CLK\n");
/***********accrease the freq accordingly************/
	if (policy->cur < 100000 && target_freq >= 800000)
		target_freq = 100000; 
	else if (policy->cur < 300000 && target_freq >= 800000)
		target_freq = 300000;
	else if (policy->cur < 500000 && target_freq >= 800000)
		target_freq = 500000;
	else if (policy->cur < 600000 && target_freq >= 800000)
		target_freq = 600000;
//add some freq interval now, no consideration of sth 		
	else if (policy->cur < 504000 && target_freq >= 600000)
		target_freq = 504000;
	else if (target_freq >= 504000)
		target_freq = 600000;
/***********accrease the freq accordingly************/		
	else//using higher freq 
#endif
//	{	
//		freq_dbg("NO_USE_LOWER_CLK:target_freq is 0d%d\n",target_freq);
//	}
	
	index = sep0611_freq_to_index(target_freq);

	if (cpufreq_frequency_table_target(policy, sep_cpufreq_table,target_freq, relation, &index)) {
		return -EINVAL;
	}

/*
	switch(relation)
	{
	case CPUFREQ_RELATION_L:
		if (sep0611_index_to_freq(index) > policy->max)
			index--;
		break;
	case CPUFREQ_RELATION_H:
		if ((sep0611_index_to_freq(index) > target_freq) && (sep0611_index_to_freq(index - 1) >= policy->min))
			index--;
		break;
	}
*/

//copy sep_freq_table to local structure sep_freq_curr_limit_table
	memcpy(&sep_freq_curr_limit_table, &(sep_freq_table[index]), sizeof(struct sep_freq_table_t));

	freqs.old = policy->cur;
#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE
	freqs.new = sep_freq_performance_table.apll_freq;
#else
	freqs.new = sep_freq_curr_limit_table.apll_freq;
#endif
	freqs.cpu = 0;
	if (freqs.old == freqs.new) {
		printk("freqs.old = freqs.new = %d target = %d\n", freqs.new, target_freq);
		return ret;
	}
//	freq_dbg("freqs.old is %d,freqs.new is %d,freqs.cpu is %d\n",freqs.old,freqs.new,freqs.cpu);
//to protect the transition of changing freq
//	spin_lock_irq(&sep0611_cpufreq_lock);

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	printk("cpufreq: changing clk to %d; target = %d, oldfreq = %d\n",freqs.new, target_freq, freqs.old);

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE
	ret = sep_cpufreq_set_clock_table(&sep_freq_performance_table);
#else
	//freq_dbg("limit_table\n");
	ret = sep_cpufreq_set_clock_table(&sep_freq_curr_limit_table);
#endif
//	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
	bus4_clk = clk_get(NULL,"bus-4");
	if(!bus4_clk || IS_ERR(bus4_clk)){
		freq_dbg(KERN_INFO "failed to get lcd clock source\n");
		ret =  -ENOENT;
	}
	bus4_rate = clk_get_rate(bus4_clk);

	sep0611_systimer_reconfig(bus4_rate);
/*for dbg*/

//	freq_dbg("tmpapll is 0x%lx\n",*(volatile unsigned long*)PMU_APLL_CFG_V);
//	freq_dbg("tmpmpll is 0x%lx\n",*(volatile unsigned long*)PMU_MPLL_CFG_V);
//	freq_dbg("tmpdpll is 0x%lx\n",*(volatile unsigned long*)PMU_DPLL_CFG_V);
//	freq_dbg("after execute target\n");
/*for dbg*/
	
	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

//	spin_unlock_irq(&sep0611_cpufreq_lock);

	return ret;
}

static int sep_cpufreq_verify(struct cpufreq_policy *policy)
{
	if (policy->cpu != 0)
		return -EINVAL;
	freq_dbg("in sep_cpufreq_verify\n");

	return cpufreq_frequency_table_verify(policy, sep_cpufreq_table);
}

static int	sep_cpufreq_init(struct cpufreq_policy *policy)
{
	int i;
	int	ret;

	freq_dbg("NUM_FREQS is 0d%d\n", NUM_FREQS);

	if (policy->cpu != 0)
		return -EINVAL;

	apll_clk = clk_get(NULL,"apll"); 
	if (!apll_clk || IS_ERR(apll_clk)) {
		freq_dbg(KERN_INFO "failed to get apll clock source\n");
		ret =  -ENOENT;
	}

	mpll_clk = clk_get(NULL,"mpll"); 
	if (!apll_clk || IS_ERR(mpll_clk)) {
		freq_dbg(KERN_INFO "failed to get mpll clock source\n");
		ret =  -ENOENT;
	}

	dpll_clk = clk_get(NULL,"dpll"); 
	if (!dpll_clk || IS_ERR(dpll_clk)) {
		freq_dbg(KERN_INFO "failed to get dpll clock source\n");
		ret =  -ENOENT;
	}

	bus5_clk = clk_get(NULL,"bus-5"); 
	if (!bus5_clk || IS_ERR(bus5_clk)) {
		freq_dbg(KERN_INFO "failed to get bus-5 clock source\n");
		ret =  -ENOENT;
	}

	for (i = 0; i < NUM_FREQS; i++) {
		sep_cpufreq_table[i].index = i;
		sep_cpufreq_table[i].frequency = sep_freq_table[i].apll_freq;
	}
	sep_cpufreq_table[i].frequency = CPUFREQ_TABLE_END;

	policy->cur = clk_get_rate(apll_clk) / 1000; //Hz->KHz
	freq_dbg("policy->cur,policy->min,policy->max are:%d\n",policy->cur);
	//policy->cpuinfo.min_freq = sep_cpufreq_table[0].frequency;
	//policy->cpuinfo.max_freq = sep_cpufreq_table[i - 1].frequency;
//转换延迟，目前是：100*1000ns	
	policy->cpuinfo.transition_latency = SEP_TRANSITION_LATENCY * NSEC_PER_USEC;

	freq_dbg("%s:call cpufreq_frequency_table_cpuinfo\n",__func__);
	cpufreq_frequency_table_cpuinfo(policy, sep_cpufreq_table);

	freq_dbg(KERN_INFO"SEP cpufreq driver initialized\n");

	return 0;
}

#ifdef CONFIG_PM
static int sep_cpufreq_suspend(struct cpufreq_policy *policy, pm_message_t pmsg)
{
	return 0;
}

static int sep_cpufreq_resume(struct cpufreq_policy *policy)
{
	struct sep_freq_table_t *curr_clk_tbl = &sep_freq_table[NUM_FREQS - 1];

	clk_set_rate(apll_clk, curr_clk_tbl->apll_freq * 1000); //kHz -> Hz for the driver layrer

	if (mpll_clk->rate == curr_clk_tbl->mpll_freq * 1000){
		freq_dbg("mpll rate not change: mpll = %d\n", mpll_clk->rate);
		goto dpll_set;
	}

	clk_set_rate(mpll_clk, curr_clk_tbl->mpll_freq * 1000);

	dpll_set:
	if (dpll_clk->rate == curr_clk_tbl->dpll_freq * 1000){
		freq_dbg("dpll rate not change: dpll = %d\n", dpll_clk->rate);
		goto sys_clk_set_ok;	
	}

	clk_set_rate(dpll_clk, curr_clk_tbl->dpll_freq * 1000);

	sys_clk_set_ok:
	policy->cur = clk_get_rate(apll_clk) / 1000; //kHz -> Hz for the up layer
	freq_dbg("policy->cur,policy->min,policy->max are:%d\n",policy->cur);
	//转换延迟，目前是：100*1000ns	

	policy->cpuinfo.transition_latency = SEP_TRANSITION_LATENCY * NSEC_PER_USEC;
	freq_dbg("%s:call cpufreq_frequency_table_cpuinfo\n",__func__);
	cpufreq_frequency_table_cpuinfo(policy, sep_cpufreq_table);

	printk(KERN_INFO"SEP cpufreq driver resumed\n");

	return 0;
}
#endif

static struct cpufreq_driver sep_cpufreq_driver = {
	.name	= "sep6200 Freq",
	.owner	= THIS_MODULE,
	.flags	= CPUFREQ_STICKY | CPUFREQ_CONST_LOOPS,
	.init	= sep_cpufreq_init,
	.verify	= sep_cpufreq_verify,
	.target	= sep_cpufreq_target,
	.get	= sep_cpufreq_get,
	.suspend = sep_cpufreq_suspend,
	.resume	= sep_cpufreq_resume,
};

static int __init sep_cpufreq_register(void)
{
	return cpufreq_register_driver(&sep_cpufreq_driver);
}

static void __exit sep_cpufreq_exit(void)
{
	cpufreq_unregister_driver(&sep_cpufreq_driver);
}

MODULE_AUTHOR("hunterzy416@gmail.com");
MODULE_DESCRIPTION("CPU frequency scaling driver for SEP6200");

late_initcall(sep_cpufreq_register);
module_exit(sep_cpufreq_exit);


