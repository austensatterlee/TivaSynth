/*
 * tables.h
 *
 *  Created on: Jul 29, 2015
 *      Author: austen
 */

#ifndef TABLES_H_
#define TABLES_H_
#include <stdint.h>
#include "arm_math.h"
q31_t getNoteFreq(q31_t);
q31_t getLFOFreq(q31_t);
q31_t getFreqSlideAmt(q31_t);
q31_t getFilterFreq(q31_t);
extern const q31_t TriTable[];
extern const q31_t SawTable[];
extern const q31_t SoftTable[];

extern const q31_t noteTable[];
extern const q31_t semitoneTable[];
extern const q31_t fcTable[];
extern const q31_t envDecayTable[];
#endif /* TABLES_H_ */
