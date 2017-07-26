#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 4

void makeMatrix1();
void printMatirx(double[], double[], double[], double[]);

int main(int argc, char *argv[]) {
	makeMatrix1();
	return 0;
}
void makeMatrix1() {
	double matrix[N][N], alpha[N], a[N], b[N], c[N], g[N], u[N], f[N] = { 40.8,0.8,0.8,200.8 };
	int i, j, k;
	for (int k = 0; k < N; k++) { // init sub and super array
		c[k] = -1; b[k] = -1; a[k] = 2.04;
	}
	alpha[0] = a[0], g[0] = f[0];

	for (j = 1; j < N; j++) { // entire calculation goes into these for loops
		alpha[j] = a[j] - (b[j - 1] * c[j - 1]) / alpha[j - 1];
		g[j] = f[j - 1] - (b[j - 1] / alpha[j - 1])*g[j - 1];
	}
	u[N - 1] = g[N - 1] / alpha[N - 1];
	for (j = 2; j >= 0; j--) {
		u[j] = (g[j] - (c[j] * u[j + 1])) / alpha[j];
	}
	printMatirx(f, u, a, b); //output
}
void printMatirx(double f[], double u[], double a[], double b[]) {
	int i, j, k;
	double matrix[N][N]; // fill in Matrix 
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (i == j) {
				matrix[i][j] = a[j];
			}
			else if (j == i - 1 || i == j - 1) {
				matrix[i][j] = b[j];
			}
			else {
				matrix[i][j] = 0;
			}
			printf("[%.2lf] ", matrix[i][j]);

		} printf("\n\n");
	}
	printf("\nX\n\n");

	for (k = 0; k < N; k++) {
		printf("[%lf]\n\n", u[k]);
	}
	printf("\n=\n\n");

	for (k = 0; k < N; k++) {
		printf("[%.1lf]\n\n", f[k]);
	}
}
