#include "maaritykset.h"

extern int hyppy_nopeus, bee, kerroin, gravitaatio, imaisu, putoamisnopeus;
extern int varasto;

std::vector <Editorisektori *> *Editor::annaSektorit() {return &sektorit;}
bool Editor::PiirraObjektit(GLOBAALI *glob_data)
{
	Editorisektori *temp = sektorit.at(sektori);

	std::vector <Editoriobjekti *> *objektit = temp->AnnaObjektit();

	int x_cam = temp->AnnaXcam();
	int y_cam = temp->AnnaYcam();

	if(alku_sektori == sektori && alku_x >= x_cam && alku_x < x_cam + 20 && alku_y >= y_cam && alku_y < y_cam + 15)
	{
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, (alku_x - x_cam)*32, (alku_y - y_cam)*32,
			&laatikot[alku_kuva], 1, 0);
	}

	for(unsigned int index = 0; index < objektit->size(); index++)
	{
		Editoriobjekti *objekti = objektit->at(index);
		int obj_x = objekti->AnnaX();
		int obj_y = objekti->AnnaY();

		if(obj_x >= x_cam && obj_x < x_cam + 20 && obj_y >= y_cam && obj_y < y_cam + 15)
		{
			int tyyppi = objekti->annaTyyppi();
			Sprite *sprite = glob_data->objektityypit.at(tyyppi)->kuvake;
			Rect laatikko;
			laatikko.x = glob_data->objektityypit.at(tyyppi)->x_koord;
			laatikko.y = glob_data->objektityypit.at(tyyppi)->y_koord;
			laatikko.height = 32;
			laatikko.width = 32;
			glob_data->gfx->PiirraSprite(sprite, (obj_x-x_cam)*32, (obj_y-y_cam)*32, &laatikko, 1, 0);
		}
	}

	return true;
}

int Editor::Kaskyt(GLOBAALI *glob_data)
{
	bool lopetus = 0;
	Editorisektori *temp = sektorit.at(sektori);

	unsigned int index;

	std::vector <Editoriobjekti *> *objektit;

	for(index = 0; index < glob_data->kaskyt.size(); index++)
	{
		unsigned int objekti;
		Editoriobjekti *obj;

		Kasky *kasky = glob_data->kaskyt.at(index);
		Tilemap *mappi;
		Editorisektori *uusi;
		char *komentoteksti;
		
		int vanhasetti, uusisetti;
		switch(kasky->tyyppi)
		{
		case kasky_lisaa_objekti:
			objektit = temp->AnnaObjektit();
			objektit->push_back((Editoriobjekti*)kasky->data);
			break;

		case kasky_tuhoa_objekti:
			objektit = temp->AnnaObjektit();

			obj = (Editoriobjekti*)kasky->data;

			for(objekti = 0; objekti < objektit->size(); objekti++)
			{
				if(objektit->at(objekti) == obj)
				{
					objektit->erase(objektit->begin()+objekti);
					break;
				}
			}

			obj->TuhoaObjekti(glob_data);
			delete obj;
			break;
		case kasky_muuta_mappia:
			mappi = temp->annamappi();
			mappi->MuutaKoko(((std::vector <int> *) (kasky->data))->at(0),((std::vector <int> *) (kasky->data))->at(1));
			if(temp->AnnaKerros() == 0) temp->TuhoaTurhat(glob_data);
			break;
		case kasky_siirra_mappia:
			mappi = temp->annamappi();
			mappi->x_siirto(((std::vector <int> *) (kasky->data))->at(0));
			mappi->y_siirto(((std::vector <int> *) (kasky->data))->at(1));

			if(temp->AnnaKerros() == 0)
			{
				objektit = temp->AnnaObjektit();
				for(objekti = 0; objekti < objektit->size(); objekti++)
				{
					objektit->at(objekti)->x_siirto(((std::vector <int> *) (kasky->data))->at(0));
					objektit->at(objekti)->y_siirto(((std::vector <int> *) (kasky->data))->at(1));
				}

				temp->TuhoaTurhat(glob_data);
			}


			break;
		case kasky_uusi_sektori:
			uusi = new Editorisektori;
			uusi->Alustus();
			sektorit.push_back(uusi);
			break;
		case kasky_valitse_sektori:
			vanhasetti = sektorit.at(sektori)->AnnaTileset();
			sektori = ((std::vector <int> *) (kasky->data))->at(0);
			uusisetti = sektorit.at(sektori)->AnnaTileset();
			
			if(uusisetti != vanhasetti)
			{
				vapauta_setti(glob_data,vanhasetti);
				lataa_setti(glob_data,uusisetti);
			}

			break;

		case kasky_tuhoa_sektori:
			sektorit.at(sektori)->Tuhoa(glob_data);
			sektorit.erase(sektorit.begin()+sektori);
			if(sektori > (int)sektorit.size() -1) sektori = sektorit.size() -1;
			break;

		case kasky_sektorisaato:
			vanhasetti = temp->AnnaTileset();
			temp->AsetaTileset(((std::vector <int> *)kasky->data)->at(0));
			temp->AsetaMusiikki(((std::vector <int> *)kasky->data)->at(1));
			uusisetti = temp->AnnaTileset();
			
			if(uusisetti != vanhasetti)
			{
				vapauta_setti(glob_data,vanhasetti);
				lataa_setti(glob_data,uusisetti);
			}

			break;

		case kasky_komento:
			komentoteksti = (char *)kasky->data;
			char *komento;
			char *argumentti;
			char *os;	
			
			komento = poimi_sana(komentoteksti,&os);
			argumentti = poimi_sana(os,&os);
			
			if(komento)
			{
				if(vertaa_sanoja("load", komento))
				{
					if(argumentti)
					{
						std::string nimi = argumentti;

						std::ifstream tiedosto(nimi.c_str(), std::ios_base::binary);
						if(tiedosto)
						{
							Tuhoa(glob_data);
							lataa(tiedosto, glob_data);
						}
					}
				}
				else if(vertaa_sanoja("save", komento))
				{
					if(argumentti)
					{
						std::string nimi = argumentti;
						tallenna(nimi, glob_data);						
					}
				}
				else if(vertaa_sanoja("new", komento))
				{
					Tuhoa(glob_data);
					alustus(glob_data);
				}
				else if(vertaa_sanoja("run", komento))
				{
					lopetus = 1;
					tallenna("current", glob_data);
				}
				else if(vertaa_sanoja("tilemap", komento))
				{
					if(argumentti)
					{
						std::string nimi = argumentti;
						tallennaKartta(nimi, glob_data);
					}
				}
				
				if(argumentti)
				{
					delete[] argumentti;
					argumentti = 0;
				}
				if(komento)
				{
					delete[] komento;
					komento = 0;
				}
			}

			if(kasky->data)
			{
				delete[] (char*)kasky->data;
				kasky->data = 0;
			}

			break;

		case kasky_ota_linkki:
			linkki = (Editoriobjekti*)kasky->data;
			linkkisektori = sektori;
			break;


		default: break;
		}

		delete glob_data->kaskyt.at(index);

	}

	glob_data->kaskyt.clear();

	if(lopetus) return 1;

	return 0;
}

std::vector <Editoriobjekti *> *Editor::Objektihaku()
{
	Editorisektori *temp = sektorit.at(sektori);
	std::vector <Editoriobjekti *> *vektori = new std::vector <Editoriobjekti *>;

	int x_cam = temp->AnnaXcam();
	int y_cam = temp->AnnaYcam();

	int x_tile = hiiri_x/32 + x_cam;
	int y_tile = hiiri_y/32 + y_cam;
	std::vector <Editoriobjekti *> *saadut = sektorit.at(sektori)->AnnaObjektit();

	for(unsigned int index = 0; index < saadut->size(); index++)
	{
		if(x_tile == saadut->at(index)->AnnaX() && y_tile == saadut->at(index)->AnnaY())
		{
			vektori->push_back(saadut->at(index));
		}
	}

	if(vektori->size() == 0)
	{
		delete vektori;
		return 0;
	}

	return vektori;
}



bool Editor::alustus(GLOBAALI *glob_data)
{
	

	ikkuna = 0;
	moodi = mapinmuokkaus;
	valintamappi = 0;
	kopiointi = 0;
	sektori = 0;
	hiiri_x = 320;
	hiiri_y = 240;
	linkki = 0;

	editoritila = editori_EDITOINTI;
	
	

	alku_sektori = 0;
	alku_x = 0;
	alku_y = 0;

	sektorit.clear();

	Editorisektori *uusi_sektori = new Editorisektori;
	uusi_sektori->Alustus();
	sektorit.push_back(uusi_sektori);

	for(int index = 0; index < glob_data->tilesetit.size(); index++)
	{
		vapauta_setti(glob_data,index);
	}

	lataa_setti(glob_data,0);

	return true;
}

bool Editor::Tuhoa(GLOBAALI *glob_data)
{
	for(unsigned int index = 0; index < sektorit.size(); index++)
	{
		sektorit.at(index)->Tuhoa(glob_data);
		delete sektorit.at(index);
	}

	sektorit.clear();
	
	return true;
}



bool Editor::lataa(std::ifstream &tiedosto, GLOBAALI *glob_data)
{
    ikkuna = 0;
	moodi = mapinmuokkaus;
	valintamappi = 0;
	sektori = 0;
	linkki = 0;
	int sektorimaara;

	sektorit.clear();

	int koodi;
	tiedosto.read((char *)&koodi, sizeof(int));

	tiedosto.read((char *)&sektorimaara, sizeof(int));
	tiedosto.read((char *)&alku_x, sizeof(int));
	tiedosto.read((char *)&alku_y, sizeof(int));
	tiedosto.read((char *)&alku_sektori, sizeof(int));

	sektori = alku_sektori;

	for(int index = 0; index < sektorimaara; index++)
	{
		Editorisektori *uusi = new Editorisektori;
		
		uusi->Lataa(tiedosto, glob_data, koodi);

		if(index == sektori) uusi->AsetaCam(alku_x-10,alku_y-7);
		sektorit.push_back(uusi);
	}

	

	for(index = 0; index < sektorimaara; index++)
	{
		std::vector <Linkkitieto *> *tiedot = sektorit.at(index)->AnnaLinkkitiedot();

		for(int index2 = 0; index2 < tiedot->size(); index2++)
		{
			std::vector <Editoriobjekti *> *objektit =
			sektorit.at(tiedot->at(index2)->sektori)->AnnaObjektit();

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
						Editorilinkki *uusi = new Editorilinkki;
						uusi->kohdesektori = tiedot->at(index2)->sektori;
						uusi->mihin = objektit->at(index3);
						uusi->mista = tiedot->at(index2)->mista;
						uusi->vektori_indeksi = tiedot->at(index2)->vektori_indeksi;
						uusi->mista->at(uusi->vektori_indeksi) = uusi;
						((Editoriobjekti*)(uusi->mihin))->LisaaLinkki(uusi);
						break;
					}
					else laskuri++;
				}
			}

			delete tiedot->at(index2);
		}

		tiedot->clear();
	}

	for(index = 0; index < glob_data->tilesetit.size(); index++)
	{
		vapauta_setti(glob_data,index);
	}

	lataa_setti(glob_data,sektorit.at(sektori)->AnnaTileset());

	
	return true;
}

bool Editor::tallenna(std::string tiedostonimi, GLOBAALI *glob_data)
{
	std::ofstream tiedosto(tiedostonimi.c_str(), std::ios_base::binary);
	int sektorimaara = sektorit.size();

	int koodi = 2;
	tiedosto.write((char*)&koodi, sizeof(int));

	tiedosto.write((char *)&sektorimaara, sizeof(int));
	tiedosto.write((char *)&alku_x, sizeof(int));
	tiedosto.write((char *)&alku_y, sizeof(int));
	tiedosto.write((char *)&alku_sektori, sizeof(int));
	
	for(int index = 0; index < sektorimaara; index++)
		sektorit.at(index)->Tallenna(tiedosto, glob_data, this, koodi);

	tiedosto.close();
	return true;
}

bool Editor::tallennaKartta(std::string tiedostonimi, GLOBAALI *glob_data)
{
	std::ofstream tiedosto(tiedostonimi.c_str(), std::ios_base::binary);
	Tilemap *os = ((sektorit.at(alku_sektori)->AnnaLayerit()))[0].annamappi();
	os->Tallenna(tiedosto,2);
	tiedosto.close();
	return true;
}



bool Editor::piirraTilet(GLOBAALI *glob_data)
{
		sektorit.at(sektori)->PiirraTilemap(glob_data);

	return true;
}

bool Editor::Piirravarjo(int x_tile, int y_tile, int x_cam, int y_cam, GLOBAALI *glob_data)
{
	int final_alku_x, final_alku_y;
	int final_loppu_x, final_loppu_y;

	if(x_tile >= leike_alku_x)
	{
		final_alku_x = leike_alku_x;
		final_loppu_x = x_tile;
	}
	else
	{
		final_alku_x = x_tile;
		final_loppu_x = leike_alku_x;
	}
	
	if(y_tile >= leike_alku_y)
	{
		final_alku_y = leike_alku_y;
		final_loppu_y = y_tile;
	}
	else
	{
		final_alku_y = y_tile;
		final_loppu_y = leike_alku_y;
	}

	final_alku_x -= x_cam;
	final_loppu_x -= x_cam;

	if(final_alku_x < 0) final_alku_x = 0;
	if(final_loppu_x > 19) final_loppu_x = 19;
	
	final_alku_y -= y_cam;
	final_loppu_y -= y_cam;

	if(final_alku_y < 0) final_alku_y = 0;
	if(final_loppu_y > 14) final_loppu_y = 14;

	for(int y = final_alku_y; y <= final_loppu_y; y++)
	{
		for(int x = final_alku_x; x <= final_loppu_x; x++)
		{
			glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, x*32, y*32, &laatikot[varjo_kuva], 0, 128);
		}
	}

	return true;
}

bool Editor::Valinta(GLOBAALI *glob_data)
{
	int x_tile = hiiri_x/32;
	int y_tile = hiiri_y/32;

	Editorisektori *temp = sektorit.at(sektori);
	
	valintamappi->Piirra(temp->AnnaTileset(), temp->AnnaKerros(), 0, 0, glob_data,0);
	valintamappi->Piirra(temp->AnnaTileset(), temp->AnnaKerros(), 0, 0, glob_data,1);
	InputData *input = glob_data->gfx->annaInput();

	Tilemap *leikepoyta = temp->annaleikepoyta();

	if(input->HIIRINAPPI_PAINETTU[OIKEA])
	{
		leike_alku_x = x_tile;
		leike_alku_y = y_tile;
		kopiointi = 1;
	}

	if(kopiointi)
	{
		Piirravarjo(x_tile, y_tile, 0, 0, glob_data);

		if(!(input->HIIRINAPPI_POHJASSA[OIKEA]))
		{
			kopiointi = 0;
			valintamappi->KopioiLeikepoydalle(leikepoyta, leike_alku_x, leike_alku_y, x_tile, y_tile);
		}
	}

	return true;
}

bool Editor::Muokkaa(GLOBAALI *glob_data)
{
	InputData *input = glob_data->gfx->annaInput();
	Editorisektori *temp = sektorit.at(sektori);

	int kerros  = temp->AnnaKerros();
	if(input->NAPPI_PAINETTU[SDLK_PAGEUP]) kerros--;
	if(kerros < 0) kerros = 0;
	if(input->NAPPI_PAINETTU[SDLK_PAGEDOWN]) kerros++;
	if(kerros > 2) kerros = 2;
	temp->SaadaKerros(kerros);

	int x_cam = temp->AnnaXcam();
	int y_cam = temp->AnnaYcam();

	int x_tile = hiiri_x/32 + x_cam;
	int y_tile = hiiri_y/32 + y_cam;

	Tilemap *mappi = temp->annamappi();
	Tilemap *leikepoyta = temp->annaleikepoyta();

	if(input->HIIRINAPPI_PAINETTU[OIKEA])
	{
		leike_alku_x = x_tile;
		leike_alku_y = y_tile;
		kopiointi = 1;
	}

	if(kopiointi)
	{
		Piirravarjo(x_tile, y_tile, x_cam, y_cam, glob_data);

		if(!(input->HIIRINAPPI_POHJASSA[OIKEA]))
		{
			kopiointi = 0;
			mappi->KopioiLeikepoydalle(leikepoyta, leike_alku_x, leike_alku_y, x_tile, y_tile);
		}
	}

	
	else
	{
		leikepoyta->PiirraLeikepoyta(temp->AnnaTileset(), temp->AnnaKerros(),
			x_tile-x_cam, y_tile-y_cam, glob_data,0);
				leikepoyta->PiirraLeikepoyta(temp->AnnaTileset(), temp->AnnaKerros(),
			x_tile-x_cam, y_tile-y_cam, glob_data,1);
	}

	if(input->HIIRINAPPI_POHJASSA[VASEN] && input->HIIRINAPPI_ESTO[VASEN] == 0)
	{
		mappi->OtaLeikepydalta(leikepoyta, x_tile, y_tile);
	}

	return true;
}

bool Editor::AlustaValintamappi(GLOBAALI *glob_data)
{
	Editorisektori *temp = sektorit.at(sektori);
	
	valintamappi = new Tilemap;
	valintamappi->Alusta(20, 15);
	unsigned char *data = valintamappi->AnnaData();

	unsigned char *mappaus = glob_data->tilesetit.at(temp->AnnaTileset())->kerrokset[temp->AnnaKerros()]->mappaus;
	for(int index = 0; index < 20*15; index++) data[index] = mappaus[index];

	return true;
}

int Editor::Toiminta(GLOBAALI *glob_data)
{
	int paluuarvo;
	switch(editoritila)
	{
	case editori_EDITOINTI:
		paluuarvo = editointi(glob_data);
		if(paluuarvo)
		{
			editoritila = editori_TESTAUS;
			testitaso.Lataa("current",glob_data);
			testitaso.AlustaObjektit(1);
			testitaso.Kaynnista(glob_data);
		}
		else if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_ESCAPE])
		{
			return 1;
		}
		break;

	case editori_TESTAUS:
		paluuarvo = tasonTestaus(glob_data);

		if(paluuarvo)
		{
			editoritila = editori_EDITOINTI;
			testitaso.Sammuta(glob_data);
			testitaso.Vapauta(glob_data);

			// tämä estää kaatumisen, kun otetaan sektori, jossa on eri tilesetti
			// kuin testitason sektorissa
			sektorit.at(sektori)->kaynnista(glob_data);
		}


		break;
	}

	return 0;
}


int Editor::tasonTestaus(GLOBAALI *glob_data)
{
	if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_r])
	{	
		testitaso.Sammuta(glob_data);
		testitaso.AlustaObjektit(1);
		testitaso.Kaynnista(glob_data);
	}


	int paluuarvo = testitaso.Toiminta(glob_data);

	if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_ESCAPE])
		testitaso.itsari(glob_data);

	if(paluuarvo != -2 || glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_F1])
	{
		return 1;
	}
/*
	char *jono;
	number_to_string(hyppy_nopeus, &jono);
	glob_data->gfx->Tulosta(jono, 20,200);
	delete[] jono;

	number_to_string(bee, &jono);
	glob_data->gfx->Tulosta(jono, 20,220);
	delete[] jono;

	number_to_string(kerroin, &jono);
	glob_data->gfx->Tulosta(jono, 20,240);
	delete[] jono;

	number_to_string(gravitaatio, &jono);
	glob_data->gfx->Tulosta(jono, 20,260);
	delete[] jono;

	number_to_string(imaisu, &jono);
	glob_data->gfx->Tulosta(jono, 20,280);
	delete[] jono;

	number_to_string(putoamisnopeus, &jono);
	glob_data->gfx->Tulosta(jono, 20,300);
	delete[] jono;

	InputData *input = glob_data->gfx->AnnaInput();

	if(input->NAPPI_PAINETTU[SDLK_e]) hyppy_nopeus -= 100;
	if(input->NAPPI_PAINETTU[SDLK_r]) hyppy_nopeus += 100;

	if(input->NAPPI_PAINETTU[SDLK_d]) bee -= 5;
	if(input->NAPPI_PAINETTU[SDLK_f]) bee += 5;

	if(input->NAPPI_PAINETTU[SDLK_c]) kerroin -= 1;
	if(input->NAPPI_PAINETTU[SDLK_v]) kerroin += 1;

	if(input->NAPPI_PAINETTU[SDLK_t]) gravitaatio -= 5;
	if(input->NAPPI_PAINETTU[SDLK_y]) gravitaatio += 5;

	if(input->NAPPI_PAINETTU[SDLK_g]) imaisu -= 5;
	if(input->NAPPI_PAINETTU[SDLK_h]) imaisu += 5;
	if(input->NAPPI_PAINETTU[SDLK_b]) putoamisnopeus -= 50;
	if(input->NAPPI_PAINETTU[SDLK_n]) putoamisnopeus += 50;
*/
	return 0;
}


int Editor::editointi(GLOBAALI *glob_data)
{
	int paluuarvo = Kaskyt(glob_data);
	if(paluuarvo) return paluuarvo;


	Editorisektori *temp = sektorit.at(sektori); // otetaan temp vasta tässä vaiheessa
	

	InputData *input = glob_data->gfx->annaInput();

	hiiri_x += input->hiiri_x_siirto;
	hiiri_y += input->hiiri_y_siirto;
	
	if(hiiri_x < 0) hiiri_x = 0;
	if(hiiri_y < 0) hiiri_y = 0;
	if(hiiri_x > 639) hiiri_x = 639;
	if(hiiri_y > 479) hiiri_y = 479;
	
	glob_data->gfx->TyhjennaRuutu();
	
	Editorilayer *layerit = temp->AnnaLayerit();

	switch(moodi)
	{
	case mapinmuokkaus:
		
		((layerit)[temp->AnnaKerros()]).Hiiriskrollaus(glob_data);
		
			((layerit)[temp->AnnaKerros()]).KorjaaKamera();
		piirraTilet(glob_data);
		

		if(temp->AnnaKerros() == 0) PiirraObjektit(glob_data);
		if(ikkuna)
		{
			int paluuarvo = ikkuna->Toiminta(glob_data, hiiri_x, hiiri_y, this);
			if(paluuarvo == ikkuna_LOPPU || paluuarvo == ikkuna_LOPPU_SULJEITSESI)
			{
				ikkuna->Tuhoa();
				delete ikkuna;
				ikkuna = 0;
				input->HIIRINAPPI_ESTO[VASEN] = 1;
			}
		}
		else
		{
			int x_tile = hiiri_x/32 + sektorit.at(sektori)->AnnaXcam();
			int y_tile = hiiri_y/32 + sektorit.at(sektori)->AnnaYcam();

			Muokkaa(glob_data);
			
			
	
			if(input->NAPPI_PAINETTU[SDLK_SPACE])
			{
				kopiointi = 0;
				moodi = tilevalinta;
				AlustaValintamappi(glob_data);
			}
			
			if(input->NAPPI_PAINETTU[SDLK_DELETE] && temp->AnnaKerros() == 0)
			{
				 temp->Objektituhous(glob_data, x_tile, y_tile);
			}
			int x_siirto = 0, y_siirto = 0;

			if(input->NAPPI_POHJASSA[SDLK_UP] && temp->AnnaKerros() == 0) y_siirto--;
			if(input->NAPPI_POHJASSA[SDLK_DOWN] && temp->AnnaKerros() == 0) y_siirto++;
			if(input->NAPPI_POHJASSA[SDLK_LEFT] && temp->AnnaKerros() == 0) x_siirto--;
			if(input->NAPPI_POHJASSA[SDLK_RIGHT] && temp->AnnaKerros() == 0) x_siirto++;

			temp->Objektisiirto(x_siirto,y_siirto,glob_data, x_tile, y_tile);

			bool veto = 0;
			if(input->NAPPI_POHJASSA[SDLK_LSHIFT]) veto = 1;

			if(input->NAPPI_PAINETTU[SDLK_a])
			{
				if(veto) temp->vasenVeto(x_tile, glob_data);
				else temp->vasenTyonto(x_tile, glob_data);
			}

			if(input->NAPPI_PAINETTU[SDLK_w])
			{
				if(veto) temp->ylaVeto(y_tile, glob_data);
				else temp->ylaTyonto(y_tile, glob_data);
			}

			if(input->NAPPI_PAINETTU[SDLK_d])
			{
				if(veto) temp->oikeaVeto(x_tile, glob_data);
				else temp->oikeaTyonto(x_tile, glob_data);
			}

			if(input->NAPPI_PAINETTU[SDLK_s])
			{
				if(veto) temp->alaVeto(y_tile, glob_data);
				else temp->alaTyonto(y_tile, glob_data);
			}
				
			
			if(input->NAPPI_PAINETTU[SDLK_RETURN] && temp->AnnaKerros() == 0)
			{
				std::vector <Editoriobjekti *> *vektori = Objektihaku();
				ikkuna = new Ikkuna;

				

				axYy *koord = new axYy;
				koord->x_tile = x_tile;
				koord->y_tile = y_tile;

				if(vektori)
				{
					Objektikuorma *kuorma = new Objektikuorma;
					kuorma->koord = koord;
					kuorma->objektit = vektori;
					ikkuna->LuoIkkuna(LUOVAIMUOKKAA, kuorma, glob_data);
				}
				else ikkuna->LuoIkkuna(LUOUUSIVALIKKO, koord, glob_data);
			}

			if(input->NAPPI_PAINETTU[SDLK_END])
			{
				Yleisdata *mapinkoko = new Yleisdata;
				mapinkoko->nimi = new std::string;
				(*(mapinkoko->nimi)) = "Muuta tilemapin kokoa";
				mapinkoko->nimet = new std::vector<std::string>;
				mapinkoko->luvut = new std::vector<int>;
				mapinkoko->minimit = new std::vector<int>;
				mapinkoko->maksimit = new std::vector<int>;

				mapinkoko->nimet->push_back("X tilet");
				mapinkoko->nimet->push_back("Y tilet");
				mapinkoko->minimit->push_back(20);
				mapinkoko->minimit->push_back(15);
				mapinkoko->maksimit->push_back(100000);
				mapinkoko->maksimit->push_back(100000);

				Editorilayer *kerrokset = temp->AnnaLayerit();
				Editorilayer kerros = (kerrokset)[temp->AnnaKerros()];
				Tilemap *mappi = kerros.annamappi();
				mapinkoko->luvut->push_back(mappi->AnnaX());
				mapinkoko->luvut->push_back(mappi->AnnaY());

				ikkuna = new Ikkuna;

				ikkuna->LuoIkkuna(YLEISIKKUNA, mapinkoko, glob_data);				
				
			}
			if(input->NAPPI_PAINETTU[SDLK_INSERT])
			{
				Yleisdata *mapinsiirto = new Yleisdata;
				mapinsiirto->nimi = new std::string;
				(*(mapinsiirto->nimi)) = "Siirra tilemappia";
				mapinsiirto->nimet = new std::vector<std::string>;
				mapinsiirto->luvut = new std::vector<int>;
				mapinsiirto->minimit = new std::vector<int>;
				mapinsiirto->maksimit = new std::vector<int>;

				mapinsiirto->nimet->push_back("X siirto");
				mapinsiirto->nimet->push_back("Y siirto");

				Editorilayer *kerrokset = temp->AnnaLayerit();
				Editorilayer kerros = (kerrokset)[temp->AnnaKerros()];
				Tilemap *mappi = kerros.annamappi();
				int x_tilet = mappi->AnnaX();
				int y_tilet = mappi->AnnaY();

				mapinsiirto->minimit->push_back(-x_tilet);
				mapinsiirto->minimit->push_back(-y_tilet);
				mapinsiirto->maksimit->push_back(x_tilet);
				mapinsiirto->maksimit->push_back(y_tilet);
				mapinsiirto->luvut->push_back(0);
				mapinsiirto->luvut->push_back(0);

				ikkuna = new Ikkuna;
				ikkuna->LuoIkkuna(YLEISIKKUNA+1, mapinsiirto, glob_data);				
			}
			if(input->NAPPI_PAINETTU[SDLK_F9])
			{
				Yleisdata *uusisektori = new Yleisdata;
				uusisektori->nimi = new std::string;
				*(uusisektori->nimi) = "Uusi sektori?";

				uusisektori->nimet = new std::vector<std::string>;
				uusisektori->luvut = new std::vector<int>;
				uusisektori->minimit = new std::vector<int>;
				uusisektori->maksimit = new std::vector<int>;

				ikkuna = new Ikkuna;
				ikkuna->LuoIkkuna(YLEISIKKUNA+2, uusisektori, glob_data);
			}
			if(input->NAPPI_PAINETTU[SDLK_F10])
			{
				Yleisdata *valitsesektori = new Yleisdata;
				valitsesektori->nimi = new std::string;
				*(valitsesektori->nimi) = "Valitse sektori";

				valitsesektori->nimet = new std::vector<std::string>;
				valitsesektori->luvut = new std::vector<int>;
				valitsesektori->minimit = new std::vector<int>;
				valitsesektori->maksimit = new std::vector<int>;

				valitsesektori->nimet->push_back("Sektori");
				valitsesektori->luvut->push_back(sektori);
				valitsesektori->minimit->push_back(0);
				valitsesektori->maksimit->push_back(sektorit.size()-1);

				ikkuna = new Ikkuna;
				ikkuna->LuoIkkuna(YLEISIKKUNA+3, valitsesektori, glob_data);
			}
			if(input->NAPPI_PAINETTU[SDLK_F11] && sektorit.size() > 1)
			{
				Yleisdata *tuhoasektori = new Yleisdata;
				tuhoasektori->nimi = new std::string;
				*(tuhoasektori->nimi) = "Tuhoa sektori?";

				tuhoasektori->nimet = new std::vector<std::string>;
				tuhoasektori->luvut = new std::vector<int>;
				tuhoasektori->minimit = new std::vector<int>;
				tuhoasektori->maksimit = new std::vector<int>;

				ikkuna = new Ikkuna;
				ikkuna->LuoIkkuna(YLEISIKKUNA+4, tuhoasektori, glob_data);
			}
			if(input->NAPPI_PAINETTU[SDLK_HOME])
			{
				Yleisdata *sektorisaato = new Yleisdata;
				sektorisaato->nimi = new std::string;
				(*(sektorisaato->nimi)) = "Sektorin ominaisuudet";
				sektorisaato->nimet = new std::vector<std::string>;
				sektorisaato->luvut = new std::vector<int>;
				sektorisaato->minimit = new std::vector<int>;
				sektorisaato->maksimit = new std::vector<int>;

				sektorisaato->nimet->push_back("Tileset");
				sektorisaato->nimet->push_back("Musiikki");

				

				sektorisaato->minimit->push_back(0);
				sektorisaato->minimit->push_back(0);
				sektorisaato->maksimit->push_back(glob_data->tilesetit.size()-1);
				sektorisaato->maksimit->push_back(10);
				sektorisaato->luvut->push_back(temp->AnnaTileset());
				sektorisaato->luvut->push_back(temp->AnnaMusiikki());

				ikkuna = new Ikkuna;
				ikkuna->LuoIkkuna(YLEISIKKUNA+5, sektorisaato , glob_data);				
			}

			if(input->NAPPI_PAINETTU[SDLK_F1])
			{
				ikkuna = new Ikkuna;
				char *rivi = new char[20];
				rivi[0] = '\0';
				ikkuna->LuoIkkuna(KOMENTORIVI, rivi, glob_data);
			}

			if(input->NAPPI_PAINETTU[SDLK_q] && temp->AnnaKerros() == 0)
			{
				alku_x = x_tile;
				alku_y = y_tile;
				alku_sektori = sektori;
			}

			char *luku;

			Tulosta("x_tilet:", 200, 20, glob_data);
			number_to_string(((temp->AnnaLayerit()))[temp->AnnaKerros()].annamappi()->AnnaX(), &luku);
			Tulosta(luku, 280, 20, glob_data);
			delete luku;

			Tulosta("y_tilet:", 200, 40, glob_data);
			number_to_string(((temp->AnnaLayerit()))[temp->AnnaKerros()].annamappi()->AnnaY(), &luku);
			Tulosta(luku, 280, 40, glob_data);
			delete luku;

			Tulosta("sektorit:", 340, 20, glob_data);
			number_to_string(sektorit.size(), &luku);
			Tulosta(luku, 400, 20, glob_data);
			delete luku;

			Tulosta("sektori:", 440, 20, glob_data);
			number_to_string(sektori, &luku);
			Tulosta(luku, 500, 20, glob_data);
			delete luku;
			


			Tulosta("x_koord:", 20, 20, glob_data);
			number_to_string(x_tile, &luku);
			Tulosta(luku, 100, 20, glob_data);
			delete luku;

			

			Tulosta("y_koord:", 20, 40, glob_data);
			number_to_string(y_tile, &luku);
			Tulosta(luku, 100, 40, glob_data);
			delete luku;

			Tulosta("layer:", 340, 40, glob_data);
			number_to_string(temp->AnnaKerros(), &luku);
			Tulosta(luku, 380, 40, glob_data);
			delete luku;
			
			
		}
		
		break;

	case tilevalinta:
		Valinta(glob_data);

		if(input->NAPPI_PAINETTU[SDLK_SPACE])
		{
			kopiointi = 0;
			moodi = mapinmuokkaus;
			AlustaValintamappi(glob_data);
		}

		break;
	default:
		break;
	}

	glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, hiiri_x, hiiri_y, &(laatikot[hiiri_kuva]), 1, 0);

	return 0;
}