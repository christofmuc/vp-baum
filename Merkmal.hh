#pragma once

#include <stdio.h> // FILE *

#include <vector>

class Merkmal
{
public:
  int     dimension;
  int     schluessel;
  std::vector<float> werte;

  Merkmal(int sch, int dim);
  //Merkmal(int sch, int dim, float *w);
  void setValues(float* w);
  Merkmal(FILE *file, int dim);

  void speichern(FILE *f);
};

