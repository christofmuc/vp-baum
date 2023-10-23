#include <stdlib.h> // malloc(), free()
#include <string.h> // memmove(), memset()

#include "Seite.hh"

Seite::Seite(int groesse)
{
  this->groesse = groesse;
  seite = (uint8_t *) malloc(groesse);
  memset(seite, 0, groesse);
  pos   = seite;
}

Seite::Seite(int g, FILE *f, long position) 
{
  groesse = g;
  seite = (uint8_t*)malloc(groesse);
  pos   = seite;
  fseek(f, position, SEEK_SET);
  fread(seite, groesse, 1, f);
}

Seite::~Seite()
{
  free(seite);
}

void *Seite::lesen(int size)
{
  uint8_t* aktuell = pos;
  pos += size;
  return aktuell;
}

void *Seite::schreiben(int inhalt)
{
  *((int *) pos) = inhalt;
  pos += sizeof(int);
  return pos;
}

void *Seite::schreiben(long inhalt)
{
  *((long *) pos) = inhalt;
  pos += sizeof(long);
  return pos;
}

void *Seite::schreiben(float inhalt)
{
  *((float *) pos) = inhalt;
  pos += sizeof(float);
  return pos;
}

void *Seite::schreiben(void *src, int size)
{
  memmove(pos, src, size);
  pos += size;
  return pos;
}

long Seite::anhaengen(FILE *f)
{
  long position;

  fseek(f, 0, SEEK_END);
  position = ftell(f);
  fwrite(seite, groesse, 1, f);

  return position;
}

long Seite::speichern(FILE *f, long pos)
{
  fseek(f, pos, SEEK_SET);
  fwrite(seite, groesse, 1, f);

  return pos;
}
