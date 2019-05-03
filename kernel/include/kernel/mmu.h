#define FL_IF	0x00000200
#define CR0_PE	0x00000001
#define CR0_WP	0x00010000
#define CR0_PG	0x80000000
#define CR4_PSE	0x00000010


#define SEG_KCODE	1
#define SEG_KDATA	2
#define SEG_UCODE	3
#define SEG_UDATA	4
#define SEG_TSS		5
#define NSEGS		6

#ifndef __ASSEMBLER__

struct segdesc
{
	uint lim_15_0:	16;
	uint base_15_0:	16;
	uint base_23_16:8;
	uint type:		4;
	uint s:			1;
	uint dpl:		2;
	uint p:			1;
	uint lim_19_16:	4;
	uint avl:		1;
	uint rsv1:		1;
	uint db:		1;
	uint g:			1;
	uint base_31_24:8;
};


#define SEG(type, base, lim, dpl)(struct segdesc){((lim)>>12) & 0xffff, (uint)(base) & 0xffff,((uint)(base)>>16)& 0xff, type, 1, dpl, 1, (uint)(lim) >> 28, 0, 0, 1, 1, (uint)(base)>>24}
#define SEG16(type, base, lim, dpl)(struct segdesc){(lim) & 0xffff, (uint)(base) & 0xffff, ((uint)(base)>>16) & 0xff, type, 1, dpl, 1, (uint)(lim) >> 16, 0, 0, 1, 0, (uint)(base) >> 24}
#endif

#define DPL_USER	0x3
#define STA_X		0x8
#define STA_W		0x2
#define STA_R		0x2
#define STS_T32A	0x9
#define STS_IG32	0xE
#define STS_TG32	0xF

#define PDX(va)	(((uint)(va) >> PDXSHIFT) & 0x3FF)
#define PTX(va)	(((uint)(va) >> PTXSHIFT) & 0x3FF)

#define PGADDR(d, t, o) ((uint)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

#define NPDENTRIES	1024
#define NPTENTRIES	1024
#define PGSIZE		4096

#define PTXSHIFT	12
#define PDXSHIFT	22

#define PGROUNDUP(sz)	(((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a)	(((a)) & ~(PGSIZE - 1))

#define PTE_P	0x001
#define PTE_W	0x002
#define PTE_U	0x004
#define PTE_PS	0x080
#define PTE_ADDR(pte)	((uint)(pte) & ~0xFFF)
#define PTE_FLAGS(pte)	((uint)(pte) &	oxFFF)

#ifndef __ASSEMBLER__
typedef uint pte_t;
struct taskstate
{
	uint link;
	uint esp0;
	ushort ss0;
	ushort padding1;
	uint *esp1;
	ushort ss1;
	ushort padding2;
	uint *esp2;
	ushort ss2;
	ushort padding3;
	void *cr3;
	uint *eip;
	uint eflags;
	uint eax;
	uint ecx;
	uint edx;
	uint ebx;
	uint *esp;
	uint *ebp;
	uint esi;
	uint edi;
	ushort es;
	ushort padding4;
	ushort cs;
	ushort padding5;
	ushort ss;
	ushort padding6;
	ushort ds;
	ushort padding7;
	ushort fs;
	ushort padding8;
	ushort gs;
	ushort padding9;
	ushort ldt;
	ushort padding10;
	ushort t;
	ushort iomb;
};


struct gatedesc
{
	uint off_15_0:	16;
	uint cs:		16;
	uint args:		5;
	uint rsv1:		3;
	uint type:		4;
	uint s:			1;
	uint dpl:		2;
	uint p:			1;
	uint off_31_16:	16;
};

#define SETGATE(gate, istrap, sel, off, d)			\
{													\
	(gate).off_15_0 = (uint)(off) & 0xffff;			\
	(gate).cs = (sel);								\
	(gate).args = 0;								\
	(gate).rsv1 = 0;								\
	(gate).type = (istrap) ? STS_TG32 : STS_IG32;	\
	(gate).s = 0;									\
	(gate).dpl = (d);								\
	(gate).p = 1;									\
	(gate).off_31_16 = (uint)(off) >> 16;			\
}

#endif
