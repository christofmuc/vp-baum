#include "KArray.hh"

#include <assert.h>
#include <spdlog/spdlog.h>

KArray::KArray(int size)
{
  entfernungen = new double[size];
  merkmale     = new Merkmal*[size];
  num          = 0;
  k            = size;
}

KArray::~KArray()
{
  delete[] entfernungen;
  delete[] merkmale;
}

double KArray::getMaxDist()
{
  if (num > 0)
    return entfernungen[num-1];
  else
    return 1e6; // Kludge!!!
}

Merkmal* KArray::getItem(int index) const {
    assert(index >= 0 && index < num);
    return merkmale[index];
}


Merkmal *KArray::nearestNeighbour()
{
  if (num > 0)
    return merkmale[0];
  else
    return NULL;
}

void KArray::insert(Merkmal *merkmal, double dist)
{
  int r, l, m;

  // Das erste Element?
  if (num == 0)
    {
      entfernungen[0] = dist;
      merkmale[0]     = merkmal;
      num++;
      return;
    }

  // Ist es auch nicht größer als das letzte?
  if ((num == k) && (dist >= entfernungen[num-1]))
    return;

  // Suchen der Position
  r = num-1;
  l = 0;
  while (r > l)
    {
      m = (r + l) / 2;
      if (dist < entfernungen[m])
	r = m - 1;
      else
	l = m + 1;
    }

  if (r < 0)
    r = 0;
  else
    if (entfernungen[r] <= dist)
      r++;

  // r zeigt auf die erste Position, die nach rechts geschoben werden muß
  if (num < k) num++;
  for (int i = num-1; i > r; i--)
    {
      entfernungen[i] = entfernungen[i-1];
      merkmale[i]     = merkmale[i-1];
    }
  entfernungen[r] = dist;
  merkmale[r]     = merkmal;
}

void KArray::list()
{
  int i;

  for (i = 0; i < num; i++) {
      spdlog::info("Item {} distance {}", i, entfernungen[i]);
  }
}

void KArray::print()
{
  int i;

  for (i = 0; i < num; i++)
    {
      spdlog::info("Item {} distance {} key {}", i, entfernungen[i], merkmale[i]->schluessel);
      //merkmale[i]->speichern(stdout);
    }
}
