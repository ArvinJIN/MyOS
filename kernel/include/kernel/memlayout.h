// memory layout

#define EXTMEM 0x100000		//start of extended memory
#define PHYSTOP 0xE000000	//TOP PHYSICAL MEMORY
#define DEVSPACE 0xFE000000	//other devices are at high addresses

#define KERNBASE 0x80000000
#define KERNLINK (KERNBASE+EXTMEM)

#define V2P(a) (((uint) (a)) - KERNBASE)
#define P2V(a) ((void *)(((char *) (a)) + KERNBASE))

#define V2P_WO(x) ((x) -KERNBASE)
#define P2V_WO(x) ((x) + KERNBASE)
