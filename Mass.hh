#ifndef _Mass_hh
#define _Mass_hh

#include "Merkmal.hh"

#define EUKLID    1
#define CITYBLOCK 2
#define MAXIMUM   3
#define DIFFERFEATURE 4

class Mass
{
public:
  int id = 0;
  int D  = 0;

  virtual double d(Merkmal *a, Merkmal *b) = 0;
};

class EuklidMass : public Mass
{
public:
  EuklidMass();
  virtual double d(Merkmal *a, Merkmal *b);
};

class CityblockMass : public Mass
{
public:
  CityblockMass();
  virtual double d(Merkmal *a, Merkmal *b);
};


class DifferFeatureMass : public Mass
{
public:
	DifferFeatureMass();
	virtual double d(Merkmal* a, Merkmal* b);
};


class MaximumsMass : public Mass
{
public:
  MaximumsMass();
  virtual double d(Merkmal *a, Merkmal *b);
};

#endif
