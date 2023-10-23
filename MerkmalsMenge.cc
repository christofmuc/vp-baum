#include <stdlib.h> // rand(), RAND_MAX

#include "MerkmalsMenge.hh"

// MerkmalsMenge::MerkmalsMenge(int n)
// {
//   anzahl  = n;
//   merkmal = new (Merkmal *)[n];
// }

MerkmalsMenge::MerkmalsMenge(int n, Merkmal **menge)
{
  anzahl    = n;
  dimension = menge[0]->dimension;
  merkmal   = menge;
}

MerkmalsMenge::MerkmalsMenge(char *filename)
{
  FILE *file = fopen(filename, "r");
  fscanf(file, "%d %d", &anzahl, &dimension);
  merkmal = new Merkmal*[anzahl];

  for (int i = 0; i < anzahl; i++)
    merkmal[i] = new Merkmal(file, dimension);
}

MerkmalsMenge::MerkmalsMenge(int n, int dim)
{
  int i, j;

  anzahl     = n;
  dimension  = dim;
  merkmal    = new Merkmal *[n];

  for (i = 0; i < n; i++)
    {
      merkmal[i]   = new Merkmal(i, dim);

      for (j = 0; j < dim; j++)
	merkmal[i]->werte[j] = ((float) rand()) / RAND_MAX;
    }
}

void MerkmalsMenge::speichern(char *filename)
{
  int   i;
  FILE *f = fopen(filename, "w");

  fprintf(f, "%d %d\n", anzahl, dimension);
  for (i = 0; i < anzahl; i++)
    merkmal[i]->speichern(f);

  fclose(f);
}

MerkmalsMenge::~MerkmalsMenge()
{
  delete merkmal;
}
