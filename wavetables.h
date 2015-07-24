/*
 * wavetables.h
 *
 *  Created on: Jul 23, 2015
 *      Author: austen
 */

#ifndef WAVETABLES_H_
#define WAVETABLES_H_

#include <stdint.h>

typedef struct Wavetable_st {
	uint16_t minval;
	uint16_t maxval;
	const uint16_t *wave;
	uint16_t size;
} Wavetable;

extern Wavetable COS256;
extern Wavetable TRI256;
extern Wavetable EXPTRI256;
extern Wavetable SAW256;


#endif /* WAVETABLES_H_ */
