#include "maaritykset.h"

extern GLOBAALI tietopankki;

askel askeltaulukko[] =
{
	1,SUUNTA_OIKEA,20,
	1,SUUNTA_VASEN,20,
	2,SUUNTA_ALAS,20,
	2,SUUNTA_YLOS,20,
	3,SUUNTA_OIKEA,20,
	4,SUUNTA_OIKEA,5,
	5,SUUNTA_ALAS,20,
	6,SUUNTA_ALAS,20,
	7,SUUNTA_ALAS,5,
	8,SUUNTA_VASEN,20,
	9,SUUNTA_VASEN,20,
	10,SUUNTA_VASEN,5,
	11,SUUNTA_YLOS,20,
	12,SUUNTA_YLOS,20,
	13,SUUNTA_YLOS,5,
	14,SUUNTA_OIKEA,20,
	14,SUUNTA_VASEN,20,
	13,SUUNTA_VASEN,5,
	12,SUUNTA_ALAS,20,
	11,SUUNTA_ALAS,20,
	10,SUUNTA_ALAS,5,
	9,SUUNTA_OIKEA,20,
	8,SUUNTA_OIKEA,20,
	7,SUUNTA_OIKEA,5,
	6,SUUNTA_YLOS,20,
	5,SUUNTA_YLOS,20,
	4,SUUNTA_YLOS,5,
	3,SUUNTA_VASEN,20,

	15,SUUNTA_ALAOIKEA,20,
	15,SUUNTA_YLAVASEN,20,
	16,SUUNTA_YLAOIKEA,20,
	16,SUUNTA_ALAVASEN,20,

	17,SUUNTA_OIKEA,20,
	18,SUUNTA_ALAVASEN,20,
	19,SUUNTA_VASEN,20,
	20,SUUNTA_YLAOIKEA,20,

	20,SUUNTA_VASEN,20,
	19,SUUNTA_ALAOIKEA,20,
	18,SUUNTA_OIKEA,20,
	17,SUUNTA_YLAVASEN,20,

	21,SUUNTA_ALAOIKEA,20,
	22,SUUNTA_ALAS,20,
	23,SUUNTA_YLAVASEN,20,
	24,SUUNTA_YLOS,20,

	24,SUUNTA_ALAVASEN,20,
	23,SUUNTA_ALAS,20,
	22,SUUNTA_YLAOIKEA,20,
	21,SUUNTA_YLOS,20
};

lahdekohde taulukko[] = {0,0,0,0,0,0,0,0,
						SUUNTA_OIKEA,SUUNTA_OIKEA,0,16,16,16,31,16,
						SUUNTA_ALAS,SUUNTA_ALAS,16,0,16,16,16,31,
						SUUNTA_OIKEA,SUUNTA_OIKEA,0,16,16,16,31,24,
						SUUNTA_OIKEA,SUUNTA_ALAS,0,24,4,28,8,31,
						SUUNTA_ALAS,SUUNTA_ALAS,8,0,16,16,16,31,
						SUUNTA_ALAS,SUUNTA_ALAS,16,0,16,16,8,31,
						SUUNTA_ALAS,SUUNTA_VASEN,8,0,4,4,0,8,
						SUUNTA_VASEN,SUUNTA_VASEN,31,8,16,16,0,16,
						SUUNTA_VASEN,SUUNTA_VASEN,31,16,16,16,0,8,
						SUUNTA_VASEN,SUUNTA_YLOS,31,8,28,4,24,0,
						SUUNTA_YLOS,SUUNTA_YLOS,24,31,16,16,16,0,
						SUUNTA_YLOS,SUUNTA_YLOS,16,31,16,16,24,0,
						SUUNTA_YLOS,SUUNTA_OIKEA,24,31,28,28,31,24,
						SUUNTA_OIKEA,SUUNTA_OIKEA,0,24,16,16,31,16,
						SUUNTA_ALAOIKEA,SUUNTA_ALAOIKEA,0,0,16,16,31,31,
						SUUNTA_YLAOIKEA,SUUNTA_YLAOIKEA,0,31,16,16,31,0,
						SUUNTA_OIKEA,SUUNTA_ALAOIKEA,0,16,16,16,31,31,
						SUUNTA_ALAVASEN,SUUNTA_VASEN,31,0,16,16,0,16,
						SUUNTA_VASEN,SUUNTA_YLAVASEN,31,16,16,16,0,0,
						SUUNTA_YLAOIKEA,SUUNTA_OIKEA,0,31,16,16,31,16,

						SUUNTA_ALAOIKEA,SUUNTA_ALAS,0,0,16,16,16,31,
						SUUNTA_ALAS,SUUNTA_ALAVASEN,16,0,16,16,0,31,
						SUUNTA_YLAVASEN,SUUNTA_YLOS,31,31,16,16,16,0,
						SUUNTA_YLOS,SUUNTA_YLAOIKEA,16,31,16,16,31,0
};


bool Kartta::AsetaPiirto(int monesko)
{
	moodi = MOODI_PIIRTO;
	laskuri = 0;
	piirtoreitti = monesko;
	a_x_tile = o_x_tile = x_tile;
	a_y_tile = o_y_tile = y_tile;
	pala_num = 0;

	if(pisteet[y_tile*leveys+x_tile]->pelinloppu)
	{
		return true;
	}
	if(!pisteet[y_tile*leveys+x_tile]->menty[piirtoreitti] && (pisteet[y_tile*leveys+x_tile]->reittimaara > piirtoreitti))
	{
		Lapaisy uusi;
		uusi.x = x_tile;
		uusi.y = y_tile;
		uusi.numero = monesko;
		lapaisyt.push_back(uusi);
	} else moodi = MOODI_VAPAA;
	return false;
}

void Kartta::TallennaPeli()
{
	char kokonimi[256];
	sprintf(kokonimi, "%s.sav",tallennuspaikka);
	std::ofstream tiedosto(kokonimi, std::ios_base::binary);

	if(tiedosto)
	{
		tiedosto.write((char*)&tallennettava_x, sizeof(int));
		tiedosto.write((char*)&tallennettava_y, sizeof(int));

		int koko = lapaisyt.size();

		tiedosto.write((char*)&koko, sizeof(int));

		for(int index = 0; index < lapaisyt.size(); index++)
		{
			int x, y, numero;
			x = lapaisyt.at(index).x;
			y = lapaisyt.at(index).y;
			numero = lapaisyt.at(index).numero;

			tiedosto.write((char*)&x, sizeof(int));
			tiedosto.write((char*)&y, sizeof(int));
			tiedosto.write((char*)&numero, sizeof(int));
		}
		tiedosto.close();
	}
}

bool Kartta::LataaPeli(char *tiedostonimi)
{
	lapaisyt.clear();
	
	for(int index = 0; index < pistemaara; index++)
	{
		pistetaulukko[index]->menty[0] = 0;
		pistetaulukko[index]->menty[1] = 0;
		pistetaulukko[index]->menty[2] = 0;
		pistetaulukko[index]->menty[3] = 0;
	}

	int ax = reittimappi.AnnaX();
	int yy = reittimappi.AnnaY();
	memset(reitit, 0, ax*yy*sizeof(unsigned char));
	
	tallennuspaikka = tiedostonimi;

	char kokonimi[256];
	sprintf(kokonimi, "%s.sav",tallennuspaikka);
	

	std::ifstream tiedosto(kokonimi, std::ios_base::binary);

	if(!tiedosto) return false;

	tiedosto.read((char*)&x_tile, sizeof(int));
	tiedosto.read((char*)&y_tile, sizeof(int));

	tallennettava_x = x_tile;
	tallennettava_y = y_tile;

	int koko;
	tiedosto.read((char*)&koko, sizeof(int));

	for(index = 0; index < koko; index++)
	{
		Lapaisy uusi;
		tiedosto.read((char*)&(uusi.x), sizeof(int));
		tiedosto.read((char*)&(uusi.y), sizeof(int));
		tiedosto.read((char*)&(uusi.numero), sizeof(int));
		lapaisyt.push_back(uusi);

		Teereitti(uusi.x,uusi.y,uusi.numero);
	}

	

	tiedosto.close();

	moodi = MOODI_VAPAA;
	s_a = 0;
	s_b = 0;
	a = 0; b = 0;
	kartta_x_cam = 0;
	kartta_y_cam = 0;
	x_loc = 16;
	y_loc = 16;

	return true;
}


void Kartta::UusiPeli(char *tiedostonimi)
{
	lapaisyt.clear();
	
	for(int index = 0; index < pistemaara; index++)
	{
		pistetaulukko[index]->menty[0] = 0;
		pistetaulukko[index]->menty[1] = 0;
		pistetaulukko[index]->menty[2] = 0;
		pistetaulukko[index]->menty[3] = 0;
	}

	moodi = MOODI_VAPAA;
	s_a = 0;
	s_b = 0;
	a = 0; b = 0;
	kartta_x_cam = 0;
	kartta_y_cam = 0;
	x_loc = 16;
	y_loc = 16;

	

	tallennuspaikka = tiedostonimi;

	x_tile = orig_x_tile;
	y_tile = orig_y_tile;

	tallennettava_x = x_tile;
	tallennettava_y = y_tile;

	int ax = reittimappi.AnnaX();
	int yy = reittimappi.AnnaY();
	memset(reittimappi.AnnaData(), 0, ax*yy*sizeof(unsigned char));

	

}

bool Kartta::Lataa(char *tiedostonimi, GLOBAALI *glob_data)
{
	lapaisyt.clear();
	moodi = MOODI_VAPAA;
	s_a = 0;
	s_b = 0;
	a = 0; b = 0;
	kartta_x_cam = 0;
	kartta_y_cam = 0;
	x_loc = 16;
	y_loc = 16;

	std::ifstream tiedosto(tiedostonimi);
	if(!tiedosto) return false;
	std::string karttanimi;

	tiedosto >> karttanimi;
	tiedosto >> orig_x_tile;
	tiedosto >> orig_y_tile;
	tiedosto >> pistemaara;

	x_tile = orig_x_tile;
	y_tile = orig_y_tile;

	for(int index = 0; index < 100; index++) pistetaulukko[index] = 0;

	for(index = 0; index < pistemaara; index++)
	{
		Piste *uusi = new Piste;
		std::string tasonimi;

		tiedosto >> tasonimi;
		uusi->tiedostonimi = new char[tasonimi.size()+1];
		memcpy(uusi->tiedostonimi, tasonimi.c_str(), tasonimi.size());
		uusi->tiedostonimi[tasonimi.size()] = '\0';
		
		tiedosto >> uusi->pelinloppu;
		tiedosto >> uusi->x;
		tiedosto >> uusi->y;
		tiedosto >> uusi->symboli;
		tiedosto >> uusi->reittimaara;

		for(int reitti = 0; reitti < 4; reitti++)
		{
			uusi->reitit[reitti] = 0;
			uusi->reittikoot[reitti] = 0;
			uusi->menty[reitti] = 0;
		}

		for(reitti = 0; reitti < uusi->reittimaara; reitti++)
		{
			int askeleet;
			tiedosto >> askeleet;
			uusi->reittikoot[reitti] = askeleet;
			uusi->menty[reitti] = 0;
			uusi->reitit[reitti] = new unsigned char[askeleet];

			for(int askel = 0; askel < askeleet; askel++)
			{
				unsigned int suunta;
				tiedosto >> suunta;
				unsigned char moi = (unsigned char) suunta;
				uusi->reitit[reitti][askel] = moi;
			}
		}

		pistetaulukko[index] = uusi;
	}
	tiedosto.close();

	std::ifstream karttatiedosto(karttanimi.c_str(), std::ios_base::binary);
	
	tilemap.Lataa(karttatiedosto,glob_data,glob_data->tilesetit.size()-2,0,2);
	karttatiedosto.close();

	leveys = tilemap.AnnaX();
	korkeus = tilemap.AnnaY();

	reitit = new unsigned char[leveys*korkeus];
	memset(reitit,0,sizeof(unsigned char)*leveys*korkeus);
	pisteet = new Piste*[leveys*korkeus];
	memset(pisteet,0,sizeof(Piste*)*leveys*korkeus);

	for(index = 0; index < pistemaara; index++)
	{
		int x = pistetaulukko[index]->x;
		int y = pistetaulukko[index]->y;
		pisteet[y*leveys + x] = pistetaulukko[index];
	}

	reittimappi.AsetaX(leveys);
	reittimappi.AsetaY(korkeus);
	reittimappi.AsetaData(reitit);

	return true;
}

bool Kartta::Lataa_binaari(char *tiedostonimi, GLOBAALI *glob_data)
{
	lapaisyt.clear();
	moodi = MOODI_VAPAA;
	s_a = 0;
	s_b = 0;
	a = 0; b = 0;
	kartta_x_cam = 0;
	kartta_y_cam = 0;
	x_loc = 16;
	y_loc = 16;

	std::ifstream tiedosto(tiedostonimi, std::ios_base::binary);
	if(!tiedosto) return false;
	std::string karttanimi;

	
	LueString(&karttanimi, tiedosto);
	tiedosto.read((char*)&orig_x_tile, sizeof(int));
	tiedosto.read((char*)&orig_y_tile, sizeof(int));
	tiedosto.read((char*)&pistemaara, sizeof(int));
	

	x_tile = orig_x_tile;
	y_tile = orig_y_tile;

	for(int index = 0; index < 100; index++) pistetaulukko[index] = 0;

	for(index = 0; index < pistemaara; index++)
	{
		Piste *uusi = new Piste;
		std::string tasonimi;

		LueString(&tasonimi, tiedosto);
		
		uusi->tiedostonimi = new char[tasonimi.size()+1];
		memcpy(uusi->tiedostonimi, tasonimi.c_str(), tasonimi.size());
		uusi->tiedostonimi[tasonimi.size()] = '\0';
		
		
		tiedosto.read((char*)&(uusi->pelinloppu), sizeof(int));
		tiedosto.read((char*)&(uusi->x), sizeof(int));
		tiedosto.read((char*)&(uusi->y), sizeof(int));
		tiedosto.read((char*)&(uusi->symboli), sizeof(int));
		tiedosto.read((char*)&(uusi->reittimaara), sizeof(int));

		for(int reitti = 0; reitti < 4; reitti++)
		{
			uusi->reitit[reitti] = 0;
			uusi->reittikoot[reitti] = 0;
			uusi->menty[reitti] = 0;
		}

		for(reitti = 0; reitti < uusi->reittimaara; reitti++)
		{
			int askeleet;
			tiedosto.read((char*)&askeleet, sizeof(int));
			uusi->reittikoot[reitti] = askeleet;
			uusi->menty[reitti] = 0;
			uusi->reitit[reitti] = new unsigned char[askeleet];

			for(int askel = 0; askel < askeleet; askel++)
			{
				unsigned int suunta;
				tiedosto.read((char*)&suunta, sizeof(unsigned int));
				unsigned char moi = (unsigned char) suunta;
				uusi->reitit[reitti][askel] = moi;
			}
		}

		pistetaulukko[index] = uusi;
	}
	tiedosto.close();

	std::ifstream karttatiedosto(karttanimi.c_str(), std::ios_base::binary);
	tilemap.Lataa(karttatiedosto,glob_data,glob_data->tilesetit.size()-2,0,2);
	karttatiedosto.close();

	leveys = tilemap.AnnaX();
	korkeus = tilemap.AnnaY();

	reitit = new unsigned char[leveys*korkeus];
	memset(reitit,0,sizeof(unsigned char)*leveys*korkeus);
	pisteet = new Piste*[leveys*korkeus];
	memset(pisteet,0,sizeof(Piste*)*leveys*korkeus);

	for(index = 0; index < pistemaara; index++)
	{
		int x = pistetaulukko[index]->x;
		int y = pistetaulukko[index]->y;
		pisteet[y*leveys + x] = pistetaulukko[index];
	}

	reittimappi.AsetaX(leveys);
	reittimappi.AsetaY(korkeus);
	reittimappi.AsetaData(reitit);

	return true;
}

void Kartta::Tuhoa()
{
	for(int index = 0; index < pistemaara; index++)
	{
		Piste *pee = pistetaulukko[index];

		delete[] pee->tiedostonimi;


		int reittimaara = pee->reittimaara;

		for(int reitti = 0; reitti < reittimaara; reitti++)
		{
			delete[] pee->reitit[reitti];
		}

		delete pee;

		pistetaulukko[index] = 0;
		pistemaara = 0;
	}

	tilemap.Tuhoa();

	delete[] reitit;
	reitit = 0;
	delete[] pisteet;
	pisteet = 0;
}

bool Kartta::uloskavelyfunktio(int laskuri, int *lokaali_x, int *lokaali_y, int tiletyyppi, int suunta)
{
	if(tiletyyppi == 0)
	{
		if(laskuri >= 16) return false;

		switch(suunta)
		{
		case SUUNTA_OIKEA:
			*lokaali_x = 16+laskuri;
			*lokaali_y = 16;
			break;
		case SUUNTA_VASEN:
			*lokaali_x = 15-laskuri;
			*lokaali_y = 16;
			break;
		case SUUNTA_ALAS:
			*lokaali_x = 16;
			*lokaali_y = 16+laskuri;
			break;
		case SUUNTA_YLOS:
			*lokaali_x = 16;
			*lokaali_y = 15-laskuri;
			break;
		}
	}
	else
	{
		lahdekohde moi = taulukko[tiletyyppi];
		int eka_x, eka_y,toka_x,toka_y;

		if(suunta == moi.kohde)
		{
			eka_x = moi.x_1;
			eka_y = moi.y_1;
			toka_x = moi.x_2;
			toka_y = moi.y_2;
		}
		else if(suunta == -moi.lahde)
		{
			eka_x = moi.x_1;
			eka_y = moi.y_1;
			toka_x = moi.x_0;
			toka_y = moi.y_0;
		} else return false;

		int delta_x = toka_x-eka_x;
		int delta_y = toka_y-eka_y;
		int pituus = (int)sqrt(delta_x*delta_x + delta_y*delta_y);

		if(laskuri >= pituus) return false;
		*lokaali_x = eka_x + (delta_x*laskuri/pituus);
		*lokaali_y = eka_y + (delta_y*laskuri/pituus);
	}


	return true;
}

bool Kartta::sisaankavelyfunktio(int laskuri, int *lokaali_x, int *lokaali_y, int tiletyyppi, int suunta)
{
	if(tiletyyppi == 0)
	{
		if(laskuri >= 16) return false;

		switch(suunta)
		{
		case SUUNTA_OIKEA:
			*lokaali_x = laskuri;
			*lokaali_y = 16;
			break;
		case SUUNTA_VASEN:
			*lokaali_x = 31-laskuri;
			*lokaali_y = 16;
			break;
		case SUUNTA_ALAS:
			*lokaali_x = 16;
			*lokaali_y = laskuri;
			break;
		case SUUNTA_YLOS:
			*lokaali_x = 16;
			*lokaali_y = 31-laskuri;
			break;
		}
	}
	else
	{
		lahdekohde moi = taulukko[tiletyyppi];
		int eka_x, eka_y,toka_x,toka_y;

		if(suunta == moi.lahde)
		{
			eka_x = moi.x_0;
			eka_y = moi.y_0;
			toka_x = moi.x_1;
			toka_y = moi.y_1;
		}
		else if(suunta == -moi.kohde)
		{
			eka_x = moi.x_2;
			eka_y = moi.y_2;
			toka_x = moi.x_1;
			toka_y = moi.y_1;
		} else return false;

		int delta_x = toka_x-eka_x;
		int delta_y = toka_y-eka_y;
		int pituus = (int)sqrt(delta_x*delta_x + delta_y*delta_y);

		if(laskuri >= pituus) return false;
		*lokaali_x = eka_x + (delta_x*laskuri/pituus);
		*lokaali_y = eka_y + (delta_y*laskuri/pituus);
	}

	return true;
}

bool Kartta::yritys()
{
	int x_lis, y_lis;
	if(suunta == SUUNTA_YLOS) {x_lis = 0; y_lis = -1;}
	if(suunta == SUUNTA_ALAS) {x_lis = 0; y_lis = 1;}
	if(suunta == SUUNTA_VASEN) {x_lis = -1; y_lis = 0;}
	if(suunta == SUUNTA_OIKEA) {x_lis = 1; y_lis = 0;}

	if(suunta == SUUNTA_YLAOIKEA) {x_lis = 1; y_lis = -1;}
	if(suunta == SUUNTA_YLAVASEN) {x_lis = -1; y_lis = -1;}
	if(suunta == SUUNTA_ALAOIKEA) {x_lis = 1; y_lis = 1;}
	if(suunta == SUUNTA_ALAVASEN) {x_lis = -1; y_lis = 1;}

	unsigned char tyyppi = reitit[leveys*(y_tile+y_lis) + x_tile+x_lis];

	lahdekohde moi = taulukko[tyyppi];
	if(moi.lahde == suunta) seuraavasuunta = moi.kohde;
	else if(-moi.kohde == suunta) seuraavasuunta = -moi.lahde;
	else if(pisteet[leveys*(y_tile+y_lis) + x_tile+x_lis]) seuraavasuunta = 0;
	else return false;

	return true;
}


bool Kartta::Piirto(GLOBAALI *glob_data)
{
	kartta_x_cam = x_tile*32+x_loc - 320;
	kartta_y_cam = y_tile*32+y_loc - 240;

	if(kartta_x_cam < 0) kartta_x_cam = 0;
	if(kartta_y_cam < 0) kartta_y_cam = 0;

	int x_tilet = tilemap.AnnaX();
	int y_tilet = tilemap.AnnaY();

	if(kartta_x_cam > (x_tilet-20)*32) kartta_x_cam = (x_tilet-20)*32;
	if(kartta_y_cam > (y_tilet-15)*32) kartta_y_cam = (y_tilet-15)*32;

	if(moodi != MOODI_GAMEOVER)
	{
		int settimaara = glob_data->tilesetit.size();
		tilemap.draw_scrolling_tilemap(glob_data, kartta_x_cam,kartta_y_cam,settimaara-2,false);
		reittimappi.draw_scrolling_tilemap(glob_data, kartta_x_cam,kartta_y_cam,settimaara-1,false);

		for(int index = 0; index < pistemaara; index++)
		{
			
			int x = pistetaulukko[index]->x*32-kartta_x_cam;
			int y = pistetaulukko[index]->y*32-kartta_y_cam;

			int maara = pistetaulukko[index]->reittimaara;

			if(maara < 2) piirraAnimaatio(glob_data->karttasymboli,x,y,&s_a,&s_b,glob_data);
			else piirraAnimaatio(glob_data->karttasymboli2,x,y,&s_a,&s_b,glob_data);
		}

		piirraAnimaatio(glob_data->karttapelaaja,
			x_tile*32+x_loc-3-kartta_x_cam,
			y_tile*32+y_loc-16-kartta_y_cam,&a,&b,glob_data);

		animoi(glob_data->karttapelaaja,&a,&b);
		animoi(glob_data->karttasymboli,&s_a,&s_b);
	}
	else
	{
		
		glob_data->gfx->TyhjennaRuutu();
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,200,100,&laatikot[gameover_kuva],1,0);
		Tulosta("Continue", 280, 220, glob_data);
		Tulosta("End", 280, 240, glob_data);

		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,220,220+kursori*20, &laatikot[valintakursori_kuva],1,0);

	}



	if(moodi == MOODI_LOPETETAANKO)
	{
		
		for(int ax = 0; ax < 10; ax++)
		{
			for(int yy = 0; yy < 5; yy++)
			{
				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,160+ax*32,160+yy*32, &laatikot[varjo_kuva],0,128);
			}
		}

		Tulosta("Continue", 240, 200, glob_data);
		Tulosta("Save and continue", 240, 220, glob_data);
		Tulosta("Save and Quit", 240, 240, glob_data);
		Tulosta("Quit without saving", 240, 260, glob_data);

		
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,220,200+kursori*20, &laatikot[valintakursori_kuva],1,0);
	}
	else if(moodi == MOODI_TALLENNETAANKO)
	{
		
		for(int ax = 0; ax < 10; ax++)
		{
			for(int yy = 0; yy < 5; yy++)
			{
				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,160+ax*32,160+yy*32, &laatikot[varjo_kuva],0,128);
			}
		}

		Tulosta("Save and continue", 240, 200, glob_data);
		Tulosta("Continue without saving", 240, 220, glob_data);

		
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,220,200+kursori*20, &laatikot[valintakursori_kuva],1,0);
	} else AnimoiTilet(glob_data, glob_data->tilesetit.size()-1);

		
	return true;
}

bool Kartta::TeePalanen(unsigned char tyyppi, int *x, int *y)
{
	unsigned char pala = askeltaulukko[tyyppi].palatyyppi;
	switch(askeltaulukko[tyyppi].suunta)
	{
	case SUUNTA_VASEN:
		(*x)--;
		break;
	case SUUNTA_OIKEA:
		(*x)++;
		break;
	case SUUNTA_YLOS:
		(*y)--;
		break;
	case SUUNTA_ALAS:
		(*y)++;
		break;
	case SUUNTA_YLAVASEN:
		(*x)--;
		(*y)--;
		break;
	case SUUNTA_YLAOIKEA:
		(*x)++;
		(*y)--;
		break;
	case SUUNTA_ALAVASEN:
		(*x)--;
		(*y)++;
		break;
	case SUUNTA_ALAOIKEA:
		(*x)++;
		(*y)++;
		break;
	default: break;
	}
	reitit[(*y)*leveys + (*x)] = pala;
	return true;
}

bool Kartta::TeeAnimPalanen(unsigned char tyyppi, int *x, int *y)
{
	switch(askeltaulukko[tyyppi].suunta)
	{
	case SUUNTA_VASEN:
		(*x)--;
		break;
	case SUUNTA_OIKEA:
		(*x)++;
		break;
	case SUUNTA_YLOS:
		(*y)--;
		break;
	case SUUNTA_ALAS:
		(*y)++;
		break;
	case SUUNTA_YLAVASEN:
		(*x)--;
		(*y)--;
		break;
	case SUUNTA_YLAOIKEA:
		(*x)++;
		(*y)--;
		break;
	case SUUNTA_ALAVASEN:
		(*x)--;
		(*y)++;
		break;
	case SUUNTA_ALAOIKEA:
		(*x)++;
		(*y)++;
		break;
	default: break;
	}
	reitit[(*y)*leveys + (*x)] = tyyppi+25;
	return true;
}

bool Kartta::Teereitti(int x, int y, int reittinumero)
{
	
	Piste *os = pisteet[y*leveys + x];
	int koko = os->reittikoot[reittinumero];
	unsigned char *palaosoitin = os->reitit[reittinumero];

	int ax = x, yy = y;
	

	for(int index = 0; index < koko; index++)
	{
		unsigned char tyyppi = palaosoitin[index];
		TeePalanen(tyyppi, &ax, &yy);
	}

	os->menty[reittinumero] = 1;

	return true;
}

bool Kartta::TeereittiVaiheittain(int reittinumero, GLOBAALI *glob_data)
{
	Piste *os = pisteet[y_tile*leveys + x_tile];
	int koko = os->reittikoot[reittinumero];
	unsigned char *palaosoitin = os->reitit[reittinumero];

	if(laskuri == 0)
	{
		unsigned char tyyppi;

		if(pala_num < koko)
		{
			tyyppi = palaosoitin[pala_num];
			laskuri = askeltaulukko[tyyppi].aika;
		}

		if(pala_num != 0)
		{
			TeePalanen(palaosoitin[pala_num-1], &o_x_tile, &o_y_tile);
		}

		if(pala_num == koko)
		{
			os->menty[reittinumero] = 1;
			return false;
		}

		pala_num++;

		TeeAnimPalanen(tyyppi, &a_x_tile, &a_y_tile);
		ResetoiAnimaatiot(glob_data,glob_data->tilesetit.size()-1);
	}

	laskuri--;


	return true;
}

int Kartta::Logiikka(GLOBAALI *glob_data, char **tiedostonimi)
{
	bool paino = 0;

	InputData *inputti = glob_data->gfx->annaInput();

	if(inputti->NAPPI_PAINETTU[SDLK_ESCAPE] && moodi!= MOODI_TALLENNETAANKO &&
		moodi != MOODI_LOPETETAANKO && moodi != MOODI_GAMEOVER)
	{
		paino = 1;
		kursori = 0;
		viimemoodi = moodi;
		moodi = MOODI_LOPETETAANKO;
		
	}

	if(moodi == MOODI_GAMEOVER)
	{

		if(inputti->NAPPI_PAINETTU[SDLK_UP]) kursori--;
		else if(inputti->NAPPI_PAINETTU[SDLK_DOWN]) kursori++;

		if(kursori < 0) kursori = 0;
		if(kursori > 1) kursori = 1;

		if(inputti->NAPPI_PAINETTU[SDLK_RETURN])
		{
			if(kursori == 0) {LataaPeli(tallennuspaikka); return 0;}
			else return 2;
		}
	}

	if(moodi == MOODI_LOPETETAANKO)
	{
		if(paino == 0 && inputti->NAPPI_PAINETTU[SDLK_ESCAPE]) moodi = viimemoodi;

		else if(inputti->NAPPI_PAINETTU[SDLK_UP]) kursori--;
		else if(inputti->NAPPI_PAINETTU[SDLK_DOWN]) kursori++;

		if(kursori < 0) kursori = 0;
		if(kursori > 3) kursori = 3;

		if(inputti->NAPPI_PAINETTU[SDLK_RETURN])
		{
			paino = 1;

			switch(kursori)
			{
			case 0:
				moodi = viimemoodi;
				break;
			case 1:
				TallennaPeli();
				moodi = viimemoodi;
				break;
			case 2:
				TallennaPeli();
				return 2;
				break;
			case 3:
				return 2;
				break;
			default: break;
			}
		}
	}
	else if(moodi == MOODI_TALLENNETAANKO)
	{
		if(inputti->NAPPI_PAINETTU[SDLK_UP]) kursori--;
		else if(inputti->NAPPI_PAINETTU[SDLK_DOWN]) kursori++;

		if(kursori < 0) kursori = 0;
		if(kursori > 1) kursori = 1;

		if(inputti->NAPPI_PAINETTU[SDLK_RETURN])
		{
			paino = 1;
			moodi = MOODI_VAPAA;
			if(kursori == 0) TallennaPeli();
		}
	}

	if(moodi == MOODI_VAPAA)
	{
		tallennettava_x = x_tile;
		tallennettava_y = y_tile;

		suunta = 0;
		if(painettu(index_YLOS,glob_data)) suunta = SUUNTA_YLOS;
		if(painettu(index_ALAS,glob_data)) suunta = SUUNTA_ALAS;
		if(painettu(index_VASEN,glob_data)) suunta = SUUNTA_VASEN;
		if(painettu(index_OIKEA,glob_data)) suunta = SUUNTA_OIKEA;

		if(suunta != 0)
		{
			
			if(yritys()) {moodi = MOODI_ULOSKAVELY; laskuri = 0;}
		}
		else
		{
			if((inputti->NAPPI_PAINETTU[SDLK_RETURN] && !paino) || painettu(index_HYPPY,glob_data))
			{
				*tiedostonimi = pisteet[y_tile*leveys+x_tile]->tiedostonimi;
				return 1;
			}
		}

	}

	if(moodi == MOODI_ULOSKAVELY)
	{
		unsigned char tiletyyppi = reitit[leveys*y_tile + x_tile];
		
		bool arvo = uloskavelyfunktio(laskuri,&x_loc,&y_loc,tiletyyppi,suunta);
		x_kohta = 32*x_tile + x_loc;
		y_kohta = 32*y_tile + y_loc;
		laskuri++;
		if(!arvo)
		{
			moodi = MOODI_SISAANKAVELY;
			laskuri = 0;

			if(suunta == SUUNTA_VASEN) x_tile--;
			if(suunta == SUUNTA_OIKEA) x_tile++;
			if(suunta == SUUNTA_YLOS) y_tile--;
			if(suunta == SUUNTA_ALAS) y_tile++;

			if(suunta == SUUNTA_YLAOIKEA) {x_tile++; y_tile--;}
			if(suunta == SUUNTA_YLAVASEN) {x_tile--; y_tile--;}
			if(suunta == SUUNTA_ALAOIKEA) {x_tile++; y_tile++;}
			if(suunta == SUUNTA_ALAVASEN) {x_tile--; y_tile++;}
		}
	}

	if(moodi == MOODI_SISAANKAVELY)
	{
		unsigned char tiletyyppi = reitit[leveys*y_tile + x_tile];
		
		bool arvo = sisaankavelyfunktio(laskuri,&x_loc,&y_loc,tiletyyppi,suunta);
		x_kohta = 32*x_tile + x_loc;
		y_kohta = 32*y_tile + y_loc;
		laskuri++;
		if(!arvo)
		{
			suunta = seuraavasuunta;

			if(pisteet[leveys*y_tile+x_tile]) moodi = MOODI_VAPAA;
			else if(suunta == 0) moodi = MOODI_VAPAA;
			else
			{
				if(yritys())
				{
					moodi = MOODI_ULOSKAVELY;
					laskuri = 0;
				} else moodi = MOODI_VAPAA;
			}
		}
	}

	if(moodi == MOODI_PIIRTO)
	{
		if(pisteet[y_tile*leveys+x_tile]->menty[piirtoreitti]) moodi = MOODI_VAPAA;
		else
		{
			if(!TeereittiVaiheittain(piirtoreitti,glob_data))
			{
				moodi = MOODI_TALLENNETAANKO;
				kursori = 0;
			}
		}

	}


	if(moodi == MOODI_PIIRTO || moodi == MOODI_VAPAA || moodi == MOODI_LOPETETAANKO || moodi == MOODI_TALLENNETAANKO) {a = 0; b = 0;}

	return 0;
}

void Kartta::kaynnista(GLOBAALI *glob_data)
{
	for(int index = 0; index < glob_data->tilesetit.size(); index++)
	{
		vapauta_setti(glob_data,index);
	}

	lataa_setti(glob_data,glob_data->tilesetit.size()-2);
	lataa_setti(glob_data,glob_data->tilesetit.size()-1);

	soitaGlobaaliBiisi(glob_data, &glob_data->karttabiisi);
}
