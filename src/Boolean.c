/*
 * Boolean.c
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include "Boolean.h"

struct Boolean {
	bool value;
};

Boolean* new_Boolean(bool value) {
	Boolean* this = (Boolean*)malloc(sizeof(Boolean));
	this->value = value;
	return this;
}

void Boolean_free(Boolean* this) {
	//fprintf(stderr, "Boolean_free: this=%p\n", this);
	free(this);
}

Boolean** new_Boolean_array(int len) {
	Boolean** this = (Boolean**)calloc(sizeof(Boolean*), len);
	return this;
}

bool Boolean_getValue(Boolean* this) {
	return this->value;
}

void Boolean_setValue(Boolean* this, bool b) {
	this->value = b;
}

void Boolean_print(Boolean* this) {
	printf("0x%p=%s", this, this->value ? "true" : "false");
}
