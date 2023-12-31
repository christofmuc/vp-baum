#include <stdlib.h> // atof(), srand()

#include "VPBaum.hh"

void usageMessage()
{
  fprintf(stderr, "VPsearch - Suchen eines Nearest Neighbour in einem VP-Index,\n"
	          "geschrieben 1996 von Christof Ruch\n\n"
	  "Usage: VPsearch <filename> <query vector> [<sigma>]\n\n"
	  "filename     - Name der Index-Datei\n"
	  "query vector - Der Anfragepunkt\n"
	  "sigma        - Startwert f�r sigma\n");
  exit(-1);
}

int main(int argc, char **argv)
{
  float sigma;

  if (argc < 3)
    usageMessage();

  argc--; argv++;

  Merkmal *suche;
  VPBaum   baum(*argv++); argc--;

  // Erzeugen des Punktes, dessen NN gesucht wird
  if (argc < baum.info.dimension)
    {
      fprintf(stderr, "Baum hat Dimensionalit�t %d, bitte den Suchvektor "
	      "entsprechend angeben\n", baum.info.dimension);
      exit(-1);
    }
  suche = new Merkmal(0, baum.info.dimension);
  for (int i = 0; i < suche->dimension; i++)
    {
      suche->werte[i] = atof(*argv++);
      argc--;
    }

  // Die eigentliche NN-Suche
  if (argc)
    sigma = atof(*argv++); 
  else
    sigma = baum.info.minsigma;

  baum.startNNSuche(suche, &sigma);

  return 0;
}
