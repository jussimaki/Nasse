#include "maaritykset.h"

void lataa_animoidut(std::ifstream &tiedosto, Layer *kerros,int oikeamaara)
{
	
			Anim_tile **animaatiotaulukko;
			animaatiotaulukko = new Anim_tile*[oikeamaara];

			int index;
			for(index = 0; index < oikeamaara; index++)
			{
				animaatiotaulukko[index] = 0;
			}

			int animoidut;
			tiedosto >> animoidut;

			for(index = 0; index < animoidut; index++)
			{
				Anim_tile *tile = new Anim_tile;
				tile->ruutu = 0;
				tile->counter = 0;

				int indeksi, ruudut;
				tiedosto >> indeksi;
				tiedosto >> ruudut;
				tiedosto >> tile->nopeus;

				
				for(int ruutu = 0; ruutu < ruudut; ruutu++)
				{
					int r;
					tiedosto >> r;
					tile->ruudut.push_back(r);
				}

				animaatiotaulukko[indeksi] = tile;
			}

			kerros->animoidut = animaatiotaulukko;
}

void lataa_animoidut_binaari(std::ifstream &tiedosto, Layer *kerros,int oikeamaara)
{
	
			Anim_tile **animaatiotaulukko;
			animaatiotaulukko = new Anim_tile*[oikeamaara];

			int index;
			for(index = 0; index < oikeamaara; index++)
			{
				animaatiotaulukko[index] = 0;
			}

			int animoidut;

			tiedosto.read((char*)&animoidut, sizeof(int));
			
			for(index = 0; index < animoidut; index++)
			{
				Anim_tile *tile = new Anim_tile;
				tile->ruutu = 0;
				tile->counter = 0;

				int indeksi, ruudut;
				
				tiedosto.read((char*)&indeksi, sizeof(int));
				tiedosto.read((char*)&ruudut, sizeof(int));
				tiedosto.read((char*)&(tile->nopeus), sizeof(int));
				
				
				for(int ruutu = 0; ruutu < ruudut; ruutu++)
				{
					int r;
					tiedosto.read((char*)&r, sizeof(int));
					tile->ruudut.push_back(r);
				}

				animaatiotaulukko[indeksi] = tile;
			}

			kerros->animoidut = animaatiotaulukko;
}


void lataa_avaimet(std::ifstream &tiedosto, Layer *kerros, int oikeamaara)
{
	unsigned char *avaimet = new unsigned char[oikeamaara];

	for(int index = 0; index < oikeamaara; index++)
	{
		int avain;
		tiedosto >> avain;
		avaimet[index] = avain;
	}

	kerros->avaimet = avaimet;
}

void lataa_avaimet_binaari(std::ifstream &tiedosto, Layer *kerros, int oikeamaara)
{
	unsigned char *avaimet = new unsigned char[oikeamaara];

	for(int index = 0; index < oikeamaara; index++)
	{
		int avain;
		
		tiedosto.read((char*)&avain, sizeof(int));
		avaimet[index] = avain;
	}

	kerros->avaimet = avaimet;
}

void lataa_lapinakyvat(std::ifstream &tiedosto, Layer *kerros, int oikeamaara)
{
	unsigned char *lapinakyvat = new unsigned char[oikeamaara];

	for(int index = 0; index < oikeamaara; index++)
	{
		int lapinakyva;
		tiedosto >> lapinakyva;
		lapinakyvat[index] = lapinakyva;
	}

	kerros->lapinakyvat = lapinakyvat;
}

void lataa_lapinakyvat_binaari(std::ifstream &tiedosto, Layer *kerros, int oikeamaara)
{
	unsigned char *lapinakyvat = new unsigned char[oikeamaara];

	for(int index = 0; index < oikeamaara; index++)
	{
		int lapinakyva;

		tiedosto.read((char*)&lapinakyva, sizeof(int));
		lapinakyvat[index] = lapinakyva;
	}

	kerros->lapinakyvat = lapinakyvat;
}

void lataa_setti(GLOBAALI *glob_data,int numero)
{
	Tileset *setti = glob_data->tilesetit.at(numero);
	if(!setti->ladattu)
	{
		setti->ladattu = 1;
		const char *kuva_1, *kuva_2, *kuva_3;

		kuva_1 = setti->kerrokset[0]->tiedostonimi.c_str();
		kuva_2 = setti->kerrokset[1]->tiedostonimi.c_str();
		kuva_3 = setti->kerrokset[2]->tiedostonimi.c_str();

		if(bmp_formaatti)
		{
			glob_data->gfx->LataaSprite(setti->kerrokset[0]->sprite, kuva_1);
			glob_data->gfx->LataaSprite(setti->kerrokset[1]->sprite, kuva_2);
			glob_data->gfx->LataaSprite(setti->kerrokset[2]->sprite, kuva_3);
		}
		else
		{
			glob_data->gfx->LataaSprite2(setti->kerrokset[0]->sprite, kuva_1);
			glob_data->gfx->LataaSprite2(setti->kerrokset[1]->sprite, kuva_2);
			glob_data->gfx->LataaSprite2(setti->kerrokset[2]->sprite, kuva_3);
		}
	}
}

void vapauta_setti(GLOBAALI *glob_data,int numero)
{
	Tileset *setti = glob_data->tilesetit.at(numero);
	if(setti->ladattu)
	{
		setti->ladattu = 0;
		
		glob_data->gfx->VapautaSprite(setti->kerrokset[0]->sprite);
		glob_data->gfx->VapautaSprite(setti->kerrokset[1]->sprite);
		glob_data->gfx->VapautaSprite(setti->kerrokset[2]->sprite);
	}
}


bool lataa_tilesetit(GLOBAALI *glob_data, const char *datatiedosto)
{
	std::ifstream tiedosto(datatiedosto);

	if(tiedosto)
	{
		int setit;

		tiedosto >> setit;
		std::string settinimi;
		std::string spritenimi;

		for(int settinro = 0; settinro < setit; settinro++)
		{

			tiedosto >> settinimi;
			Tileset *uusi_setti = new Tileset;

			uusi_setti->ladattu = 0;
			uusi_setti->nimi = settinimi;

			Layer *maastokerros = new Layer;
			Layer *taustakerros_1 = new Layer;
			Layer *taustakerros_2 = new Layer;
			
			// MAASTOKERROS

			tiedosto >> spritenimi;

			maastokerros->tiedostonimi = spritenimi;

			maastokerros->sprite = new Sprite;

			for(int index = 0; index < 300; index++)
			{
				int luku;
				tiedosto >> luku;
				unsigned char l = (unsigned char)luku;
				maastokerros->mappaus[index] = l;
			}
			
			int oikeamaara;
			tiedosto >> oikeamaara;
			maastokerros->tilemaara = oikeamaara;

			int tyyppi;

			int *tiletyypit = new int[oikeamaara];
			
			for(index = 0; index < oikeamaara; index++)
			{
				tiedosto >> tyyppi;
				tiletyypit[index] = tyyppi;
			}

			maastokerros->tyypit = tiletyypit;

			lataa_avaimet(tiedosto, maastokerros, oikeamaara);

			// tämä on uusi rivi
			lataa_lapinakyvat(tiedosto, maastokerros, oikeamaara);

			lataa_animoidut(tiedosto, maastokerros, oikeamaara);

			uusi_setti->kerrokset[0] = maastokerros;

			// TAUSTAKERROS 1

			tiedosto >> spritenimi;

			taustakerros_1->tiedostonimi = spritenimi;

			taustakerros_1->sprite = new Sprite;

			for(index = 0; index < 300; index++)
			{
				int luku;
				tiedosto >> luku;
				unsigned char l = (unsigned char)luku;
				taustakerros_1->mappaus[index] = l;
			}

			tiedosto >> oikeamaara;
			taustakerros_1->tilemaara = oikeamaara;

			lataa_avaimet(tiedosto, taustakerros_1, oikeamaara);

			lataa_animoidut(tiedosto, taustakerros_1, oikeamaara);

			uusi_setti->kerrokset[1] = taustakerros_1;

			// TAUSTAKERROS 2

			tiedosto >> spritenimi;

			taustakerros_2->tiedostonimi = spritenimi;

			taustakerros_2->sprite = new Sprite;

			for(index = 0; index < 300; index++)
			{
				int luku;
				tiedosto >> luku;
				unsigned char l = (unsigned char)luku;
				taustakerros_2->mappaus[index] = l;
			}

			tiedosto >> oikeamaara;
			taustakerros_2->tilemaara = oikeamaara;

			lataa_animoidut(tiedosto, taustakerros_2, oikeamaara);

			uusi_setti->kerrokset[2] = taustakerros_2;

			glob_data->tilesetit.push_back(uusi_setti);
		}

		tiedosto.close();

	} else return false;

	return true;
}

bool lataa_tilesetit_binaari(GLOBAALI *glob_data, const char *datatiedosto)
{
	std::ifstream tiedosto(datatiedosto, std::ios_base::binary);

	if(tiedosto)
	{
		int setit;

		tiedosto.read((char*)&setit, sizeof(int));

		std::string settinimi;
		std::string spritenimi;

		for(int settinro = 0; settinro < setit; settinro++)
		{

			LueString(&settinimi, tiedosto);
			Tileset *uusi_setti = new Tileset;

			uusi_setti->ladattu = 0;
			uusi_setti->nimi = settinimi;

			Layer *maastokerros = new Layer;
			Layer *taustakerros_1 = new Layer;
			Layer *taustakerros_2 = new Layer;
			
			// MAASTOKERROS

			LueString(&spritenimi, tiedosto);

			maastokerros->tiedostonimi = spritenimi;

			maastokerros->sprite = new Sprite;

			for(int index = 0; index < 300; index++)
			{
				int luku;
				tiedosto.read((char*)&luku, sizeof(int));
				unsigned char l = (unsigned char)luku;
				maastokerros->mappaus[index] = l;
			}
			
			int oikeamaara;
			tiedosto.read((char*)&oikeamaara, sizeof(int));
			maastokerros->tilemaara = oikeamaara;

			int tyyppi;

			int *tiletyypit = new int[oikeamaara];
			
			for(index = 0; index < oikeamaara; index++)
			{
				tiedosto.read((char*)&tyyppi, sizeof(int));
				tiletyypit[index] = tyyppi;
			}

			maastokerros->tyypit = tiletyypit;

			lataa_avaimet_binaari(tiedosto, maastokerros, oikeamaara);

			lataa_lapinakyvat_binaari(tiedosto, maastokerros, oikeamaara);
			lataa_animoidut_binaari(tiedosto, maastokerros, oikeamaara);

			uusi_setti->kerrokset[0] = maastokerros;

			// TAUSTAKERROS 1

			LueString(&spritenimi, tiedosto);

			taustakerros_1->tiedostonimi = spritenimi;

			taustakerros_1->sprite = new Sprite;

			for(index = 0; index < 300; index++)
			{
				int luku;
				tiedosto.read((char*)&luku, sizeof(int));
				unsigned char l = (unsigned char)luku;
				taustakerros_1->mappaus[index] = l;
			}

			tiedosto.read((char*)&oikeamaara, sizeof(int));
			taustakerros_1->tilemaara = oikeamaara;

			lataa_avaimet_binaari(tiedosto, taustakerros_1, oikeamaara);
			lataa_animoidut_binaari(tiedosto, taustakerros_1, oikeamaara);

			uusi_setti->kerrokset[1] = taustakerros_1;

			// TAUSTAKERROS 2

			LueString(&spritenimi, tiedosto);

			taustakerros_2->tiedostonimi = spritenimi;

			taustakerros_2->sprite = new Sprite;

			for(index = 0; index < 300; index++)
			{
				int luku;
				tiedosto.read((char*)&luku, sizeof(int));
				unsigned char l = (unsigned char)luku;
				taustakerros_2->mappaus[index] = l;
			}

			tiedosto.read((char*)&oikeamaara, sizeof(int));
			taustakerros_2->tilemaara = oikeamaara;

			lataa_animoidut_binaari(tiedosto, taustakerros_2, oikeamaara);

			uusi_setti->kerrokset[2] = taustakerros_2;

			glob_data->tilesetit.push_back(uusi_setti);
		}

		tiedosto.close();

	} else return false;

	return true;
}

void vapauta_tilesetit(GLOBAALI *glob_data)
{
	for(int settinro = 0; settinro < glob_data->tilesetit.size(); settinro++)
		vapauta_setti(glob_data, settinro);


	for(settinro = 0; settinro < glob_data->tilesetit.size(); settinro++)
	{
		Tileset *setti = glob_data->tilesetit.at(settinro);

		setti->nimi.erase();

		for(int layernro = 0; layernro  < 3; layernro++)
		{
			Layer *layer = setti->kerrokset[layernro];

			// spriteä ei vapauteta, sillä se on jo vapautettu ylempänä
			delete layer->sprite;

			for(int index = 0; index < layer->tilemaara; index++)
			{
				Anim_tile *animaatio = layer->animoidut[index];

				if(animaatio)
				{
					animaatio->ruudut.clear();
					delete animaatio;
				}
			}

			delete[] layer->animoidut;

			if(!layernro == 0) delete[] layer->lapinakyvat;

			if(layernro != 2) delete[] layer->avaimet;
			delete layer;
		}

		delete setti;
	}

	glob_data->tilesetit.clear();
}