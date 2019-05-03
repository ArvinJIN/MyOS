#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/driver/tty.h>

#include <kernel/driver/vga.h>
#include <kernel/x86.h>


//unsigned int		uint32_t
//unsigned short	uint16_t
//unsigned char		uint8_t

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;
//The 32K address space from b8000 to BFFFF is video memory

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK); //VGA_COLOR_LIGHT_GREY:7   VGA_COLOR_BLACK:0    	0x07
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for(size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}


void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}


void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

/*
void terminal_putchar(char c)
{
	unsigned char uc = c;
	if(uc == '\n')
		terminal_column = 0;
	else
	{
		terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);	//Calculate the location to output
		if(++terminal_column == VGA_WIDTH)
			terminal_column =0;
	}
	
	//Beyond the screen, text is scrolled up and the last line is left blank
	if(++terminal_row >= VGA_HEIGHT){
		memmove(VGA_MEMORY, VGA_MEMORY + VGA_WIDTH, VGA_WIDTH*VGA_HEIGHT);
		--terminal_row;
		memset(,0,)
	}
		


}
*/
#define BACKSPACE 0x100
#define CRTPORT 0x3d4
static unsigned short* crt = (unsigned short*) 0xB8000;

void terminal_putchar(char c)
{
	int pos;
	//Cursor position: col+80*row.
	outb(CRTPORT, 14);
	pos = inb(CRTPORT+1) << 8;
	outb(CRTPORT, 15);
	pos |= inb(CRTPORT + 1);

	if(c=='\n')
		pos += 80 -pos%80; //new line
	else if(c==BACKSPACE)
	{
		if(pos > 0)
			--pos;
	}
	else
		crt[pos++] = (c&0xff)|0x0700;

	if((pos/80) >= 24)//scroll up
	{
		memmove(crt, crt+80, sizeof(crt[0])*(VGA_HEIGHT-1)*VGA_WIDTH);
		pos -= 80;
		memset(crt+pos, 0, sizeof(crt[0])*(VGA_HEIGHT*VGA_WIDTH-pos));
	}
	outb(CRTPORT, 14);
	outb(CRTPORT+1, pos >> 8);
	outb(CRTPORT, 15);
	outb(CRTPORT+1, pos);
	crt[pos] = ' '|0x0700;
}



//output data to terminal
void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		terminal_putchar(data[i]); //output every char of data
	}
}

/*
void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}
*/