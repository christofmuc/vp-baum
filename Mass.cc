#include <assert.h> // assert()
#include <math.h>   // sqrt()

#include "Mass.hh"

#define SQR(x) ((x) * (x))
#define MAX(x,y) ((x > y) ? x : y)

EuklidMass::EuklidMass()
{
  id = EUKLID;
}

double EuklidMass::d(Merkmal *a, Merkmal *b)
{
  int    i;
  double summe = 0.0;

  assert(a != NULL);
  assert(b != NULL);
  assert(a->dimension == b->dimension);

  // Für statistische Zwecke
  D++;

  for (i = 0; i < a->dimension; i++)
    summe += SQR(a->werte[i] - b->werte[i]);

  return sqrt(summe);
}

CityblockMass::CityblockMass()
{
  id = CITYBLOCK;
}

double CityblockMass::d(Merkmal *a, Merkmal *b)
{
  int    i;
  double summe = 0.0;

  assert(a != NULL);
  assert(b != NULL);
  assert(a->dimension == b->dimension);

  // Für statistische Zwecke
  D++;

  for (i = 0; i < a->dimension; i++)
    summe += fabs(a->werte[i] - b->werte[i]);

  return summe;
}

DifferFeatureMass::DifferFeatureMass()
{
    id = DIFFERFEATURE;
}

double DifferFeatureMass::d(Merkmal* a, Merkmal* b)
{
    int    i;
    double summe = 0.0;

    assert(a != NULL);
    assert(b != NULL);
    assert(a->dimension == b->dimension);

    // Für statistische Zwecke
    D++;

    for (i = 0; i < a->dimension; i++) {
        if (a->werte[i] != b->werte[i]) {
            summe += 1.0;
        }
    }

    return summe;
}


MaximumsMass::MaximumsMass()
{
  id = MAXIMUM;
}

double MaximumsMass::d(Merkmal *a, Merkmal *b)
{
  int    i;
  double max = 0.0;

  assert(a != NULL);
  assert(b != NULL);
  assert(a->dimension == b->dimension);

  // Für statistische Zwecke
  D++;

  for (i = 0; i < a->dimension; i++)
    max = MAX(max, fabs(a->werte[i] - b->werte[i]));

  return max;
}

