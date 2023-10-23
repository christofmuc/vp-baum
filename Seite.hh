#ifndef _Seite_hh
#define _Seite_hh

#include <stdio.h> // FILE *

class Seite
{
private:
  void *seite;
  void *pos;
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
