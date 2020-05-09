#include "maaritykset.h"

extern int ohjelmatila;

std::vector <Pelisektori *> *Pelitaso::AnnaSektorit() {return &sektorit;}


void Pelitaso::lisaaElama()
{
	if(peli)
	{
		peli->lisaaElama();
	}
}

void Pelitaso::lisaaKolikko()
{
	if(peli)
	{
		peli->lisaaKolikko();
	}
}

void Pelitaso::itsari(GLOBAALI *glob_data)
{
	sektorit.at(sektori)->itsari(glob_data);
}

bool Pelitaso::Kaynnista(GLOBAALI *glob_data)
{

	sektori = alku_sektori;
	sektorit.at(sektori)->kaynnista(glob_data,alku_x,alku_y,1);

	return true;
}

void Pelitaso::checkpoint(int sekt, int x, int y, int checkpoint)
{
	if(checkpoint_nro < checkpoint)
	{
		alku_sektori = sekt;
		alku_x = x;
		alku_y = y;
		checkpoint_nro = checkpoint;
	}
}

bool Pelitaso::VaihdaSektori(int sekt, int x, int y, GLOBAALI *glob_data, int v_energ)
{
	vanhaenergia = v_energ;
	vaihto = 1;
	seuraavasektori = sekt;
	seuraava_x = x;
	seuraava_y = y;

	return true;
}

bool Pelitaso::AlustaObjektit(bool ekakerta)
{

	for(int index = 0; index < sektorit.size(); index++)
		sektorit.at(index)->alustaObjektit(ekakerta);

	return true;
}


void Pelitaso::pauseta()
{
	sektorit.at(sektori)->pauseta();
}

int Pelitaso::Toiminta(GLOBAALI *glob_data, Peli *p)
{
	peli = p;

	vaihto = 0;
	Pelisektori *temp = sektorit.at(sektori);
	int arvo = temp->toiminta(glob_data, this);



	if(vaihto)
	{
		sektorit.at(sektori)->sammuta(glob_data); //uusi rivi versioon 1.02
		sektori = seuraavasektori;
		sektorit.at(sektori)->kaynnista(glob_data,seuraava_x,seuraava_y,vanhaenergia);
	}

	return arvo;
}

bool Pelitaso::Lataa(std::string tiedostonimi, GLOBAALI *glob_data)
{
	std::ifstream tiedosto(tiedostonimi.c_str(), std::ios_base::binary);

	int sektorimaara;
	
			
	int koodi;
	tiedosto.read((char *)&koodi, sizeof(int));

	tiedosto.read((char *)&sektorimaara, sizeof(int));
	tiedosto.read((char *)&alku_x, sizeof(int));
	tiedosto.read((char *)&alku_y, sizeof(int));
	tiedosto.read((char *)&alku_sektori, sizeof(int));

	checkpoint_nro = -1;
	sektori = alku_sektori;

	for(int index = 0; index < sektorimaara; index++)
	{
		Pelisektori *uusi = new Pelisektori;

		uusi->lataa(tiedosto, glob_data, koodi);
		sektorit.push_back(uusi);
	}


	// asetetaan linkit osoittamaan oikeisiin objekteihin
	for(index = 0; index < sektorimaara; index++)
	{
		std::vector <Linkkitieto *> *tiedot = sektorit.at(index)->annaLinkkitiedot();

		for(int index2 = 0; index2 < tiedot->size(); index2++)
		{
			std::vector <Peliobjekti *> *objektit =
			sektorit.at(tiedot->at(index2)->sektori)->annaObjektit();

			int numero = tiedot->at(index2)->monesko;
			int tyyppi = tiedot->at(index2)->tyyppi;
			int indeksi = tiedot->at(index2)->vektori_indeksi;

			int laskuri = 0;
			for(int index3 = 0; index3 < objektit->size(); index3++)
			{
				if(objektit->at(index3)->annaTyyppi() == tyyppi)
				{
					if(laskuri == numero)
					{
						tiedot->at(index2)->mista->at(indeksi) = objektit->at(index3);
						break;
					}
					else laskuri++;
				}
			}

			delete tiedot->at(index2);
		}

		tiedot->clear();
	}
	
	tiedosto.close();

	for(index = 0; index < glob_data->tilesetit.size(); index++)
	{
		if(index == sektorit.at(sektori)->annaSetti()) continue;
		vapauta_setti(glob_data,index);
	}

	lataa_setti(glob_data,sektorit.at(sektori)->annaSetti());

	return true;
}


void Pelitaso::Sammuta(GLOBAALI *glob_data)
{
	// versiossa 1.02 siirretty tähän sektorin sammutuksesta
	pysaytaGlobaaliBiisi(glob_data);

	sektorit.at(sektori)->sammuta(glob_data);
}

bool Pelitaso::Vapauta(GLOBAALI *glob_data)
{
	for(int index = 0; index < sektorit.size(); index++)
	{
		sektorit.at(index)->vapauta(glob_data);
		delete sektorit.at(index);
	}

	sektorit.clear();
	
	return true;
};