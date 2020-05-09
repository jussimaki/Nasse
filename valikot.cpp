#include "maaritykset.h"

extern char savegame[20];

bool Valikot::alusta(int nro)
{
	rivi[0] = '\0';
	ikkunanro = nro;
	uusi = 0;
	kursori_y = 0;
	valitus = 0;


	if(ikkunanro == 4)
	{
		scroll_y = 0;
		tiedostolista.clear();
		GetCurrentDirectory(256,hakemistonimi);
		sprintf(polku, "%s\\*", hakemistonimi);
		hFind = FindFirstFile(polku, &fd);
		FindNextFile(hFind, &fd);

		
		int maara = 0;
		
		while(FindNextFile(hFind, &fd))
		{
			
			char *ptr = fd.cFileName;
			int koko = 0;

			while(ptr[koko] != '\0')
			{
				koko++;
			}

			if(koko >= 5 && ptr[koko-4] == '.' && ptr[koko-3] == 's'
				&& ptr[koko-2] == 'a' && ptr[koko-1] == 'v')
			{
				std::string uusitiedosto;
				for(int index = 0; index < koko-4; index++)
				{
					uusitiedosto += ptr[index];
				}

				tiedostolista.push_back(uusitiedosto);
				maara++;
			}
		}
		if(maara == 0) valitus = 1;
		
	}

	if(ikkunanro == 5 || ikkunanro == 6) nappi_index = 0;
	return true;
}

void Valikot::tuhoa()
{
	if(uusi)
	{
		uusi->tuhoa();
		delete uusi;
		uusi = 0;
	}
}

int Valikot::logiikka(GLOBAALI *glob_data)
{
	char *nappi_jonot[] = {"Left", "Right", "Up", "Down", "Action", "Jump"};
	char *paavalikko_jonot[] = {"New game", "Load game", "Options", "Exit"};
	char *options_jonot[] = {"Controls", "Back"};
	char *controls_jonot[] = {"Configure keys", "Configure gamepad", "Back"};

	int paavalikko_ikkunat[] = {3, 4, 1, -1};
	int options_ikkunat[] = {2,-1};
	int controls_ikkunat[] = {5,6,-1};
	int pituudet[] = {4,2,3};
	//glob_data->gfx->TyhjennaRuutu();
	
	if(uusi)
	{
		int arvo = uusi->logiikka(glob_data);

		if(arvo == 1)
		{
			delete uusi;
			uusi = 0;
		}
		else if(arvo > 1)
		{
			delete uusi;
			uusi = 0;
			return arvo;
		}
	}
	else
	{
		if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_ESCAPE])
		{
			if(ikkunanro == 0) return 4;
			else return 1;
		}
		if(ikkunanro <= 2)
		{
			char **osoitin;
			int *ikkunaosoitin;

			switch(ikkunanro)
			{
			case 0: osoitin = paavalikko_jonot; ikkunaosoitin = paavalikko_ikkunat; break;
			case 1: osoitin = options_jonot; ikkunaosoitin = options_ikkunat; break;
			case 2: osoitin = controls_jonot; ikkunaosoitin = controls_ikkunat; break;
			default: break;
			}

			
			for(int rivi = 0; rivi < pituudet[ikkunanro]; rivi++)
			{
				Tulosta(osoitin[rivi], 240, 200 + rivi*20, glob_data);
			}

			glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 220,200+kursori_y*20,&laatikot[valintakursori_kuva],1,0);

			if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_UP])
			{
				kursori_y--;
				if(kursori_y < 0) kursori_y = 0;
			}
			if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_DOWN])
			{
				kursori_y++;
				if(kursori_y > pituudet[ikkunanro]-1) kursori_y = pituudet[ikkunanro]-1;
			}

			if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_RETURN])
			{
				int arvo = ikkunaosoitin[kursori_y];
				if(arvo == -1)
				{
					if(ikkunanro == 0) return 4;
					else return 1;
				}
				else
				{
					uusi = new Valikot;
					uusi->alusta(arvo);
				}
			}
		}
		else
		{
			int x;
			InputData *input =  glob_data->gfx->annaInput();
			int koko;
			int index;

			switch(ikkunanro)
			{
			case 3:
				if(valitus)
				{
					Tulosta("Overwrite existing savegame?", 240, 180, glob_data);
					Tulosta("No", 240, 200, glob_data);
					Tulosta("Yes", 240, 220, glob_data);

					if(input->NAPPI_PAINETTU[SDLK_UP]) kursori_y--;
					if(input->NAPPI_PAINETTU[SDLK_DOWN]) kursori_y++;

					if(kursori_y < 0) kursori_y = 0;
					if(kursori_y > 1) kursori_y = 1;

										
					glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 220,200+kursori_y*20,&laatikot[valintakursori_kuva],1,0);

					

					if(input->NAPPI_PAINETTU[SDLK_RETURN])
					{
						if(kursori_y == 1)
						{
							for(index = 0; index < 20; index++) savegame[index] = rivi[index];
							return UUSIPELI;
						}

						valitus = 0;
					}
				}
				else
				{
					Tulosta("Enter name for savegame:", 200,200, glob_data);
					Tulosta(rivi, 240,220, glob_data);
					koko = 0;
					for(index = 0; index < 20; index++)
					{
						if(rivi[index] == '\0') break;
						koko++;
					}
					
					if(input->NAPPI_PAINETTU[SDLK_RETURN] && koko > 0)
					{
						
						char kokonimi[256];
						sprintf(kokonimi, "%s.sav",rivi);

						std::ifstream tiedosto(kokonimi, std::ios_base::binary);
						
						valitus = 0;
						if(tiedosto)
						{
							tiedosto.close();
							valitus = 1;
						}
						else
						{
							for(index = 0; index < 20; index++) savegame[index] = rivi[index];
							return UUSIPELI;
						}
					}
				}
				
				
				
				if(input->NAPPI_PAINETTU[SDLK_BACKSPACE] && koko > 0) rivi[koko-1] = '\0';
				else for(char merkki = SDLK_SPACE; merkki <= SDLK_z; merkki++)
				{
					if(input->NAPPI_PAINETTU[merkki] && koko < 19)
					{
						rivi[koko] = merkki;
						koko++;
						rivi[koko] = '\0';
						break;
					}
				}

				break;

			case 4:
				
				if(valitus)
				{
					Tulosta("No savegames available",240,200, glob_data);
					if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_RETURN]) return 1;
				}
				else
				{
					if(scroll_y > 0) Tulosta("...more...", 240, 180, glob_data);
					for(index = scroll_y; index < scroll_y+5 && index < tiedostolista.size(); index++)
					{
						Tulosta((char*)tiedostolista.at(index).c_str(),240,200+20*(index-scroll_y), glob_data);
					}
					if(tiedostolista.size() > scroll_y + 5) Tulosta("...more...", 240, 300, glob_data);
					

					if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_UP])
					{
						kursori_y--;
						if(kursori_y < 0)
						{
							kursori_y = 0;
							scroll_y--;
							if(scroll_y < 0) scroll_y = 0;
						}
					}
					if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_DOWN])
					{
						kursori_y++;
						if(kursori_y > 4)
						{
							kursori_y = 4;
							scroll_y++;
							if(scroll_y + 4 > tiedostolista.size()-1)
								scroll_y = tiedostolista.size()-5;
						}
						//if(kursori_y > tiedostolista.size()-1) kursori_y = tiedostolista.size()-1;
					}

					if(kursori_y + scroll_y > tiedostolista.size()-1)kursori_y = tiedostolista.size()-1-scroll_y;

					
					if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[SDLK_RETURN])
					{
						std::string nimi = tiedostolista.at(kursori_y+scroll_y);
					

						for(index = 0; index < nimi.size(); index++)
						{
							savegame[index] = nimi.at(index);
						}
						savegame[index] = '\0';

						return LATAAPELI;
					}
				}

				
				glob_data->gfx->PiirraSprite(glob_data->pikkukuvat, 220,200+kursori_y*20,&laatikot[valintakursori_kuva],1,0);

				

				break;
			case 5:
				if(nappi_index == 6)
				{
					for(int index = 0; index < 6; index++)
						glob_data->keyboard_indeksit[index] = nappidata[index];

					unsigned char saadot = glob_data->saadot;
					glob_data->saadot = (saadot | 1);
					tallenna_configit(glob_data);
					return 1;
				}
				x = Tulosta("Press ", 240,200, glob_data);
				x = Tulosta(nappi_jonot[nappi_index], x, 200, glob_data);
				Tulosta(" Button", x,200, glob_data);

				for(index = 0; index < 1000; index++)
				{
					if(glob_data->gfx->annaInput()->NAPPI_PAINETTU[index])
					{
						nappidata[nappi_index] = index;
						//glob_data->keyboard_indeksit[nappi_index] = index;
						nappi_index++;
						break;
					}
				}
				break;
			case 6:
				if(nappi_index == 6)
				{
					for(int index = 0; index < 6; index++)
						glob_data->gamepad_indeksit[index] = nappidata[index];

					unsigned char saadot = glob_data->saadot;
					glob_data->saadot = (saadot | 2);
					tallenna_configit(glob_data);
					return 1;
				}
				x = Tulosta("Press ", 240,200, glob_data);
				x = Tulosta(nappi_jonot[nappi_index], x, 200, glob_data);
				Tulosta(" Button", x,200, glob_data);

				for(index = 0; index < 20; index++)
				{
					if(glob_data->gfx->annaInput()->JOYSTICK_PAINETTU[index])
					{
						nappidata[nappi_index] = index;
						//glob_data->gamepad_indeksit[nappi_index] = index;
						nappi_index++;
						break;
					}
				}
				break;
			default: break;
			}
		}
	}
	return 0;
}