#include "maaritykset.h"

bool Ikkuna::Tuhoa()
{
	return true;
}

bool Ikkuna::LuoIkkuna(int t, void *d, GLOBAALI *glob_data)
{
	
	tyyppi = t;
	data = d;

	return true;
}

bool Ikkuna::Piirra(GLOBAALI *glob_data, Editor *editori)
{

	for(int aksa = 2; aksa < 12; aksa++)
	{
		for(int yy = 2; yy < 12; yy++)
		{
			glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, aksa*32, yy*32,
				&laatikot[ikkuna_kuva], 0,224);
		}
	}

	

	if(tyyppi >= YLEISIKKUNA && tyyppi <= YLEISIKKUNA + 5)
	{
		Yleisdata *yleinen = (Yleisdata*)data;
		Tulosta((char*)yleinen->nimi->c_str(), 100, 100, glob_data);

		for(unsigned int index = 0; index < yleinen->nimet->size(); index++)
		{
			Tulosta((char *)yleinen->nimet->at(index).c_str(), 100, 120+20*index, glob_data);
			char *luku;
			number_to_string(yleinen->luvut->at(index),&luku);


			Tulosta(luku, 300, 120+20*index, glob_data);

			delete luku;

			glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,
					250, 120+index*20, &laatikot[vnuoli_kuva], 1, 0);
				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,
					270, 120+index*20, &laatikot[onuoli_kuva], 1, 0);
		}

		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 300, &laatikot[ok_kuva], 0, 0);
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 200, 300, &laatikot[no_kuva], 0, 0);

		
	}
	else if(tyyppi == MUOKKAALISTAA)
	{
		Listamuokkaus *muokkausdata = (Listamuokkaus *)data;
		int alkioita = muokkausdata->lista_os->size();
		char *luku;
		number_to_string(alkioita, &luku);
		Tulosta(luku, Tulosta("Alkioita: ", 100, 100, glob_data), 100, glob_data);
		delete luku;
		number_to_string(skrollaus2, &luku);
		Tulosta(luku, Tulosta("Alkio: ", 100, 120, glob_data), 120, glob_data);
		delete luku;

		Listamaaritys *maaritys = muokkausdata->maaritys;
		
		if(alkioita)
		for(unsigned int index = 0; index < maaritys->attribuutit.size(); index++)
		{
			Tulosta((char*)maaritys->attribuutit.at(index)->nimi.c_str(),100, 140+20*index, glob_data);
			Editorilinkki *os;
			int linkkisektori = 0;
			Editoriobjekti *linkki;
			switch(maaritys->attribuutit.at(index)->tyyppi)
			{
			case attrib_LUKU:
				number_to_string(*((int*)(muokkausdata->lista_os->at(skrollaus2)->at(index))),&luku);
				Tulosta(luku, 250, 140+20*index, glob_data);
				delete luku;

				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 300,140+20*index,&laatikot[vnuoli_kuva],1,0);
				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 320,140+20*index,&laatikot[onuoli_kuva],1,0);
				break;

			case attrib_LISTA:
				number_to_string(

					((std::vector <std::vector <void*>*> *)
					(muokkausdata->lista_os->at(skrollaus2)->at(index)))->size(),
					
					&luku);
				Tulosta(luku, 250, 140+20*index, glob_data);
				delete luku;
				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 300,140+20*index,&laatikot[muokkaa_kuva],1,0);
				break;

			case attrib_LINKKI:
				os = (Editorilinkki*)(muokkausdata->lista_os->at(skrollaus2)->at(index));

				if(os)
				{
					Tulosta("Linkitetty", 250, 100+index*20, glob_data);
				}

				linkki = editori->annaLinkki();
				linkkisektori = editori->annaLinkkisektori();

				if(linkki)
				{
					int tyyppi = linkki->annaTyyppi();
					
					bool loytyi = 0;
					
					Linkkimaaritys *maar = (Linkkimaaritys*)(maaritys->attribuutit.at(index)->maarite);
					
					for(int tyyppi_index = 0; tyyppi_index < maar->tyyppinumerot.size(); tyyppi_index++)
					{
						if(tyyppi == maar->tyyppinumerot.at(tyyppi_index)) loytyi = 1;
					}

					if(loytyi)
					{
						glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,
						320, 100+index*20, &laatikot[linkita_kuva], 1, 0);
					}
				}

				break;
			default: break;
			}
		}

		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,100,300,&laatikot[uusi_kuva],0,0);
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,100,320,&laatikot[poista_kuva],0,0);
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,150,300,&laatikot[ok_kuva],0,0);
	}
	else if(tyyppi == LUOOBJEKTI || tyyppi == MUOKKAAOBJEKTIA)
	{
		Editoriobjekti *objekti = (Editoriobjekti*)data;
		std::vector <void *> *objektidata = objekti->AnnaData();

		for(int index = 0; index < (int)objektidata->size(); index++)
		{
			std::vector <Attribuutti *> *attribuutit =
				&(glob_data->objektityypit.at(objekti->annaTyyppi())->attribuutit);

			const char *nimi = attribuutit->at(index)->nimi.c_str();
			Tulosta((char*)nimi, 100, 100+index*20, glob_data);

			if(attribuutit->at(index)->tyyppi == attrib_LUKU)
			{
				int arvo = *((int *)objektidata->at(index));
				char *a;
				number_to_string(arvo, &a);
				Tulosta(a, 300, 100+index*20, glob_data);
				delete a;

				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,
					250, 100+index*20, &laatikot[vnuoli_kuva], 1, 0);
				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,
					270, 100+index*20, &laatikot[onuoli_kuva], 1, 0);
			}
			else if(attribuutit->at(index)->tyyppi == attrib_LISTA)
			{
				int alkiot = ((std::vector < std::vector <void *> * > *) objektidata->at(index))->size();
				char *mjono;
				number_to_string(alkiot, &mjono);
				Tulosta(mjono, 300, 100+index*20, glob_data);
				delete mjono;
				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 250, 100+index*20, &laatikot[muokkaa_kuva], 0, 0);
			}
			else if(attribuutit->at(index)->tyyppi == attrib_LINKKI)
			{
				Editorilinkki *os = (Editorilinkki*)objektidata->at(index);

				if(os)
				{
					Tulosta("Linkitetty", 250, 100+index*20, glob_data);
				}

				Editoriobjekti *linkki = editori->annaLinkki();
				int linkkisektori = editori->annaLinkkisektori();

				if(linkki)
				{
					int tyyppi = linkki->annaTyyppi();
					
					bool loytyi = 0;
					
					Linkkimaaritys *maaritys = (Linkkimaaritys*)(attribuutit->at(index)->maarite);
					
					for(int tyyppi_index = 0; tyyppi_index < maaritys->tyyppinumerot.size(); tyyppi_index++)
					{
						if(tyyppi == maaritys->tyyppinumerot.at(tyyppi_index)) loytyi = 1;
					}

					if(loytyi)
					{
						glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,
						320, 100+index*20, &laatikot[linkita_kuva], 1, 0);
					}
				}
			}
		}

		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,100,200,&laatikot[ok_kuva],0,0);
		if(tyyppi == LUOOBJEKTI) glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,180,200,&laatikot[no_kuva],0,0);
	}
	else if(tyyppi == LUOUUSIVALIKKO)
	{
		int tyyppimaara = glob_data->editoriobjektit;
		int skrollausvara = 0;
		int rivit = tyyppimaara/5;
		if(tyyppimaara%5 > 0) rivit++;
		if(rivit > 3) skrollausvara = rivit -3;

		int alku = skrollaus*5;
		int indeksi = 0;

		while(1)
		{
			if(indeksi == 3*5) break;
			if(indeksi+alku == tyyppimaara) break;
			Sprite *kuvake = glob_data->objektityypit.at(indeksi+alku)->kuvake;
			Rect laatikko;
			laatikko.height = 32;
			laatikko.width = 32;
			laatikko.x = glob_data->objektityypit.at(indeksi+alku)->x_koord;
			laatikko.y = glob_data->objektityypit.at(indeksi+alku)->y_koord;
			glob_data->gfx->PiirraSprite(kuvake, 200 + 40 * (indeksi%5), 200 + 40 * (indeksi/5), &laatikko,0,0);
			indeksi++;
		}

		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 100, &laatikot[no_kuva],0, 0);
	}
	else if(tyyppi == MUOKKAUSVALIKKO || tyyppi == POISTOVALIKKO || tyyppi == LINKINOTTOVALIKKO)
	{
		std::vector <Editoriobjekti *> *objektit = (std::vector <Editoriobjekti *> *)(data);
		int skrollausvara = 0;
		int rivit = objektit->size()/5;
		if(objektit->size()%5 > 0) rivit++;
		if(rivit > 3) skrollausvara = rivit -3;

		int alku = skrollaus*5;
		int indeksi = 0;

		while(1)
		{
			if(indeksi == 3*5) break;
			if(indeksi+alku == objektit->size()) break;
			int tyyppi = objektit->at(indeksi+alku)->annaTyyppi();
			Sprite *kuvake = glob_data->objektityypit.at(tyyppi)->kuvake;
			Rect laatikko;
			laatikko.height = 32;
			laatikko.width = 32;
			laatikko.x = glob_data->objektityypit.at(tyyppi)->x_koord;
			laatikko.y = glob_data->objektityypit.at(tyyppi)->y_koord;
			glob_data->gfx->PiirraSprite(kuvake, 200 + 40 * (indeksi%5), 200 + 40 * (indeksi/5), &laatikko,0,0);
			indeksi++;
		}

		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 100, &laatikot[no_kuva],0, 0);
	}
	else if(tyyppi == LUOVAIMUOKKAA)
	{
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 100, &laatikot[uusi_kuva], 0, 0);
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 120, &laatikot[muokkaa_kuva], 0, 0);
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 140, &laatikot[poista_kuva], 0, 0);
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 160, &laatikot[otalinkki_kuva], 0, 0);
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 180, &laatikot[no_kuva], 0, 0);
	}
	else if(tyyppi == KOMENTORIVI)
	{
		Tulosta((char*)data, Tulosta("Komento:", 100, 100, glob_data), 100, glob_data);
	}

	return true;
}

int Ikkuna::Toiminta(GLOBAALI *glob_data, int hiiri_x, int hiiri_y, Editor *editori)
{
	InputData *input = glob_data->gfx->annaInput();

	Piirra(glob_data,editori);

	if(pointer)
	{
		int paluu = pointer->Toiminta(glob_data, hiiri_x, hiiri_y,editori);
		
		if(paluu == ikkuna_LOPPU)
		{
			pointer->Tuhoa();
			delete pointer;
			pointer = 0;
		}
		else if(paluu == ikkuna_LOPPU_SULJEITSESI)
		{
			pointer->Tuhoa();
			delete pointer;
			pointer = 0;
			return ikkuna_LOPPU_SULJEITSESI;
		}

	}
	else if(tyyppi >= YLEISIKKUNA && tyyppi <= YLEISIKKUNA + 5)
	{
		Yleisdata *yleinen = (Yleisdata*)data;
		
		for(int index = 0; index < (int)yleinen->nimet->size(); index++)
		{
			if(input->HIIRINAPPI_PAINETTU[VASEN])
			{
				if(hiiri_y >= 120+index*20 && hiiri_y < 120+index*20+10)
				{
					int luku = 1;

					if(input->NAPPI_POHJASSA[SDLK_LCTRL]) luku *= 10;
					if(input->NAPPI_POHJASSA[SDLK_LALT]) luku *= 10;

					if(hiiri_x >= 250 && hiiri_x < 260) yleinen->luvut->at(index)-= luku;
					if(hiiri_x >= 270 && hiiri_x < 280) yleinen->luvut->at(index)+= luku;

					if(yleinen->luvut->at(index) < yleinen->minimit->at(index))
						yleinen->luvut->at(index) = yleinen->minimit->at(index);

					if(yleinen->luvut->at(index) > yleinen->maksimit->at(index))
						yleinen->luvut->at(index) = yleinen->maksimit->at(index);
				}
			}
		}

		if(input->HIIRINAPPI_PAINETTU[VASEN])
		{
			if(hiiri_y >= 300 && hiiri_y < 340)
			{
				if(hiiri_x >= 100 && hiiri_x <= 140)
				{
					Kasky *kasky;
					kasky = new Kasky;

					if(tyyppi == YLEISIKKUNA) kasky->tyyppi = kasky_muuta_mappia;
					if(tyyppi == YLEISIKKUNA+1) kasky->tyyppi = kasky_siirra_mappia;
					if(tyyppi == YLEISIKKUNA+2) kasky->tyyppi = kasky_uusi_sektori;
					if(tyyppi == YLEISIKKUNA+3) kasky->tyyppi = kasky_valitse_sektori;
					if(tyyppi == YLEISIKKUNA+4) kasky->tyyppi = kasky_tuhoa_sektori;
					if(tyyppi == YLEISIKKUNA+5) kasky->tyyppi = kasky_sektorisaato;
					

					kasky->data = yleinen->luvut;
					glob_data->kaskyt.push_back(kasky);
					return ikkuna_LOPPU;
				}
				if(hiiri_x >= 200 && hiiri_x < 240)
				{
					return ikkuna_LOPPU;
				}
			}
		}
	}
	else if(tyyppi == MUOKKAALISTAA)
	{
		Listamuokkaus *muokkausdata = (Listamuokkaus *)data;
		int alkioita = muokkausdata->lista_os->size();
		
		Listamaaritys *maaritys = muokkausdata->maaritys;
		
		if(alkioita)
		for(int index = 0; index < (int)maaritys->attribuutit.size(); index++)
		{
			Editorilinkki *os;
			Editoriobjekti *linkki;
			int linkkisektori;

			switch(maaritys->attribuutit.at(index)->tyyppi)
			{
			case attrib_LUKU:
				if(input->HIIRINAPPI_PAINETTU[VASEN])
				{
					if(hiiri_y >= 140+index*20 && hiiri_y < 140+index*20+11)
					{
						int luku = *((int*)muokkausdata->lista_os->at(skrollaus2)->at(index));

						int maara = 1;

						if(input->NAPPI_POHJASSA[SDLK_LCTRL]) maara *= 10;
						if(input->NAPPI_POHJASSA[SDLK_LALT]) maara *= 10;

						if(hiiri_x >= 300 && hiiri_x <= 311) luku -= maara;
						else if(hiiri_x >= 320 && hiiri_x <= 331) luku += maara;

						if(luku < ((MinMaxAlku*)(maaritys->attribuutit.at(index)->maarite))->min)
							luku = ((MinMaxAlku*)(maaritys->attribuutit.at(index)->maarite))->min;

						if(luku > ((MinMaxAlku*)(maaritys->attribuutit.at(index)->maarite))->max)
							luku = ((MinMaxAlku*)(maaritys->attribuutit.at(index)->maarite))->max;

						*((int*)muokkausdata->lista_os->at(skrollaus2)->at(index)) = luku;

					}
					
				}
				break;
			case attrib_LISTA:
				if(input->HIIRINAPPI_PAINETTU[VASEN])
				{
					if(hiiri_y >= 140+index*20 && hiiri_y < 140+index*20+11)
					{
						if(hiiri_x >= 300 && hiiri_x < 347)
						{
							pointer = new Ikkuna;
							
							Listamuokkaus *muokkaus = new Listamuokkaus;
							muokkaus->lista_os = (std::vector<std::vector<void*>*>*)muokkausdata->lista_os->at(skrollaus2)->at(index);
							muokkaus->maaritys = (Listamaaritys*)(muokkausdata->maaritys->attribuutit.at(index)->maarite);

							pointer->LuoIkkuna(MUOKKAALISTAA,muokkaus,glob_data); 
						}
					}
				}
				break;

			case attrib_LINKKI:
				os = (Editorilinkki*)(muokkausdata->lista_os->at(skrollaus2)->at(index));

				
				linkki = editori->annaLinkki();
				linkkisektori = editori->annaLinkkisektori();

				if(linkki)
				{
					int tyyppi = linkki->annaTyyppi();
					
					bool loytyi = 0;
					
					Linkkimaaritys *maar = (Linkkimaaritys*)(maaritys->attribuutit.at(index)->maarite);
					
					for(int tyyppi_index = 0; tyyppi_index < maar->tyyppinumerot.size(); tyyppi_index++)
					{
						if(tyyppi == maar->tyyppinumerot.at(tyyppi_index)) loytyi = 1;
					}

					if(loytyi)
					{
						if(input->HIIRINAPPI_PAINETTU[VASEN])
						{
							if(hiiri_x >= 320 && hiiri_x < 367)
							{
								if(hiiri_y >= 100+index*20 && hiiri_y < 100+index*20 +10)
								{
									Editorilinkki *uusi = new Editorilinkki;
									uusi->kohdesektori = linkkisektori;
									uusi->mihin = linkki;

									uusi->mista = muokkausdata->lista_os->at(skrollaus2);
									uusi->vektori_indeksi = index;

									muokkausdata->lista_os->at(skrollaus2)->at(index) = uusi;

									linkki->LisaaLinkki(uusi);
								}
							}
						}
					}
				}
				break;
			default: break;
			}
		}

		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,100,300,&laatikot[uusi_kuva],0,0);
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,100,320,&laatikot[poista_kuva],0,0);

		if(input->HIIRINAPPI_PAINETTU[VASEN])
		{
			if(hiiri_x >= 100 && hiiri_x < 147)
			{
				if(hiiri_y >= 300 && hiiri_y < 310)
				{
					UusiAlkio(muokkausdata->lista_os, muokkausdata->maaritys, skrollaus2);
				}
				if(hiiri_y >= 320 && hiiri_y < 330)
				{
					if(alkioita)
					{
						TuhoaAlkio(muokkausdata->lista_os, muokkausdata->maaritys, skrollaus2);
						alkioita = muokkausdata->lista_os->size();
					}
				}
			}

			if(hiiri_x >= 150 && hiiri_x < 190 && hiiri_y >= 300 && hiiri_y < 340)
				return ikkuna_LOPPU;
			
		}

		if(input->NAPPI_PAINETTU[SDLK_LEFT]) skrollaus2--;
		if(input->NAPPI_PAINETTU[SDLK_RIGHT]) skrollaus2++;
		if(skrollaus2 < 0) skrollaus2 = 0;
		if(alkioita > 0)
		{
			if(skrollaus2 > alkioita-1) skrollaus2 = alkioita-1;
		}
		else if(skrollaus2 > 0) skrollaus2 = 0;
		
	}
	else if(tyyppi == LUOOBJEKTI || tyyppi == MUOKKAAOBJEKTIA)
	{
		Editoriobjekti *objekti = (Editoriobjekti*)data;
		std::vector <void *> *objektidata = objekti->AnnaData();
		
		const char *tyyppinimi = glob_data->objektityypit.at(objekti->annaTyyppi())->nimi.c_str();
		Tulosta((char*)tyyppinimi, 100, 80, glob_data);


		for(int index = 0; index < (int)objektidata->size(); index++)
		{
			std::vector <Attribuutti *> *attribuutit =
				&(glob_data->objektityypit.at(objekti->annaTyyppi())->attribuutit);

			if(attribuutit->at(index)->tyyppi == attrib_LUKU)
			{
				if(input->HIIRINAPPI_PAINETTU[VASEN])
				{
					if(hiiri_y >= 100+index*20 && hiiri_y < 100+index*20+11)
					{
						int arvo = *((int *)objektidata->at(index));

						int maara = 1;

						if(input->NAPPI_POHJASSA[SDLK_LCTRL]) maara *= 10;
						if(input->NAPPI_POHJASSA[SDLK_LALT]) maara *= 10;

						if(hiiri_x >= 250 && hiiri_x < 261) arvo -= maara;
						if(hiiri_x >= 270 && hiiri_x < 281) arvo += maara;

						if(arvo < ((MinMaxAlku*)attribuutit->at(index)->maarite)->min)
							arvo = ((MinMaxAlku*)attribuutit->at(index)->maarite)->min;

						if(arvo > ((MinMaxAlku*)attribuutit->at(index)->maarite)->max)
							arvo = ((MinMaxAlku*)attribuutit->at(index)->maarite)->max;

						*((int *)objektidata->at(index)) = arvo;
					}
				}
			}
			else if(attribuutit->at(index)->tyyppi == attrib_LISTA)
			{
				if(input->HIIRINAPPI_PAINETTU[VASEN])
				{
					if(hiiri_x >= 250 && hiiri_x < 297)
					{
						if(hiiri_y >= 100+index*20 && hiiri_y < 100+index*20 +10)
						{
							pointer = new Ikkuna;
							Listamuokkaus *muokkausdata = new Listamuokkaus;
							
							
							muokkausdata->lista_os = (std::vector < std::vector < void *> * > *) objektidata->at(index);
							muokkausdata->maaritys = (Listamaaritys*)attribuutit->at(index)->maarite;


							pointer->LuoIkkuna(MUOKKAALISTAA,muokkausdata,glob_data);
						}
					}
				}
			}
			else if(attribuutit->at(index)->tyyppi == attrib_LINKKI)
			{
				Editorilinkki *os = (Editorilinkki*)objektidata->at(index);

				
				Editoriobjekti *linkki = editori->annaLinkki();
				int linkkisektori = editori->annaLinkkisektori();

				if(linkki)
				{
					int tyyppi = linkki->annaTyyppi();
					
					bool loytyi = 0;
					
					Linkkimaaritys *maaritys = (Linkkimaaritys*)(attribuutit->at(index)->maarite);
					
					for(int tyyppi_index = 0; tyyppi_index < maaritys->tyyppinumerot.size(); tyyppi_index++)
					{
						if(tyyppi == maaritys->tyyppinumerot.at(tyyppi_index)) loytyi = 1;
					}

					if(loytyi)
					{
						if(input->HIIRINAPPI_PAINETTU[VASEN])
						{
							if(hiiri_x >= 320 && hiiri_x < 367)
							{
								if(hiiri_y >= 100+index*20 && hiiri_y < 100+index*20 +10)
								{
									Editorilinkki *uusi = new Editorilinkki;
									uusi->kohdesektori = linkkisektori;
									uusi->mihin = linkki;

									uusi->mista = objektidata;
									uusi->vektori_indeksi = index;

									objektidata->at(index) = uusi;

									linkki->LisaaLinkki(uusi);
								}
							}
						}
					}
				}
			}
		}

		
		if(input->HIIRINAPPI_PAINETTU[VASEN])
		{
			if(hiiri_x >= 100 && hiiri_x < 140 && hiiri_y >= 200 && hiiri_y < 240)
			{
				if(tyyppi == LUOOBJEKTI)
				{
					Kasky *lisays = new Kasky;
					lisays->tyyppi = kasky_lisaa_objekti;
					lisays->data = data;
					glob_data->kaskyt.push_back(lisays);
					return ikkuna_LOPPU_SULJEITSESI;
				} else return ikkuna_LOPPU;
			}

			if(tyyppi == LUOOBJEKTI)
			{
				if(hiiri_x >= 180 && hiiri_x < 220 && hiiri_y >= 200 && hiiri_y < 240)
				{
					((Editoriobjekti *)data)->TuhoaObjekti(glob_data);
					delete ((Editoriobjekti *)data);
					data = 0;
					return ikkuna_LOPPU;
				}
			}
		}
				
	}
	else if(tyyppi == LUOUUSIVALIKKO)
	{
		int tyyppimaara = glob_data->editoriobjektit;
		int skrollausvara = 0;
		int rivit = tyyppimaara/5;
		if(tyyppimaara%5 > 0) rivit++;
		if(rivit > 3) skrollausvara = rivit -3;

		int alku = skrollaus*5;
		int indeksi = 0;

		while(1)
		{
			if(indeksi == 3*5) break;
			if(indeksi+alku == tyyppimaara) break;
			Sprite *kuvake = glob_data->objektityypit.at(indeksi+alku)->kuvake;
			Rect laatikko;
			laatikko.height = 32;
			laatikko.width = 32;
			laatikko.x = glob_data->objektityypit.at(indeksi+alku)->x_koord;
			laatikko.y = glob_data->objektityypit.at(indeksi+alku)->y_koord;
			glob_data->gfx->PiirraSprite(kuvake, 200 + 40 * (indeksi%5), 200 + 40 * (indeksi/5), &laatikko,0,0);
			indeksi++;
		}

		indeksi = 0;

		
		while(1)
		{
			if(indeksi == 3*5) break;
			if(indeksi+alku == tyyppimaara) break;
			
			int x = 200 + 40 * (indeksi%5);
			int y =  200 + 40 * (indeksi/5);

			if(input->HIIRINAPPI_PAINETTU[VASEN])
			{
				if(hiiri_x >= x && hiiri_y >= y &&
					hiiri_x < x + 32 && hiiri_y < y + 32)
				{
					pointer = new Ikkuna;
					Editoriobjekti *objekti = new Editoriobjekti;

					int x = ((axYy*)data)->x_tile;
					int y = ((axYy*)data)->y_tile;
					objekti->LuoObjekti(indeksi+alku,x,y,glob_data);
					pointer->LuoIkkuna(LUOOBJEKTI, objekti, glob_data);
				}
			}
			
			indeksi++;
		}

		if(input->HIIRINAPPI_PAINETTU[VASEN])
		{
			if(hiiri_x >= 100 && hiiri_x < 140 && hiiri_y >= 100 && hiiri_y < 140)
			{
				return ikkuna_LOPPU;
			}
		}

		if(input->NAPPI_PAINETTU[SDLK_UP]) skrollaus--;
		if(skrollaus < 0) skrollaus = 0;
		if(input->NAPPI_PAINETTU[SDLK_DOWN]) skrollaus++;
		if(skrollaus > skrollausvara) skrollaus = skrollausvara;

	}
	else if(tyyppi == MUOKKAUSVALIKKO || tyyppi == POISTOVALIKKO || tyyppi == LINKINOTTOVALIKKO)
	{
		int skrollausvara = 0;
		std::vector <Editoriobjekti *> *objektit = (std::vector <Editoriobjekti *> *)data;

		int rivit = objektit->size()/5;
		if(objektit->size()%5 > 0) rivit++;
		if(rivit > 3) skrollausvara = rivit -3;

		int alku = skrollaus*5;
		int indeksi = 0;		
		
		while(1)
		{
			if(indeksi == 3*5) break;
			if(indeksi+alku == objektit->size()) break;
			
			int x = 200 + 40 * (indeksi%5);
			int y =  200 + 40 * (indeksi/5);

			if(input->HIIRINAPPI_PAINETTU[VASEN])
			{
				if(hiiri_x >= x && hiiri_y >= y &&
					hiiri_x < x + 32 && hiiri_y < y + 32)
				{
					if(tyyppi == MUOKKAUSVALIKKO)
					{
						pointer = new Ikkuna;
						
						pointer->LuoIkkuna(MUOKKAAOBJEKTIA, objektit->at(indeksi+alku), glob_data);
					}
					else if(tyyppi == LINKINOTTOVALIKKO)
					{
						Kasky *linkkikasky = new Kasky;
						linkkikasky->tyyppi = kasky_ota_linkki;
						linkkikasky->data = objektit->at(indeksi+alku);
						glob_data->kaskyt.push_back(linkkikasky);
						return ikkuna_LOPPU_SULJEITSESI;
					}
					else
					{
						Kasky *tuhouskasky = new Kasky;
						tuhouskasky->tyyppi = kasky_tuhoa_objekti;
						Editoriobjekti *obj = objektit->at(indeksi+alku);
						tuhouskasky->data = obj;
						glob_data->kaskyt.push_back(tuhouskasky);
						if(editori->annaLinkki() == obj)
							editori->poistaLinkki();
						return ikkuna_LOPPU_SULJEITSESI;
					}
				}
			}
			
			indeksi++;
		}

		if(input->HIIRINAPPI_PAINETTU[VASEN])
		{
			if(hiiri_x >= 100 && hiiri_x < 140 && hiiri_y >= 100 && hiiri_y < 140)
			{
				return ikkuna_LOPPU;
			}
		}

		if(input->NAPPI_PAINETTU[SDLK_UP]) skrollaus--;
		if(skrollaus < 0) skrollaus = 0;
		if(input->NAPPI_PAINETTU[SDLK_DOWN]) skrollaus++;
		if(skrollaus > skrollausvara) skrollaus = skrollausvara;

	}
	else if(tyyppi == LUOVAIMUOKKAA)
	{
		if(input->HIIRINAPPI_PAINETTU[VASEN])
		{
			if(hiiri_x >= 100)
			{
				if(hiiri_x < 147)
				{
					if(hiiri_y >= 100 && hiiri_y < 110)
					{
						pointer = new Ikkuna;
						pointer->LuoIkkuna(LUOUUSIVALIKKO,((Objektikuorma*)data)->koord,glob_data);
					}
					if(hiiri_y >= 120 && hiiri_y < 130)
					{
						pointer = new Ikkuna;
						pointer->LuoIkkuna(MUOKKAUSVALIKKO,((Objektikuorma*)data)->objektit,glob_data);
					}
					if(hiiri_y >= 140 && hiiri_y < 150)
					{
						pointer = new Ikkuna;
						pointer->LuoIkkuna(POISTOVALIKKO,((Objektikuorma*)data)->objektit,glob_data);
					}
					if(hiiri_y >= 160 && hiiri_y < 170)
					{
						pointer = new Ikkuna;
						pointer->LuoIkkuna(LINKINOTTOVALIKKO,((Objektikuorma*)data)->objektit,glob_data);
					}
				}

				if(hiiri_x < 140)
				{
					if(hiiri_y >= 180 && hiiri_y < 220)
					{
						((Objektikuorma *)data)->objektit->clear();
						delete ((Objektikuorma *)data)->objektit;
						delete ((Objektikuorma *)data)->koord;
						delete (Objektikuorma *)data;
						return ikkuna_LOPPU;
					}
				}
			}
			glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 100, &laatikot[uusi_kuva], 0, 0);
			glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 120, &laatikot[muokkaa_kuva], 0, 0);
			glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 100, 140, &laatikot[no_kuva], 0, 0);
		}
	}
	else if(tyyppi == KOMENTORIVI)
	{
		int koko = 0;
		char *komentoteksti = (char*)data;

		for(int index = 0; index < 20; index++)
		{
			if(komentoteksti[index] == '\0') break;
			koko++;
		}
		
		if(input->NAPPI_PAINETTU[SDLK_RETURN])
		{
			Kasky *komento = new Kasky;
			komento->tyyppi = kasky_komento;
			komento->data = komentoteksti;
			glob_data->kaskyt.push_back(komento);
			return ikkuna_LOPPU;
		}
		else if(input->NAPPI_PAINETTU[SDLK_F1])
		{
			delete (char*)data;
			return ikkuna_LOPPU;
		}
		
		if(input->NAPPI_PAINETTU[SDLK_BACKSPACE] && koko > 0) komentoteksti[koko-1] = '\0';
		else for(char merkki = SDLK_SPACE; merkki <= SDLK_z; merkki++)
		{
			if(input->NAPPI_PAINETTU[merkki] && koko < 19)
			{
				komentoteksti[koko] = merkki;
				koko++;
				komentoteksti[koko] = '\0';
	
				break;
			}
		}
	}

	

	return ikkuna_JATKA;
}