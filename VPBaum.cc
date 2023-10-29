#include <assert.h> /* assert() */
#include <math.h>   /* ceil(), fabs(), pow() */
#include <stdio.h>  /* fopen(), fclose(), fseek(), fread() */
#include <stdlib.h> /* malloc(), free(), rand() */
#include <string.h> /* memcpy(), memset() */
#include <exception>
#include <string>
#include <vector>
#include <errno.h>

#include "Seite.hh"
#include "VPBaum.hh"

#undef NDEBUG

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

VPBaum::VPBaum(char* filename)
{
	int   id;
	Seite* s;

	karray = NULL;
	initHash(10000);

	// Oeffnen eines existierenden VP-Baumes auf der Platte
	baum = fopen(filename, "rb+");
	if (baum == NULL)
	{
		throw std::exception("Failed to open file");
	}

	fread(&id, sizeof(int), 1, baum);
	if (id != magic)
	{
		fprintf(stderr, "Dies ist keine VP-Baum-Datei!\n");
		exit(-1);
	}
	fread(&seitengroesse, sizeof(int), 1, baum);

	s = new Seite(seitengroesse, baum, 0);

	// Überlesen der bereits zuvor gelesenen Informationen
	s->lesen(sizeof(int));
	s->lesen(sizeof(int));

	// Lesen der Bauminformationen
	info = *((struct info*)s->lesen(sizeof(struct info)));

	if (info.minsigma == 0.0)
		info.minsigma = 0.1; // Kludge!

	delete s;

	// Erzeugen des entsprechenden Maßes
	switch (info.massid)
	{
	case 0:
	case EUKLID:
		mass = new EuklidMass();
		break;
	case CITYBLOCK:
		mass = new CityblockMass();
		break;
	case MAXIMUM:
		mass = new MaximumsMass();
		break;
	default:
		fprintf(stderr, "Maßtyp %d nicht bekannt!\n", info.massid);
		exit(-1);
	}
}

VPBaum::VPBaum(char* filename, Mass* mass, int dimension, int seitengroesse)
{
	this->seitengroesse = seitengroesse;
	info.dimension = dimension;
	info.elementeProBlatt = calcProBlatt(seitengroesse);
	info.blaetterProKnoten = calcProKnoten(seitengroesse);
	info.kandidatenAnzahl = 10;
	info.testAnzahl = 10;
	info.minsigma = 0.0;
	info.knotenzahl = 0;
	info.blattzahl = 0;
	info.merkmalzahl = 0;
	info.massid = 0;
	this->mass = mass;

	// Statusseite
	assert((2 * sizeof(int) + sizeof(struct info)) <= seitengroesse);
	// Man sollte wenigstens ein Element speichern können
	assert(info.elementeProBlatt >= 1);
	// Und wenigstens einen binären Baum haben
	assert(info.blaetterProKnoten >= 2);

	// Und erzeugen der Datei
	baum = fopen(filename, "wb+");
	speichereInfo();
}

VPBaum::VPBaum(char* filename, Mass* mass, int dimension, int proBlatt,
	int proKnoten) : mass(nullptr), ergebnis(nullptr), karray(nullptr), hashtab(nullptr), hashmerk(nullptr)
{
	info.dimension = dimension;
	this->seitengroesse = MAX(blattGroesse(proBlatt),
		knotenGroesse(proKnoten));
	this->seitengroesse = MAX(this->seitengroesse,
		sizeof(int) * 2 + sizeof(struct info));
	info.elementeProBlatt = proBlatt;
	info.blaetterProKnoten = proKnoten;
	info.kandidatenAnzahl = 10;
	info.testAnzahl = 10;
	info.minsigma = 0.0;
	info.knotenzahl = 0;
	info.blattzahl = 0;
	info.merkmalzahl = 0;
	info.massid = 0;
	this->mass = mass;

	// Man sollte wenigstens ein Element speichern können
	assert(info.elementeProBlatt >= 1);
	// Und wenigstens einen binären Baum haben
	assert(info.blaetterProKnoten >= 2);

	// Und Erzeugen der Datei
	baum = fopen(filename, "wb+");
	if (baum == nullptr) {
		printf("Error: %s\n", std::strerror(errno));
	}
	speichereInfo();
}

VPBaum::~VPBaum()
{
	delete hashtab;
	delete hashmerk;

	delete karray;

	// Schreiben der Statusseite
	speichereInfo();

	// Schliessen der Baumdatei
	fclose(baum);
}

int VPBaum::calcProBlatt(int s)
{
	return (s - 2 * sizeof(int)) / (info.dimension * sizeof(float) +
		sizeof(int));
}

int VPBaum::blattGroesse(int speicher)
{
	return 2 * sizeof(int) + speicher *
		(info.dimension * sizeof(float) + sizeof(int));
}

int VPBaum::calcProKnoten(int s)
{
	return  (s - 2 * sizeof(int) - (info.dimension + 1) * sizeof(float)) /
		(sizeof(long) + sizeof(float));
}

int VPBaum::knotenGroesse(int verzweig)
{
	return 2 * sizeof(int) + (info.dimension + 1) * sizeof(float) +
		verzweig * (sizeof(long) + sizeof(float));
}

void VPBaum::speichereInfo()
{
	Seite s(seitengroesse);

	info.massid = mass->id;

	// Schreiben der Bauminformationen
	s.schreiben(&magic, sizeof(int));
	s.schreiben(&seitengroesse, sizeof(int));
	s.schreiben(&info, sizeof(struct info));

	s.speichern(baum, 0);

	// Verify
	auto v = new Seite(seitengroesse, baum, 0);
	auto vmagic = (int*) v->lesen(sizeof(int));
	assert(*vmagic == magic);
	auto vgroesse = (int*)v->lesen(sizeof(int));
	assert(*vgroesse == seitengroesse);
	auto vinfo = (struct info*)v->lesen(sizeof(struct info));
	assert(vinfo->blaetterProKnoten == info.blaetterProKnoten);	
	assert(vinfo->merkmalzahl== info.merkmalzahl);	
	assert(vinfo->massid == info.massid);	
}

void VPBaum::baumInfo()
{
	fprintf(stdout, "Seitengroesse: %d\n"
		"Dimension: %d\n"
		"Elemente pro Blatt: %d\n"
		"Blaetter pro Knoten: %d\n"
		"VP Kandidaten: %d\n"
		"Testpunkte: %d\n"
		"Start Sigma: %f\n"
		"Start-Seite: %ld\n"
		"Zahl der Knoten: %d\n"
		"Zahl der Blätter: %d\n"
		"Zahl der Merkmale: %d\n",
		seitengroesse,
		info.dimension,
		info.elementeProBlatt,
		info.blaetterProKnoten,
		info.kandidatenAnzahl,
		info.testAnzahl,
		info.minsigma,
		info.startSeite,
		info.knotenzahl,
		info.blattzahl,
		info.merkmalzahl);
}

Merkmal* VPBaum::waehleVantagePoint(MerkmalsMenge* m)
{
	int   i, j;
	int   kandidat, test;
	double dist, maxstd, mittel, varianz, stdabw;
	int   groesster;

	// Fuer eine bestimmte Anzahl von Kandidaten 
	maxstd = 0.0;
	for (i = 0; i < info.kandidatenAnzahl; i++)
	{
		// Waehle zufaellig einen Kandidaten aus der Punktmenge 
		kandidat = rand() % m->anzahl;

		mittel = 0.0;
		varianz = 0.0;
		for (j = 0; j < info.testAnzahl; j++)
		{
			// Berechne Durchschnitt und Standardabweichung der
			// Entfernung von dem Kandidaten zu einer zufaelligen
			// Teilmenge der Punktmenge.
			test = rand() % m->anzahl;
			dist = mass->d(m->merkmal[kandidat], m->merkmal[test]);
			mittel += dist;
			varianz += dist * dist;
		}
		mittel /= info.testAnzahl;
		varianz = (varianz / info.testAnzahl) - (mittel * mittel);
		stdabw = sqrt(varianz);

		// groesser als bisherige Standardabweichung? 
		if (stdabw >= maxstd)
		{
			// Dann merken 
			maxstd = stdabw;
			groesster = test;
		}
	}

	// Waehle den Kandidaten mit der maximalen Standardabweichung 
	return m->merkmal[groesster];
}

void VPBaum::distanzSort(double* dist, Merkmal** merkmal, int l, int r)
{
	// Quicksort wie in [Sed91], S. 148
	double v, t;
	Merkmal* tt;
	int    i, j;

	if (r > l)
	{
		v = dist[r];
		i = l - 1;
		j = r;
		do
		{
			do i++; while (dist[i] < v);
			do j--; while (dist[j] > v);
			t = dist[i]; dist[i] = dist[j]; dist[j] = t;
			tt = merkmal[i]; merkmal[i] = merkmal[j]; merkmal[j] = tt;
	}
      while (j > i);
		dist[j] = dist[i]; dist[i] = dist[r]; dist[r] = t;
		merkmal[j] = merkmal[i]; merkmal[i] = merkmal[r]; merkmal[r] = tt;
		distanzSort(dist, merkmal, l, i - 1);
		distanzSort(dist, merkmal, i + 1, r);
	}
}

long VPBaum::schreibeMenge(MerkmalsMenge* m)
{
	long  loc;
	Seite* s = new Seite(seitengroesse);

	assert(s != NULL);                     // Speicheranforderung klappt immer
	assert(m != NULL);                     // Der Parameter stimmt auch
	assert(m->anzahl <= info.elementeProBlatt); // Und niemals werden zuviele
	// Elemente in ein Blatt gequetscht

	s->schreiben(2);                       // Schreiben der Kennung 
	s->schreiben(m->anzahl);               // Schreiben der Anzahl
	info.merkmalzahl += m->anzahl;

	// Schreiben der einzelnen Schluessel
	for (int i = 0; i < m->anzahl; i++)
	{
		s->schreiben(m->merkmal[i]->schluessel);
		s->schreiben(m->merkmal[i]->werte, sizeof(float) * info.dimension);
	}

	// Und als letztes das Speichern auf der Festplatte
	loc = s->anhaengen(baum);
	info.blattzahl++;

	// Verify
	auto v = new Seite(seitengroesse, baum, loc);
	assert(2 == *(int*)v->lesen(sizeof(int)));

	delete s;
	return loc;
}

long VPBaum::speichereMenge(MerkmalsMenge* m)
{
	int i, num, elem, pos;
	int            b;

	MerkmalsMenge* menge;
	Merkmal** merkmale;
	int* blaetter;
	float* sigma;
	long           loc;

	Merkmal* vp;
	double* dist;

	assert(m != NULL); // Natürlich wird immer ein Parameter angegeben

	if (m->anzahl <= info.elementeProBlatt)
		// Speichern der Punktmenge in neue Seite 
		return schreibeMenge(m);

	// Das passt nicht - also Split notwendig!
	Seite* s = new Seite(seitengroesse);
	assert(s != NULL);                     // Speicheranforderung klappt immer
	s->schreiben(1);    // Schreiben der Kennung 
	blaetter = (int*)s->schreiben(1);    // Schreiben der Anzahl
	sigma = (float*)s->schreiben(0.0f);  // Schreiben des minSigmas

	// Erst wird der Vantage Point bestimmt
	vp = waehleVantagePoint(m);
	s->schreiben(vp->werte, sizeof(float) * info.dimension);

	// Jetzt wird der Abstand aller Punkte zu dem VantagePoint berechnet
	dist = new double[m->anzahl];
	for (i = 0; i < m->anzahl; i++)
		dist[i] = mass->d(vp, m->merkmal[i]);

	// Sortieren der Punkte nach dem Abstand zum VP 
	distanzSort(dist, m->merkmal, 0, m->anzahl - 1);

	// Splitte in n Teile der Groesse m->anzahl / n 
	merkmale = m->merkmal;
	num = m->anzahl;
	b = MIN(info.blaetterProKnoten,
		(int)ceil(num / (double)info.elementeProBlatt));
	elem = (int)ceil(m->anzahl / (double)b);
	pos = 0;
	std::vector<long> subpages;
	for (i = 0; i < b && num > 0; i++)
	{
		// Bilde aus den naechsten elem Merkmalen eine neue Merkmalsmenge
		if (num < elem) elem = num;
		menge = new MerkmalsMenge(elem, merkmale);
		num -= elem;
		pos += elem;
		merkmale += elem;

		if (pos == m->anzahl) {
			// Dies ist der Rest, als Intervallgrenze die maximale  Entfernung
			// benutzen
			s->schreiben((float)dist[pos - 1]);
		}
		else
		{
			float  d = fabs(dist[pos - 1] - dist[pos]) / 2.0;
			*sigma = MAX(*sigma, d);
			s->schreiben((float)((dist[pos - 1] + dist[pos]) / 2.0));
		}
		long subpage = speichereMenge(menge);
		s->schreiben(subpage);
		subpages.push_back(subpage);
	}
	info.minsigma = MAX(*sigma, info.minsigma);
	*blaetter = b;

	loc = s->anhaengen(baum);
	info.knotenzahl++;

	// Verify
	auto v = new Seite(seitengroesse, baum, loc);
	assert(1 == *(int*)v->lesen(sizeof(int)));
	assert(*blaetter == *(int*)v->lesen(sizeof(int)));
	float sigmastored = *(float*)v->lesen(sizeof(float));
//	assert(sigmaVerify == sigmastored);

	delete s;
	std::string result;
	for (long page : subpages) {
		result += " " + std::to_string(page);
	}
	printf("Generated page %d (%s)\n", loc, result.c_str());
	return loc;
}

Merkmal* VPBaum::startNNSuche(Merkmal* punkt, float* sigma)
{
	if (info.merkmalzahl == 0)
		return NULL;

	ergebnis = NULL;
	T = K = B = P = 0;
	for (int i = 0; ergebnis == NULL; i++)
	{
		*sigma *= 2;
		suche(punkt, sigma, info.startSeite, NN);
		T++;
	}

	//  fprintf(stderr,"Versuche, Knoten, Blätter, Punkte: ");
	fprintf(stderr, "%d %d %d %d\n", T, K, B, P);
	ergebnis->speichern(stdout);

	return ergebnis;
}

Merkmal* VPBaum::startKNNSuche(Merkmal* punkt, int k, float* sigma)
{
	T = K = B = P = 0;
	do
	{
		// Löschen des Ergebnisfeldes und Anlegen eines neuen
		delete karray;
		karray = new KArray(k);

		// Start der Suche
		suche(punkt, sigma, info.startSeite, KNN);

		// Anpassen des Sigmas für den nächsten Versuch
		*sigma *= 2;
		T++;
	}
	// Solange, wie noch nicht genug Nachbarn gefunden sind
	while (karray->getNum() < MIN(info.merkmalzahl, k));

	//  fprintf(stderr,"Versuche, Knoten, Blätter, Punkte: ");
	fprintf(stderr, "%d %d %d %d\n", T, K, B, P);
	karray->print();
	delete karray;

	return karray->nearestNeighbour();
}

void VPBaum::suche(Merkmal* punkt, float* sigma, long position, int MODE)
{
	Merkmal* vp, * gefunden;
	Merkmal* lokalesErgebnis = NULL;
	double  dist;
	int     blaetter, punkte, sch, i;
	float   mu1, mu2;
	float   minsigma;
	long    neupos;

	Seite* s = new Seite(seitengroesse, baum, position);

	switch (*((int*)s->lesen(sizeof(int))))
	{
	case 1:
		// Innerer Knoten des Baums
		K++;
		blaetter = *((int*)s->lesen(sizeof(int)));
		minsigma = *((float*)s->lesen(sizeof(float)));

		// Jetzt den VantagePoint erzeugen
		vp = new Merkmal(0, info.dimension,
			(float*)s->lesen(sizeof(float) * info.dimension));

		// Den Abstand unseres Suchpunktes zum VantagePoint berechnen
		dist = mass->d(punkt, vp);

		// Die gespeicherten Intervalle durchgehen
		mu1 = 0.0;
		for (i = 0; i < blaetter; i++)
		{
			mu2 = *((float*)s->lesen(sizeof(float)));
			neupos = *((long*)s->lesen(sizeof(long)));

			// Überprüfen, ob das Intervall in Frage kommt
			if ((mu1 - *sigma < dist) && (dist <= mu2 + *sigma))
				suche(punkt, sigma, neupos, MODE);

			mu1 = mu2;
		}
		break;
	case 2:
		// Ein Blatt
		punkte = *((int*)s->lesen(sizeof(int)));

		// Wurde diese Blatt bereits einmal bearbeitet?
		lokalesErgebnis = istBekannt(position);
		if (lokalesErgebnis == NULL)
		{
			// Nein, dann beginnt die Arbeit
			float lokalesSigma = 1e6; // Kludge!!!
			B++;

			// Jetzt müssen leider alle sequentiell durchgegangen werden
			for (i = 0; i < punkte; i++)
			{
				// Erzeugen des neuen Punktes
				sch = *((int*)s->lesen(sizeof(int)));
				gefunden = new Merkmal(sch, info.dimension, (float*)
					s->lesen(sizeof(float) * info.dimension));

				// Berechnen der Distanz zum Suchpunkt
				P++;
				dist = mass->d(punkt, gefunden);

				if (MODE == NN)
				{
					// Erster Fall: es wird nur der nächste Nachbar gesucht,
					// dann kann das Sigma verringert werden
					if (dist < lokalesSigma)
					{
						lokalesErgebnis = gefunden;
						lokalesSigma = dist;
					}
				}
				else
					// Zweiter Fall: wir suchen die k nächsten Nachbarn
					// Dann müssen wir jeden Punkt einsortieren
					if (dist < *sigma)
						karray->insert(gefunden, dist);
			}

			if (MODE == NN && lokalesErgebnis != NULL)
				// Und in der Hashtabelle merken!
				jetztBekannt(position, lokalesErgebnis);
		}

		if (MODE == NN && lokalesErgebnis != NULL)
		{
			// Ist der dichteste Punkt dieser Seite auch der dichteste überhaupt?
			dist = mass->d(punkt, lokalesErgebnis);
			if (dist < *sigma)
			{
				ergebnis = lokalesErgebnis;
				*sigma = dist;
			}
		}
		break;
	default:
		fprintf(stderr, "Fehler in Dateistruktur\n");
		exit(-1);
	}

	// Und die Seite wieder aus dem Speicher löschen
	//  delete s;
	// Ups - geht nicht mehr wegen der Hashtabelle
}

//
// Funktionen der Hashing-Tabelle
// Dies merkt sich, welche Seiten bereits berechnet wurden.
// Wird bei einem erneuten Versuch eine Seite zum zweitenmal getroffen,
// braucht nichts getan zu werden, dies wird ja kein neues Ergebnis 
// liefern.
//

void VPBaum::initHash(int size)
{
	hashSize = size;
	hashtab = new int[size];
	for (int i = 0; i < size; ++i) {
		hashtab[i] = -1;
	}
	hashmerk = new Merkmal * [size];
}

Merkmal* VPBaum::istBekannt(long pos, Merkmal* punkt)
{
	// Hashfunktion berechnen
	int p;
	int position = pos % hashSize;

	for (p = position; (hashtab[p] != 0) && (p != position - 1);
		p = (p + 1) % hashSize)
	{
		if (hashtab[p] == pos)
		{
			if (punkt)
			{
				fprintf(stderr, "Gibt es schon, Fehler!\n");
				exit(-1);
			}
			return hashmerk[p];
		}
		else if (hashtab[p] == -1) {
			// Eintrag frei, break
			break;
		}
	}

	if (p == position - 1)
	{
		fprintf(stderr, "Hashtabelle voll!\n");
		exit(-4);
	}

	if (!punkt)
		return NULL;

	hashtab[p] = pos;
	hashmerk[p] = punkt;
	return punkt;
}

// Funktion zum Einfuegen
void VPBaum::jetztBekannt(long pos, Merkmal* punkt)
{
	istBekannt(pos, punkt);
}


void vp_search(char* indexname, float* params, int dim)
{
	Merkmal* suche;
	VPBaum   baum(indexname);

	// Erzeugen des Punktes, dessen NN gesucht wird
	suche = new Merkmal(0, baum.info.dimension);
	for (int i = 0; i < suche->dimension; i++)
	{
		suche->werte[i] = params[i];
	}

	// Die eigentliche NN-Suche
	float sigma;
	sigma = baum.info.minsigma;

	baum.startNNSuche(suche, &sigma);
}
