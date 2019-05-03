#include <stdio.h>
#include <kernel/driver/tty.h>
#include <memory/global_descriptor_table.h>
#include <kernel/interrupt_descriptor_table.h>


void kernel_main(void)
{
	terminal_initialize();
	printf("Hello,kernel World!\nTerminal initialization complete...\n");
	
	gdt_install();
	printf("GDT install completely...\n");

	idt_install();
	printf("IDT install completely....\n");

	
}
