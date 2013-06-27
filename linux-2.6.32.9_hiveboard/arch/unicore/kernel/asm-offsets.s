	.file	"asm-offsets.c"
@ GNU C (UC4_1.0-beta_20100415) version 4.4.2 (unicore32-linux)
@	compiled by GNU C version 4.1.2 20080704 (Red Hat 4.1.2-46), GMP version 4.3.1, MPFR version 2.4.1.
@ GGC heuristics: --param ggc-min-expand=92 --param ggc-min-heapsize=116647
@ options passed:  -nostdinc -Iinclude
@ -I/home/daniel/linux-2.6.32.9_hiveboard/arch/unicore/include
@ -Iarch/unicore/mach-sep0611/include -Acpu=unicore1 -Amachine=unity
@ -D__UNICORE_ARCH_1__ -D__SOFTFP__ -D__UNICORE32EL__ -D__unicore32__
@ -D__KERNEL__ -Uunicore32 -DKBUILD_STR(s)=#s
@ -DKBUILD_BASENAME=KBUILD_STR(asm_offsets)
@ -DKBUILD_MODNAME=KBUILD_STR(asm_offsets) -isystem
@ /usr/unicore/gnu-toolchain-unicore/uc4-1.0-beta-hard-RHELAS5/lib/gcc/unicore32-linux/4.4.2/include
@ -include include/linux/autoconf.h -MD
@ arch/unicore/kernel/.asm-offsets.s.d arch/unicore/kernel/asm-offsets.c
@ -mlittle-endian -msoft-float -auxbase-strip
@ arch/unicore/kernel/asm-offsets.s -Os -Wall -Wundef -Wstrict-prototypes
@ -Wno-trigraphs -Werror-implicit-function-declaration -Wno-format-security
@ -Wframe-larger-than=1024 -Wdeclaration-after-statement -Wno-pointer-sign
@ -fno-strict-aliasing -fno-common -fno-delete-null-pointer-checks
@ -fno-stack-protector -fomit-frame-pointer -fno-strict-overflow
@ -fno-dwarf2-cfi-asm -fconserve-stack -fverbose-asm
@ options enabled:  -falign-loops -fargument-alias -fauto-inc-dec
@ -fbranch-count-reg -fcaller-saves -fcprop-registers -fcrossjumping
@ -fcse-follow-jumps -fdefer-pop -fearly-inlining
@ -feliminate-unused-debug-types -fexpensive-optimizations
@ -fforward-propagate -ffunction-cse -fgcse -fgcse-lm
@ -fguess-branch-probability -fident -fif-conversion -fif-conversion2
@ -findirect-inlining -finline -finline-functions
@ -finline-functions-called-once -finline-small-functions -fipa-cp
@ -fipa-pure-const -fipa-reference -fira-share-save-slots
@ -fira-share-spill-slots -fivopts -fkeep-static-consts
@ -fleading-underscore -fmath-errno -fmerge-constants -fmerge-debug-strings
@ -fmove-loop-invariants -fomit-frame-pointer -foptimize-register-move
@ -foptimize-sibling-calls -fpeephole -fpeephole2 -freg-struct-return
@ -fregmove -freorder-blocks -freorder-functions -frerun-cse-after-loop
@ -fsched-interblock -fsched-spec -fsched-stalled-insns-dep
@ -fschedule-insns -fschedule-insns2 -fsigned-zeros -fsplit-ivs-in-unroller
@ -fsplit-wide-types -fthread-jumps -ftoplevel-reorder -ftrapping-math
@ -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-copy-prop
@ -ftree-copyrename -ftree-cselim -ftree-dce -ftree-dominator-opts
@ -ftree-dse -ftree-fre -ftree-loop-im -ftree-loop-ivcanon
@ -ftree-loop-optimize -ftree-parallelize-loops= -ftree-pre -ftree-reassoc
@ -ftree-scev-cprop -ftree-sink -ftree-sra -ftree-switch-conversion
@ -ftree-ter -ftree-vect-loop-version -ftree-vrp -funit-at-a-time
@ -fverbose-asm -fzero-initialized-in-bss -mglibc -mlittle-endian

@ Compiler executable checksum: e7a0fbfb1c74d4766b1155736abcae10

	.text
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	@ lr needed for prologue	@
#APP
@ 41 "arch/unicore/kernel/asm-offsets.c" 1
	
->TSK_ACTIVE_MM #224 offsetof(struct task_struct, active_mm)	@
@ 0 "" 2
@ 42 "arch/unicore/kernel/asm-offsets.c" 1
	
->
@ 0 "" 2
@ 43 "arch/unicore/kernel/asm-offsets.c" 1
	
->TI_FLAGS #0 offsetof(struct thread_info, flags)	@
@ 0 "" 2
@ 44 "arch/unicore/kernel/asm-offsets.c" 1
	
->TI_PREEMPT #4 offsetof(struct thread_info, preempt_count)	@
@ 0 "" 2
@ 45 "arch/unicore/kernel/asm-offsets.c" 1
	
->TI_ADDR_LIMIT #8 offsetof(struct thread_info, addr_limit)	@
@ 0 "" 2
@ 46 "arch/unicore/kernel/asm-offsets.c" 1
	
->TI_TASK #12 offsetof(struct thread_info, task)	@
@ 0 "" 2
@ 47 "arch/unicore/kernel/asm-offsets.c" 1
	
->TI_EXEC_DOMAIN #16 offsetof(struct thread_info, exec_domain)	@
@ 0 "" 2
@ 48 "arch/unicore/kernel/asm-offsets.c" 1
	
->TI_CPU #20 offsetof(struct thread_info, cpu)	@
@ 0 "" 2
@ 49 "arch/unicore/kernel/asm-offsets.c" 1
	
->TI_CPU_SAVE #24 offsetof(struct thread_info, cpu_context)	@
@ 0 "" 2
@ 50 "arch/unicore/kernel/asm-offsets.c" 1
	
->TI_USED_CP #132 offsetof(struct thread_info, used_cp)	@
@ 0 "" 2
@ 52 "arch/unicore/kernel/asm-offsets.c" 1
	
->TI_FPSTATE #152 offsetof(struct thread_info, fpstate)	@
@ 0 "" 2
@ 54 "arch/unicore/kernel/asm-offsets.c" 1
	
->
@ 0 "" 2
@ 55 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R0 #0 offsetof(struct pt_regs, UCreg_00)	@
@ 0 "" 2
@ 56 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R1 #4 offsetof(struct pt_regs, UCreg_01)	@
@ 0 "" 2
@ 57 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R2 #8 offsetof(struct pt_regs, UCreg_02)	@
@ 0 "" 2
@ 58 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R3 #12 offsetof(struct pt_regs, UCreg_03)	@
@ 0 "" 2
@ 59 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R4 #16 offsetof(struct pt_regs, UCreg_04)	@
@ 0 "" 2
@ 60 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R5 #20 offsetof(struct pt_regs, UCreg_05)	@
@ 0 "" 2
@ 61 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R6 #24 offsetof(struct pt_regs, UCreg_06)	@
@ 0 "" 2
@ 62 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R7 #28 offsetof(struct pt_regs, UCreg_07)	@
@ 0 "" 2
@ 63 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R8 #32 offsetof(struct pt_regs, UCreg_08)	@
@ 0 "" 2
@ 64 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R9 #36 offsetof(struct pt_regs, UCreg_09)	@
@ 0 "" 2
@ 65 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R10 #40 offsetof(struct pt_regs, UCreg_10)	@
@ 0 "" 2
@ 66 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R11 #44 offsetof(struct pt_regs, UCreg_11)	@
@ 0 "" 2
@ 67 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R12 #48 offsetof(struct pt_regs, UCreg_12)	@
@ 0 "" 2
@ 68 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R13 #52 offsetof(struct pt_regs, UCreg_13)	@
@ 0 "" 2
@ 69 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R14 #56 offsetof(struct pt_regs, UCreg_14)	@
@ 0 "" 2
@ 70 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R15 #60 offsetof(struct pt_regs, UCreg_15)	@
@ 0 "" 2
@ 71 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R16 #64 offsetof(struct pt_regs, UCreg_16)	@
@ 0 "" 2
@ 72 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R17 #68 offsetof(struct pt_regs, UCreg_17)	@
@ 0 "" 2
@ 73 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R18 #72 offsetof(struct pt_regs, UCreg_18)	@
@ 0 "" 2
@ 74 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R19 #76 offsetof(struct pt_regs, UCreg_19)	@
@ 0 "" 2
@ 75 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R20 #80 offsetof(struct pt_regs, UCreg_20)	@
@ 0 "" 2
@ 76 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R21 #84 offsetof(struct pt_regs, UCreg_21)	@
@ 0 "" 2
@ 77 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R22 #88 offsetof(struct pt_regs, UCreg_22)	@
@ 0 "" 2
@ 78 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R23 #92 offsetof(struct pt_regs, UCreg_23)	@
@ 0 "" 2
@ 79 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R24 #96 offsetof(struct pt_regs, UCreg_24)	@
@ 0 "" 2
@ 80 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R25 #100 offsetof(struct pt_regs, UCreg_25)	@
@ 0 "" 2
@ 81 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_R26 #104 offsetof(struct pt_regs, UCreg_26)	@
@ 0 "" 2
@ 82 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_FP #108 offsetof(struct pt_regs, UCreg_fp)	@
@ 0 "" 2
@ 83 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_IP #112 offsetof(struct pt_regs, UCreg_ip)	@
@ 0 "" 2
@ 84 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_SP #116 offsetof(struct pt_regs, UCreg_sp)	@
@ 0 "" 2
@ 85 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_LR #120 offsetof(struct pt_regs, UCreg_lr)	@
@ 0 "" 2
@ 86 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_PC #124 offsetof(struct pt_regs, UCreg_pc)	@
@ 0 "" 2
@ 87 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_PSR #128 offsetof(struct pt_regs, UCreg_csr)	@
@ 0 "" 2
@ 88 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_OLD_R0 #132 offsetof(struct pt_regs, UCreg_ORIG_00)	@
@ 0 "" 2
@ 89 "arch/unicore/kernel/asm-offsets.c" 1
	
->S_FRAME_SIZE #136 sizeof(struct pt_regs)	@
@ 0 "" 2
@ 90 "arch/unicore/kernel/asm-offsets.c" 1
	
->
@ 0 "" 2
@ 91 "arch/unicore/kernel/asm-offsets.c" 1
	
->VMA_VM_MM #0 offsetof(struct vm_area_struct, vm_mm)	@
@ 0 "" 2
@ 92 "arch/unicore/kernel/asm-offsets.c" 1
	
->VMA_VM_FLAGS #20 offsetof(struct vm_area_struct, vm_flags)	@
@ 0 "" 2
@ 93 "arch/unicore/kernel/asm-offsets.c" 1
	
->
@ 0 "" 2
@ 94 "arch/unicore/kernel/asm-offsets.c" 1
	
->VM_EXEC #4 VM_EXEC	@
@ 0 "" 2
@ 95 "arch/unicore/kernel/asm-offsets.c" 1
	
->
@ 0 "" 2
@ 96 "arch/unicore/kernel/asm-offsets.c" 1
	
->PAGE_SZ #4096 PAGE_SIZE	@
@ 0 "" 2
@ 97 "arch/unicore/kernel/asm-offsets.c" 1
	
->
@ 0 "" 2
@ 98 "arch/unicore/kernel/asm-offsets.c" 1
	
->SYS_ERROR0 #10420224 0x9f0000	@
@ 0 "" 2
@ 99 "arch/unicore/kernel/asm-offsets.c" 1
	
->
@ 0 "" 2
@ 100 "arch/unicore/kernel/asm-offsets.c" 1
	
->SIZEOF_MACHINE_DESC #52 sizeof(struct machine_desc)	@
@ 0 "" 2
@ 101 "arch/unicore/kernel/asm-offsets.c" 1
	
->MACHINFO_TYPE #0 offsetof(struct machine_desc, nr)	@
@ 0 "" 2
@ 102 "arch/unicore/kernel/asm-offsets.c" 1
	
->MACHINFO_NAME #12 offsetof(struct machine_desc, name)	@
@ 0 "" 2
@ 103 "arch/unicore/kernel/asm-offsets.c" 1
	
->MACHINFO_PHYSIO #4 offsetof(struct machine_desc, phys_io)	@
@ 0 "" 2
@ 104 "arch/unicore/kernel/asm-offsets.c" 1
	
->MACHINFO_PGOFFIO #8 offsetof(struct machine_desc, io_pg_offst)	@
@ 0 "" 2
@ 105 "arch/unicore/kernel/asm-offsets.c" 1
	
->
@ 0 "" 2
@ 106 "arch/unicore/kernel/asm-offsets.c" 1
	
->PROC_INFO_SZ #52 sizeof(struct proc_info_list)	@
@ 0 "" 2
@ 107 "arch/unicore/kernel/asm-offsets.c" 1
	
->PROCINFO_INITFUNC #16 offsetof(struct proc_info_list, __cpu_flush)	@
@ 0 "" 2
@ 108 "arch/unicore/kernel/asm-offsets.c" 1
	
->PROCINFO_MM_MMUFLAGS #8 offsetof(struct proc_info_list, __cpu_mm_mmu_flags)	@
@ 0 "" 2
@ 109 "arch/unicore/kernel/asm-offsets.c" 1
	
->PROCINFO_IO_MMUFLAGS #12 offsetof(struct proc_info_list, __cpu_io_mmu_flags)	@
@ 0 "" 2
@ 110 "arch/unicore/kernel/asm-offsets.c" 1
	
->PBE_ADDRESS #0 offsetof(struct pbe, address)	@
@ 0 "" 2
@ 111 "arch/unicore/kernel/asm-offsets.c" 1
	
->PBE_ORIN_ADDRESS #4 offsetof(struct pbe, orig_address)	@
@ 0 "" 2
@ 112 "arch/unicore/kernel/asm-offsets.c" 1
	
->PBE_NEXT #8 offsetof(struct pbe, next)	@
@ 0 "" 2
@ 113 "arch/unicore/kernel/asm-offsets.c" 1
	
->SWSUSP_CPU #0 offsetof(struct swsusp_arch_regs, cpu_context)	@
@ 0 "" 2
@ 115 "arch/unicore/kernel/asm-offsets.c" 1
	
->SWSUSP_FPSTATE #104 offsetof(struct swsusp_arch_regs, fpstate)	@
@ 0 "" 2
	mov	r0, #0	@,
	jump	lr
	.size	main, .-main
	.ident	"GCC: (UC4_1.0-beta_20100415) 4.4.2"
