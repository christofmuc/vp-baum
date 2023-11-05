#ifndef _VPtree_hh
#define _VPtree_hh

#include <stdio.h>

#include "Mass.hh"
#include "Merkmal.hh"
#include "MerkmalsMenge.hh"

#include "KArray.hh"

#define NN  0
#define KNN 1

class VPBaum
{
public:
  FILE *baum;

private:
    int magic = 0x020870;
  Mass *mass{nullptr};

  int  T, B, K, P;

  Merkmal *ergebnis{ nullptr };
  
  int calcProBlatt(int s);
  int calcProKnoten(int s);
  int blattGroesse(int speicher);
  int knotenGroesse(int verzweig);

  Merkmal *waehleVantagePoint(MerkmalsMenge *m);
  void distanzSort(double *dist, Merkmal **merkmal, int l, int r);

  long schreibeMenge(MerkmalsMenge *m);

  void suche(Merkmal *punkt, float *sigma, long position, int MODE, KArray* karray = nullptr);

  // Die Hashtabelle, die bereits durchsuchte Blätter merkt
  int hashSize;
  int *hashtab{ nullptr };
  Merkmal **hashmerk{ nullptr };

  void initHash(int size);
  Merkmal *istBekannt(long pos, Merkmal *punkt = NULL);
  void jetztBekannt(long pos, Merkmal *punkt);

public:
  int seitengroesse;

  struct info
  {
    int   dimension;
    int   blaetterProKnoten;
    int   elementeProBlatt;
    int   kandidatenAnzahl;
    int   testAnzahl;
    float minsigma;
    long  startSeite;
    int   knotenzahl;
    int   blattzahl;
    int   merkmalzahl;
    int   massid;
  } info;

  // Öffnen eines existierenden VP-Baums
  VPBaum(char *filename);

  // Erzeugen eines neuen Indexes mit Angabe der Seitengröße
  VPBaum(char *filename, Mass *mass, int dimension, int seitengroesse);

  // Erzeugen eines neuen Indexes mit Angabe von Verzweigungsgrad
  // und Speicherkapazität der Blätter
  VPBaum(char *filename, Mass *mass, int dimension, int proBlatt,
	 int proKnoten);

  // Destruktor
  ~VPBaum();

  long speichereMenge(MerkmalsMenge *m);

  Merkmal *startNNSuche(Merkmal *punkt, float *sigma);
  Merkmal *startKNNSuche(Merkmal *punkt, int k, float *sigma);
  KArray *kNNSuche(Merkmal *punkt, int k, float *sigma);

  void speichereInfo();
  void baumInfo();
};

extern "C" void vp_search(char *indexname, float *params, int dim);

#endif
