/*
 * Gates.h
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */

#ifndef GATES_H_
#define GATES_H_

#include "Boolean.h"

typedef struct Gate Gate;

extern Gate** new_Gate_array(int len);

extern void Gate_update(Gate *this);
extern void Gate_print(Gate *this);
extern int Gate_numInputs(Gate* this);
extern Boolean* Gate_getInput(Gate* this, int index);
extern Boolean* Gate_getOutput(Gate* this);

extern Gate* new_Inverter();
extern Gate* new_AndGate();
extern Gate* new_OrGate();
extern Gate* new_And3Gate();
extern Gate* new_Or4Gate();

extern void Gate_free(Gate* this);

#endif /* GATES_H_ */
