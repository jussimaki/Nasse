#include "maaritykset.h"
extern int varasto;

bool Peli::uusiPeli(GLOBAALI *glob_data, char *save)
{
	elamat = ELAMAT;
	kolikot = 0;

	pelitila = tila_KARTTA;
	if(binaarimuoto) kartta.Lataa_binaari("data/kartta.dat", glob_data);
	else kartta.Lataa("data/kartta.txt", glob_data);

	kartta.UusiPeli(save);
	kartta.TallennaPeli();
	kartta.kaynnista(glob_data);
	return true;
}

bool Peli::lataaPeli(GLOBAALI *glob_data, char *save)
{
	elamat = ELAMAT;
	kolikot = 0;

	pelitila = tila_KARTTA;
	if(binaarimuoto) kartta.Lataa_binaari("data/kartta.dat", glob_data);
	else kartta.Lataa("data/kartta.txt", glob_data);

	kartta.LataaPeli(save);
	kartta.kaynnista(glob_data);
	return true;
}

bool Peli::alusta(GLOBAALI *glob_data)
{
	return true;
}

void Peli::tulostaTiedot(GLOBAALI *glob_data)
{
	char *luku;
	Tulosta("Coins: ", 0, 0, glob_data);
	number_to_string(kolikot, &luku);
	Tulosta(luku, 60, 0, glob_data);
	delete[] luku;
	Tulosta("Lives: ", 0, 20, glob_data);
	number_to_string(elamat, &luku);
	Tulosta(luku, 60, 20, glob_data);
	delete[] luku;
}

void Peli::continueruutu(GLOBAALI *glob_data)
{
	InputData *input = glob_data->gfx->annaInput();

	if(input->NAPPI_PAINETTU[SDLK_UP]) valikkonuoli--;
	if(input->NAPPI_PAINETTU[SDLK_DOWN]) valikkonuoli++;

	if(valikkonuoli < 0) valikkonuoli = 0;
	if(valikkonuoli > 1) valikkonuoli = 1;

	if(input->NAPPI_PAINETTU[SDLK_RETURN])
	{
		if(valikkonuoli == 0)
		{
			pelitila = tila_TASO;
			taso.AlustaObjektit(1);
			taso.Kaynnista(glob_data);
		}
		else
		{
			pelitila = tila_KARTTA;
			taso.Vapauta(glob_data);
			kartta.kaynnista(glob_data);
		}
	}
	

	glob_data->gfx->TyhjennaRuutu();

	Tulosta("Continue?", 200,200, glob_data);
	Tulosta("Yes",200, 220, glob_data);
	Tulosta("No",200, 240, glob_data);

	glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,180,220+20*valikkonuoli,&laatikot[onuoli_kuva],1,255);
}

int Peli::gameoverruutu(GLOBAALI *glob_data)
{
	glob_data->gfx->TyhjennaRuutu();

	glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,100,50,&laatikot[gameover_kuva],1,255);

	Tulosta("Continue?", 200,200, glob_data);
	Tulosta("Yes",200, 220, glob_data);
	Tulosta("No",200, 240, glob_data);

	glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,180,220+20*valikkonuoli,&laatikot[onuoli_kuva],1,255);


	InputData *input = glob_data->gfx->annaInput();

	if(input->NAPPI_PAINETTU[SDLK_UP]) valikkonuoli--;
	if(input->NAPPI_PAINETTU[SDLK_DOWN]) valikkonuoli++;

	if(valikkonuoli < 0) valikkonuoli = 0;
	if(valikkonuoli > 1) valikkonuoli = 1;

	if(input->NAPPI_PAINETTU[SDLK_RETURN])
	{
		if(valikkonuoli == 0)
		{
			pelitila = tila_KARTTA;
			kartta.kaynnista(glob_data);
			elamat = ELAMAT;
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

int Peli::lopputekstit(GLOBAALI *glob_data)
{
	glob_data->gfx->TyhjennaRuutu();

	InputData *input;
	char* tekstit[] =
	{
		"You've defeated Satan and saved the world!",
		"Congratulations and thanks for playing!"
	};
	
	for(int index = 0; index < 2; index++)
	{
		Tulosta(tekstit[index],200,laskuri+index*20,glob_data);
	}
	if(laskuri > 0) laskuri--;
	input = glob_data->gfx->annaInput();

	if(painettu(index_HYPPY,glob_data) ||
		painettu(index_JUOKSU,glob_data) ||
		input->NAPPI_PAINETTU[SDLK_RETURN]) return 1;

	return 0;
}

int Peli::logiikka(GLOBAALI *glob_data)
{
	int paluuarvo;

	char *nimi;



	switch(pelitila)
	{
	case tila_TASO:

		if(aloitus)
		{
			rullaus -= 4;
			if(rullaus < 0)
			{
				rullaus = 0;
				aloitus = 0;
			}

			glob_data->gfx->asetaR(rullaus);
		}

		paluuarvo = taso.Toiminta(glob_data, this);
		tulostaTiedot(glob_data);

		if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_p]) taso.pauseta();
		if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_ESCAPE]) taso.itsari(glob_data);

		if(paluuarvo >= 0) // taso läpi
		{
			taso.Sammuta(glob_data);
			taso.Vapauta(glob_data);

			if(kartta.AsetaPiirto(paluuarvo)) {pelitila = tila_LOPPUTEKSTIT; laskuri = 480;}
			else
			{
				pelitila = tila_KARTTA;
				kartta.kaynnista(glob_data);
			}
		}
		else if(paluuarvo == -1) // kuolema
		{
			if(elamat == 0)
			{
				pelitila = tila_GAMEOVER;
				valikkonuoli = 0;
				taso.Sammuta(glob_data);
				taso.Vapauta(glob_data);
			}
			else
			{
				pelitila = tila_CONTINUE;

				valikkonuoli = 0;
				taso.Sammuta(glob_data);
				
				elamat--;
			}
		}
		break;

	case tila_KARTTA:
		paluuarvo = kartta.Logiikka(glob_data, &nimi);
		kartta.Piirto(glob_data);
		tulostaTiedot(glob_data);

		if(paluuarvo == 1)
		{
			aloitus = 1;
			rullaus = 550;
			glob_data->gfx->asetaR(rullaus);
			pelitila = tila_TASO;
			taso.Lataa(nimi,glob_data);
			taso.AlustaObjektit(1);
			taso.Kaynnista(glob_data);

		}
		else if(paluuarvo == 2)
		{
			return 1;
		}

		break;

	case tila_CONTINUE:

		continueruutu(glob_data);
		
		break;

	case tila_GAMEOVER:

		if(gameoverruutu(glob_data))
			return 1;

		break;

	case tila_LOPPUTEKSTIT:

		return lopputekstit(glob_data);
		
		break;

	default:
		break;
	}

	return 0;
}

void Peli::tuhoa()
{
	kartta.Tuhoa();
}

void Peli::lisaaKolikko()
{
	kolikot++;

	if(kolikot >= 100)
	{
		kolikot = 0;
		elamat++;
	}
}

void Peli::lisaaElama()
{
	elamat++;
}