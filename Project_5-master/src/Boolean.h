/*
 * Boolean.h
 * Creator: George Ferguson
 * Created: Sun Nov 27 14:08:51 2016
 */

#ifndef BOOLEAN_H_
#define BOOLEAN_H_

#include <stdbool.h>

/**
 * A Boolean represents a mutable boolean value.
 */

typedef struct Boolean Boolean;

extern Boolean* new_Boolean(bool value);
extern void Boolean_free(Boolean* this);

extern Boolean** new_Boolean_array(int len);

extern bool Boolean_getValue(Boolean* this);
extern void Boolean_setValue(Boolean* this, bool b);
extern void Boolean_print(Boolean* this);

#endif /* BOOLEAN_H_ */
