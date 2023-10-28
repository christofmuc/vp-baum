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

void search2d()
{
	char featurename[512];
	char indexname[512];
	char number[10];
	int num, dim, d, branch, points;

	fprintf(stderr, "# Merkmalanzahl Dimension Verzweigung Blattgröße "
		"Knotenzahl Blattzahl Dateigröße "
		"Versuche Knoten Blätter Punkte\n");

	for (num = 1000; num <= 125000; num *= 5)
		for (dim = 2; dim < 18; dim += 6)
		{
			sprintf(featurename, "tmp/TEST.%d.%d", num, dim);
			test_set(num, dim, featurename);

			for (branch = 2; branch <= 128; branch <<= 1)
				for (points = 10; points <= 640; points *= 2)
				{
					sprintf(indexname, "tmp/TEST.INDEX.%d.%d.%d.%d",
						num, dim, branch, points);
					fprintf(stderr, "%d %d %d %d ",
						num, dim, branch, points);
					fflush(stderr);

					vp_generate(featurename, indexname, branch, points);

					
					float* params = malloc(sizeof(float) * dim);
					for (d = 0; d < dim; d++)
					{
						float pos = 0.5;
						params[d] = pos;
					}
					vp_search(indexname, params, dim);
					free(params);

					/* Löschen des Indexes */
					UNLINK(indexname);
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
