#include "MerkmalsMenge.hh"

#include "VPBaum.hh"

#include <stdlib.h>

void usageMessage()
{
  fprintf(stderr, "NNsearch - Suche nach den nächsten Nachbarn,\n"
	          "geschrieben 1996 von Christof Ruch\n\n"
	  "Usage: NNsearch <filename> <query vector>\n\n"
	  "filename     - Name der Index-Datei\n"
	  "query vector - Der Anfragepunkt\n");
  exit(-1);
}

void distanzSort(double *dist, Merkmal **merkmal, int l, int r)
{
  // Quicksort wie in [Sed91], S. 148
  double v, t;
  Merkmal *tt;
  int    i, j;

  if (r > l)
    {
      v = dist[r];
      i = l-1;
      j = r;
      do
	{
	  do i++; while (dist[i] < v);
	  do j--; while (dist[j] > v);
	  t = dist[i]; dist[i] = dist[j]; dist[j] = t;
	  tt = merkmal[i]; merkmal[i] = merkmal[j]; merkmal[j] = tt;
	}
      while (j > i);
      dist[j] = dist[i]; dist[i] = dist[r]; dist[r] = t;
      merkmal[j] = merkmal[i]; merkmal[i] = merkmal[r]; merkmal[r] = tt;
      distanzSort(dist, merkmal, l, i - 1);
      distanzSort(dist, merkmal, i + 1, r);
    }
}


int main(int argc, char **argv)
{
  if (argc < 2)
    usageMessage();

  argc--; argv++;

  VPBaum baum(*argv);

  argc--; argv++;

  Merkmal *suche;
  MerkmalsMenge menge(*argv++); argc--;

  // Erzeugen des Punktes, dessen NN gesucht wird
  if (argc != baum.info.dimension)
    {
      fprintf(stderr, "Baum hat Dimensionalität %d, bitte den Suchvektor "
	      "entsprechend angeben\n", baum.info.dimension);
      exit(-1);
    }
  suche = new Merkmal(0, argc);
  for (int i = 0; i < argc; i++)
    suche->werte[i] = atof(*argv++);

  // Die eigentliche NN-Suche
  fprintf(stderr, "Starte Suche...\n");

  // Berechnen aller Abstände
  //for (int i = 0; i < 

  return 0;
}
