#include "Merkmal.hh"

Merkmal::Merkmal(int sch, int dim)
{
	dimension = dim;
	schluessel = sch;
	werte.resize(dim, 0.0f);
}

Merkmal::Merkmal(FILE* file, int dim)
{
	dimension = dim;
	fscanf(file, "%d", &schluessel);
	werte.resize(dim, 0.0f);
	for (int i = 0; i < dim; i++) {
		fscanf(file, "%f", &werte[i]);
	}
}

void Merkmal::setValues(float* w) {
	for (int i = 0; i < dimension; i++) {
		werte[i] = w[i];
	}
}

void Merkmal::speichern(FILE* f)
{
	int i;

	fprintf(f, "%d", schluessel);
	for (i = 0; i < dimension; i++) {
		fprintf(f, " %f", werte[i]);
	}
	fprintf(f, "\n");
}
