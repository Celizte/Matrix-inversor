#ifndef INPUT_H
#define INPUT_H

#include "matrix.h"

#include <stdbool.h>

#define BUFFER_SIZE 64

typedef struct Input {
	char text[BUFFER_SIZE];
	Fraction fraction;

	bool hasFraction;
} Input;

Input inputs();

Fraction input_to_fraction(Input *self);

#endif
