#include <stdio.h>  /* fprintf(), sprintf(), stderr */
#include <stdlib.h> /* system() */
#include <string.h> /* strcat() */
#ifdef WIN32
#include <io.h> /* unlink() */
#define UNLINK _unlink
#else
#include <unistd.h> /* unlink() */
#define UNLINK unlink
#endif

int sys(char *command)
{
    int error;

    fprintf(stdout, "%s\n", command);
    error = system(command);

    if (error != 0)
	{
	    fprintf(stdout, "Failed with %d\n", error);
	    fprintf(stderr, "\n");
	    fflush(stderr);
	    return 0;
	}
    return 1;
}

void search2d()
{
    char featurename[512];
    char indexname[512];
    char command[512];
    char number[10];
    int num, dim, d, branch, points;

    fprintf(stderr, "# Merkmalanzahl Dimension Verzweigung Blattgröße "
	    "Knotenzahl Blattzahl Dateigröße "
	    "Versuche Knoten Blätter Punkte\n");

    for (num = 1000; num <= 125000; num *= 5)
	for (dim = 2; dim < 18; dim += 6)
	    {
		sprintf(featurename, "/tmp/TEST.%d.%d", num, dim);
		sprintf(command, "testset %s %d %d", featurename, num, dim);
		sys(command);

		for (branch = 2; branch <= 128; branch <<= 1)
		    for (points = 10; points <= 640; points *= 2)
			{
			    sprintf(indexname, "/tmp/TEST.INDEX.%d.%d.%d.%d",
				    num, dim, branch, points);
			    sprintf(command, "VPgenerate %s %s %d %d", 
				    featurename, indexname, branch, points);
			    fprintf(stderr, "%d %d %d %d ",
				    num, dim, branch, points);
			    fflush(stderr);

			    if (sys(command))
				{
				    sprintf(command, "VPsearch %s ", indexname);
				    for (d = 0; d < dim; d++)
					{
					    float pos = 0.5;
					    sprintf(number, "%f ", pos);
					    strcat(command, number); 
					}
				    sys(command);

				    /* Löschen des Indexes */
				    UNLINK(indexname);
				}
			}
		/* Löschen des Testdatensatzes */
		UNLINK(featurename);
	    }
}

int main()
{
    search2d();

    return 0;
}
