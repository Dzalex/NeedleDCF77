/*
 * vfd_font.h
 *
 *  Created on: Nov 28, 2023
 *      Author: Dzale
 */

#ifndef INC_VFD_FONT_H_
#define INC_VFD_FONT_H_

const uint8_t VFD_FONT_NUMBERS[] =
{
	0x3f,		//0
	0x06,		//1
	0x5b,		//2
	0x4f,		//3
	0x66,		//4
	0x6d,		//5
	0x7d,		//6
	0x07,		//7
	0x7f,		//8
	0x6f		//9
};

const uint8_t VFD_FONT_LETTERS[] =
{
		0x77,//a
		0x7c,//b
		0x58,//c
		0x5e,//d
		0x79,//e
		0x71,//f
		0x3d,//g
		0x74,//h
		0x04,//i
		0x1e,//j
		0x75,//k
		0x38,//l
		0x55,//m
		0x54,//n
		0x5c,//o
		0x73,//p
		0x67,//q
		0x50,//r
		0x6d,//s
		0x78,//t
		0x3e,//u
		0x1c,//v
		0x6a,//w
		0x76,//x
		0x6e,//y
		0x5b//z
};

const uint8_t VFD_ACTIVATE_DOT = 0x80;	/* Do an OR operation with character that you want to show DOT with	*/

#endif /* INC_VFD_FONT_H_ */
