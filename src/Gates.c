/*
 * File: Gates.c
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include "Gates.h"

/**
 * A Gate has one or more inputs and a single output, and can update its
 * output based on the values of its inputs.
 */
struct Gate {
	int numInputs;
	Boolean **inputs;
	Boolean *output;
	void (*update)(struct Gate*);
	void (*print)(struct Gate*);
};

static Gate* new_Gate() {
	Gate* this = (Gate*)malloc(sizeof(Gate));
	this->numInputs = 0;
	this->inputs = NULL;
	this->output = new_Boolean(false);
	return this;
}

void Gate_free(Gate* this) {
	if (this->inputs != NULL) {
		for (int i=0; i < this->numInputs; i++) {
			Boolean_free(this->inputs[i]);
		}
		free(this->inputs);
	}
	Boolean_free(this->output);
	free(this);
}

Gate** new_Gate_array(int len) {
	Gate** this = (Gate**)calloc(sizeof(Gate*), len);
	return this;
}

void Gate_update(Gate *this) {
	this->update(this);
}

void Gate_print(Gate *this) {
	this->print(this);
}

int Gate_numInputs(Gate* this) {
	return this->numInputs;
}

Boolean* Gate_getInput(Gate* this, int index) {
	return this->inputs[index];
}

Boolean* Gate_getOutput(Gate* this) {
	return this->output;
}

/**
 * UnaryGates have a single input (and a single output).
 */
static Gate* new_UnaryGate() {
	Gate* this = new_Gate();
	this->numInputs = 1;
	this->inputs = new_Boolean_array(this->numInputs);
	return this;
}

static void UnaryGate_print(Gate *this, char* op) {
	printf("%s(", op);
	Boolean_print(this->inputs[0]);
	printf(") -> ");
	Boolean_print(this->output);
}

/**
 * BinaryGates have two inputs (and a single output).
 */
static Gate* new_BinaryGate() {
	Gate* this = new_Gate();
	this->numInputs = 2;
	this->inputs = new_Boolean_array(this->numInputs);
	return this;
}

static void BinaryGate_print(Gate *this, char *op) {
	printf("%s(", op);
	Boolean_print(this->inputs[0]);
	printf(",");
	Boolean_print(this->inputs[1]);
	printf(") -> ");
	Boolean_print(this->output);
}

//
// Inverter
//

static void Inverter_update(Gate *this) {
	Boolean_setValue(this->output, !Boolean_getValue(this->inputs[0]));
}

static void Inverter_print(Gate *this) {
	UnaryGate_print(this, "NEG");
}

Gate* new_Inverter() {
	Gate* this = new_UnaryGate();
	this->inputs[0] = new_Boolean(false);
	this->update = Inverter_update;
	this->print = Inverter_print;
	return this;
}

//
// AndGate
//

static void AndGate_update(Gate *this) {
	Boolean_setValue(this->output, Boolean_getValue(this->inputs[0]) && Boolean_getValue(this->inputs[1]));
}

static void AndGate_print(Gate *this) {
	BinaryGate_print(this, "AND");
}

Gate* new_AndGate() {
	Gate* this = new_BinaryGate();
	this->inputs[0] = new_Boolean(false);
	this->inputs[1] = new_Boolean(false);
	this->update = AndGate_update;
	this->print = AndGate_print;
	return this;
}

//
// OrGate
//

static void OrGate_update(Gate *this) {
	Boolean_setValue(this->output, Boolean_getValue(this->inputs[0]) || Boolean_getValue(this->inputs[1]));
}

static void OrGate_print(Gate *this) {
	BinaryGate_print(this, "OR");
}

Gate* new_OrGate() {
	Gate* this = new_BinaryGate();
	this->inputs[0] = new_Boolean(false);
	this->inputs[1] = new_Boolean(false);
	this->update = OrGate_update;
	this->print = OrGate_print;
	return this;
}

//
// And3Gate
//

static void And3Gate_update(Gate *this) {
	Boolean_setValue(this->output, Boolean_getValue(this->inputs[0]) && Boolean_getValue(this->inputs[1]) && Boolean_getValue(this->inputs[2]));
}

static void And3Gate_print(Gate *this) {
	printf("AND(");
	Boolean_print(this->inputs[0]);
	printf(",");
	Boolean_print(this->inputs[1]);
	printf(",");
	Boolean_print(this->inputs[2]);
	printf(") -> ");
	Boolean_print(this->output);
}

Gate* new_And3Gate() {
	Gate* this = new_Gate();
	this->numInputs = 3;
	this->inputs = new_Boolean_array(this->numInputs);
	this->inputs[0] = new_Boolean(false);
	this->inputs[1] = new_Boolean(false);
	this->inputs[2] = new_Boolean(false);
	this->update = And3Gate_update;
	this->print = And3Gate_print;
	return this;
}

//
// Or4Gate
//

static void Or4Gate_update(Gate *this) {
	Boolean_setValue(this->output, Boolean_getValue(this->inputs[0]) || Boolean_getValue(this->inputs[1]) || Boolean_getValue(this->inputs[2]) || Boolean_getValue(this->inputs[3]));
}

static void Or4Gate_print(Gate *this) {
	printf("OR(");
	Boolean_print(this->inputs[0]);
	printf(",");
	Boolean_print(this->inputs[1]);
	printf(",");
	Boolean_print(this->inputs[2]);
	printf(",");
	Boolean_print(this->inputs[3]);
	printf(") -> ");
	Boolean_print(this->output);
}

Gate* new_Or4Gate() {
	Gate* this = new_Gate();
	this->numInputs = 4;
	this->inputs = new_Boolean_array(this->numInputs);
	this->inputs[0] = new_Boolean(false);
	this->inputs[1] = new_Boolean(false);
	this->inputs[2] = new_Boolean(false);
	this->inputs[3] = new_Boolean(false);
	this->update = Or4Gate_update;
	this->print = Or4Gate_print;
	return this;
}
