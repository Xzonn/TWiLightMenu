@---------------------------------------------------------------------------------
	.section ".init"
@---------------------------------------------------------------------------------
	.global _start
	.align	4
	.arm

.global card_engine_start
.global card_engine_start_sync
.global card_engine_end
.global cardStruct
.global patches_offset
.global sdk_version
.global fileCluster
.global saveCluster
.global saveSize
.global language
.global gameSoftReset

#define ICACHE_SIZE	0x2000
#define DCACHE_SIZE	0x1000
#define CACHE_LINE_SIZE	32


patches_offset:
	.word	patches
intr_vblank_orig_return:
	.word	0x00000000
language:
	.word	0x00000000
gameSoftReset:
	.word	0x00000000
cheat_data_offset:    
	.word	cheat_data
#if NP3D_DIAGNOSTIC
	.word traceInit
	.word 0
	.word traceIrqEntry
traceIrqReturn:
	.word 0
	.word traceBootRead
	.word traceBootComplete

.global traceIrqReturn

traceIrqEntry:
	stmdb sp!, {r0-r12, lr}
	mov r0, sp
	ldr sp, =traceIrqStackEnd
	ldr r3, =traceIrq
	bl _blx_r3_stub
	mov sp, r0
	ldmia sp!, {r0-r12, lr}
	stmdb sp!, {lr}
	mov ip, #0x04000000
	ldr pc, traceIrqReturn
#endif

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

card_engine_start:

vblankHandler:
@ Hook the return address, then go back to the original function
	stmdb	sp!, {lr}
	adr 	lr, code_handler_start_vblank
	ldr 	r0,	intr_vblank_orig_return
	bx  	r0

code_handler_start_vblank:
	push	{r0-r12} 
	ldr	r3, =myIrqHandlerVBlank
	bl	_blx_r3_stub		@ jump to myIrqHandler

	@ exit after return
	b	exit

@---------------------------------------------------------------------------------
_blx_r3_stub:
@---------------------------------------------------------------------------------
	bx	r3

@---------------------------------------------------------------------------------

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

exit:
	pop   	{r0-r12} 
	pop  	{lr}
	bx  lr

.pool

card_engine_end:

patches:
.word	vblankHandler
.pool
