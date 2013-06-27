#include <linux/linkage.h>
#include <mach/hardware.h>
#include <asm/ptrace.h>
ENTRY(move2esram)

// address of  sleep_esram () (r0)
		mov r1, r30
// jump to power-off code
		ldw r2, =0x40f05000

//store r4-r28:
      		stm.w (r4-r15), [r2+]
      		stm.w (r16-r28), [r2+]
		
//Save MODE registers
		mov r3, asr
		andn	r3, r3, #0x1f
		or	r3, r3, #0x11
		mov.a asr, r3		@real
		/***************/
		mov r0, bsr
		add r2, r2, #4
		stw.w r0, [r2+]
		/***************/
		stm.w (r24-r30), [r2+]

		andn	r3, r3, #0x1f
		or	r3, r3, #0x12
		mov.a asr, r3		@intr
		/***************/
		mov r0, bsr
		add r2, r2, #4
		stw.w r0, [r2+]
		/***************/
		stm.w (r29-r30), [r2+]

		andn	r3, r3, #0x1f
		or	r3, r3, #0x17
		mov.a asr, r3		@trap
		/***************/
		mov r0, bsr
		add r2, r2, #4
		stw.w r0, [r2+]
		/***************/
		stm.w (r29-r30), [r2+]

		andn	r3, r3, #0x1f
		or	r3, r3, #0x1b
		mov.a asr, r3		@extn
		/***************/
		mov r0, bsr
		add r2, r2, #4
		stw.w r0, [r2+]
		/***************/
		stm.w (r29-r30), [r2+]

		andn	r3, r3, #0x1f
		or	r3, r3, #0x1f
		mov.a asr, r3		@susr
		/***************/
		mov r0, bsr
		add r2, r2, #4
		stw.w r0, [r2+]
		/***************/
		stm.w (r29-r30), [r2+]


// back-to priv mode
		andn r3, r3, #0xf
		or r3, r3, #0x3
		mov.a asr, r3
		/***************/
		mov r0, bsr
		add r2, r2, #4
		stw.w r0, [r2+]
		/***************/
		stm.w (r29-r30), [r2+]
                                                                    
//#########################################################        
                                                                   
//3. flush cache                                                   
                mov ip , #0                                        
                movc p0.c5,ip,#14                                  
                nop                                                
                nop                                                
                nop                                                
                nop                                                
                nop                                                
                nop                                                
                nop                                                
                nop  



//store  p0.c2 
      		add r2, r2, #4		
      		movc r0, p0.c2, #0
      		stw r0, [r2+]

// store  p0.c1 
      		add r2, r2, #4		
      		movc r0, p0.c1, #0
      		stw r0, [r2+]
		
		nop
		nop
		nop
		nop
		nop
// Move to esram

		mov	pc, r1

