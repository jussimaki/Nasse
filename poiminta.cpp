#include "maaritykset.h"

Peliobjekti *Poiminta::annaAlkio(int *indeksi, Poiminta **pointteri)
{
	Peliobjekti *palautettava = 0;

	if((*indeksi) == maara) return 0;
	palautettava = poimitut[(*indeksi)];

	if((*indeksi) == maara -1)
	{
		if(seuraava)
		{
			*pointteri = next;
			*indeksi = -1;
		}
	}

	*indeksi = *indeksi + 1;

	return palautettava;
}


void Poiminta::Vapauta()
{
	maara = 0;
	seuraava = 0;
	if(next)
	{
		next->Vapauta();
		delete next;
		next = 0;
	}
}


Poiminta::~Poiminta()
{
	Vapauta();
}
bool Poiminta::Alusta()
{
	maara = 0;
	seuraava = 0;

	if(next) next->Alusta();
	return true;
};

bool Poiminta::Lisaaobjekti(Peliobjekti *objekti)
{
	if(maara < 1000)
	{
		poimitut[maara] = objekti;
		maara++;
	}
	else
	{
		if(!next)
		{
			next = new Poiminta;
		}

		next->Lisaaobjekti(objekti);

		seuraava = 1;
	}
	return true;
}