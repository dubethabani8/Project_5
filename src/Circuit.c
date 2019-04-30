
/*
 * Circuit.c
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include "Circuit.h"

/**
 * A Connection in a Circuit is a directed link from a src Boolean to
 * a dst Boolean.
 */
typedef struct Connection {
	Boolean *src;
	Boolean *dst;
	struct Connection *next;
} Connection;

static Connection* new_Connection(Boolean* src, Boolean* dst) {
	Connection* this = (Connection*)malloc(sizeof(Connection));
	this->src = src;
	this->dst = dst;
	this->next = NULL;
	return this;
}

/**
 * A ConnectionList is a linked list of Connections.
 * This implementation uses a ``last'' pointer for quick append.
 */
typedef struct ConnectionList {
	Connection *first;
	Connection *last;
} ConnectionList;

static ConnectionList* new_ConnectionList() {
	ConnectionList* this = (ConnectionList*)malloc(sizeof(Connection));
	this->first = NULL;
	this->last = NULL;
	return this;
}

/**
 * Free the given ConnectionList and all of its Connections (but not
 * their constituent Booleans).
 */
static void ConnectionList_free(ConnectionList* this) {
	Connection* c = this->first;
	while (c != NULL) {
		Connection *next = c->next;
		free(c);
		c = next;
	}
	free(this);
}

/**
 * Append the given Connection to this ConnectionList.
 */
static void ConnectionList_append(ConnectionList* this, Connection* conn) {
	if (this->first == NULL) {
		this->first = conn;
		this->last = conn;
	} else {
		this->last->next = conn;
		this->last = conn;
	}
}

/**
 * Add a new Connection between src and dst to this ConnectionList.
 */
static void ConnectionList_add(ConnectionList* list, Boolean* src, Boolean* dst) {
	ConnectionList_append(list, new_Connection(src, dst));
}

/**
 * Return true if there is a (direct) connection between src and dst in this
 * COnnectionList.
 */
static bool ConnectionList_find(ConnectionList* list, Boolean* src, Boolean* dst) {
	for (Connection* c=list->first; c != NULL; c=c->next) {
		if (c->src == src && c->dst == dst) {
			return true;
		}
	}
	return false;
}

/**
 * A Circuit is a graph of Gates.
 */
struct Circuit {
	int numInputs;
	Boolean** inputs;
	int numOutputs;
	Boolean** outputs;
	int numGates;
	Gate** gates;
	ConnectionList* connections;
	bool needsTopsort;
};

Circuit *new_Circuit(int numInputs, Boolean** inputs, int numOutputs, Boolean** outputs, int numGates, Gate** gates) {
	Circuit* this = (Circuit*)malloc(sizeof(Circuit));
	this->numInputs = numInputs;
	this->inputs = inputs;
	this->numOutputs = numOutputs;
	this->outputs = outputs;
	this->numGates = numGates;
	this->gates = gates;
	this->connections = new_ConnectionList();
	this->needsTopsort = false;
	return this;
}

void Circuit_free(Circuit* this) {
	for (int i=0; i < this->numInputs; i++) {
		Boolean_free(this->inputs[i]);
	}
	free(this->inputs);
	for (int i=0; i < this->numOutputs; i++) {
		Boolean_free(this->outputs[i]);
	}
	free(this->outputs);
	for (int i=0; i < this->numGates; i++) {
		Gate_free(this->gates[i]);
	}
	free(this->gates);
	ConnectionList_free(this->connections);
	free(this);
}

int Circuit_numInputs(Circuit* this) {
	return this->numInputs;
}

Boolean** Circuit_getInputs(Circuit* this) {
	return this->inputs;
}

void Circuit_setInput(Circuit* this, int index, bool value) {
	Boolean_setValue(this->inputs[index], value);
}

int Circuit_numOutputs(Circuit* this) {
	return this->numOutputs;
}

Boolean** Circuit_getOutputs(Circuit* this) {
	return this->outputs;
}

bool Circuit_getOutput(Circuit* this, int index) {
	return Boolean_getValue(this->outputs[index]);
}

int Circuit_numGates(Circuit* this) {
	return this->numGates;
}

void Circuit_dump(Circuit* this) {
	printf("Inputs:\n");
	for (int i=0; i < this->numInputs; i++) {
		printf("  %d: ", i);
		Boolean_print(this->inputs[i]);
		printf("\n");
	}
	printf("Outputs:\n");
	for (int i=0; i < this->numOutputs; i++) {
		printf("  %d: ", i);
		Boolean_print(this->outputs[i]);
		printf("\n");
	}
	printf("Gates:\n");
	for (int i=0; i < this->numGates; i++) {
		printf("  %d: ", i);
		Gate_print(this->gates[i]);
		printf("\n");
	}
	printf("Connections:\n");
	for (Connection *c=this->connections->first; c != NULL; c=c->next) {
		printf("  %p -> %p\n", c->src, c->dst);
	}
}

/**
 * Connect the src Boolean to the dst Boolean in this Circuit.
 */
void Circuit_connect(Circuit* this, Boolean* src, Boolean* dst) {
	ConnectionList_add(this->connections, src, dst);
	this->needsTopsort = true;
}

/**
 * Gate g1 ``feeds into'' Gate g2 (directly) iff the output of g1 is
 * connected to one (or more) of the inputs of g2 in this Circuit.
 */
static bool feedsInto(Circuit* this, Gate* g1, Gate* g2) {
	Boolean* out = Gate_getOutput(g1);
	for (int i=0; i < Gate_numInputs(g2); i++) {
		Boolean* in = Gate_getInput(g2, i);
		if (ConnectionList_find(this->connections, out, in)) {
			return true;
		}
	}
	return false;
}

/**
 * Topologically sort the gates array of this Circuit using the condition that
 * if the output of gate i feeds an input of gate j, then i < j.
 * Destructively modifies the given array (could be otherwise with some work).
 * @see https://en.wikipedia.org/wiki/Topological_sorting
 */
static Gate** topsort(Circuit* this) {
	int ngates = this->numGates;
	Gate** gates = this->gates;
	// Thanks Rohun Bhagat for fixing an earlier version of my sorting code!
	for (int i=0; i < ngates; i++) {
		for (int j=0; j < ngates; j++) {
			if (i > j && feedsInto(this, gates[i], gates[j])) {
				Gate *tmp = gates[i];
				gates[i] = gates[j];
				gates[j] = tmp;
			}
		}
	}
	return gates;
}

/**
 * Propagate the given Boolean value through any connections in this Circuit.
 */
static void propagate(Circuit* this, Boolean* b) {
	for (Connection* c=this->connections->first; c != NULL; c=c->next) {
		if (c->src == b) {
			Boolean_setValue(c->dst, Boolean_getValue(b));
		}
	}
}

/**
 * Update all the values in this Circuit by updating the gates in topological
 * order (topsorting if necessary), and propagating values from outputs to
 * inputs based on the Circuit's connections.
 */
void Circuit_update(Circuit* this) {
	// Topsort if topology has changed
	if (this->needsTopsort) {
		topsort(this);
		this->needsTopsort = false;
	}
	// Propagate circuit inputs initially
	for (int i=0; i < this->numInputs; i++) {
		propagate(this, this->inputs[i]);
	}
	// Update gates in topological order
	for (int i=0; i < this->numGates; i++) {
		Gate* gate_i = this->gates[i];
		Gate_update(gate_i);
		// Propoagate output value to any connected inputs
		propagate(this, Gate_getOutput(gate_i));
	}
}
