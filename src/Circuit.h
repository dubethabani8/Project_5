/*
 * Circuit.h
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */

#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include "Gates.h"

typedef struct Circuit Circuit;

extern Circuit *new_Circuit(int numInputs, Boolean** inputs, int numOutputs, Boolean** outputs, int numGates, Gate** gates);
extern void Circuit_free(Circuit* this);

extern int Circuit_numInputs(Circuit* this);
extern Boolean** Circuit_getInputs(Circuit* this);
extern void Circuit_setInput(Circuit* this, int index, bool value);
extern int Circuit_numOutputs(Circuit* this);
extern Boolean** Circuit_getOutputs(Circuit* this);
extern bool Circuit_getOutput(Circuit* this, int index);
extern int Circuit_numGates(Circuit* this);
extern void Circuit_dump(Circuit* this);

extern void Circuit_connect(Circuit* this, Boolean* src, Boolean* dst);
extern void Circuit_update(Circuit* this);

#endif /* CIRCUIT_H_ */
