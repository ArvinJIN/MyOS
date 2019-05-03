#ifndef INTERRUPT_DESCRIPTOR_TABLE_H
#define INTERRUPT_DESCRIPTOR_TABLE_H


struct IDT_entry{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
}__attribute__((packed));

struct idt_ptr{
    unsigned short limit;
    struct IDT_entry *base;
}__attribute__((packed));


struct registers{
    int gs,fs,es,ds;
    int edi, esi, ebp, _esp, ebx, edx, ecx, eax;
    int int_no, err_code;
    int eip, cs, eflags, esp, ss;
}__attribute__((packed));

struct IDT_entry IDT[256];
struct idt_ptr idtp;

//interrupt service routiness
void isrs_install();
void isr_handler(struct registers *r);

//interrupt request
void irq_remap(void);
void irq_set_mask(unsigned char IRQline);
void irq_clear_mask(unsigned char IRQline);
void irq_install();
void irq_handler(struct registers *r);
void irq_install_handler(int irq, void(*handler)(struct registers*r));
void irq_uninstall_handler(int irq);

void idt_init(void);
void idt_set_gate(int,unsigned long,unsigned short, unsigned char);



#endif