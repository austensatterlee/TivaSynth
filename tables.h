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
extern const q15_t TriTable[];
extern const q15_t SawTable[];
#endif /* TABLES_H_ */
