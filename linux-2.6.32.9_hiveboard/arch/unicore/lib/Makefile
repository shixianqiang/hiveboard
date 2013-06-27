#
# linux/arch/unicore/lib/Makefile
#
# Copyright (C) 2001-2008 GUAN Xue-tao
#

lib-y		:= backtrace.o changebit.o csumipv6.o csumpartial.o   \
		   csumpartialcopy.o csumpartialcopyuser.o clearbit.o \
		   delay.o findbit.o memchr.o memcpy.o		      \
		   memmove.o memset.o memzero.o setbit.o              \
		   strncpy_from_user.o strnlen_user.o                 \
		   strchr.o strrchr.o                                 \
		   testchangebit.o testclearbit.o testsetbit.o        \
		   ashldi3.o ashrdi3.o lshrdi3.o muldi3.o             \
		   ucmpdi2.o lib1funcs.o div64.o sha1.o               \
		   io-readsb.o io-writesb.o io-readsl.o io-writesl.o  \
		   io-readsw.o io-writesw.o

mmu-y	:= clear_user.o copy_page.o getuser.o putuser.o
mmu-y	+= copy_from_user.o copy_to_user.o

lib-$(CONFIG_MMU) += $(mmu-y)

$(obj)/csumpartialcopy.o:	$(obj)/csumpartialcopygeneric.S
$(obj)/csumpartialcopyuser.o:	$(obj)/csumpartialcopygeneric.S
