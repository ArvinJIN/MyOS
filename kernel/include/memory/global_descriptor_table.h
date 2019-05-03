#ifndef GLOBAL_DESCRIPTOR_TABLE_H
#define GLOBAL_DESCRIPTOR_TABLE_H

#include <kernel/typedef.h>

/* Defines a GDT entry. We say packed, because it prevents the
*  compiler from doing things that it thinks is best: Prevent
*  compiler "optimization" by packing */
struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

/*
struct gdt_entry
{
    uint Limit_low      :16; //Low 16 bits of segment limit
    uint Base_low       :16; //Low 16 bits of segment base address
    uint Base_middle    :8;  //Middle 8 bits of segment base address
    uint Type           :4;  //segment type
    uint S              :1;  //Descriptor Type 0=system
    uint DPL            :2;  //Descriptor Privilege Level
    uint P              :1;  //segment present
    uint Limit_high     :4;  //High 4 bits of segment limit
    uint AVL            :1;
    uint L              :1;
    uint DB             :1;
    uint G              :1;
    uint Base_high      :8;  //High 8 bits of segment base address
}__attribute__((packed));
*/
/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct gdt_ptr
{
    ushort limit;
    uint base;
}__attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[3];
struct gdt_ptr gdtp;

/* This will be a function in start.asm. We use this to properly
*  reload the new segment registers */
//This function is in the boot.S
extern void gdt_flush();

#endif
