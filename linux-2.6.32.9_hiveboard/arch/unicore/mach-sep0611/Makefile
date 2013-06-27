#
# Makefile for the linux kernel.
#

# Object file lists.

obj-y			:= irq.o io.o time.o devices.o gpio.o clock.o sep_misc.o
obj-m			:=
obj-n			:=
obj-			:=

obj-$(CONFIG_HIVEBOARD) += hiveboard.o 
obj-$(CONFIG_TIGER_DEMO_BOARD) += mach-tiger-demo.o 
obj-$(CONFIG_TIGER_TEST_BOARD) += mach-tiger-test.o
obj-$(CONFIG_TIGER_SPDW_BOARD) += mach-tiger-spdw.o

obj-$(CONFIG_GPIOLIB) += gpiolib.o

obj-$(CONFIG_PM) +=sep_pm.o 
obj-$(CONFIG_PM) +=sep_asm.o idle.o
obj-$(CONFIG_CPU_FREQ_SEP6200)	+= cpufreq.o
