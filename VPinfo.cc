#include "VPBaum.hh"

#ifdef WIN32
#include <Windows.h>
#endif
#include <stdlib.h>
#include "Seite.hh"

#include <vector>

void usageMessage()
{
  fprintf(stderr,"VPinfo - zeigt Informationen über eine VPBaum-Datei an,\n"
	         "         geschrieben 1996 von Christof Ruch\n\n");
  fprintf(stderr,"Usage: VPinfo <filename>\n\n"
	  "filename  - Name der VPBaum-Datei\n");
  exit(-1);
}

int main(int argc, char **argv)
{
#ifdef WIN32
  SetConsoleOutputCP(1252);
#endif

  if (argc < 2)
    usageMessage();

  argv++;

  VPBaum baum(*argv);

  baum.baumInfo();

  // Lade Start Seite
  std::vector<long> seiten;
  long start = baum.info.startSeite;
  seiten.push_back(start);

  while (seiten.size() > 0) {
	  long start = seiten.front();
	  seiten.erase(seiten.begin());
	  auto seite = Seite(baum.seitengroesse, baum.baum, start);
	  int* kennung = (int*)seite.lesen(sizeof(int));
	  int* anzahl = (int*)seite.lesen(sizeof(int));
	  switch (*kennung) {
	  case 1: {
		  // Innerer Knoten
		  float* minSigma = (float*)seite.lesen(sizeof(float));
		  for (int d = 0; d < baum.info.dimension; d++) seite.lesen(sizeof(float)); // VP überlesen
		  printf("Innere Seite an %d Anzahl %d\n", start, *anzahl);
		  for (int b = 0; b < *anzahl; b++) {
			  float* mu2 = (float*)seite.lesen(sizeof(float));
			  long* neueseite = (long*)seite.lesen(sizeof(long));
			  printf("Seite %f bei %d\n", *mu2, *neueseite);
			  seiten.push_back(*neueseite);
		  }

		  break;
	  }
	  case 2: {
		  // Blatt
		  float* minSigma = (float*)seite.lesen(sizeof(float));
		  printf("Blatt an %d Anzahl %d sigma %f\n", start, *anzahl, *minSigma);
		  break;
	  }
	  default:
		  printf("Unbekannte kennung %d", *kennung);
		  exit(-1);
	  }
  }
  
}
