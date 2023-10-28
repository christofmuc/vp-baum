#include <stdio.h>  // fprintf()
#include <stdlib.h> // atoi(), srand()

#include "MerkmalsMenge.hh"

void usageMessage()
{
  fprintf(stderr, "testset - Erzeugen einer Merkmalsmenge zum Testen,\n"
	          "          geschrieben 1996 von Christof Ruch\n\n"
	  "Usage: testset <filename> <number> <dimensionality>\n\n"
	  "filename       - Name der zu erzeugenden Datei\n"
	  "number         - Anzahl der Merkmale\n"
	  "dimensionality - Dimensionalität der Merkmale\n"
	  "\n");
  exit(-1);
}

int main(int argc, char **argv)
{
  if (argc < 4)
    usageMessage();

  argv++;

  char *filename = *argv++;
  int   number   = atoi(*argv++);
  int   dim      = atoi(*argv++);

  test_set(number, dim, filename);
}
