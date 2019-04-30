/*
 * File: main.c
 * Creator: George Ferguson
 * Created: Mon Nov 28 14:11:17 2016
 * Time-stamp: <Tue Jul 17 16:02:29 EDT 2018 ferguson>
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Circuit.h"

/**
 * Two AND gates connected to make a 3-input AND circuit.
 */
static Circuit* and3_Circuit() {
	Boolean* x = new_Boolean(false);
	Boolean* y = new_Boolean(false);
	Boolean* z = new_Boolean(false);
	Boolean** inputs = new_Boolean_array(3);
	inputs[0] = x;
	inputs[1] = y;
	inputs[2] = z;

	Boolean* out = new_Boolean(false);
	Boolean** outputs = new_Boolean_array(1);
	outputs[0] = out;

	Gate* A1 = new_AndGate();
	Gate* A2 = new_AndGate();
	Gate** gates = new_Gate_array(2);
	gates[0] = A1;
	gates[1] = A2;

	Circuit *circuit = new_Circuit(3, inputs, 1, outputs, 2, gates);
	Circuit_connect(circuit, x, Gate_getInput(A1, 0));
	Circuit_connect(circuit, y, Gate_getInput(A1, 1));
	Circuit_connect(circuit, Gate_getOutput(A1), Gate_getInput(A2, 0));
	Circuit_connect(circuit, z, Gate_getInput(A2, 1));
	Circuit_connect(circuit, Gate_getOutput(A2), out);
	return circuit;
}

static Circuit* Circuit_a() {
	Boolean* x = new_Boolean(false);
	Boolean* y = new_Boolean(false);
	Boolean* z = new_Boolean(false);
	Boolean** inputs = new_Boolean_array(3);
	inputs[0] = x;
	inputs[1] = y;
	inputs[2] = z;

	Boolean* out = new_Boolean(false);
	Boolean** outputs = new_Boolean_array(1);
	outputs[0] = out;

	Gate* A1 = new_Inverter();
	Gate* A2 = new_AndGate();
	Gate* A3 = new_AndGate();
	Gate* A4 = new_OrGate();
	Gate** gates = new_Gate_array(4);
	gates[0] = A1;
	gates[1] = A2;
	gates[2] = A3;
	gates[3] = A4;

	Circuit *circuit = new_Circuit(3, inputs, 1, outputs, 4, gates);
	Circuit_connect(circuit, x, Gate_getInput(A2, 0));
	Circuit_connect(circuit, y, Gate_getInput(A1, 0));
	Circuit_connect(circuit, y, Gate_getInput(A3, 0));
	Circuit_connect(circuit, z, Gate_getInput(A3, 1));
	Circuit_connect(circuit, Gate_getOutput(A1), Gate_getInput(A2, 1));
	Circuit_connect(circuit, Gate_getOutput(A2), Gate_getInput(A4, 0));
	Circuit_connect(circuit, Gate_getOutput(A3), Gate_getInput(A4, 1));
	Circuit_connect(circuit, Gate_getOutput(A4), out);

	return circuit;
}

static Circuit* CircuitB() {
	Boolean* x = new_Boolean(false);
	Boolean* y = new_Boolean(false);
	Boolean* z = new_Boolean(false);
	Boolean** inputs = new_Boolean_array(3);
	inputs[0] = x;
	inputs[1] = y;
	inputs[2] = z;

	Boolean* out = new_Boolean(false);
	Boolean** outputs = new_Boolean_array(1);
	outputs[0] = out;

	Gate* A0 = new_Inverter();
	Gate* A1 = new_NandGate();
	Gate* A2 = new_NandGate();
	Gate* A3 = new_NOrGate();
	Gate** gates = new_Gate_array(4);
	gates[0] = A0;
	gates[1] = A1;
	gates[2] = A2;
	gates[3] = A3;

	Circuit *circuitB = new_Circuit(3, inputs, 1, outputs, 4, gates);

	Circuit_connect(circuitB, y, Gate_getInput(A0, 0));
	
	Circuit_connect(circuitB, x, Gate_getInput(A1, 0));
	Circuit_connect(circuitB, Gate_getOutput(A0), Gate_getInput(A1, 1));

	Circuit_connect(circuitB, y, Gate_getInput(A2, 0));
	Circuit_connect(circuitB, z, Gate_getInput(A2, 1));

	Circuit_connect(circuitB, Gate_getOutput(A1), Gate_getInput(A3, 0));
	Circuit_connect(circuitB, Gate_getOutput(A2), Gate_getInput(A3, 1));

	Circuit_connect(circuitB, Gate_getOutput(A3), out);

	return circuitB;
}


static Circuit* CircuitC() {
	Boolean* x = new_Boolean(false);
	Boolean* y = new_Boolean(false);
	Boolean* z = new_Boolean(false);
	Boolean** inputs = new_Boolean_array(3);
	inputs[0] = x;
	inputs[1] = y;
	
	Boolean* out = new_Boolean(false);
	Boolean** outputs = new_Boolean_array(1);
	outputs[0] = out;

	Gate* A0 = new_AndGate();
	Gate* A1 = new_Inverter();
	Gate* A2 = new_Inverter();
	Gate* A3 = new_AndGate();
	Gate* A4 = new_OrGate();
	Gate** gates = new_Gate_array(5);
	gates[0] = A0;
	gates[1] = A1;
	gates[2] = A2;
	gates[3] = A3;
	gates[4] = A4;

	Circuit *circuitC = new_Circuit(2, inputs, 1, outputs, 5, gates);

	Circuit_connect(circuitC, y, Gate_getInput(A0, 0));
	Circuit_connect(circuitC, x, Gate_getInput(A0, 1));

	Circuit_connect(circuitC, x, Gate_getInput(A1, 0));

	Circuit_connect(circuitC, y, Gate_getInput(A2, 0));
	
	Circuit_connect(circuitC, Gate_getOutput(A1), Gate_getInput(A3, 0));
	Circuit_connect(circuitC, Gate_getOutput(A2), Gate_getInput(A3, 1));

	Circuit_connect(circuitC, Gate_getOutput(A0), Gate_getInput(A4, 0));
	Circuit_connect(circuitC, Gate_getOutput(A3), Gate_getInput(A4, 1));

	Circuit_connect(circuitC, Gate_getOutput(A4), out);
	
	return circuitC;
}

static char* b2s(bool b) {
	return b ? "T" : "F";
}

static void test3In1Out(Circuit* circuit, bool in0, bool in1, bool in2) {
	Circuit_setInput(circuit, 0, in0);
	Circuit_setInput(circuit, 1, in1);
	Circuit_setInput(circuit, 2, in2);
	Circuit_update(circuit);
	bool out0 = Circuit_getOutput(circuit, 0);
	printf("%s %s %s -> %s\n", b2s(in0), b2s(in1), b2s(in2), b2s(out0));
}

static void test(Circuit* circuit){
	int n = Circuit_numInputs(circuit);
	int combo[n];
	int mask = 1;
	int numCombos = pow(2,n);
	for(int i=0; i<numCombos; i++){
		for(int j=0; j<n; j++){
			combo[j] = i >> j & mask;
			Circuit_setInput(circuit, j, combo[j]);
		}
		Circuit_update(circuit);
		bool out0 = Circuit_getOutput(circuit, 0);
		for(int j=0; j<n; j++) printf("%s ", b2s(combo[j]));
		printf(" - > %s ", b2s(out0));
		printf("\n");
	}
	
}


int main(int argc, char **argv) {

	printf("\n\nTESTING CIRCUIT A:\n\n\n");
	Circuit* circuit = Circuit_a();
	Circuit_dump(circuit);
	printf("\n");
	printf("\n\nRESULTS FOR A\n\n");
	test(circuit);
	Circuit_free(circuit);

	printf("\n\nTESTING CIRCUIT B:\n");
	Circuit* circuitB = CircuitB();
	Circuit_dump(circuitB);
	printf("\n");
	printf("\n\nRESULTS FOR B\n\n");
	test(circuitB);
	Circuit_free(circuitB);

	printf("\n\nTESTING CIRCUIT C:\n");
	Circuit* circuitC = CircuitC();
	Circuit_dump(circuitC);
	printf("\n");
	printf("\n\nRESULTS FOR C\n\n");
	test(circuitC);
	Circuit_free(circuitC);


	// printf("The and3 circuit (AND of three inputs):\n");
	// Circuit_dump(circuit);
	// printf("\n");
	// printf("Testing: Some input(s) false: should be false\n");
	// test3In1Out(circuit, true, true, false);
	// printf("Testing: All inputs true: should be true\n");
	// test3In1Out(circuit, true, true, true);
	// printf("Note: Your program needs to test all possible combinations of input values,\nin order from all false to all true, using a single function.\n");
	// Circuit_free(circuit);
}


