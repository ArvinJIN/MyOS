#include <kernel/interrupt_descriptor_table.h>
#include <kernel/x86.h>
#include <string.h>


//interrupt service routiness
static const char * const exception_names[] = {
    "Divide error",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection",
    "Page Fault",
    "(unknown trap)",
    "x87 FPU Floating-Point Error",
    "Alignment Check",
    "Machine-Check",
    "SIMD Floating-Point Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};


extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void isrs_install(){
    idt_set_gate(0,(unsigned)isr0,0x08,0x8F);
    idt_set_gate(1,(unsigned)isr1,0x08,0x8F);
    idt_set_gate(2,(unsigned)isr2,0x08,0x8F);
    idt_set_gate(3,(unsigned)isr3,0x08,0x8F);
    idt_set_gate(4,(unsigned)isr4,0x08,0x8F);
    idt_set_gate(5,(unsigned)isr5,0x08,0x8F);
    idt_set_gate(6,(unsigned)isr6,0x08,0x8F);
    idt_set_gate(7,(unsigned)isr7,0x08,0x8F);
    idt_set_gate(8,(unsigned)isr8,0x08,0x8F);
    idt_set_gate(9,(unsigned)isr9,0x08,0x8F);
    idt_set_gate(10,(unsigned)isr10,0x08,0x8F);
    idt_set_gate(11,(unsigned)isr11,0x08,0x8F);
    idt_set_gate(12,(unsigned)isr12,0x08,0x8F);
    idt_set_gate(13,(unsigned)isr13,0x08,0x8F);
    idt_set_gate(14,(unsigned)isr14,0x08,0x8F);
    idt_set_gate(15,(unsigned)isr15,0x08,0x8F);
    idt_set_gate(16,(unsigned)isr16,0x08,0x8F);
    idt_set_gate(17,(unsigned)isr17,0x08,0x8F);
    idt_set_gate(18,(unsigned)isr18,0x08,0x8F);
    idt_set_gate(19,(unsigned)isr19,0x08,0x8F);
    idt_set_gate(20,(unsigned)isr20,0x08,0x8F);
    idt_set_gate(21,(unsigned)isr21,0x08,0x8F);
    idt_set_gate(22,(unsigned)isr22,0x08,0x8F);
    idt_set_gate(23,(unsigned)isr23,0x08,0x8F);
    idt_set_gate(24,(unsigned)isr24,0x08,0x8F);
    idt_set_gate(25,(unsigned)isr25,0x08,0x8F);
    idt_set_gate(26,(unsigned)isr26,0x08,0x8F);
    idt_set_gate(27,(unsigned)isr27,0x08,0x8F);
    idt_set_gate(28,(unsigned)isr28,0x08,0x8F);
    idt_set_gate(29,(unsigned)isr29,0x08,0x8F);
    idt_set_gate(30,(unsigned)isr30,0x08,0x8F);
    idt_set_gate(31,(unsigned)isr31,0x08,0x8F);
}

void isr_handler(struct registers *r){
    if(r->int_no<32){
        puts(exception_names[r->int_no]);
        for(;;);
    }
}

/**********************************************************/
//interrupt request

/*
 * remap the irq controllers. 
 * before we remap, IRQ0 to IRQ7 are mapped the entries 8 to
 * 15 in idt, but entries 0 to 31 is reserved for the isrs
 * if we do not remap, something must be wrong.
 * so now, we remap IRQ0-15 to entries 32 to 47 in idt.
*/

void irq_remap(void){
    outb(0x20,0x11);
    outb(0xA0,0x11);
    outb(0x21,0x20);
    outb(0xA1,0x28);
    outb(0x21,0x04);
    outb(0xA1,0x02);
    outb(0x21,0x01);
    outb(0xA1,0x01);

    outb(0x21,0xFC);
    outb(0xA1,0xFF);
}

//enable an irq
void irq_set_mask(unsigned char irq){
    unsigned short irq_mask = (inb(0xA1<<8) + inb(0x21));
    irq_mask &= ~(1<<irq);
    outb(0xA1, irq_mask);
    outb(0x21, irq_mask >> 8); 
}

//disable an irq
void irq_clear_mask(unsigned char IRQline){
    int port;
    int value;

    if(IRQline < 8) {
        port = 0x21;
    }else{
        port = 0xA1;
        IRQline -= 8;
    }
    value = inb(port) & ~(1<<IRQline);
    outb(port, value);
}
//install entries 32 to 47 in idt
void irq_install(){
    irq_remap();
    idt_set_gate(32,(unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(33,(unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(34,(unsigned)irq2, 0x08, 0x8E);
    idt_set_gate(35,(unsigned)irq3, 0x08, 0x8E);
    idt_set_gate(36,(unsigned)irq4, 0x08, 0x8E);
    idt_set_gate(37,(unsigned)irq5, 0x08, 0x8E);
    idt_set_gate(38,(unsigned)irq6, 0x08, 0x8E);
    idt_set_gate(39,(unsigned)irq7, 0x08, 0x8E);
    idt_set_gate(40,(unsigned)irq8, 0x08, 0x8E);
    idt_set_gate(41,(unsigned)irq9, 0x08, 0x8E);
    idt_set_gate(42,(unsigned)irq10, 0x08, 0x8E);
    idt_set_gate(43,(unsigned)irq11, 0x08, 0x8E);
    idt_set_gate(44,(unsigned)irq12, 0x08, 0x8E);
    idt_set_gate(45,(unsigned)irq13, 0x08, 0x8E);
    idt_set_gate(46,(unsigned)irq14, 0x08, 0x8E);
    idt_set_gate(47,(unsigned)irq15, 0x08, 0x8E);
}

void *irq_routines[16]={0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

void info(struct registers *r){
    printf("gs = %d, fs = %d, es = %d, ds = %d\n", r->gs, r->fs, r->es, r->ds);
    printf("edi = %d, esi = %d, ebp = %d \n",r->edi, r->esi, r->ebp);
    printf("ebx = %d, edx = %d, ecx = %d, eax = %d \n",r->ebx, r->edx, r->ecx, r->eax);
    printf("int_no = %d, err_code = %d\n", r->int_no, r->err_code);
    printf("eip = %d, cs = %d, eflags = %d\n", r->eip, r->cs, r->eflags);
    printf("esp = %d, ss = %d \n", r->esp, r->ss);
}

void irq_handler(struct registers *r){
    void (*handler)(struct registers *r);

    handler = irq_routines[r->int_no-32];
    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller
    */
    if(r->int_no >= 40){
        outb(0xA0,0x20);
    }
    outb(0x20,0x20);
    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    if(handler){
        handler(r);
    }
    
}

void irq_install_handler(int irq, void(*handler)(struct registers *r)){
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq){
    irq_routines[irq]=0;
}

/**********************************************************/
void idt_clean(){
    memset(&IDT, 0, sizeof(struct IDT_entry)*256);
}

//set Gate Desciptor
void idt_set_gate(int num, unsigned long base, unsigned short selector, unsigned char type_attr){
    IDT[num].offset_lowerbits=(base & 0xFFFF);
    IDT[num].offset_higherbits=((base >> 16) & 0xFFFF);
    IDT[num].selector=(selector & 0xFF);
    IDT[num].zero=0;
    IDT[num].type_attr=(type_attr & 0xFF);
}


//install interrupt descriptor table
void idt_install(){
    idtp.limit = (sizeof(struct IDT_entry)*256)-1;
    idtp.base = &IDT;
//before we install IDT we init the memory of the IDT first
    idt_clean();
    
    isrs_install();

    irq_install();
    lidt(&idtp, sizeof(struct IDT_entry)*256);
}


