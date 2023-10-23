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

int main()
{
    char featurename[512];
    char indexname[512];
    char command[512];
    int num, dim, branch, points;
    double x, y;

    fprintf(stderr, "# Dimension x y Versuche Knoten Blätter Punkte\n");

    dim = 2;
    for (num = 1000; num <= 125000; num *= 5)
	{
	    sprintf(featurename, "/tmp/TEST.%d.%d", num, dim);
	    sprintf(command, "testset %s %d %d", featurename, num, dim);
	    sys(command);

	    branch = 8;
	    points = 80;

	    sprintf(indexname, "/tmp/TEST.INDEX.%d.%d.%d.%d",
		    num, dim, branch, points);
	    sprintf(command, "VPgenerate %s %s %d %d", 
		    featurename, indexname, branch, points);

	    sys(command);

	    for (x = 0.0; x <= 1.0; x += 0.05)
		{
		    for (y = 0.0; y <= 1.0; y += 0.0)
			{
			    fprintf(stderr, "%d %f %f ", dim, x, y);
			    fflush(stderr);
			    sprintf(command, "VPsearch %s %f %f ",
				    indexname, x, y);
			    sys(command);
			}
		    fprintf(stderr, "\n");
		}
	    fprintf(stderr, "\n");

	    /* Löschen des Indexes */
	    UNLINK(indexname);

	    /* Löschen des Testdatensatzes */
		UNLINK(featurename);
	}

    return 0;
}
