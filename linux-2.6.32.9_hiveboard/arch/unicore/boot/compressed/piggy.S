	.section .piggydata,#alloc
	.globl	input_data
input_data:
	.incbin	"arch/unicore/boot/compressed/piggy.gz"
	.globl	input_data_end
input_data_end:
