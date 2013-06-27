#
# Makefile for the machine PKUnity v3
#

obj-y	:= clock.o irq.o time.o core.o

obj-$(CONFIG_PUV3_PM)			+= pm.o
obj-$(CONFIG_PM_SLEEP)			+= sleep.o
obj-$(CONFIG_HIBERNATION)		+= hibernate.o hibernate_asm.o

obj-$(CONFIG_CPU_FREQ)			+= cpu-ucv2.o

# for machines
obj-$(CONFIG_PUV3_NB0916)		+= nb0916-setup.o
obj-$(CONFIG_PUV3_SMW0919)		+= smw0919-setup.o
