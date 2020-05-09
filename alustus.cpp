#include "maaritykset.h"

bool tallenna_configit(GLOBAALI *glob_data)
{
	std::ofstream tiedosto("config", std::ios_base::binary);

	if(tiedosto)
	{
		unsigned char saadot = glob_data->saadot;
		tiedosto.write((char*)&saadot, sizeof(unsigned char));

		if(saadot & 1)
		{
			for(int index = 0; index < 6; index++)
			{
				tiedosto.write((char*)&(glob_data->keyboard_indeksit[index]),sizeof(int));
			}
		}

		if(saadot & 2)
		{
			for(int index = 0; index < 6; index++)
			{
				tiedosto.write((char*)&(glob_data->gamepad_indeksit[index]),sizeof(int));
			}
		}

		tiedosto.close();
	}  else return false;
	
	return true;	
}

void lataa_configit(GLOBAALI *glob_data)
{
	std::ifstream tiedosto("config", std::ios_base::binary);

	if(tiedosto)
	{
		unsigned char saadot;
		tiedosto.read((char*)&saadot, sizeof(unsigned char));

		glob_data->saadot = saadot;

		if(saadot & 1)
		{
			for(int index = 0; index < 6; index++)
			{
				tiedosto.read((char*)&(glob_data->keyboard_indeksit[index]),sizeof(int));
			}
		}
		if(saadot & 2)
		{
			for(int index = 0; index < 6; index++)
			{
				tiedosto.read((char*)&(glob_data->gamepad_indeksit[index]),sizeof(int));
			}
		}

		tiedosto.close();
	}
	else
	{
		glob_data->keyboard_indeksit[0] = SDLK_LEFT;
		glob_data->keyboard_indeksit[1] = SDLK_RIGHT;
		glob_data->keyboard_indeksit[2] = SDLK_UP;
		glob_data->keyboard_indeksit[3] = SDLK_DOWN;
		
		glob_data->keyboard_indeksit[4] = SDLK_LCTRL;
		glob_data->keyboard_indeksit[5] = SDLK_LALT;
		//glob_data->keyboard_indeksit[4] = SDLK_LCTRL;
		//glob_data->keyboard_indeksit[5] = SDLK_LALT;
		glob_data->saadot = 1;
	}
}

bool alustus(GLOBAALI *glob_data)
{
	lataa_configit(glob_data);	
	glob_data->gfx = new Grafiikka;

	if(!glob_data->gfx->Alusta(640, 480,640,480, 32, 1, 0, 0,1)) return false;

	lataa_nimet((Alkio*)tunnukset, glob_data, oMaara);

	if(binaarimuoto)
	{
		if(!lataa_biisit_binaari(glob_data, "data/biisidata.dat")) return false;
		if(!lataa_aanet_binaari(glob_data, "data/aanidata.dat")) return false;
		if(!lataa_kuvat_binaari(glob_data, "data/spritedata.dat")) return false;
		if(!lataa_animaatiot_binaari(glob_data, "data/animaatiodata.dat")) return false;
		if(!lataa_objektityypit_binaari("data/objektidata.dat", "data/erikoisobjektidata.dat", glob_data)) return false;
		if(!lataa_tilesetit_binaari(glob_data, "data/tilesetit.dat")) return false;
	}
	else
	{
		if(!lataa_biisit(glob_data, "data/biisidata.txt")) return false;
		if(!lataa_aanet(glob_data, "data/aanidata.txt")) return false;
		if(!lataa_kuvat(glob_data, "data/spritedata.txt")) return false;
		if(!lataa_animaatiot(glob_data, "data/animaatiodata.txt")) return false;
		if(!lataa_objektityypit("data/objektidata.txt", "data/erikoisobjektidata.txt", glob_data)) return false;
		if(!lataa_tilesetit(glob_data, "data/tilesetit.txt")) return false;
	}
	

	glob_data->pikkukuvat = new Sprite;
	if(bmp_formaatti) glob_data->gfx->LataaSprite(glob_data->pikkukuvat, "kuvat/pictures.bmp");
	else glob_data->gfx->LataaSprite2(glob_data->pikkukuvat, "kuvat/pictures.gfx");

	std::map <std::string, Animaatio *>::iterator anim_iter;
	std::string animaationimi;

	

	animaationimi = "karttapelaaja";
	anim_iter = glob_data->animaatiomappi.find(animaationimi);
	if(anim_iter == glob_data->animaatiomappi.end()) return false;
	glob_data->karttapelaaja = (*anim_iter).second;

	animaationimi = "karttasymboli";
	
	anim_iter = glob_data->animaatiomappi.find(animaationimi);
	if(anim_iter == glob_data->animaatiomappi.end()) return false;
	glob_data->karttasymboli = (*anim_iter).second;

	animaationimi = "karttasymboli2";

	anim_iter = glob_data->animaatiomappi.find(animaationimi);
	if(anim_iter == glob_data->animaatiomappi.end()) return false;
	glob_data->karttasymboli2 = (*anim_iter).second;

	std::map <std::string, Biisi>::iterator biisi_iter;

	glob_data->soivabiisi = 0;
	biisi_iter = glob_data->biisimappi.find("valikkobiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->valikkobiisi = (*biisi_iter).second;

	biisi_iter = glob_data->biisimappi.find("karttabiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->karttabiisi = (*biisi_iter).second;

	biisi_iter = glob_data->biisimappi.find("perusbiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->perusbiisi = (*biisi_iter).second;

	biisi_iter = glob_data->biisimappi.find("luolabiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->luolabiisi = (*biisi_iter).second;

	biisi_iter = glob_data->biisimappi.find("lumibiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->lumibiisi = (*biisi_iter).second;

	biisi_iter = glob_data->biisimappi.find("rantabiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->rantabiisi = (*biisi_iter).second;

	biisi_iter = glob_data->biisimappi.find("linnabiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->linnabiisi = (*biisi_iter).second;

	biisi_iter = glob_data->biisimappi.find("kuolemabiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->kuolemabiisi = (*biisi_iter).second;

	biisi_iter = glob_data->biisimappi.find("tasolapibiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->tasolapibiisi = (*biisi_iter).second;

	biisi_iter = glob_data->biisimappi.find("bossibiisi");
	if(biisi_iter == glob_data->biisimappi.end()) return false;
	glob_data->bossibiisi = (*biisi_iter).second;

	std::map <std::string, Aani*>::iterator aani_iter;


	aani_iter = glob_data->aanimappi.find("jousi");
	if(aani_iter == glob_data->aanimappi.end()) return false;
	glob_data->jousiaani = (*aani_iter).second;

	aani_iter = glob_data->aanimappi.find("hyppy");
	if(aani_iter == glob_data->aanimappi.end()) return false;
	glob_data->hyppyaani = (*aani_iter).second;

	aani_iter = glob_data->aanimappi.find("kolikko");
	if(aani_iter == glob_data->aanimappi.end()) return false;
	glob_data->kolikkoaani = (*aani_iter).second;

	aani_iter = glob_data->aanimappi.find("uinti");
	if(aani_iter == glob_data->aanimappi.end()) return false;
	glob_data->uintiaani = (*aani_iter).second;


	glob_data->kirjaimet = new Sprite;
	if(bmp_formaatti) glob_data->gfx->LataaSprite(glob_data->kirjaimet, "kuvat/alphabet.bmp");
	else glob_data->gfx->LataaSprite2(glob_data->kirjaimet, "kuvat/alphabet.gfx");

	
	return true;
}

bool lopetus(GLOBAALI *glob_data)
{	
	glob_data->gfx->VapautaSprite(glob_data->pikkukuvat);
	delete glob_data->pikkukuvat;
	vapauta_tilesetit(glob_data);
	vapauta_objektityypit(glob_data);
	glob_data->tyyppinimet.clear();
	vapauta_animaatiot(glob_data);
	vapauta_kuvat(glob_data);
	glob_data->gfx->Sulje();
	delete glob_data->gfx;

	return true;
}