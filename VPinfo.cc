#include "VPBaum.hh"

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
  if (argc < 2)
    usageMessage();

  argv++;

  VPBaum baum(*argv);

  baum.baumInfo();
}
