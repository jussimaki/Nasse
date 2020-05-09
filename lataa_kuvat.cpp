#include "maaritykset.h"

bool lataa_kuvat_binaari(GLOBAALI *glob_data, const char *datatiedosto)
{
	std::ifstream tiedosto(datatiedosto, std::ios_base::binary);
	std::string rivi = "";

	glob_data->spritemappi.clear();

	if(tiedosto)
	{
		int rivit;
		tiedosto.read((char*)&rivit, sizeof(int));
		for(int rivi = 0; rivi < rivit; rivi++)
		{
			std::string kuvanimi, kuvatiedostonimi;
			LueString(&kuvanimi, tiedosto);
			LueString(&kuvatiedostonimi, tiedosto);

			std::map <std::string, Sprite*>::iterator tieto;
			tieto = glob_data->spritemappi.find(kuvanimi);
			
			
			if(tieto != glob_data->spritemappi.end())
			{
				return false;
			}
	
			Sprite *moi = new Sprite;
			if(!moi) return false;

			if(bmp_formaatti)
			{
				if(!glob_data->gfx->LataaSprite(moi, kuvatiedostonimi.c_str())) return false;
			}
			else if(!glob_data->gfx->LataaSprite2(moi, kuvatiedostonimi.c_str())) return false;
	
			(glob_data->spritemappi)[kuvanimi] = moi;
			
		}
	} else return false;

	tiedosto.close();

	return true;
};

bool lataa_kuvat(GLOBAALI *glob_data, const char *datatiedosto)
{
	std::ifstream tiedosto(datatiedosto);
	std::string rivi = "";

	glob_data->spritemappi.clear();

	if(tiedosto)
	{
		while(std::getline(tiedosto, rivi))
		{
			std::istringstream virta(rivi);
			std::string kuvanimi, kuvatiedostonimi;
			virta >> kuvanimi;
			virta >> kuvatiedostonimi;

			std::map <std::string, Sprite*>::iterator tieto;
			tieto = glob_data->spritemappi.find(kuvanimi);
			
			
			if(tieto != glob_data->spritemappi.end())
			{
				return false;
			}
	
			Sprite *moi = new Sprite;
			if(!moi) return false;

			if(bmp_formaatti)
			{
				if(!glob_data->gfx->LataaSprite(moi, kuvatiedostonimi.c_str())) return false;
			}
			else if(!glob_data->gfx->LataaSprite2(moi, kuvatiedostonimi.c_str())) return false;
	
			(glob_data->spritemappi)[kuvanimi] = moi;
			
		}
	} else return false;

	tiedosto.close();

	return true;
};


bool lataa_aanet_binaari(GLOBAALI *glob_data, const char *datatiedosto)
{
	std::ifstream tiedosto(datatiedosto, std::ios_base::binary);
	std::string rivi = "";

	glob_data->aanimappi.clear();

	if(tiedosto)
	{
		int rivit;
		tiedosto.read((char*)&rivit, sizeof(int));
		for(int rivi = 0; rivi < rivit; rivi++)
		{
			std::string aaninimi, aanitiedostonimi;
			LueString(&aaninimi, tiedosto);
			LueString(&aanitiedostonimi, tiedosto);

			std::map <std::string, Aani*>::iterator tieto;
			tieto = glob_data->aanimappi.find(aaninimi);
			
			
			if(tieto != glob_data->aanimappi.end())
			{
				return false;
			}
	
			Aani *moi = new Aani;

			glob_data->gfx->lataaAani(aanitiedostonimi.c_str(), moi);

			(glob_data->aanimappi)[aaninimi] = moi;
			
		}
	} else return false;

	tiedosto.close();

	return true;
};

bool lataa_aanet(GLOBAALI *glob_data, const char *datatiedosto)
{
	std::ifstream tiedosto(datatiedosto);
	std::string rivi = "";

	if(tiedosto)
	{
		while(std::getline(tiedosto, rivi))
		{
			std::istringstream virta(rivi);
			std::string aaninimi, aanitiedostonimi;
			virta >> aaninimi;
			virta >> aanitiedostonimi;

			std::map <std::string, Aani*>::iterator tieto;
			tieto = glob_data->aanimappi.find(aaninimi);

			if(tieto != glob_data->aanimappi.end())
			{
				return false;
			}

			Aani *moi = new Aani;

			glob_data->gfx->lataaAani(aanitiedostonimi.c_str(), moi);

			(glob_data->aanimappi)[aaninimi] = moi;
		}
	}

	return true;
}

bool lataa_biisit(GLOBAALI *glob_data, const char *datatiedosto)
{
	std::ifstream tiedosto(datatiedosto);
	std::string rivi = "";

	if(tiedosto)
	{
		while(std::getline(tiedosto, rivi))
		{
			std::istringstream virta(rivi);
			std::string biisinimi, biisitiedostonimi;
			virta >> biisinimi;
			virta >> biisitiedostonimi;

			std::map <std::string, Biisi>::iterator tieto;
			tieto = glob_data->biisimappi.find(biisinimi);
			
			
			if(tieto != glob_data->biisimappi.end())
			{
				return false;
			}
	
			Biisi moi;

			glob_data->gfx->lataaBiisi(biisitiedostonimi.c_str(), &moi);
	
			(glob_data->biisimappi)[biisinimi] = moi;

			
			
		}

		return true;
	} else return false;
}

bool lataa_biisit_binaari(GLOBAALI *glob_data, const char *datatiedosto)
{
	std::ifstream tiedosto(datatiedosto, std::ios_base::binary);
	std::string rivi = "";

	glob_data->biisimappi.clear();

	if(tiedosto)
	{
		int rivit;
		tiedosto.read((char*)&rivit, sizeof(int));
		for(int rivi = 0; rivi < rivit; rivi++)
		{
			std::string biisinimi, biisitiedostonimi;
			LueString(&biisinimi, tiedosto);
			LueString(&biisitiedostonimi, tiedosto);

			std::map <std::string, Biisi>::iterator tieto;
			tieto = glob_data->biisimappi.find(biisinimi);
			
			
			if(tieto != glob_data->biisimappi.end())
			{
				return false;
			}
	
			Biisi moi;

			glob_data->gfx->lataaBiisi(biisitiedostonimi.c_str(), &moi);

			(glob_data->biisimappi)[biisinimi] = moi;
			
		}
	} else return false;

	tiedosto.close();

	return true;
};

bool lataa_animaatiot(GLOBAALI *glob_data, const char *datatiedosto)
{
	glob_data->animaatiomappi.clear();
	std::string animaationimi;
	std::string kuvanimi;
	std::string rivi = "";

	int nopeus;
	int ruutuja;
	
	std::ifstream tiedosto(datatiedosto);

	if(tiedosto)
	{
		while(std::getline(tiedosto, rivi))
		{
			std::istringstream virta(rivi);

			virta >> animaationimi;
			std::map <std::string, Animaatio*>::iterator tieto;
			tieto = glob_data->animaatiomappi.find(animaationimi);
			
			if(tieto != glob_data->animaatiomappi.end())
			{
				return false;
			}

			std::vector <Rect> laatikot;
			std::vector <Offset> offsetit;

			virta >> kuvanimi;
			std::map<std::string, Sprite*>::iterator iteraattori;

			iteraattori = glob_data->spritemappi.find(kuvanimi);
			if(iteraattori == glob_data->spritemappi.end()) return false;
			Sprite *sprite = (*iteraattori).second;

			virta >> ruutuja;
			virta >> nopeus;

			for(int index = 0; index < ruutuja; index++)
			{
				Rect laatikko;
				virta >> laatikko.x;
				virta >> laatikko.y;
				virta >> laatikko.width;
				virta >> laatikko.height;
				Offset offset;
				virta >> offset.x;
				virta >> offset.y;

				laatikot.push_back(laatikko);
				offsetit.push_back(offset);
			}

			Animaatio *uusi = new Animaatio;
			uusi->osoitin = sprite;
			uusi->nopeus = nopeus;
			uusi->laatikot = laatikot;
			uusi->offsetit = offsetit;

			(glob_data->animaatiomappi)[animaationimi] = uusi;
			

		}
	}
	else return false;

	tiedosto.close();

	

	return true;
}

bool lataa_animaatiot_binaari(GLOBAALI *glob_data, const char *datatiedosto)
{
	glob_data->animaatiomappi.clear();
	

	int nopeus;
	int ruutuja;
	
	std::ifstream tiedosto(datatiedosto, std::ios_base::binary);
	int rivit;

	tiedosto.read((char*)&rivit, sizeof(int));

	if(tiedosto)
	{
		for(int ar = 0; ar < rivit; ar++)
		{
			std::string animaationimi;
			std::string kuvanimi;
			LueString(&animaationimi, tiedosto);
			
			std::map <std::string, Animaatio*>::iterator tieto;
			tieto = glob_data->animaatiomappi.find(animaationimi);
			
			if(tieto != glob_data->animaatiomappi.end())
			{
				return false;
			}

			std::vector <Rect> laatikot;
			std::vector <Offset> offsetit;

			LueString(&kuvanimi, tiedosto);
			std::map<std::string, Sprite*>::iterator iteraattori;

			iteraattori = glob_data->spritemappi.find(kuvanimi);
			if(iteraattori == glob_data->spritemappi.end()) return false;
			Sprite *sprite = (*iteraattori).second;

			tiedosto.read((char*)&ruutuja, sizeof(int));
			tiedosto.read((char*)&nopeus, sizeof(int));

			for(int index = 0; index < ruutuja; index++)
			{
				Rect laatikko;
				Offset offset;

				tiedosto.read((char*)&laatikko.x, sizeof(int));
				tiedosto.read((char*)&laatikko.y, sizeof(int));
				tiedosto.read((char*)&laatikko.width, sizeof(int));
				tiedosto.read((char*)&laatikko.height, sizeof(int));
				tiedosto.read((char*)&offset.x, sizeof(int));
				tiedosto.read((char*)&offset.y, sizeof(int));

				laatikot.push_back(laatikko);
				offsetit.push_back(offset);
			}

			Animaatio *uusi = new Animaatio;
			uusi->osoitin = sprite;
			uusi->nopeus = nopeus;
			uusi->laatikot = laatikot;
			uusi->offsetit = offsetit;

			(glob_data->animaatiomappi)[animaationimi] = uusi;
			

		}
	}
	else return false;

	tiedosto.close();

	

	return true;
}

// vapauttaa animaatiomappiin ladatut animaatiot
void vapauta_animaatiot(GLOBAALI *glob_data)
{
	std::map <std::string, Animaatio *> *mappiosoitin = &(glob_data->animaatiomappi);
	
	std::map <std::string, Animaatio *>::iterator iter;
	iter = mappiosoitin->begin();

	for(int index = 0; index < mappiosoitin->size(); index++)
	{
		if(iter->second)
		{
			((Animaatio*)(iter->second))->laatikot.clear();
			delete iter->second;
			iter->second = 0;
		}
		iter++;
	}

	mappiosoitin->clear();
}

// vapauttaa spritemappiin ladatut kuvat
void vapauta_kuvat(GLOBAALI *glob_data)
{
	std::map <std::string, Sprite *> *mappiosoitin = &(glob_data->spritemappi);
	
	std::map <std::string, Sprite *>::iterator iter;
	iter = mappiosoitin->begin();

	for(int index = 0; index < mappiosoitin->size(); index++)
	{
		if(iter->second)
		{
			glob_data->gfx->VapautaSprite((Sprite*)(iter->second));
			delete iter->second;
			iter->second = 0;
		}
		iter++;
	}

	mappiosoitin->clear();
}