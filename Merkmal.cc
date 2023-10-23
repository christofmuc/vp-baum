#include "Merkmal.hh"

Merkmal::Merkmal(int sch, int dim)
{
  dimension  = dim;
  schluessel = sch;
  werte      = new float[dim];
}

Merkmal::Merkmal(int sch, int dim, float *w)
{
  schluessel = sch;
  dimension  = dim;
  werte      = w;
}

Merkmal::Merkmal(FILE *file, int dim)
{
  dimension = dim;
  fscanf(file, "%d", &schluessel);
  werte     = new float[dimension];
  for (int i = 0; i < dim; i++)
    fscanf(file, "%f", &werte[i]);
}

Merkmal::~Merkmal()
{
  delete werte;
}

void Merkmal::speichern(FILE *f)
{
  int i;

  fprintf(f, "%d", schluessel);
  for (i = 0; i < dimension; i++)
    fprintf(f, " %f",werte[i]);
  fprintf(f, "\n");
}

