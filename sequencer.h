/*
 * sequencer.h
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_
#include <stdint.h>
#include <stdbool.h>
void initSequencer(void);
void pokeSequencer(void);

#define INIT_SEQUENCER_PERIOD 360
uint32_t sequencerPeriod;

#endif /* SEQUENCER_H_ */
