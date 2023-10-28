#include <stdlib.h> // rand(), RAND_MAX

#include "MerkmalsMenge.hh"

#include "VPBaum.hh"

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

void test_set(int number, int dim, char *filename)
{
    srand(1);
    MerkmalsMenge m(number, dim);
    m.speichern(filename);
}


void vp_generate(char *features, char *filename, int branch, int elements)
{
    srand(1);

    fprintf(stdout, "Lade Merkmalsdatei\n");
    MerkmalsMenge* m = new MerkmalsMenge(features);

    Mass* e = new EuklidMass();

    fprintf(stdout, "Erzeuge Baum\n");

    auto baum = new VPBaum(filename, e, m->dimension, elements, branch);
    
    {
        /* Nur die Seitengröße ist angegeben */
    //    pagesize = atoi(*argv++);
        //baum = new VPBaum(filename, e, m->dimension, pagesize);
    }

    long start = baum->speichereMenge(m);
    baum->info.startSeite = start;

    fprintf(stderr, "%d %d %d ", baum->info.knotenzahl, baum->info.blattzahl,
        baum->info.startSeite + baum->seitengroesse);
    fflush(stdout);

    delete baum;
}