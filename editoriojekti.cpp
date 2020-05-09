#include "maaritykset.h"

int Objekti::annaTyyppi() {return tyyppinumero;}
int Objekti::AnnaX() {return x_;}
int Objekti::AnnaY() {return y_;}

void Editoriobjekti::x_siirto(int siirto) {x_ += siirto;}
void Editoriobjekti::y_siirto(int siirto) {y_ += siirto;}

void Editoriobjekti::LisaaLinkki(Editorilinkki *uusi) {linkit_tahan.push_back(uusi);}

std::vector < std::vector <void *> *> * Objekti::LataaLista(std::ifstream &tiedosto, Listamaaritys *maaritys, std::vector <Linkkitieto*> *linkkitiedot, GLOBAALI *glob_data)
{
	std::vector < std::vector <void *> *> * Listadata = new std::vector <std::vector <void *> *>;
	
	int koko;
	tiedosto.read((char*)&koko, sizeof(int));

	int *luku;
	std::vector <std::vector <void *> *> *l_os;

	for(int listaalkio = 0; listaalkio < koko; listaalkio++)
	{
		std::vector<void *> *attribuutit = new std::vector<void *>;

		for(unsigned int index = 0; index < maaritys->attribuutit.size(); index++)
		{
			Linkkitieto *uusi = 0;
			int sektori;
			std::string tyyppinimi;
			int indeksi;
			int tyyppi;
			
			switch(maaritys->attribuutit.at(index)->tyyppi)
			{
			case attrib_LUKU:
				luku = new int;
				tiedosto.read((char*)luku, sizeof(int));
				attribuutit->push_back(luku);
				break;

			case attrib_LISTA:
				l_os = LataaLista(tiedosto, (Listamaaritys*)maaritys->attribuutit.at(index)->maarite, linkkitiedot, glob_data);
				attribuutit->push_back(l_os);
				break;

			case attrib_LINKKI:
				tiedosto.read((char*)&sektori, sizeof(int));
				
				if(sektori != -1)
				{
					LueString(&tyyppinimi, tiedosto);
					
					tyyppi = -1;
					for(indeksi = 0; indeksi < glob_data->objektityypit.size(); indeksi++)
					{
						if(tyyppinimi == glob_data->objektityypit.at(indeksi)->nimi)
						{
							tyyppi = indeksi;
							break;
						}
					}

					if(tyyppi == -1) return false;
					
					uusi = new Linkkitieto;
					uusi->sektori = sektori;
					uusi->tyyppi = tyyppi;
					uusi->mista = data;
					uusi->vektori_indeksi = index;
					
					tiedosto.read((char*)(&(uusi->monesko)), sizeof(int));
					linkkitiedot->push_back(uusi);
				}
				attribuutit->push_back(0);
				break;

			default: break;
			}
		}

		Listadata->push_back(attribuutit);
	}
		
	return Listadata;
}


bool Objekti::LataaData(std::ifstream &tiedosto, GLOBAALI *glob_data, int t, std::vector <Linkkitieto*> *linkkitiedot)
{
	tyyppinumero = t;
	std::vector <Attribuutti *> *kuvaukset = &glob_data->objektityypit.at(tyyppinumero)->attribuutit;

	tiedosto.read((char*)&x_, sizeof(int));
	tiedosto.read((char*)&y_, sizeof(int));

	int triggerit;

	tiedosto.read((char*)&triggerit, sizeof(int));

	data = new std::vector <void *>;
	
	int *luku = 0;
	std::vector < std::vector <void *> *> *listaosoitin;

	for(unsigned int index = 0; index < kuvaukset->size(); index++)
	{
		Linkkitieto *uusi = 0;
		int sektori;
		std::string tyyppinimi;
		int indeksi;
		int tyyppi;

		switch(kuvaukset->at(index)->tyyppi)
		{
		case attrib_LUKU:
			luku = new int;
			tiedosto.read((char*)luku, sizeof(int));
			data->push_back(luku);
			break;
		case attrib_LISTA:
			listaosoitin = LataaLista(tiedosto, (Listamaaritys*)kuvaukset->at(index)->maarite, linkkitiedot, glob_data);
			data->push_back(listaosoitin);
			break;
		case attrib_LINKKI:
			tiedosto.read((char*)&sektori, sizeof(int));
			if(sektori != -1)
			{
				LueString(&tyyppinimi, tiedosto);

				tyyppi = -1;
				for(indeksi = 0; indeksi < glob_data->objektityypit.size(); indeksi++)
				{
					if(tyyppinimi == glob_data->objektityypit.at(indeksi)->nimi)
					{
						tyyppi = indeksi;
						break;
					}
				}

				if(tyyppi == -1) return false;

				uusi = new Linkkitieto;
				uusi->sektori = sektori;
				uusi->tyyppi = tyyppi;
				uusi->mista = data;
				uusi->vektori_indeksi = index;
				
				tiedosto.read((char*)(&(uusi->monesko)), sizeof(int));
				linkkitiedot->push_back(uusi);
			}
			
			data->push_back(0);
			break;

		default: break;
		}
	}

	return true;
}





std::vector <void *> *Editoriobjekti::AnnaData() {return data;}

void Editoriobjekti::TuhoaLinkki(Editorilinkki *os)
{
	for(int index = 0; index < linkit_tahan.size(); index++)
	{
		if(os == linkit_tahan.at(index))
		{
			linkit_tahan.erase(linkit_tahan.begin()+index);
			break;
		}
	}
	
}

bool Editoriobjekti::LuoObjekti(int t, int x_tile, int y_tile, GLOBAALI *glob_data)
{
	x_ = x_tile;
	y_ = y_tile;
	tyyppinumero = t;
	data = new std::vector <void *>;

	
	for(unsigned int index = 0; index < glob_data->objektityypit.at(tyyppinumero)->attribuutit.size(); index++)
	{
		int type = glob_data->objektityypit.at(tyyppinumero)->attribuutit.at(index)->tyyppi;
		void *maarite = glob_data->objektityypit.at(tyyppinumero)->attribuutit.at(index)->maarite;

		int *luku;
		void *lisattava;
		std::vector < std::vector <void *> * >*lista;
		

		switch(type)
		{
		case attrib_LUKU:
			luku = new int;
			*luku = ((MinMaxAlku*)maarite)->alku;
			lisattava = luku;
			break;
		case attrib_LISTA:
			lista = new std::vector < std::vector <void *> * >;
			lisattava = lista;
			break;
		case attrib_LINKKI:
			lisattava = 0;
		default: break;
		}

		data->push_back(lisattava);
	}

	return true;
}

void Objekti::TuhoaData(GLOBAALI *glob_data, bool editori)
{
	for(unsigned int index = 0; index < data->size(); index++)
	{
		Editorilinkki *linkki;
		switch(glob_data->objektityypit.at(tyyppinumero)->attribuutit.at(index)->tyyppi)
		{
		case attrib_LUKU:
			delete (int*)data->at(index);
			data->at(index) = 0;
			break;
		case attrib_LISTA:
			TuhoaLista((std::vector < std::vector <void *> * > *)data->at(index),
				(Listamaaritys*)(glob_data->objektityypit.at(tyyppinumero)->attribuutit.at(index)->maarite),editori);

			delete (std::vector < std::vector <void *> * > *)data->at(index);
			data->at(index) = 0;
			break;
		case attrib_LINKKI:
			linkki = (Editorilinkki*)data->at(index);
			if(linkki)
			{
				if(editori)
				{
					((Editoriobjekti*)linkki->mihin)->TuhoaLinkki(linkki);
					delete linkki;
				}
			}
			break;
		default: break;
		}
	}

	data->clear();

	delete data;
	data = 0;
}

bool Editoriobjekti::TuhoaObjekti(GLOBAALI *glob_data)
{
	TuhoaData(glob_data,true);

	// Tuhotaan tähän objektiin osoittavat linkit
	for(int index = 0; index < linkit_tahan.size(); index++)
	{
		Editorilinkki *linkki = linkit_tahan.at(index);

		// laitetaan osoitin osoittamaan tyhjään = ei linkkiä
		linkki->mista->at(linkki->vektori_indeksi) = 0;
		delete linkki;
	}

	linkit_tahan.clear();

	return true;
}

bool Editoriobjekti::TallennaLista(std::ofstream &tiedosto, Listamaaritys *maaritys, std::vector <std::vector<void *>*> *Listadata, Editor *editori, GLOBAALI *glob_data)
{
	int koko = Listadata->size();
	tiedosto.write((char*)&koko, sizeof(int));

	for(int listaalkio = 0; listaalkio < koko; listaalkio++)
	{
		for(unsigned int index = 0; index < maaritys->attribuutit.size(); index++)
		{
			Editorilinkki *linkki = 0;
			Editorisektori *sektori = 0;
			std::vector <Editoriobjekti*> *objektit = 0;
			int indeksi;
			int laskuri, tulos;
			int tyyppi;
			std::string tyyppinimi;

			switch(maaritys->attribuutit.at(index)->tyyppi)
			{
			case attrib_LUKU:
				tiedosto.write((char*)Listadata->at(listaalkio)->at(index),sizeof(int));
				break;
			case attrib_LISTA:
				TallennaLista(tiedosto,(Listamaaritys*)maaritys->attribuutit.at(index)->maarite,
					(std::vector <std::vector<void *>*>*)Listadata->at(listaalkio)->at(index), editori, glob_data);
				break;
			case attrib_LINKKI:
				linkki = (Editorilinkki*)data->at(index);
				if(linkki)
				{
					sektori = editori->annaSektorit()->at(linkki->kohdesektori);
					objektit = sektori->AnnaObjektit();
					laskuri = 0; tulos = 0;
					tyyppi = linkki->mihin->annaTyyppi();
		
					for(indeksi = 0; indeksi < objektit->size(); indeksi++)
					{
						if(objektit->at(indeksi) == linkki->mihin)
							tulos = laskuri;
						else if(objektit->at(indeksi)->annaTyyppi() == tyyppi)
							laskuri++;
					}
					
					tiedosto.write((char*)&linkki->kohdesektori, sizeof(int));
					tyyppinimi = glob_data->objektityypit.at(tyyppi)->nimi;
					KirjoitaString(&tyyppinimi, tiedosto);
					tiedosto.write((char*)&tulos, sizeof(int));
				}
				else
				{
					tyyppi = -1;
					tiedosto.write((char*)&tyyppi, sizeof(int));
				}
				break;
			default: break;
			}
		}
	}

	return true;
}


bool Editoriobjekti::TallennaData(std::ofstream &tiedosto, GLOBAALI *glob_data, Editor *editori)
{
	std::vector <Attribuutti *> *kuvaukset = &(glob_data->objektityypit.at(tyyppinumero)->attribuutit);

	int koko = 0;
	tiedosto.write((char*)&koko, sizeof(int));

	tiedosto.write((char*)&x_, sizeof(int));
	tiedosto.write((char*)&y_, sizeof(int));

	int triggerit = 0;

	tiedosto.write((char*)&triggerit, sizeof(int));
	
	for(unsigned int index = 0; index < kuvaukset->size(); index++)
	{
		
		Editorilinkki *linkki = 0;
		Editorisektori *sektori = 0;
		std::vector <Editoriobjekti*> *objektit = 0;
		int indeksi;
		int laskuri, tulos;
		int tyyppi;
		std::string tyyppinimi;

		switch(kuvaukset->at(index)->tyyppi)
		{
		case attrib_LUKU:
			tiedosto.write((char*)data->at(index), sizeof(int));
			break;
		case attrib_LISTA:
			TallennaLista(tiedosto, (Listamaaritys *)kuvaukset->at(index)->maarite, (std::vector <std::vector<void *>*>*)data->at(index), editori, glob_data);
			break;

		case attrib_LINKKI:
			linkki = (Editorilinkki*)data->at(index);
			if(linkki)
			{
				sektori = editori->annaSektorit()->at(linkki->kohdesektori);
				
				objektit = sektori->AnnaObjektit();
				laskuri = 0;
				tulos = 0;
				tyyppi = linkki->mihin->annaTyyppi();

				for(indeksi = 0; indeksi < objektit->size(); indeksi++)
				{
					if(objektit->at(indeksi) == linkki->mihin)
						tulos = laskuri;

					else if(objektit->at(indeksi)->annaTyyppi() == tyyppi)
						laskuri++;
				}
	
				tiedosto.write((char*)&(linkki->kohdesektori), sizeof(int));
				tyyppinimi = glob_data->objektityypit.at(tyyppi)->nimi;
				KirjoitaString(&tyyppinimi, tiedosto);
				tiedosto.write((char*)&tulos, sizeof(int));	
			}
			else
			{
				tyyppi = -1;
				tiedosto.write((char*)&tyyppi, sizeof(int));
			}
			break;
		default: break;
		}
	}

	return true;
}




void Editoriobjekti::PiirraKuvake(GLOBAALI *glob_data, int x_cam, int y_cam)
{
	Sprite *sprite = glob_data->objektityypit.at(tyyppinumero)->kuvake;
	int x_koord = glob_data->objektityypit.at(tyyppinumero)->x_koord;
	int y_koord = glob_data->objektityypit.at(tyyppinumero)->y_koord;

	Rect lahde;
	lahde.x = x_koord;
	lahde.y = y_koord;
	lahde.width = 32;
	lahde.height = 32;
	glob_data->gfx->PiirraSprite(sprite, (x_-x_cam)*32, (y_-y_cam)*32, &lahde, 1, 0);
}