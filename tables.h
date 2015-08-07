/*
 * tables.h
 *
 *  Created on: Jul 29, 2015
 *      Author: austen
 */

#ifndef TABLES_H_
#define TABLES_H_

extern const float soundFreqTable[];
extern const float lfoFreqTable[];
extern const float freqStepTable[];
float getNoteFreq(float);
float getFreqSlideAmt(float);
float getFilterFreq(float);
#endif /* TABLES_H_ */
