#ifndef _MerkmalsMenge_hh
#define _MerkmalsMenge_hh

#include "Merkmal.hh"

class MerkmalsMenge
{
public:
  int     anzahl;
  int     dimension;
  Merkmal **merkmal;

  //  MerkmalsMenge(int n);
  MerkmalsMenge(int n, Merkmal **menge);
  MerkmalsMenge(int n, int dim);
  MerkmalsMenge(char *filename);
  ~MerkmalsMenge();

  void speichern(char *filename);
};

extern "C" void test_set(int number, int dim, char* filename);
extern "C" void vp_generate(char* features, char* filename, int branch, int elements);

#endif
