#ifndef _KARRAY_HH
#define _KARRAY_HH

#include "Merkmal.hh"

class KArray
{
private:
  int       num;
  int       k;
  double   *entfernungen;
  Merkmal **merkmale;

public:
  KArray(int size);
  ~KArray();

  void insert(Merkmal *merkmal, double dist);

  int getNum() { return num; };

  double getMaxDist();
  Merkmal *nearestNeighbour();

  void list();
  void print();
};

#endif
