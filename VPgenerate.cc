#include <stdio.h>  // fprintf()
#include <stdlib.h> // atoi(), srand()

#include "VPBaum.hh"

void usageMessage()
{
  fprintf(stderr, "VPgenerate - Erzeugen eines VP-Baums, geschrieben 1996 von"
	  " Christof Ruch\n\n"
          "Usage: VPgenerate <features> <filename> (<pagesize> | <branch> <elements>)\n\n"
	  "features    - Name der Merkmalsdatei\n"
	  "filename    - Name der Indexdatei\n"
	  "pagesize    - Größe der Einheiten\n"
	  "branch      - Zahl der Verzweigungen pro Knoten\n"
	  "elements    - Zahl der Elemente pro Blatt\n"
	  "\n");
  exit(-1);
}

int main(int argc, char **argv)
{
  VPBaum *baum;
  int     branch, elements, pagesize;

  if (argc < 4 || argc > 5)
    usageMessage();

  argv++;

  char *features  = *argv++;
  char *filename  = *argv++;

  srand(1);

  if (argc == 5)
  {
      /* Verzweigungsgrad und Speicherkapazität der Blätter sind angegeben */
      branch = atoi(*argv++);
      elements = atoi(*argv++);
  }
  else
  {
      /* Nur die Seitengröße ist angegeben */
      pagesize = atoi(*argv++);
  }
  vp_generate(features, filename, elements, branch);

  fprintf(stdout, "Fertig\n");

  return 0;
}
