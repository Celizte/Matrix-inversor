#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Fraction {
	int numerator;		// The top part
	int denominator;	// The bottom part
} Fraction;

typedef struct Matrix Matrix;

Fraction sum_fraction 	(Fraction n1, Fraction n2); 	// Sums a fraction with another
Fraction sub_fraction	(Fraction n1, Fraction n2);		// Substracts a fraction with another
Fraction mult_fraction	(Fraction n1, Fraction n2);		// Multplies a fraction with another
Fraction div_fraction	(Fraction n1, Fraction n2);		// Divides a fraction with another

Fraction simplify 		(Fraction n);					// Simplifies the original fraction
Fraction to_fraction	(int n, Fraction d);			// Turns a whole number into a fraction, needs a denominator

bool is_int				(Fraction n);					// Checks if the fraction is an integer, pairs well with to_int
int to_int				(Fraction n);					// Converts a fraction into a whole number

int gcd					(Fraction n);					// Function to find the gcd becuase uhhhh

Matrix *create_matrix(size_t size, Fraction fractions[size][size]);
void free_matrix(Matrix *self);


void print_matrix(const Matrix *matrix);
void print_process(const Matrix *oldMatrix, const Matrix *newMatrix);

void invert_matrix(Matrix *self);						// The ACTUAL function we all fucking love
void modify_matrix_row
	(Matrix *self, Fraction *fractions, int row);		// Modifies row of matrix

#endif
