/*
 * tables.c
 *
 *  Created on: Jul 29, 2015
 *      Author: austen
 */
#include "tables.h"
/*
 * Triangle wavetable
 *
 * Q: 31
 *
 */
const q31_t TriTable[256] = {
		-2147483648,-2113929216,-2080374784,-2046820352,-2013265920,-1979711488,-1946157056,-1912602624,-1879048192,-1845493760,-1811939328,-1778384896,-1744830464,-1711276032,-1677721600,-1644167168,
		-1610612736,-1577058304,-1543503872,-1509949440,-1476395008,-1442840576,-1409286144,-1375731712,-1342177280,-1308622848,-1275068416,-1241513984,-1207959552,-1174405120,-1140850688,-1107296256,
		-1073741824,-1040187392,-1006632960,-973078528,-939524096,-905969664,-872415232,-838860800,-805306368,-771751936,-738197504,-704643072,-671088640,-637534208,-603979776,-570425344,
		-536870912,-503316480,-469762048,-436207616,-402653184,-369098752,-335544320,-301989888,-268435456,-234881024,-201326592,-167772160,-134217728,-100663296,-67108864,-33554432,
		0,33554432,67108864,100663296,134217728,167772160,201326592,234881024,268435456,301989888,335544320,369098752,402653184,436207616,469762048,503316480,
		536870912,570425344,603979776,637534208,671088640,704643072,738197504,771751936,805306368,838860800,872415232,905969664,939524096,973078528,1006632960,1040187392,
		1073741824,1107296256,1140850688,1174405120,1207959552,1241513984,1275068416,1308622848,1342177280,1375731712,1409286144,1442840576,1476395008,1509949440,1543503872,1577058304,
		1610612736,1644167168,1677721600,1711276032,1744830464,1778384896,1811939328,1845493760,1879048192,1912602624,1946157056,1979711488,2013265920,2046820352,2080374784,2113929216,
		2113929216,2080374784,2046820352,2013265920,1979711488,1946157056,1912602624,1879048192,1845493760,1811939328,1778384896,1744830464,1711276032,1677721600,1644167168,1610612736,
		1577058304,1543503872,1509949440,1476395008,1442840576,1409286144,1375731712,1342177280,1308622848,1275068416,1241513984,1207959552,1174405120,1140850688,1107296256,1073741824,
		1040187392,1006632960,973078528,939524096,905969664,872415232,838860800,805306368,771751936,738197504,704643072,671088640,637534208,603979776,570425344,536870912,
		503316480,469762048,436207616,402653184,369098752,335544320,301989888,268435456,234881024,201326592,167772160,134217728,100663296,67108864,33554432,0,
		-33554432,-67108864,-100663296,-134217728,-167772160,-201326592,-234881024,-268435456,-301989888,-335544320,-369098752,-402653184,-436207616,-469762048,-503316480,-536870912,
		-570425344,-603979776,-637534208,-671088640,-704643072,-738197504,-771751936,-805306368,-838860800,-872415232,-905969664,-939524096,-973078528,-1006632960,-1040187392,-1073741824,
		-1107296256,-1140850688,-1174405120,-1207959552,-1241513984,-1275068416,-1308622848,-1342177280,-1375731712,-1409286144,-1442840576,-1476395008,-1509949440,-1543503872,-1577058304,-1610612736,
		-1644167168,-1677721600,-1711276032,-1744830464,-1778384896,-1811939328,-1845493760,-1879048192,-1912602624,-1946157056,-1979711488,-2013265920,-2046820352,-2080374784,-2113929216,-2147483648,};
/*
 * Sawtooth wavetable
 *
 * Q: 31
 *
 */
const q31_t SawTable[256] = {
0x80000000,0x81000000,0x82000000,0x83000000,0x84000000,0x85000000,0x86000000,0x87000000,0x88000000,0x89000000,0x8a000000,0x8b000000,0x8c000000,0x8d000000,0x8e000000,0x8f000000,
0x90000000,0x91000000,0x92000000,0x93000000,0x94000000,0x95000000,0x96000000,0x97000000,0x98000000,0x99000000,0x9a000000,0x9b000000,0x9c000000,0x9d000000,0x9e000000,0x9f000000,
0xa0000000,0xa1000000,0xa2000000,0xa3000000,0xa4000000,0xa5000000,0xa6000000,0xa7000000,0xa8000000,0xa9000000,0xaa000000,0xab000000,0xac000000,0xad000000,0xae000000,0xaf000000,
0xb0000000,0xb1000000,0xb2000000,0xb3000000,0xb4000000,0xb5000000,0xb6000000,0xb7000000,0xb8000000,0xb9000000,0xba000000,0xbb000000,0xbc000000,0xbd000000,0xbe000000,0xbf000000,
0xc0000000,0xc1000000,0xc2000000,0xc3000000,0xc4000000,0xc5000000,0xc6000000,0xc7000000,0xc8000000,0xc9000000,0xca000000,0xcb000000,0xcc000000,0xcd000000,0xce000000,0xcf000000,
0xd0000000,0xd1000000,0xd2000000,0xd3000000,0xd4000000,0xd5000000,0xd6000000,0xd7000000,0xd8000000,0xd9000000,0xda000000,0xdb000000,0xdc000000,0xdd000000,0xde000000,0xdf000000,
0xe0000000,0xe1000000,0xe2000000,0xe3000000,0xe4000000,0xe5000000,0xe6000000,0xe7000000,0xe8000000,0xe9000000,0xea000000,0xeb000000,0xec000000,0xed000000,0xee000000,0xef000000,
0xf0000000,0xf1000000,0xf2000000,0xf3000000,0xf4000000,0xf5000000,0xf6000000,0xf7000000,0xf8000000,0xf9000000,0xfa000000,0xfb000000,0xfc000000,0xfd000000,0xfe000000,0xff000000,
0x000000,0x1000000,0x2000000,0x3000000,0x4000000,0x5000000,0x6000000,0x7000000,0x8000000,0x9000000,0xa000000,0xb000000,0xc000000,0xd000000,0xe000000,0xf000000,
0x10000000,0x11000000,0x12000000,0x13000000,0x14000000,0x15000000,0x16000000,0x17000000,0x18000000,0x19000000,0x1a000000,0x1b000000,0x1c000000,0x1d000000,0x1e000000,0x1f000000,
0x20000000,0x21000000,0x22000000,0x23000000,0x24000000,0x25000000,0x26000000,0x27000000,0x28000000,0x29000000,0x2a000000,0x2b000000,0x2c000000,0x2d000000,0x2e000000,0x2f000000,
0x30000000,0x31000000,0x32000000,0x33000000,0x34000000,0x35000000,0x36000000,0x37000000,0x38000000,0x39000000,0x3a000000,0x3b000000,0x3c000000,0x3d000000,0x3e000000,0x3f000000,
0x40000000,0x41000000,0x42000000,0x43000000,0x44000000,0x45000000,0x46000000,0x47000000,0x48000000,0x49000000,0x4a000000,0x4b000000,0x4c000000,0x4d000000,0x4e000000,0x4f000000,
0x50000000,0x51000000,0x52000000,0x53000000,0x54000000,0x55000000,0x56000000,0x57000000,0x58000000,0x59000000,0x5a000000,0x5b000000,0x5c000000,0x5d000000,0x5e000000,0x5f000000,
0x60000000,0x61000000,0x62000000,0x63000000,0x64000000,0x65000000,0x66000000,0x67000000,0x68000000,0x69000000,0x6a000000,0x6b000000,0x6c000000,0x6d000000,0x6e000000,0x6f000000,
0x70000000,0x71000000,0x72000000,0x73000000,0x74000000,0x75000000,0x76000000,0x77000000,0x78000000,0x79000000,0x7a000000,0x7b000000,0x7c000000,0x7d000000,0x7e000000,0x7f000000,
};

q31_t getNoteFreq(q31_t semitones){
	semitones = __SSAT(semitones+0x08000000,31);
	return arm_linear_interp_q31(&noteTable[0],semitones,256);
}
q31_t getFreqSlideAmt(q31_t semitones){
	semitones = __SSAT(semitones+0x08000000,31);
	return arm_linear_interp_q31(&semitoneTable[0],semitones,256);
}
q31_t getLFOFreq(q31_t semitones){
	return arm_linear_interp_q31(&noteTable[0],semitones,256);
}
q31_t getFilterFreq(q31_t cutoff_freq){
	return arm_linear_interp_q31(&fcTable[0],cutoff_freq>>4,256);
}
