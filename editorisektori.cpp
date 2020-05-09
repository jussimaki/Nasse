#include "maaritykset.h"


bool Editorisektori::kaynnista(GLOBAALI *glob_data)
{

	for(int setti = 0; setti < glob_data->tilesetit.size(); setti++)
	{
		if(setti == tileset) continue;
		vapauta_setti(glob_data,setti);
	}

	lataa_setti(glob_data,tileset);

	return true;
}

bool KirjoitaString(std::string *str, std::ofstream &tiedosto)
{
	const char *c_jono = str->c_str();

	int index = 0;
	while(1)
	{
		char merkki = c_jono[index];
		tiedosto.write(&merkki, sizeof(char));
		index++;
		if(merkki == '\0') break;
	}

	return true;
}

bool LueString(std::string *str, std::ifstream &tiedosto)
{
	str->erase();
	while(1)
	{
		char merkki;
		tiedosto.read(&merkki, sizeof(char));
		if(merkki == '\0') break;
		(*str) = (*str) + merkki;

	}

	return true;
}

Tilemap *Editorisektori::annamappi(){return layerit[kerros].annamappi();}
Tilemap *Editorisektori::annaleikepoyta(){return layerit[kerros].annaleikepoyta();}
std::vector <Editoriobjekti *> *Editorisektori::AnnaObjektit() {return &objektit;}
int Editorisektori::AnnaKerros() {return kerros;}
int Editorisektori::AnnaTileset() {return tileset;}
int Editorisektori::AnnaMusiikki() {return musiikki;}
void Editorisektori::AsetaTileset(int set) {tileset = set;}
void Editorisektori::AsetaMusiikki(int m) {musiikki = m;}
void Editorisektori::SaadaKerros(int k) {kerros = k;}
int Editorisektori::AnnaXcam(){return layerit[kerros].AnnaXcam();}
int Editorisektori::AnnaYcam(){return layerit[kerros].AnnaYcam();}


std::vector <Linkkitieto *> *Editorisektori::AnnaLinkkitiedot() {return &linkit;}
Editorilayer* Editorisektori::AnnaLayerit() {return layerit;}

bool Editorisektori::TuhoaTurhat(GLOBAALI *glob_data)
{
	int x_tilet = layerit[0].annamappi()->AnnaX();
	int y_tilet = layerit[0].annamappi()->AnnaY();

	for(unsigned int index = 0; index < objektit.size(); index++)
	{
		int x = objektit.at(index)->AnnaX();
		int y = objektit.at(index)->AnnaY();
		if(x > x_tilet-1 || y > y_tilet-1 || x < 0 || y < 0)
		{
			objektit.at(index)->TuhoaObjekti(glob_data);
			objektit.at(index) = 0;
		}
	}

	std::vector <Editoriobjekti *>::iterator iter;

	int indeksi = 0;
	int koko = objektit.size();

	while(indeksi < koko)
	{
		if(objektit.at(indeksi) == 0)
		{
			iter = objektit.begin() + indeksi;
			objektit.erase(iter);
			koko --;
		}
		else indeksi++;
	}

	return true;
}

bool Editorisektori::Tallenna(std::ofstream &tiedosto, GLOBAALI *glob_data, Editor *editori, int koodi)
{
	std::string setinnimi = glob_data->tilesetit.at(tileset)->nimi;
	
	const char *nimi = setinnimi.c_str();

	KirjoitaString(&setinnimi, tiedosto);
	
	tiedosto.write((char*)&musiikki, sizeof(int));

	layerit[0].Tallenna(tiedosto,koodi);
	layerit[1].Tallenna(tiedosto,koodi);
	layerit[2].Tallenna(tiedosto,koodi);
	
	
	std::vector<std::vector<Editoriobjekti *>*> objektityypit;

	unsigned int index, tyyppi;
	for(index = 0; index < glob_data->objektityypit.size(); index++)
	{
		std::vector<Editoriobjekti*> *pointteri = new std::vector<Editoriobjekti*>;
		objektityypit.push_back(pointteri);
	}

	for(index = 0; index < objektit.size(); index++)
	{
		Editoriobjekti *objekti = objektit.at(index);
		objektityypit.at(objekti->annaTyyppi())->push_back(objekti);
	}

	int tyyppimaara = 0;
	for(tyyppi = 0; tyyppi < glob_data->objektityypit.size(); tyyppi++)
	{
		if(objektityypit.at(tyyppi)->size() != 0) tyyppimaara++;
	}

	tiedosto.write((char*)&tyyppimaara, sizeof(int));

	for(tyyppi = 0; tyyppi < glob_data->objektityypit.size(); tyyppi++)
	{
		if(objektityypit.at(tyyppi)->size() != 0)
		{
			std::string nimi = glob_data->objektityypit.at(tyyppi)->nimi;
			KirjoitaString(&nimi, tiedosto);
			int maara = objektityypit.at(tyyppi)->size();
			tiedosto.write((char*)&maara, sizeof(int));

			for(unsigned int numero = 0; numero < objektityypit.at(tyyppi)->size(); numero++)
			{
				objektityypit.at(tyyppi)->at(numero)->TallennaData(tiedosto, glob_data, editori);
			}
		}
	}
	return true;
}

bool Editorisektori::Alustus()
{
	tileset = 0;
	musiikki = 0;
	kerros = 0;
	
	
	for(int index = 0; index < 3; index++)
	{
		layerit[index].Alusta();
	}

	return true;
}

bool Editorisektori::Lataa(std::ifstream &tiedosto, GLOBAALI *glob_data, int koodi)
{
	
	tileset = 0;
	musiikki = 0;
	kerros = 0;
		

	std::string setinnimi = "";
	
	
	LueString(&setinnimi, tiedosto);

	int numero = 0;
	bool loytyi = 0;

	unsigned int index;
	for(index = 0; index < glob_data->tilesetit.size(); index++)
	{
		if(glob_data->tilesetit.at(index)->nimi == setinnimi)
		{
			numero = index;
			loytyi = 1;
			break;
		}
	}

	tileset = numero;
	tiedosto.read((char*)&musiikki, sizeof(int));

	for(index = 0; index < 3; index++)
	{
		layerit[index].Lataa(tiedosto,glob_data,tileset,index, koodi);
	}

	

	
	std::vector<std::vector<Editoriobjekti *>*> objektityypit;

	unsigned int tyyppimaara;

	tiedosto.read((char*)&tyyppimaara, sizeof(int));

	for(index = 0; index < tyyppimaara; index++)
	{
		std::string tyyppinimi;
		LueString(&tyyppinimi, tiedosto);

		loytyi = 0;

		unsigned int indeksi;
		for(indeksi = 0; indeksi < glob_data->objektityypit.size(); indeksi++)
		{
			if(glob_data->objektityypit.at(indeksi)->nimi == tyyppinimi)
			{
				numero = indeksi;
				loytyi = 1;
			}
		}
		if(!loytyi) return false;

		unsigned int maara;
		tiedosto.read((char*)&maara, sizeof(int));

		for(indeksi = 0; indeksi < maara; indeksi++)
		{
			int koko;
			tiedosto.read((char*)&koko, sizeof(int));

			if(loytyi)
			{
				Editoriobjekti *uusi = new Editoriobjekti;
				uusi->LataaData(tiedosto, glob_data, numero, &linkit);
				objektit.push_back(uusi);
			}
			else
			{
				for(int tyhja = 0; tyhja < koko; tyhja++)
				{
					int jep;
					tiedosto.read((char*)&jep, sizeof(int));
				}
			}
		}

	}

	return true;
}

bool Editorisektori::PiirraTilemap(GLOBAALI *glob_data)
{
	layerit[kerros].PiirraTilemap(tileset, kerros, glob_data);
	return true;
}



bool Editorisektori::Tuhoa(GLOBAALI *glob_data)
{
	
	unsigned int index;
	for(index = 0; index < objektit.size(); index++)
	{
		objektit.at(index)->TuhoaObjekti(glob_data);
		delete objektit.at(index);
		objektit.at(index) = 0;
	}
	objektit.clear();

	for(index = 0; index < 3; index++)
	{
		layerit[index].Tuhoa();
	}

	return true;
}

void Editorisektori::Objektituhous(GLOBAALI *glob_data, int x_tile, int y_tile)
{
	std::vector <Editoriobjekti *> *vektori = &objektit;

	int koko = vektori->size();

	int index = koko-1;
	while(index >= 0)
	{
		

		if(x_tile == vektori->at(index)->AnnaX() && y_tile == vektori->at(index)->AnnaY())
		{
			vektori->at(index)->TuhoaObjekti(glob_data);
			delete vektori->at(index);

			vektori->erase(vektori->begin()+index);

			return;

			//koko--;
		}// else
		
		index--;
	}

}

void Editorisektori::Objektisiirto(int x, int y, GLOBAALI *glob_data, int x_tile, int y_tile)
{
	std::vector <Editoriobjekti *> *vektori = &objektit;

	int koko = vektori->size();

	int index = 0;
	while(index < koko)
	{
		

		if(x_tile == vektori->at(index)->AnnaX() && y_tile == vektori->at(index)->AnnaY())
		{
			vektori->at(index)->x_siirto(x);
			vektori->at(index)->y_siirto(y);

			TuhoaTurhat(glob_data);

			return;
		} 

		index++;
	}
}


void Editorisektori::vasenTyonto(int x, GLOBAALI *glob_data)
{
	int koko = objektit.size();
	int index = 0;

	if(kerros == 0)
	while(index < koko)
	{
		if(objektit.at(index)->AnnaX() <= x)
		{
			objektit.at(index)->x_siirto(-1);

			if(objektit.at(index)->AnnaX() == -1)
			{
				objektit.at(index)->TuhoaObjekti(glob_data);
				delete objektit.at(index);

				objektit.erase(objektit.begin()+index);
				koko--;
				index--;
			}
		}
		index++;
	}

	layerit[kerros].annamappi()->vasenTyonto(x);
}

void Editorisektori::vasenVeto(int x, GLOBAALI *glob_data)
{
	int koko = objektit.size();
	int index = 0;

	if(kerros == 0)
	while(index < koko)
	{
		if(objektit.at(index)->AnnaX() >= x)
		{
			objektit.at(index)->x_siirto(-1);

			if(objektit.at(index)->AnnaX() < x)
			{
				objektit.at(index)->TuhoaObjekti(glob_data);
				delete objektit.at(index);

				objektit.erase(objektit.begin()+index);
				koko--;
				index--;
			}
		}

		index++;
	}

	layerit[kerros].annamappi()->vasenVeto(x);
}


void Editorisektori::oikeaTyonto(int x, GLOBAALI *glob_data)
{
	int koko = objektit.size();
	int index = 0;

	int x_tilet = layerit[0].annamappi()->AnnaX();

	if(kerros == 0)
	while(index < koko)
	{
		if(objektit.at(index)->AnnaX() >= x)
		{
			objektit.at(index)->x_siirto(1);

			if(objektit.at(index)->AnnaX() == x_tilet)
			{
				objektit.at(index)->TuhoaObjekti(glob_data);
				delete objektit.at(index);

				objektit.erase(objektit.begin()+index);
				koko--;
				index--;
			}
		}
		index++;
	}

	layerit[kerros].annamappi()->oikeaTyonto(x);
}

void Editorisektori::oikeaVeto(int x, GLOBAALI *glob_data)
{
	int koko = objektit.size();
	int index = 0;

	if(kerros == 0)
	while(index < koko)
	{
		if(objektit.at(index)->AnnaX() <= x)
		{
			objektit.at(index)->x_siirto(1);

			if(objektit.at(index)->AnnaX() > x)
			{
				objektit.at(index)->TuhoaObjekti(glob_data);
				delete objektit.at(index);

				objektit.erase(objektit.begin()+index);
				koko--;
				index--;
			}
		}
		index++;
	}

	layerit[kerros].annamappi()->oikeaVeto(x);
}

void Editorisektori::ylaTyonto(int y, GLOBAALI *glob_data)
{
	int koko = objektit.size();
	int index = 0;

	if(kerros == 0)
	while(index < koko)
	{
		if(objektit.at(index)->AnnaY() <= y)
		{
			objektit.at(index)->y_siirto(-1);

			if(objektit.at(index)->AnnaY() == -1)
			{
				objektit.at(index)->TuhoaObjekti(glob_data);
				delete objektit.at(index);

				objektit.erase(objektit.begin()+index);
				koko--;
				index--;
			}
		}

		index++;
	}

	layerit[kerros].annamappi()->ylaTyonto(y);
}

void Editorisektori::ylaVeto(int y, GLOBAALI *glob_data)
{
	int koko = objektit.size();
	int index = 0;

	if(kerros == 0)
	while(index < koko)
	{
		if(objektit.at(index)->AnnaY() >= y)
		{
			objektit.at(index)->y_siirto(-1);

			if(objektit.at(index)->AnnaY() < y)
			{
				objektit.at(index)->TuhoaObjekti(glob_data);
				delete objektit.at(index);

				objektit.erase(objektit.begin()+index);
				koko--;
				index--;
			}
		}

		index++;
	}

	layerit[kerros].annamappi()->ylaVeto(y);
}


void Editorisektori::alaTyonto(int y, GLOBAALI *glob_data)
{
	int koko = objektit.size();
	int index = 0;

	int y_tilet = layerit[0].annamappi()->AnnaY();

	if(kerros == 0)
	while(index < koko)
	{
		if(objektit.at(index)->AnnaY() >= y)
		{
			objektit.at(index)->y_siirto(1);

			if(objektit.at(index)->AnnaY() == y_tilet)
			{
				objektit.at(index)->TuhoaObjekti(glob_data);
				delete objektit.at(index);

				objektit.erase(objektit.begin()+index);
				koko--;
				index--;
			}
		}
		index++;
	}

	layerit[kerros].annamappi()->alaTyonto(y);
}

void Editorisektori::alaVeto(int y, GLOBAALI *glob_data)
{
	int koko = objektit.size();
	int index = 0;

	if(kerros == 0)
	while(index < koko)
	{
		if(objektit.at(index)->AnnaY() <= y)
		{
			objektit.at(index)->y_siirto(1);

			if(objektit.at(index)->AnnaY() > y)
			{
				objektit.at(index)->TuhoaObjekti(glob_data);
				delete objektit.at(index);

				objektit.erase(objektit.begin()+index);
				koko--;
				index--;
			}
		}
		index++;
	}

	layerit[kerros].annamappi()->alaVeto(y);
}