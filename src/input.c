#include "input.h"
#include "matrix.h"

#include <stdio.h>

Input inputs() {
	Input input = { 0 };

	input.hasFraction = false;

	fgets(input.text, BUFFER_SIZE, stdin);
	input.fraction = input_to_fraction(&input);

	return input;
}

Fraction input_to_fraction(Input *self) {
	if (self->hasFraction)
		return self->fraction;

	Fraction f = {0, 0};
	char c;

	if (sscanf(self->text, "%d%*[ /]%d", &f.numerator, &f.denominator) == 2)
		;
	else if (sscanf(self->text, "%d", &f.numerator) > 0)
		f.denominator = 1;
	else {
		self->hasFraction = false;
		return (Fraction) {0, 0};
	}

	self->hasFraction = true;
	return simplify(f);
}
