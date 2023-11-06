#ifndef _Merkmal_hh
#define _Merkmal_hh

#include <stdio.h> // FILE *

class Merkmal
{
public:
  int     dimension;
  int     schluessel;
  float   *werte;

  Merkmal(int sch, int dim);
  //Merkmal(int sch, int dim, float *w);
  void setValues(float* w);
  Merkmal(FILE *file, int dim);
  ~Merkmal();

  void speichern(FILE *f);
};

#endif
