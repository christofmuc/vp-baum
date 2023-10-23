#ifndef _Seite_hh
#define _Seite_hh

#include <stdio.h> // FILE *
#include <cstdint> // uint8_t

class Seite
{
private:
  uint8_t *seite;
  uint8_t *pos;
  int   groesse;

public:
  Seite(int groesse);
  Seite(int groesse, FILE *f, long position);
  ~Seite();

  void *lesen(int size);
  void *schreiben(int inhalt);
  void *schreiben(long inhalt);
  void *schreiben(float inhalt);
  void *schreiben(void *src, int size);

  long anhaengen(FILE *f);
  long speichern(FILE *f, long pos);
};

#endif
