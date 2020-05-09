#include "maaritykset.h"
#define rullausaika 180
#define tekstiaika  215

int Valikko::logiikka(GLOBAALI *glob_data)
{
	taso.Toiminta(glob_data);
	Rect laatikko = laatikot[logo_kuva];
	int paluuarvo;

	switch(valikonTila)
	{
	case 0: // alku
		if(rullaus > 0)
		{
			rullaus -= 3;
			if(rullaus < 0) rullaus = 0;
			glob_data->gfx->asetaR(rullaus);	
		}

		if(laskuri > rullausaika) 
		{
			int kirkkaus = (laskuri - rullausaika)*2;
			if(kirkkaus > 255) kirkkaus = 255;

			int x_koord = -400 + (laskuri-rullausaika)*4;

			if(x_koord > 150) x_koord = 150;

			glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,x_koord,50,&laatikko,1,kirkkaus);
		}

		if(laskuri < rullausaika + tekstiaika)
		{
			laskuri++;
		}
		else  valikonTila = 1;

		return 0;

		break;

	case 1: // valikko päällä

		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,150,50,&laatikko,1,255);

		laatikko = laatikot[jussimaki_kuva];
		glob_data->gfx->PiirraSprite(glob_data->pikkukuvat,450,400,&laatikko,1,255);

		paluuarvo = valikot.logiikka(glob_data);

		if(paluuarvo == LOPETA)
		{
			valikonTila = 2;
			rullaus = 0;
			return 0;
		} else return paluuarvo;

		break;

	case 2: // loppurullaus;
		rullaus += 3;
		glob_data->gfx->asetaR(rullaus);

		if(rullaus >= 550)
			return LOPETA;

		break;


	default:
		break;
	}
	

	return 0;
}

bool Valikko::alusta(GLOBAALI *glob_data)
{
	
	
	valikonTila = 0;
	rullaus = 550;
	glob_data->gfx->asetaR(rullaus);
	laskuri = 0;

	taso.Lataa("tasot/demo", glob_data);
	taso.AlustaObjektit(1);
	taso.Kaynnista(glob_data);

	soitaGlobaaliBiisi(glob_data, &glob_data->valikkobiisi);
	valikot.alusta(0);
	return true;
}

void Valikko::vapauta(GLOBAALI *glob_data)
{
	taso.Sammuta(glob_data);
	taso.Vapauta(glob_data);

	valikot.tuhoa();
}