#include "font_data.h"

static const unsigned char font_002e[] = {//.
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

static const unsigned char font_0030[] = {//0
0x00,0xc0,0xf0,0x38,0x08,0x08,0x08,0x38,0xf0,0xc0,0x00,
0x00,0x3f,0xff,0xc0,0x00,0x00,0x00,0xc0,0xff,0x3f,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
};

static const unsigned char font_0031[] = {//1
0x00,0x00,0x00,0x10,0x10,0xf8,0xf8,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,
};

static const unsigned char font_0032[] = {//2
0x00,0xe0,0xf0,0x18,0x08,0x08,0x08,0x18,0xf0,0xe0,0x00,
0x00,0x80,0xc0,0x60,0x30,0x18,0x0c,0x06,0xc3,0xc1,0x00,
0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
};

static const unsigned char font_0033[] = {//3
0x00,0x70,0x78,0x08,0x08,0x08,0x18,0xf0,0xe0,0x00,0x00,
0x00,0xe0,0xe0,0x00,0x02,0x02,0x03,0x87,0xfc,0x78,0x00,
0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
};

static const unsigned char font_0034[] = {//4
0x00,0x00,0x00,0x00,0xc0,0xf0,0xf8,0xf8,0x00,0x00,0x00,
0x00,0x18,0x1e,0x17,0x11,0x10,0xff,0xff,0x10,0x10,0x00,
0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
};

static const unsigned char font_0035[] = {//5
0x00,0x00,0xf8,0xf8,0x08,0x08,0x08,0x08,0x08,0x08,0x00,
0x00,0xe0,0xe3,0x03,0x03,0x01,0x01,0x83,0xfe,0x7c,0x00,
0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
};

static const unsigned char font_0036[] = {//6
0x00,0xc0,0xe0,0x30,0x18,0x08,0x08,0x38,0x30,0x00,0x00,
0x00,0x3f,0xff,0xc3,0x01,0x01,0x01,0x83,0xfe,0x7c,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
};

static const unsigned char font_0037[] = {//7
0x00,0x38,0x38,0x08,0x08,0x08,0xc8,0xe8,0x38,0x18,0x00,
0x00,0x00,0x00,0x00,0xf8,0xfe,0x07,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
};

static const unsigned char font_0038[] = {//8
0x00,0xe0,0xf0,0x18,0x08,0x08,0x08,0x18,0xf0,0xe0,0x00,
0x00,0x78,0xfd,0x87,0x02,0x02,0x02,0x87,0xfd,0x78,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
};

static const unsigned char font_0039[] = {//9
0x00,0xe0,0xf0,0x18,0x08,0x08,0x08,0x38,0xf0,0xc0,0x00,
0x00,0x03,0xc7,0xcc,0x08,0x08,0x8c,0xe6,0x7f,0x1f,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
};

static const unsigned char font_0041[] = {//A
0x00,0x00,0x00,0xc0,0xf0,0xf0,0xc0,0x00,0x00,0x00,0x00,
0x00,0xc0,0xfc,0x3f,0x13,0x13,0x3f,0xfc,0xc0,0x00,0x00,
0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x00,
};

static const unsigned char font_0045[] = {//E
0x10,0xf0,0xf0,0x10,0x10,0x10,0x90,0xb0,0x70,0x40,0x00,
0x00,0xff,0xff,0x02,0x02,0x02,0x0f,0x8f,0xc0,0x40,0x00,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,
};

static const unsigned char font_0046[] = {//F
0x10,0xf0,0xf0,0x10,0x10,0x10,0x90,0xb0,0x70,0x40,0x00,
0x00,0xff,0xff,0x02,0x02,0x02,0x0f,0x0f,0x00,0x00,0x00,
0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

static const unsigned char font_0047[] = {//G
0x00,0x80,0xc0,0x60,0x30,0x10,0x10,0x30,0xf0,0xf0,0x00,
0x00,0x3f,0xff,0xc0,0x80,0x00,0x00,0x08,0xf8,0xf8,0x08,
0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,
};

static const unsigned char font_004c[] = {//L
0x10,0xf0,0xf0,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x80,0xc0,0x40,0x00,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,
};

static const unsigned char font_004e[] = {//N
0x10,0xf0,0xf0,0xf0,0xc0,0x00,0x10,0xf0,0xf0,0x10,0x00,
0x00,0xff,0xff,0x01,0x07,0x1e,0xf8,0xff,0xff,0x00,0x00,
0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,
};

static const unsigned char font_0055[] = {//U
0x10,0xf0,0xf0,0x10,0x00,0x00,0x10,0xf0,0xf0,0x10,0x00,
0x00,0x7f,0xff,0x80,0x00,0x00,0x80,0xff,0x7f,0x00,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
};

static const unsigned char font_0069[] = {//i
0x00,0x00,0x00,0x30,0x30,0x30,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x01,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
};

static const unsigned char font_006d[] = {//m
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0xff,0xff,0x03,0xff,0xff,0x01,0xff,0xfe,0x00,0x00,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
};

static const unsigned char font_006f[] = {//o
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x7c,0xfe,0x83,0x01,0x01,0x01,0x83,0xfe,0x7c,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
};

static const unsigned char font_0070[] = {//p
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0xff,0xff,0x83,0x01,0x01,0x83,0xfe,0x7c,0x00,
0x00,0x08,0x0f,0x0f,0x09,0x01,0x01,0x01,0x00,0x00,0x00,
};

static const unsigned char font_0072[] = {//r
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0x01,0xff,0xff,0x06,0x03,0x01,0x03,0x03,0x00,
0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
};

static const unsigned char font_0077[] = {//w
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0x7f,0xff,0xf1,0x7f,0x7f,0xf1,0xff,0x7f,0x01,0x00,
0x00,0x00,0x01,0x01,0x00,0x00,0x01,0x01,0x00,0x00,0x00,
};

const char_info_t inconsolata_bold_20[] = {
	{0x002e, font_002e, 11, 20}, //. '.'
	{0x0030, font_0030, 11, 20}, //0 '0'
	{0x0031, font_0031, 11, 20}, //1 '1'
	{0x0032, font_0032, 11, 20}, //2 '2'
	{0x0033, font_0033, 11, 20}, //3 '3'
	{0x0034, font_0034, 11, 20}, //4 '4'
	{0x0035, font_0035, 11, 20}, //5 '5'
	{0x0036, font_0036, 11, 20}, //6 '6'
	{0x0037, font_0037, 11, 20}, //7 '7'
	{0x0038, font_0038, 11, 20}, //8 '8'
	{0x0039, font_0039, 11, 20}, //9 '9'
	{0x0041, font_0041, 11, 20}, //A 'A'
	{0x0045, font_0045, 11, 20}, //E 'E'
	{0x0046, font_0046, 11, 20}, //F 'F'
	{0x0047, font_0047, 11, 20}, //G 'G'
	{0x004c, font_004c, 11, 20}, //L 'L'
	{0x004e, font_004e, 11, 20}, //N 'N'
	{0x0055, font_0055, 11, 20}, //U 'U'
	{0x0069, font_0069, 11, 20}, //i 'i'
	{0x006d, font_006d, 11, 20}, //m 'm'
	{0x006f, font_006f, 11, 20}, //o 'o'
	{0x0070, font_0070, 11, 20}, //p 'p'
	{0x0072, font_0072, 11, 20}, //r 'r'
	{0x0077, font_0077, 11, 20}, //w 'w'
};
