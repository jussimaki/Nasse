#ifdef WIN32
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#endif

#include "maaritykset.h"
#include "tiedot.h"




int main(int argc, char *argv[])
{
	int fps;
	if(!alustus(&glob_data)) return false;

	
	
	editor.alustus(&glob_data);	
	ohjelmatila = EDITORITILA;

	
	//valikko.alusta(&glob_data);
	//ohjelmatila = VALIKKOTILA;
	
	while(ohjelmatila != LOPPUI)
	{
		if(alku == 0) alku = glob_data.gfx->AnnaAika();

		
		glob_data.gfx->Input();
		InputData *input = glob_data.gfx->annaInput();

	
		int paluuarvo = 0;

		
		switch(ohjelmatila)
		{
		
		case VALIKKOTILA:
			paluuarvo = valikko.logiikka(&glob_data);

			if(paluuarvo != 0)
			{
				switch(paluuarvo)
				{
					case UUSIPELI:
						ohjelmatila = PELITILA;
						valikko.vapauta(&glob_data);
						peli.uusiPeli(&glob_data, savegame);
						break;

					case LATAAPELI:
						ohjelmatila = PELITILA;
						valikko.vapauta(&glob_data);
						peli.lataaPeli(&glob_data, savegame);
						break;

					case LOPETA:
						valikko.vapauta(&glob_data);
						ohjelmatila = LOPPUI;
						break;

					default: break;
				}
			}
			break;

		case PELITILA:
			paluuarvo = peli.logiikka(&glob_data);

			if(paluuarvo)
			{
				ohjelmatila = VALIKKOTILA;
				peli.tuhoa();
				valikko.alusta(&glob_data);
			}
			break;

		case EDITORITILA:
			paluuarvo = editor.Toiminta(&glob_data);
			
			if(paluuarvo)
			{
				ohjelmatila = LOPPUI;
				editor.Tuhoa(&glob_data);
			}
			break;

		default: break;
		}


		if(kehityksessa)
		{
			char *jono;
			number_to_string(fps, &jono);
			Tulosta(jono, 200, 400, &glob_data);
			delete[] jono;

					
			if(glob_data.gfx->annaInput()->NAPPI_POHJASSA[SDLK_n]) zooom--;
			if(glob_data.gfx->annaInput()->NAPPI_POHJASSA[SDLK_m]) zooom++;

			if(zooom < 0) zooom = 0;
			if(zooom > 319) zooom = 319;

			if(glob_data.gfx->annaInput()->NAPPI_POHJASSA[SDLK_0]) glob_data.gfx->asetaS(0);
			if(glob_data.gfx->annaInput()->NAPPI_POHJASSA[SDLK_1]) glob_data.gfx->asetaS(1);
			if(glob_data.gfx->annaInput()->NAPPI_POHJASSA[SDLK_2]) glob_data.gfx->asetaS(2);


			if(glob_data.gfx->annaInput()->NAPPI_POHJASSA[SDLK_LSHIFT])
			{
				if(glob_data.gfx->annaInput()->NAPPI_POHJASSA[SDLK_k]) glob_data.gfx->asetaR(glob_data.gfx->annaR()-6);
				if(glob_data.gfx->annaInput()->NAPPI_POHJASSA[SDLK_l]) glob_data.gfx->asetaR(glob_data.gfx->annaR()+6);


			}

		}

		int l = 640-zooom*2;
		int k = 480-(zooom*3/2);
		if(l <= 0) l = 1;
		if(k <= 0) k = 1;

		glob_data.gfx->zoom(zooom,zooom*3/4,l,k);


		
		glob_data.gfx->Skaalaus();


		int tiks = glob_data.gfx->AnnaAika();
		fps = tiks-alku;
				
		//do{tiks = glob_data.gfx->AnnaAika();} while(tiks-alku < 26);

		if(tiks-alku >= timestep)
		{
			//if(ohjelmatila == PELITILA)
				varasto += tiks-alku -timestep;
		}
		else
		{
			
			if(timestep -(tiks -alku) >= 2) glob_data.gfx->nuku(timestep -(tiks-alku)-1);

			do{tiks = glob_data.gfx->AnnaAika();} while(tiks-alku < timestep);
			
		}

		alku = glob_data.gfx->AnnaAika();
				
		glob_data.gfx->Flip();
		
		
	}

	lopetus(&glob_data);

	return 0;
}
