#include "SDL.h"
#include <fmod.h>
#include <fstream>
#include <math.h>
#include "grafiikka.h"

#define luku 1
#define kaksp_luku 2



int number_to_string(int number, char **string)
{
	int temp_number;
	int lenght = 1;
	char *temp_string;
	int index, index_2;
	int miinus = 0;
	
	if(number < 0)
	{
		miinus = 1;
		number = -number;
	}

	for(index = 1; index <= 9; index++)
	{
		temp_number = 1;
		for(index_2 = 0; index_2 < index; index_2++) temp_number *= 10;
		
		if(number < temp_number) break;
		
		lenght++;
	}
	
	//temp_string = (char *)malloc((lenght+1+miinus) * sizeof(char));
	temp_string = new char[lenght+1+miinus];
	
	for(index = lenght; index >= 1; index--)
	{
	
		temp_number = 1;
		
		for(index_2 = 1; index_2 < index; index_2++) temp_number *= 10;
		
		for(index_2 = 9; index_2 >= 0; index_2--)
		{
			if(number >= temp_number * index_2)
			{
				temp_string[lenght - index+miinus] = 48 + index_2;
				number -= temp_number * index_2;
				break;
			}
		}
	
	}
	
	if(miinus) temp_string[0] = '-';

	temp_string[lenght+miinus] = '\0';
	*string = temp_string;
	
	return 0;
}


SDL_Surface* Grafiikka::AnnaOsoitin(){return sdl_screen;}
InputData* Grafiikka::annaInput() {return &inputti;}

int Grafiikka::AnnaAika()
{
	if(!moodi) return 0;

	int aika;
	switch(moodi)
	{
	case 1:
		aika = SDL_GetTicks();
		break;
	default: break;
	}

	return aika;

}


void Grafiikka::ValaiseRivi2(int y, float aksa, Uint32 *kohdepixelit_32)
{
	unsigned int taso = (int)(aksa*128);
	#define maski_1pois 0xFEFEFE
	#define maski_2pois 0xFCFCFC
	#define maski_3pois 0xF8F8F8
	#define maski_4pois 0xF0F0F0
	#define maski_5pois 0xE0E0E0
	#define maski_6pois 0xC0C0C0


	int indeksi = y*sdl_screen->pitch/4;
	int kohta;
	Uint32 pikseli;

	for(kohta = indeksi; kohta < indeksi+640; kohta++)
	{
		pikseli = kohdepixelit_32[kohta];

		Uint32 summa = 0;

		if(taso & 128) summa = pikseli;

		pikseli = pikseli & maski_1pois;
		pikseli = pikseli >> 1;


		if(taso & 64) summa += pikseli;

		pikseli = pikseli & maski_1pois;
		pikseli = pikseli >> 1;

		if(taso & 32) summa += pikseli;

		pikseli = pikseli & maski_1pois;
		pikseli = pikseli >> 1;

		if(taso & 16) summa += pikseli;

		pikseli = pikseli & maski_1pois;
		pikseli = pikseli >> 1;

		if(taso & 8) summa += pikseli;

		pikseli = pikseli & maski_1pois;
		pikseli = pikseli >> 1;

		if(taso & 4) summa += pikseli;

		pikseli = pikseli & maski_1pois;
		pikseli = pikseli >> 1;

		if(taso & 2) summa += pikseli;

		pikseli = pikseli & maski_1pois;
		pikseli = pikseli >> 1;

		if(taso & 1) summa += pikseli;
		
		
		kohdepixelit_32[kohta] = summa;
	}
	return;

	switch(taso)
	{
	case 0:
		memset(&kohdepixelit_32[indeksi],0,640*sizeof(Uint32));
		break;
	case 1:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_4pois;
			pikseli = pikseli >> 4;
			kohdepixelit_32[kohta] = pikseli;
		}
		break;
	case 2:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_3pois;
			pikseli = pikseli >> 3;
			kohdepixelit_32[kohta] = pikseli;
		}
		break;
	case 3:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos;
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_3pois;
			pikseli = pikseli >> 3;			// 1/8
			valitulos = pikseli;
			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;			// 1/16
			kohdepixelit_32[kohta] = pikseli + valitulos;
		}
		break;
	case 4:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_2pois;
			pikseli = pikseli >> 2;
			kohdepixelit_32[kohta] = pikseli;
		}
		break;
	case 5:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos;
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_2pois;
			pikseli = pikseli >> 2;				// 1/4
			valitulos = pikseli;
			pikseli = pikseli & maski_2pois;
			pikseli = pikseli >> 2;				// 1/16
			kohdepixelit_32[kohta] = pikseli + valitulos;
		}
		break;
	case 6:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos;
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_2pois;
			pikseli = pikseli >> 2;				// 1/4
			valitulos = pikseli;
			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/8
			kohdepixelit_32[kohta] = pikseli + valitulos;
		}
		break;
	case 7:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos,valitulos2;
			pikseli = kohdepixelit_32[kohta];

			pikseli = pikseli & maski_2pois;
			pikseli = pikseli >> 2;				// 1/4
			valitulos = pikseli;

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/8
			valitulos2 = pikseli;

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/16

			kohdepixelit_32[kohta] = pikseli + valitulos + valitulos2;
		}
		break;
	case 8:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;
			kohdepixelit_32[kohta] = pikseli;
		}
		break;
	case 9:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos;
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;
			valitulos = pikseli;				// 1/2

			pikseli = pikseli & maski_3pois;
			pikseli = pikseli >> 3;				// 1/16
			kohdepixelit_32[kohta] = pikseli + valitulos;
		}
		break;
	case 10:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos;
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;
			valitulos = pikseli;				// 1/2

			pikseli = pikseli & maski_2pois;
			pikseli = pikseli >> 2;				// 1/8
			kohdepixelit_32[kohta] = pikseli + valitulos;
		}
		break;
	case 11:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos,valitulos2;
			pikseli = kohdepixelit_32[kohta];

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/2
			valitulos = pikseli;

			pikseli = pikseli & maski_2pois;
			pikseli = pikseli >> 2;				// 1/8
			valitulos2 = pikseli;

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/16

			kohdepixelit_32[kohta] = pikseli + valitulos + valitulos2;
		}
		break;
	case 12:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos;
			pikseli = kohdepixelit_32[kohta];
			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;
			valitulos = pikseli;				// 1/2

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/4
			kohdepixelit_32[kohta] = pikseli + valitulos;
		}
		break;
	case 13:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos,valitulos2;
			pikseli = kohdepixelit_32[kohta];

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/2
			valitulos = pikseli;

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/4
			valitulos2 = pikseli;

			pikseli = pikseli & maski_2pois;
			pikseli = pikseli >> 2;				// 1/16

			kohdepixelit_32[kohta] = pikseli + valitulos + valitulos2;
		}
		break;
	case 14:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos,valitulos2;
			pikseli = kohdepixelit_32[kohta];

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/2
			valitulos = pikseli;

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/4
			valitulos2 = pikseli;

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/8

			kohdepixelit_32[kohta] = pikseli + valitulos + valitulos2;
		}
		break;

		case 15:
		for(kohta = indeksi; kohta < indeksi+640; kohta++)
		{
			Uint32 valitulos,valitulos2,valitulos3;
			pikseli = kohdepixelit_32[kohta];

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/2
			valitulos = pikseli;

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/4
			valitulos2 = pikseli;

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/8
			valitulos3 = pikseli;

			pikseli = pikseli & maski_1pois;
			pikseli = pikseli >> 1;				// 1/16

			kohdepixelit_32[kohta] = pikseli + valitulos + valitulos2 + valitulos3;
		}
		break;

	case 16: break;
	default: break;
	}
	
}

// piirt‰‰ kolmion yhden rivin
void piirraRivi(Uint32 *source, Uint32 *dest, int kuvaleveys, int dest_delta_x, int source_delta_x, int source_delta_y)
{

	if(dest_delta_x == 0)
	{
		*dest = *source;
		return;
	}

	const x_nega = 1, x_iso = 2, y_iso = 4;
	int summa = 0;
	

	if(source_delta_x < 0) {summa += x_nega; source_delta_x *= -1;}
	if(source_delta_y < 0) {kuvaleveys *= -1; source_delta_y *= -1;}

	if(source_delta_x > dest_delta_x) summa += x_iso;
	if(source_delta_y > dest_delta_x) summa += y_iso;
		
	int x_laskuri = -dest_delta_x/2;
	int y_laskuri = -dest_delta_x/2;

	int x;

	int x_hyppyvakio;
	int y_hyppyvakio;

	switch(summa)
	{
	case 0:
		for(x = 0; x <= dest_delta_x; x++)
		{
			*dest = *source;
			dest++;

			x_laskuri += source_delta_x;
			y_laskuri += source_delta_y;

			if(x_laskuri > 0)
			{
				x_laskuri -= dest_delta_x;
				source++;
			}

			if(y_laskuri > 0)
			{
				y_laskuri -= dest_delta_x;
				source += kuvaleveys;
			}
		}
		break;

	case x_nega:
		for(x = 0; x <= dest_delta_x; x++)
		{
			*dest = *source;
			dest++;

			x_laskuri += source_delta_x;
			y_laskuri += source_delta_y;

			if(x_laskuri > 0)
			{
				x_laskuri -= dest_delta_x;
				source--;
			}

			if(y_laskuri > 0)
			{
				y_laskuri -= dest_delta_x;
				source += kuvaleveys;
			}
		}
		break;

	case x_iso:
		

		x_hyppyvakio = source_delta_x/dest_delta_x;
		source_delta_x = source_delta_x%dest_delta_x;

		for(x = 0; x <= dest_delta_x; x++)
		{
			*dest = *source;
			dest++;

			x_laskuri += source_delta_x;
			y_laskuri += source_delta_y;

			if(x_laskuri > 0)
			{
				x_laskuri -= dest_delta_x;
				source++;
			}

			source += x_hyppyvakio;

			if(y_laskuri > 0)
			{
				y_laskuri -= dest_delta_x;
				source += kuvaleveys;
			}
		}
		
		break;

	case x_nega | x_iso:

		x_hyppyvakio = source_delta_x/dest_delta_x;
		source_delta_x = source_delta_x%dest_delta_x;

		for(x = 0; x <= dest_delta_x; x++)
		{
			*dest = *source;
			dest++;

			x_laskuri += source_delta_x;
			y_laskuri += source_delta_y;

			if(x_laskuri > 0)
			{
				x_laskuri -= dest_delta_x;
				source--;
			}

			source -= x_hyppyvakio;

			if(y_laskuri > 0)
			{
				y_laskuri -= dest_delta_x;
				source += kuvaleveys;
			}
		}

		break;

	case y_iso:

		y_hyppyvakio = source_delta_y/dest_delta_x;
		y_hyppyvakio *= kuvaleveys;

		source_delta_y = source_delta_y%dest_delta_x;

		for(x = 0; x <= dest_delta_x; x++)
		{
			*dest = *source;
			dest++;

			x_laskuri += source_delta_x;
			y_laskuri += source_delta_y;

			if(x_laskuri > 0)
			{
				x_laskuri -= dest_delta_x;
				source++;
			}

			if(y_laskuri > 0)
			{
				y_laskuri -= dest_delta_x;
				source += kuvaleveys;
			}

			source += y_hyppyvakio;
		}
		break;

	case x_nega | y_iso:

		y_hyppyvakio = source_delta_y/dest_delta_x;
		y_hyppyvakio *= kuvaleveys;

		source_delta_y = source_delta_y%dest_delta_x;

		for(x = 0; x <= dest_delta_x; x++)
		{
			*dest = *source;
			dest++;

			x_laskuri += source_delta_x;
			y_laskuri += source_delta_y;

			if(x_laskuri > 0)
			{
				x_laskuri -= dest_delta_x;
				source--;
			}

			if(y_laskuri > 0)
			{
				y_laskuri -= dest_delta_x;
				source += kuvaleveys;
			}

			source += y_hyppyvakio;
		}
		break;

	case x_iso | y_iso:


		x_hyppyvakio = source_delta_x/dest_delta_x;
		source_delta_x = source_delta_x%dest_delta_x;

		y_hyppyvakio = source_delta_y/dest_delta_x;
		y_hyppyvakio *= kuvaleveys;

		source_delta_y = source_delta_y%dest_delta_x;

		for(x = 0; x <= dest_delta_x; x++)
		{
			*dest = *source;
			dest++;

			x_laskuri += source_delta_x;
			y_laskuri += source_delta_y;

			if(x_laskuri > 0)
			{
				x_laskuri -= dest_delta_x;
				source++;
			}

			source += x_hyppyvakio;

			if(y_laskuri > 0)
			{
				y_laskuri -= dest_delta_x;
				source += kuvaleveys;
			}

			source += y_hyppyvakio;
		}
		break;

	case x_nega | x_iso | y_iso:


		x_hyppyvakio = source_delta_x/dest_delta_x;
		x_hyppyvakio *= -1;

		source_delta_x = source_delta_x%dest_delta_x;

		y_hyppyvakio = source_delta_y/dest_delta_x;
		y_hyppyvakio *= kuvaleveys;

		source_delta_y = source_delta_y%dest_delta_x;

		for(x = 0; x <= dest_delta_x; x++)
		{
			*dest = *source;
			dest++;

			x_laskuri += source_delta_x;
			y_laskuri += source_delta_y;

			if(x_laskuri > 0)
			{
				x_laskuri -= dest_delta_x;
				source--;
			}

			source += x_hyppyvakio;

			if(y_laskuri > 0)
			{
				y_laskuri -= dest_delta_x;
				source += kuvaleveys;
			}

			source += y_hyppyvakio;
		}
		break;

	default: break;
	}

}



void kolmio(int *piirto_koord_x, int *piirto_koord_y, int *text_koord_x, int *text_koord_y,
			int l, int k, Uint32 *sprite, SDL_Surface *pinta)
{

	int x_reso = pinta->w;
	int y_reso = pinta->h;

	for(int index = 0; index < 3; index++)
	{
		int x = piirto_koord_x[index];
		int y = piirto_koord_y[index];

		if(x < 0 || y < 0 || x >= x_reso || y >= y_reso) return;
	}

	int a_x = piirto_koord_x[1]-piirto_koord_x[0];
	int a_y = piirto_koord_y[1]-piirto_koord_y[0];

	int b_x = piirto_koord_x[2]-piirto_koord_x[0];
	int b_y = piirto_koord_y[2]-piirto_koord_y[0];

	int ristitulo = a_x*b_y - a_y*b_x;

	if(ristitulo < 0)
	{
		int temp;

		temp = piirto_koord_x[1];
		piirto_koord_x[1] = piirto_koord_x[2];
		piirto_koord_x[2] = temp;

		temp = piirto_koord_y[1];
		piirto_koord_y[1] = piirto_koord_y[2];
		piirto_koord_y[2] = temp;

		temp = text_koord_x[1];
		text_koord_x[1] = text_koord_x[2];
		text_koord_x[2] = temp;

		temp = text_koord_y[1];
		text_koord_y[1] = text_koord_y[2];
		text_koord_y[2] = temp;
	}

	Uint32 *naytto = (Uint32*)pinta->pixels;
	int pitch = pinta->pitch >> 2;


	// Lasketaan ylin koordinaatti

	int ylinIndeksi = 0;
	int ylinY = piirto_koord_y[0];
	int ylinX = piirto_koord_x[0];

	for(index = 1; index < 3; index++)
	{
		if((piirto_koord_y[index] < ylinY) ||
			((piirto_koord_y[index] == ylinY) && (piirto_koord_x[index] < ylinX)))
		{
			ylinY = piirto_koord_y[index];
			ylinX = piirto_koord_x[index];
			ylinIndeksi = index;
		}
	}

	// pyˆritet‰‰n koordinaatteja siten, ett‰ ylin on indeksiss‰ 0

	for(index = 0; index < ylinIndeksi; index++)
	{
		int temp_x = piirto_koord_x[0];
		int temp_y = piirto_koord_y[0];

		int temp_text_x = text_koord_x[0];
		int temp_text_y = text_koord_y[0];

		text_koord_x[0] = text_koord_x[1];
		text_koord_x[1] = text_koord_x[2];
		text_koord_x[2] = temp_text_x;

		text_koord_y[0] = text_koord_y[1];
		text_koord_y[1] = text_koord_y[2];
		text_koord_y[2] = temp_text_y;

		piirto_koord_x[0] = piirto_koord_x[1];
		piirto_koord_x[1] = piirto_koord_x[2];
		piirto_koord_x[2] = temp_x;

		piirto_koord_y[0] = piirto_koord_y[1];
		piirto_koord_y[1] = piirto_koord_y[2];
		piirto_koord_y[2] = temp_y;
	}

	if(piirto_koord_y[0] == piirto_koord_y[1]) // l‰tt‰n‰ huippu
	{
		int vasen_x = piirto_koord_x[0];
		int oikea_x = piirto_koord_x[1];
		int ala_x = piirto_koord_x[2];

		int yla_y = piirto_koord_y[0];
		int ala_y = piirto_koord_y[2];

		int vasen_text_x = text_koord_x[0];
		int oikea_text_x = text_koord_x[1];
		int ala_text_x = text_koord_x[2];

		int vasen_text_y = text_koord_y[0];
		int oikea_text_y = text_koord_y[1];
		int ala_text_y = text_koord_y[2];

		Uint32 *dest = &naytto[yla_y*pitch];

		//yht‰suuruus
		for(int rivi_y = yla_y; rivi_y <= ala_y; rivi_y++)
		{
			int kohde_x1 = vasen_x + (double)(ala_x-vasen_x)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);
			int kohde_x2 = oikea_x + (double)(ala_x-oikea_x)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);

			int lahde_x1 = vasen_text_x + (double)(ala_text_x-vasen_text_x)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);
			int lahde_y1 = vasen_text_y + (double)(ala_text_y-vasen_text_y)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);

			int lahde_x2 = oikea_text_x + (double)(ala_text_x-oikea_text_x)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);
			int lahde_y2 = oikea_text_y + (double)(ala_text_y-oikea_text_y)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);

			piirraRivi(&sprite[lahde_y1*l + lahde_x1],dest + kohde_x1, l, kohde_x2-kohde_x1, lahde_x2-lahde_x1, lahde_y2-lahde_y1);
			dest += pitch;
/*
			if(kohde_x2-kohde_x1 == 0)
			{
				naytto[rivi_y*pitch+kohde_x1] = sprite[l*lahde_y1 + kohde_x1];
			}
			else for(int x = kohde_x1; x < kohde_x2; x++)
			{
				int lahde_x = lahde_x1 + (double)(lahde_x2 - lahde_x1)/(double)(kohde_x2-kohde_x1)*(double)(x-kohde_x1);
				int lahde_y = lahde_y1 + (double)(lahde_y2 - lahde_y1)/(double)(kohde_x2-kohde_x1)*(double)(x-kohde_x1);

				

				naytto[rivi_y*pitch+x] = sprite[l*lahde_y + lahde_x];
			}
*/
			
		}
	}
	else if(piirto_koord_y[1] == piirto_koord_y[2]) //l‰tt‰n‰ pohja
	{
		int vasen_x = piirto_koord_x[2];
		int oikea_x = piirto_koord_x[1];
		int yla_x = piirto_koord_x[0];

		int yla_y = piirto_koord_y[0];
		int ala_y = piirto_koord_y[2];

		int vasen_text_x = text_koord_x[2];
		int oikea_text_x = text_koord_x[1];
		int yla_text_x = text_koord_x[0];

		int vasen_text_y = text_koord_y[2];
		int oikea_text_y = text_koord_y[1];
		int yla_text_y = text_koord_y[0];

		Uint32 *dest = &naytto[yla_y*pitch];

		//yht‰suuruus
		for(int rivi_y = yla_y; rivi_y <= ala_y; rivi_y++)
		{
			int kohde_x1 = yla_x + (double)(vasen_x-yla_x)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);
			int kohde_x2 = yla_x + (double)(oikea_x-yla_x)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);

			int lahde_x1 = yla_text_x + (double)(vasen_text_x-yla_text_x)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);
			int lahde_y1 = yla_text_y + (double)(vasen_text_y-yla_text_y)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);

			int lahde_x2 = yla_text_x + (double)(oikea_text_x-yla_text_x)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);
			int lahde_y2 = yla_text_y + (double)(oikea_text_y-yla_text_y)/(double)(ala_y-yla_y)*(double)(rivi_y-yla_y);

			piirraRivi(&sprite[lahde_y1*l + lahde_x1],dest + kohde_x1, l, kohde_x2-kohde_x1, lahde_x2-lahde_x1, lahde_y2-lahde_y1);
			dest += pitch;
			/*
			if(kohde_x2-kohde_x1 == 0)
			{
				naytto[rivi_y*pitch+kohde_x1] = sprite[l*lahde_y1 + kohde_x1];
			}
			else for(int x = kohde_x1; x < kohde_x2; x++)
			{
				int lahde_x = lahde_x1 + (double)(lahde_x2 - lahde_x1)/(double)(kohde_x2-kohde_x1)*(double)(x-kohde_x1);
				int lahde_y = lahde_y1 + (double)(lahde_y2 - lahde_y1)/(double)(kohde_x2-kohde_x1)*(double)(x-kohde_x1);

				naytto[rivi_y*pitch+x] = sprite[l*lahde_y + lahde_x];
			}
			*/
		}
	}
	else // kolmio jaettava
	{
		int j_koord_x[3];
		int j_koord_y[3];

		int j_text_x[3];
		int j_text_y[3];

		// j‰rjestet‰‰n kolmion koordinaatit myˆs korkeuden mukaan
		memcpy(&(j_koord_x[0]), &(piirto_koord_x[0]), sizeof(int)*3);
		memcpy(&(j_koord_y[0]), &(piirto_koord_y[0]), sizeof(int)*3);
		memcpy(&(j_text_x[0]), &(text_koord_x[0]), sizeof(int)*3);
		memcpy(&(j_text_y[0]), &(text_koord_y[0]), sizeof(int)*3);

		for(int i = 0; i < 2; i++)
		{
			for(int j = i+1; j < 3; j++)
			{
				if(j_koord_y[j] < j_koord_y[i])			
				{
					double temp = j_koord_y[j];
					j_koord_y[j] = j_koord_y[i];
					j_koord_y[i] = temp;

					temp = j_koord_x[j];
					j_koord_x[j] = j_koord_x[i];
					j_koord_x[i] = temp;

					temp = j_text_x[j];
					j_text_x[j] = j_text_x[i];
					j_text_x[i] = temp;

					temp = j_text_y[j];
					j_text_y[j] = j_text_y[i];
					j_text_y[i] = temp;			
				}
			}	
		}

		int yla_x = piirto_koord_x[0];
		int yla_y = piirto_koord_y[0];

		int yla_text_x = text_koord_x[0];
		int yla_text_y = text_koord_y[0];

		int vali_x = j_koord_x[1];
		int vali_y = j_koord_y[1];

		int ala_x = j_koord_x[2];
		int ala_y = j_koord_y[2];

		int ala_text_x = j_text_x[2];
		int ala_text_y = j_text_y[2];

		int vasen_x = piirto_koord_x[2];
		int vasen_y = piirto_koord_y[2];

		int vasen_text_x = text_koord_x[2];
		int vasen_text_y = text_koord_y[2];

		int oikea_x = piirto_koord_x[1];
		int oikea_y = piirto_koord_y[1];

		int oikea_text_x = text_koord_x[1];
		int oikea_text_y = text_koord_y[1];

		Uint32 *dest = &naytto[yla_y*pitch];

		for(int rivi_y = yla_y; rivi_y < vali_y; rivi_y++)
		{
			int kohde_x1 = yla_x + (double)(vasen_x-yla_x)/(double)(vasen_y-yla_y)*(double)(rivi_y-yla_y);
			int kohde_x2 = yla_x + (double)(oikea_x-yla_x)/(double)(oikea_y-yla_y)*(double)(rivi_y-yla_y);

			int lahde_x1 = yla_text_x + (double)(vasen_text_x-yla_text_x)/(double)(vasen_y-yla_y)*(double)(rivi_y-yla_y);
			int lahde_y1 = yla_text_y + (double)(vasen_text_y-yla_text_y)/(double)(vasen_y-yla_y)*(double)(rivi_y-yla_y);

			int lahde_x2 = yla_text_x + (double)(oikea_text_x-yla_text_x)/(double)(oikea_y-yla_y)*(double)(rivi_y-yla_y);
			int lahde_y2 = yla_text_y + (double)(oikea_text_y-yla_text_y)/(double)(oikea_y-yla_y)*(double)(rivi_y-yla_y);

			piirraRivi(&sprite[lahde_y1*l + lahde_x1],dest + kohde_x1, l, kohde_x2-kohde_x1, lahde_x2-lahde_x1, lahde_y2-lahde_y1);
			dest += pitch;
			/*
			if(kohde_x2-kohde_x1 == 0)
			{
				naytto[rivi_y*pitch+kohde_x1] = sprite[l*lahde_y1 + kohde_x1];
			}
			else  for(int x = kohde_x1; x < kohde_x2; x++)
			{
				int lahde_x = lahde_x1 + (double)(lahde_x2 - lahde_x1)/(double)(kohde_x2-kohde_x1)*(double)(x-kohde_x1);
				int lahde_y = lahde_y1 + (double)(lahde_y2 - lahde_y1)/(double)(kohde_x2-kohde_x1)*(double)(x-kohde_x1);

				naytto[rivi_y*pitch+x] = sprite[l*lahde_y + lahde_x];
			}*/

		}

		int vasen_alku_x, vasen_alku_y, oikea_alku_x, oikea_alku_y;
		int vasen_alku_text_x, vasen_alku_text_y, oikea_alku_text_x, oikea_alku_text_y;

		if(piirto_koord_y[1] < piirto_koord_y[2])
		{
			vasen_alku_x = piirto_koord_x[0];
			vasen_alku_y = piirto_koord_y[0];

			oikea_alku_x = piirto_koord_x[1];
			oikea_alku_y = piirto_koord_y[1];

			vasen_alku_text_x = text_koord_x[0];
			vasen_alku_text_y = text_koord_y[0];

			oikea_alku_text_x = text_koord_x[1];
			oikea_alku_text_y = text_koord_y[1];
		}
		else
		{
			vasen_alku_x = piirto_koord_x[2];
			vasen_alku_y = piirto_koord_y[2];

			oikea_alku_x = piirto_koord_x[0];
			oikea_alku_y = piirto_koord_y[0];

			vasen_alku_text_x = text_koord_x[2];
			vasen_alku_text_y = text_koord_y[2];

			oikea_alku_text_x = text_koord_x[0];
			oikea_alku_text_y = text_koord_y[0];
		}

		dest = &naytto[vali_y*pitch];

		//yht‰suuruus
		for(rivi_y = vali_y; rivi_y <= ala_y; rivi_y++)
		{
			int kohde_x1 = vasen_alku_x + (double)(ala_x - vasen_alku_x)/(double)(ala_y-vasen_alku_y)*(double)(rivi_y-vasen_alku_y);
			int kohde_x2 = oikea_alku_x + (double)(ala_x - oikea_alku_x)/(double)(ala_y-oikea_alku_y)*(double)(rivi_y-oikea_alku_y);

			int lahde_x1 = vasen_alku_text_x + (double)(ala_text_x - vasen_alku_text_x)/(double)(ala_y-vasen_alku_y)*(double)(rivi_y-vasen_alku_y);
			int lahde_x2 = oikea_alku_text_x + (double)(ala_text_x - oikea_alku_text_x)/(double)(ala_y-oikea_alku_y)*(double)(rivi_y-oikea_alku_y);

			int lahde_y1 = vasen_alku_text_y + (double)(ala_text_y - vasen_alku_text_y)/(double)(ala_y-vasen_alku_y)*(double)(rivi_y-vasen_alku_y);
			int lahde_y2 = oikea_alku_text_y + (double)(ala_text_y - oikea_alku_text_y)/(double)(ala_y-oikea_alku_y)*(double)(rivi_y-oikea_alku_y);

			piirraRivi(&sprite[lahde_y1*l + lahde_x1],dest + kohde_x1, l, kohde_x2-kohde_x1, lahde_x2-lahde_x1, lahde_y2-lahde_y1);
			dest += pitch;
/*
			if(kohde_x2-kohde_x1 == 0)
			{
				naytto[rivi_y*pitch+kohde_x1] = sprite[l*lahde_y1 + kohde_x1];
			}
			else for(int x = kohde_x1; x < kohde_x2; x++)
			{
				int lahde_x = lahde_x1 + (double)(lahde_x2 - lahde_x1)/(double)(kohde_x2-kohde_x1)*(double)(x-kohde_x1);
				int lahde_y = lahde_y1 + (double)(lahde_y2 - lahde_y1)/(double)(kohde_x2-kohde_x1)*(double)(x-kohde_x1);

				naytto[rivi_y*pitch + x] = sprite[l*lahde_y + lahde_x];
			}
*/			

		}
	}

}

void Grafiikka::piirraKolmiot()
{
	if(kolmiomaara == 0) return;

	SDL_LockSurface(valipinta);
	Uint32 *pixelit_32 = (Uint32 *)valipinta->pixels;
	memcpy(buffer_32, pixelit_32, (x_reso)*(y_reso)*sizeof(Uint32));
	SDL_UnlockSurface(valipinta);
		

	SDL_LockSurface(sdl_screen);

	int p = sdl_screen->pitch >> 2;
	memset(sdl_screen->pixels, 0, sizeof(Uint32)*p*y_reso);

	
	for(int index = 0; index < kolmiomaara; index++)
	{

		kolmio((int*)&(kolmio_x[index*3]),(int*)&(kolmio_y[index*3]),(int*)&(kolmio_text_x[index*3]),(int*)&(kolmio_text_y[index*3]),x_reso,y_reso,buffer_32,sdl_screen);
	}

	SDL_UnlockSurface(sdl_screen);

}

void Grafiikka::ValaiseRivi(int y, float aksa, Uint32 *kohdepixelit_32, Uint32 punamaski, Uint32 vihermaski, Uint32 sinimaski)
{
	SDL_PixelFormat *fmt;
	fmt = sdl_screen->format;

	for(int ags = 0; ags < 640; ags++)
	{
		Uint32 pikseli = kohdepixelit_32[y*sdl_screen->pitch/4 + ags];
		Uint32 puna = (pikseli & punamaski) >> fmt->Rshift;
		Uint32 viher = (pikseli & vihermaski) >> fmt->Gshift;
		Uint32 sini = (pikseli & sinimaski) >> fmt->Bshift;
										
		puna *= aksa;
		viher *= aksa;
		sini *= aksa;

		Uint32 tulos = ((puna) << fmt->Rshift)
		| ((viher) << fmt->Gshift)
		| ((sini) << fmt->Bshift);

		kohdepixelit_32[y*sdl_screen->pitch/4 + ags] = tulos;
										
	}
}


bool Grafiikka::rullaus32()
{
	Uint32 *pixelit_32, *kohdepixelit_32;

	SDL_PixelFormat *fmt;
	fmt = sdl_screen->format;

	Uint32 punamaski = 0, vihermaski = 0, sinimaski = 0;
	Uint32 pikselimaski = 0;
	Uint32 temp;

	temp = kaksp_luku;
	for(int index = 0; index < 8-fmt->Rloss-luku; index++)
	{
		punamaski += temp;
		temp = temp << 1;
	}	
					
	punamaski = punamaski << fmt->Rshift;
					
	temp = kaksp_luku;
	for(index = 0; index < 8-fmt->Gloss-luku; index++)
	{
		vihermaski += temp;
		temp = temp << 1;
	}

	vihermaski = vihermaski << fmt->Gshift;

	temp = kaksp_luku;
	for(index = 0; index < 8-fmt->Bloss-luku; index++)
	{
		sinimaski += temp;
		temp = temp << 1;
	}

	int korkeus = 480;
	int sade = 60;
	#define pi 3.141592654

	float alfa = (float)rullaus_y /(float)sade;
	int dy = sin(alfa)*sade;
	int y_alku = rullaus_y - dy;
	int raja_1 = pi*sade/2;
					
	int alk_lev = zoom_lev, alk_kork = zoom_kork, lop_lev = x_oikea, lop_kork = y_oikea;
										
	SDL_LockSurface(valipinta);
	pixelit_32 = (Uint32 *)valipinta->pixels;
	memcpy(buffer_32, pixelit_32, (x_reso)*(y_reso)*sizeof(Uint32));
	SDL_UnlockSurface(valipinta);
	
	SDL_LockSurface(sdl_screen);

	kohdepixelit_32 = (Uint32 *)sdl_screen->pixels;
	
	Uint32 *muisti = buffer_32;
	Uint32 *raw_pixels = kohdepixelit_32;

						
	if(alfa < pi/2)
	{
		memset(kohdepixelit_32,0,sizeof(Uint32)*x_oikea*y_alku);
		for(int y = y_alku; y <= rullaus_y; y++)
		{
			float yksikko_y = ((float)(rullaus_y-y)/(float)sade);
			float kulma = asin(yksikko_y);
			float aksa = cos(kulma);

			float kulma2 = alfa-kulma;
        
			int y_final = kulma2/alfa*rullaus_y;
			
			memcpy(&kohdepixelit_32[y*sdl_screen->pitch/4],&buffer_32[y_final*x_oikea],sizeof(Uint32)*x_oikea);
			
			//ValaiseRivi(y,aksa, kohdepixelit_32, punamaski,vihermaski,sinimaski);
			ValaiseRivi2(y,aksa, kohdepixelit_32);
		}
						

		for(int rivi = rullaus_y; rivi < y_oikea; rivi++)
		{
			memcpy(&kohdepixelit_32[rivi*sdl_screen->pitch/4],&buffer_32[rivi*x_oikea],sizeof(Uint32)*x_oikea);
		}
	}
	else if(alfa <= /*3*pi/2*/pi)
	{
		int yla_y = rullaus_y-sade;
		int raja_y = rullaus_y - sin((float)rullaus_y/(float)sade)*sade;

		memset(kohdepixelit_32,0,sizeof(Uint32)*x_oikea*yla_y);

		for(int y = yla_y; y < raja_y; y++)
		{
			float yksikko_y = ((float)(rullaus_y-y)/(float)sade);
			float kulma = asin(yksikko_y);
			float aksa = cos(kulma);

			float k = pi/2 - (kulma-pi/2);

			int y_final = (float)rullaus_y -(float)rullaus_y/(float)alfa*k;
			if(y_final > 479 || y_final < 0) break;
										
			memcpy(&kohdepixelit_32[y*sdl_screen->pitch/4],&buffer_32[y_final*x_oikea],sizeof(Uint32)*x_oikea);
			//ValaiseRivi(y,aksa, kohdepixelit_32, punamaski,vihermaski,sinimaski);
			ValaiseRivi2(y,aksa, kohdepixelit_32);
		}

		for(y = raja_y; y < rullaus_y; y++)
		{
			float yksikko_y = ((float)(rullaus_y-y)/(float)sade);
			float kulma = asin(yksikko_y);
			float aksa = cos(kulma);
																
			float kulma2 = alfa-kulma;
        
			int y_final = kulma2/alfa*rullaus_y;
										
			memcpy(&kohdepixelit_32[y*sdl_screen->pitch/4],&buffer_32[y_final*x_oikea],sizeof(Uint32)*x_oikea);

			//ValaiseRivi(y,aksa, kohdepixelit_32, punamaski,vihermaski,sinimaski);
			ValaiseRivi2(y,aksa, kohdepixelit_32);
		}

		for(int rivi = rullaus_y; rivi < y_oikea; rivi++)
		{
			memcpy(&kohdepixelit_32[rivi*sdl_screen->pitch/4],&buffer_32[rivi*x_oikea],sizeof(Uint32)*x_oikea);
		}
	}
	else
	{
		int yla_y = rullaus_y-sade;
		int ala_y = rullaus_y+sade;
		if(alfa < pi*3/2) ala_y = rullaus_y - sade*sin(alfa);


		if(yla_y <= 480) // t‰m‰ kohta on muutettu
		{
			memset(kohdepixelit_32,0,sizeof(Uint32)*x_oikea*yla_y);

			for(int y =  yla_y; y < ala_y && y < 480; y++)
			{
				float yksikko_y = ((float)(rullaus_y-y)/(float)sade);
				float kulma = asin(yksikko_y);
				float aksa = cos(kulma);
									
				float k = pi/2 - (kulma-pi/2);

				int y_final = (float)rullaus_y -(float)rullaus_y/(float)alfa*k;
				if(y_final > 479 || y_final < 0) break;
											
				memcpy(&kohdepixelit_32[y*sdl_screen->pitch/4],&buffer_32[y_final*x_oikea],sizeof(Uint32)*x_oikea);

				//ValaiseRivi(y,aksa, kohdepixelit_32, punamaski,vihermaski,sinimaski);
				ValaiseRivi2(y,aksa, kohdepixelit_32);
			}

			for(int rivi = ala_y; rivi < y_oikea; rivi++)
			{
				memcpy(&kohdepixelit_32[rivi*sdl_screen->pitch/4],&buffer_32[rivi*x_oikea],sizeof(Uint32)*x_oikea);
			}
		} else memset(kohdepixelit_32,0,sizeof(Uint32)*x_oikea*y_oikea);

	} 

	SDL_UnlockSurface(sdl_screen);

	return true;
}

bool Grafiikka::zoom32moodi0()
{
	SDL_LockSurface(valipinta);
	Uint32 *pixelit_32 = (Uint32 *)valipinta->pixels;
	memcpy(buffer_32, pixelit_32, x_reso*y_reso*sizeof(Uint32));
	SDL_UnlockSurface(valipinta);
		

	SDL_LockSurface(sdl_screen);

	Uint32 *kohdepixelit_32 = (Uint32 *)sdl_screen->pixels;
	Uint32 *muisti = buffer_32;
	Uint32 *raw_pixels = kohdepixelit_32;

	int alk_lev = zoom_lev, alk_kork = zoom_kork, lop_lev = x_oikea, lop_kork = y_oikea;

	int s_leveys = (alk_lev-1);
	int d_leveys = (lop_lev-1);

	int s_korkeus = (alk_kork-1);
	int d_korkeus = (lop_kork-1);
											
	int y_lahde = zoom_y*x_reso + zoom_x;

	Uint32 *kohde_os = raw_pixels;
	Uint32 kohta_y_summa = 0;

	for(int y = 0; y < lop_kork; y++)
	{
						
		Uint32 *riviosoitin = &muisti[y_lahde];
		Uint32 kohta_x_summa = 0;
						
		for(int x = 0; x < lop_lev;x++)
		{
			*kohde_os = *riviosoitin;

			kohde_os++;
			kohta_x_summa += s_leveys;

			while(kohta_x_summa >= d_leveys)
			{
				kohta_x_summa -= d_leveys;
				riviosoitin++;
			}
		}
					
		kohta_y_summa += s_korkeus;

		while(kohta_y_summa >= d_korkeus)
		{
			kohta_y_summa -= d_korkeus;
			y_lahde += x_reso;
		}

						
		kohde_os += sdl_screen->pitch/4 -lop_lev; 

	}


	SDL_UnlockSurface(sdl_screen);


	return true;
}

bool Grafiikka::zoom32moodi1()
{
	int alk_lev = zoom_lev, alk_kork = zoom_kork, lop_lev = x_oikea, lop_kork = y_oikea;
	SDL_PixelFormat *fmt;
					
	SDL_LockSurface(valipinta);
	Uint32 *pixelit_32 = (Uint32 *)valipinta->pixels;
	memcpy(buffer_32, pixelit_32, (x_reso)*(y_reso)*sizeof(Uint32));
	SDL_UnlockSurface(valipinta);

	SDL_LockSurface(sdl_screen);

	Uint32 *kohdepixelit_32 = (Uint32 *)sdl_screen->pixels;
	fmt = sdl_screen->format;

	Uint32 *muisti = buffer_32;
	Uint32 *raw_pixels = kohdepixelit_32;


	Uint32 punamaski = 0, vihermaski = 0, sinimaski = 0;
	Uint32 pikselimaski = 0;
	Uint32 temp;

	SDL_UnlockSurface(sdl_screen);

	temp = kaksp_luku;
	for(int index = 0; index < 8-fmt->Rloss-luku; index++)
	{
		punamaski += temp;
		temp = temp << 1;
	}	
					
	punamaski = punamaski << fmt->Rshift;
					
	temp = kaksp_luku;
	for(index = 0; index < 8-fmt->Gloss-luku; index++)
	{
		vihermaski += temp;
		temp = temp << 1;
	}

	vihermaski = vihermaski << fmt->Gshift;

	temp = kaksp_luku;
	for(index = 0; index < 8-fmt->Bloss-luku; index++)
	{
		sinimaski += temp;
		temp = temp << 1;
	}

	sinimaski = sinimaski << fmt->Bshift;
	pikselimaski = punamaski | vihermaski | sinimaski;

	Uint32 pikseli_1, pikseli_2, pikseli_3, pikseli_4;

	int s_leveys = (alk_lev-1);
	int d_leveys = (lop_lev-1);

	int s_korkeus = (alk_kork-1);
	int d_korkeus = (lop_kork-1);

										
	int y_lahde = zoom_y*x_reso + zoom_x;

	Uint32 *kohde_os = raw_pixels;
	Uint32 kohta_y_summa = 0;

	for(int y = 0; y < lop_kork; y++)
	{
						
		Uint32 *riviosoitin = &muisti[y_lahde];
		Uint32 *riviosoitin2 = &muisti[y_lahde+x_reso];

		if(kohta_y_summa <= d_korkeus>>2)
		{
			Uint32 kohta_x_summa = 0;
							
			for(int x = 0; x < lop_lev;x++)
			{
				//Uint32 temp_x_suhde = kohta_x_summa%d_leveys;
				//Uint32 x_suhde = (temp_x_suhde<<luku)/d_leveys;

				if(kohta_x_summa <= d_leveys>>2)
				{
					*kohde_os = *riviosoitin;
				}
				else
				{
					//Uint32 x_ksuhde = (kaksp_luku)-x_suhde;
					pikseli_1 = *riviosoitin;
					pikseli_2 = riviosoitin[1];

					pikseli_1 = pikseli_1 & pikselimaski;
					pikseli_1 = pikseli_1 >> luku;
					pikseli_2 = pikseli_2 & pikselimaski;
					pikseli_2 = pikseli_2 >> luku;
					Uint32 tulos = pikseli_1+pikseli_2;
					*kohde_os = tulos;

				}

				kohde_os++;
				kohta_x_summa += s_leveys;

				while(kohta_x_summa >= d_leveys)
				{
					kohta_x_summa -= d_leveys;
					riviosoitin++;
				}
			}						
		}
		else
		{
			Uint32 kohta_x_summa = 0;
			//Uint32 y_ksuhde = (kaksp_luku)-y_suhde;

			for(int x = 0; x < lop_lev; x++)
			{
								
				//Uint32 temp_x_suhde = kohta_x_summa%d_leveys;
				//Uint32 x_suhde = (temp_x_suhde<<luku)/d_leveys;

				if(kohta_x_summa <= d_leveys>>2)
				{
					pikseli_1 = *riviosoitin;
					pikseli_2 = *riviosoitin2;

					pikseli_1 = pikseli_1 & pikselimaski;
					pikseli_1 = pikseli_1 >> luku;

					pikseli_2 = pikseli_2 & pikselimaski;
					pikseli_2 = pikseli_2 >> luku;
					Uint32 tulos = pikseli_1 + pikseli_2;
					*kohde_os = tulos;
				}
				else
				{
					//Uint32 x_ksuhde = (kaksp_luku)-x_suhde;

					pikseli_1 = *riviosoitin;
					pikseli_2 = riviosoitin[1];
					pikseli_3 = *riviosoitin2;
					pikseli_4 = riviosoitin2[1];

					pikseli_1 = pikseli_1 & pikselimaski;
					pikseli_2 = pikseli_2 & pikselimaski;
					pikseli_3 = pikseli_3 & pikselimaski;
					pikseli_4 = pikseli_4 & pikselimaski;

					pikseli_1 = pikseli_1 >> luku;
					pikseli_2 = pikseli_2 >> luku;
					pikseli_3 = pikseli_3 >> luku;
					pikseli_4 = pikseli_4 >> luku;

					Uint32 ekatulos = pikseli_1 + pikseli_2;
					Uint32 tokatulos = pikseli_3 + pikseli_4;
									
					ekatulos = ekatulos & pikselimaski;
					tokatulos = tokatulos & pikselimaski;

					ekatulos = ekatulos >> luku;
					tokatulos = tokatulos >> luku;

					Uint32 tulos = ekatulos + tokatulos;
					*kohde_os = tulos;

				}


				kohde_os++;
				kohta_x_summa += s_leveys;

				//if(kohta_x_summa >= d_leveys)
				//{
				//	kohta_x_summa -= d_leveys;
				//	riviosoitin++;
				//	riviosoitin2++;
				//}

				while(kohta_x_summa >= d_leveys)
				{
					kohta_x_summa -= d_leveys;
					riviosoitin++;
					riviosoitin2++;
				}
			}
		}

		kohta_y_summa += s_korkeus;

		//if(kohta_y_summa >= d_korkeus)
		//{
		//	kohta_y_summa -= d_korkeus;
		//	y_lahde += alk_lev;
		//}

		while(kohta_y_summa >= d_korkeus)
		{
			kohta_y_summa -= d_korkeus;
			y_lahde += x_reso;
		}

				
		kohde_os += sdl_screen->pitch/4 -lop_lev; 

	} // end y_loop


	return true;
}

bool Grafiikka::zoom32moodi2()
{
	int alk_lev = zoom_lev, alk_kork = zoom_kork, lop_lev = x_oikea, lop_kork = y_oikea;
					SDL_PixelFormat *fmt;
					
					SDL_LockSurface(valipinta);
					Uint32 *pixelit_32 = (Uint32 *)valipinta->pixels;
					memcpy(buffer_32, pixelit_32, x_reso*y_reso*sizeof(Uint32));
					SDL_UnlockSurface(valipinta);

					SDL_LockSurface(sdl_screen);

					Uint32 *kohdepixelit_32 = (Uint32 *)sdl_screen->pixels;
					fmt = sdl_screen->format;

					Uint32 *muisti = buffer_32;
					Uint32 *raw_pixels = kohdepixelit_32;		

					Uint32 punamaski = 0, vihermaski = 0, sinimaski = 0;
					Uint32 temp;

					

					temp = 1;
					for(int index = 0; index < 8-fmt->Rloss-luku; index++)
					{
						punamaski += temp;
						temp = temp << 1;
					}	
					
					punamaski = punamaski << fmt->Rshift;
					
					temp = 1;
					for(index = 0; index < 8-fmt->Gloss-luku; index++)
					{
						vihermaski += temp;
						temp = temp << 1;
					}

					vihermaski = vihermaski << fmt->Gshift;

					temp = 1;
					for(index = 0; index < 8-fmt->Bloss-luku; index++)
					{
						sinimaski += temp;
						temp = temp << 1;
					}

					sinimaski = sinimaski << fmt->Bshift;

					punamaski = 0x00FF0000;
					vihermaski = 0x00FF00;
					sinimaski = 0x0000FF;
					
					Uint32 pikseli_1, pikseli_2, pikseli_3, pikseli_4;

					int s_leveys = (alk_lev-1);
					int d_leveys = (lop_lev-1);

					int s_korkeus = (alk_kork-1);
					int d_korkeus = (lop_kork-1);

											
					int y_lahde = zoom_y*x_reso + zoom_x;

					Uint32 *kohde_os = raw_pixels;
					Uint32 kohta_y_summa = 0;

					for(int y = 0; y < lop_kork; y++)
					{
						
						Uint32 *riviosoitin = &muisti[y_lahde];
						Uint32 *riviosoitin2 = &muisti[y_lahde+x_reso];

						if(kohta_y_summa == 0)
						{
							Uint32 kohta_x_summa = 0;
							
							for(int x = 0; x < lop_lev;x++)
							{
								
								
								if(kohta_x_summa == 0)
								{
									*kohde_os = *riviosoitin;
									

								}
								else
								{
									float x_suhde = (float)kohta_x_summa/(float)d_leveys;
									
									float k_x_suhde = 1-x_suhde;



									pikseli_1 = *riviosoitin;
									pikseli_2 = riviosoitin[1];

									Uint32 puna_1 = (pikseli_1 & punamaski) >> fmt->Rshift;
									Uint32 puna_2 = (pikseli_2 & punamaski) >> fmt->Rshift;

									Uint32 viher_1 = (pikseli_1 & vihermaski) >> fmt->Gshift;
									Uint32 viher_2 = (pikseli_2 & vihermaski) >> fmt->Gshift;

									Uint32 sini_1 = (pikseli_1 & sinimaski) >> fmt->Bshift;
									Uint32 sini_2 = (pikseli_2 & sinimaski) >> fmt->Bshift;

									Uint32 f_puna = (Uint32)((float)puna_1*k_x_suhde + (float)puna_2*x_suhde);
									Uint32 f_viher = (Uint32)((float)viher_1*k_x_suhde + (float)viher_2*x_suhde);
									Uint32 f_sini = (Uint32)((float)sini_1*k_x_suhde + (float)sini_2*x_suhde);
									
									Uint32 tulos = ((f_puna) << fmt->Rshift)
										| ((f_viher) << fmt->Gshift)
										| ((f_sini) << fmt->Bshift);

									*kohde_os = tulos;

								}

								kohde_os++;
								kohta_x_summa += s_leveys;

								while(kohta_x_summa >= d_leveys)
								{
									kohta_x_summa -= d_leveys;
									riviosoitin++;
								}
							}

							
						}
						else
						{

							float y_suhde = (float)kohta_y_summa/(float)d_korkeus;
							
							float k_y_suhde = 1-y_suhde;

							Uint32 kohta_x_summa = 0;
							
							for(int x = 0; x < lop_lev; x++)
							{
								
								

								if(kohta_x_summa == 0)
								{
									pikseli_1 = *riviosoitin;
									pikseli_2 = *riviosoitin2;

									Uint32 puna_1 = (pikseli_1 & punamaski) >> fmt->Rshift;
									Uint32 puna_2 = (pikseli_2 & punamaski) >> fmt->Rshift;

									Uint32 viher_1 = (pikseli_1 & vihermaski) >> fmt->Gshift;
									Uint32 viher_2 = (pikseli_2 & vihermaski) >> fmt->Gshift;

									Uint32 sini_1 = (pikseli_1 & sinimaski) >> fmt->Bshift;
									Uint32 sini_2 = (pikseli_2 & sinimaski) >> fmt->Bshift;

									Uint32 f_puna = (Uint32)((float)puna_1*k_y_suhde + (float)puna_2*y_suhde);
									Uint32 f_viher = (Uint32)((float)viher_1*k_y_suhde + (float)viher_2*y_suhde);
									Uint32 f_sini = (Uint32)((float)sini_1*k_y_suhde + (float)sini_2*y_suhde);
									
									Uint32 tulos = (f_puna << fmt->Rshift)
										| (f_viher << fmt->Gshift)
										| (f_sini << fmt->Bshift);


									*kohde_os = tulos;
								}
								else
								{
									float x_suhde = (float)kohta_x_summa/(float)d_leveys;
									
									float k_x_suhde = 1-x_suhde;


									pikseli_1 = *riviosoitin;
									pikseli_2 = riviosoitin[1];
									pikseli_3 = *riviosoitin2;
									pikseli_4 = riviosoitin2[1];

									Uint32 puna_1 = (pikseli_1 & punamaski) >> fmt->Rshift;
									Uint32 puna_2 = (pikseli_2 & punamaski) >> fmt->Rshift;
									Uint32 puna_3 = (pikseli_3 & punamaski) >> fmt->Rshift;
									Uint32 puna_4 = (pikseli_4 & punamaski) >> fmt->Rshift;

									Uint32 viher_1 = (pikseli_1 & vihermaski) >> fmt->Gshift;
									Uint32 viher_2 = (pikseli_2 & vihermaski) >> fmt->Gshift;
									Uint32 viher_3 = (pikseli_3 & vihermaski) >> fmt->Gshift;
									Uint32 viher_4 = (pikseli_4 & vihermaski) >> fmt->Gshift;

									Uint32 sini_1 = (pikseli_1 & sinimaski) >> fmt->Bshift;
									Uint32 sini_2 = (pikseli_2 & sinimaski) >> fmt->Bshift;
									Uint32 sini_3 = (pikseli_3 & sinimaski) >> fmt->Bshift;
									Uint32 sini_4 = (pikseli_4 & sinimaski) >> fmt->Bshift;

									Uint8 f_puna = (Uint8)(((float)puna_1*k_x_suhde + (float)puna_2*x_suhde)*k_y_suhde
										+ ((float)puna_3*k_x_suhde + (float)puna_4*x_suhde)*y_suhde);

									Uint8 f_viher = (Uint8)(((float)viher_1*k_x_suhde + (float)viher_2*x_suhde)*k_y_suhde
										+ ((float)viher_3*k_x_suhde + (float)viher_4*x_suhde)*y_suhde);

									Uint8 f_sini = (Uint8)(((float)sini_1*k_x_suhde + (float)sini_2*x_suhde)*k_y_suhde
										+ ((float)sini_3*k_x_suhde + (float)sini_4*x_suhde)*y_suhde);


									Uint32 tulos = ((f_puna) << fmt->Rshift)
										| ((f_viher) << fmt->Gshift)
										| ((f_sini) << fmt->Bshift);

									*kohde_os = tulos;

								}


								kohde_os++;
								kohta_x_summa += s_leveys;

								while(kohta_x_summa >= d_leveys)
								{
									kohta_x_summa -= d_leveys;
									riviosoitin++;
									riviosoitin2++;
								}
							}
						}

						kohta_y_summa += s_korkeus;

						while(kohta_y_summa >= d_korkeus)
						{
							kohta_y_summa -= d_korkeus;
							y_lahde += x_reso;
						}

						
						kohde_os += sdl_screen->pitch/4 -lop_lev; 

					} // end y_loop

					SDL_UnlockSurface(sdl_screen);

	return true;
}


bool Grafiikka::zoom16moodi0()
{
	int alk_lev = zoom_lev, alk_kork = zoom_kork, lop_lev = x_oikea, lop_kork = y_oikea;
										
	SDL_LockSurface(valipinta);
	Uint16 *pixelit_16 = (Uint16 *)valipinta->pixels;
	memcpy(buffer_16, pixelit_16, x_reso*y_reso*sizeof(Uint16));
	SDL_UnlockSurface(valipinta);


	SDL_LockSurface(sdl_screen);


	Uint16 *kohdepixelit_16 = (Uint16 *)sdl_screen->pixels;				
	Uint16 *muisti = buffer_16;
	Uint16 *raw_pixels = kohdepixelit_16;

	int s_leveys = (alk_lev-1);
	int d_leveys = (lop_lev-1);

	int s_korkeus = (alk_kork-1);
	int d_korkeus = (lop_kork-1);
											
	int y_lahde = zoom_y*x_reso + zoom_x;

	Uint16 *kohde_os = raw_pixels;
	Uint16 kohta_y_summa = 0;

	for(int y = 0; y < lop_kork; y++)
	{
						
		Uint16 *riviosoitin = &muisti[y_lahde];
		Uint16 kohta_x_summa = 0;
						
		for(int x = 0; x < lop_lev;x++)
		{
			*kohde_os = *riviosoitin;

			kohde_os++;
			kohta_x_summa += s_leveys;

			while(kohta_x_summa >= d_leveys)
			{
				kohta_x_summa -= d_leveys;
				riviosoitin++;
			}
		}
						

		kohta_y_summa += s_korkeus;

		while(kohta_y_summa >= d_korkeus)
		{
			kohta_y_summa -= d_korkeus;
			y_lahde += x_reso;
		}

						
		kohde_os += sdl_screen->pitch/2 -lop_lev; 

	}

	SDL_UnlockSurface(sdl_screen);

	return true;
}

bool Grafiikka::Skaalaus()
{
	if(!moodi) return false;


	switch(moodi)
	{
	case 1:
		if(bittia == 32)
		{
			if(kolmiot) piirraKolmiot();
			else if(rullaus_y > 0) rullaus32();

			else if(zoom_lev != x_reso || zoom_kork != y_reso)

			switch(skaalaus)
			{
			case 0:
				zoom32moodi0();
				break;
			case 1:
				zoom32moodi1();
				break;
			case 2:
				zoom32moodi2();
				break;
			default:
				break;
			}
		}	// end if 32		
		else
		{
			if(zoom_lev != x_reso || zoom_kork != y_reso)
			{
				zoom16moodi0();
			}
		} // end 16

		break;

	default:
		break;
	}

	///zoom_x = 0;
	//zoom_y = 0;
	//zoom_lev = x_reso;
	//zoom_kork = y_reso;

	
	return true;
}

bool Grafiikka::Flip()
{
	switch(moodi)
	{
	case 1:
		
		//SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
		SDL_Flip(sdl_screen);
		break;
	}

	zoom_x = seuraava_zoom_x;
	zoom_y = seuraava_zoom_y;

	zoom_lev = seuraava_zoom_lev;
	zoom_kork = seuraava_zoom_kork;

	rullaus_y = seuraava_rullaus_y;

	kolmiot = seuraava_kolmiot;

	return true;
}

bool Grafiikka::Sulje()
{
	FSOUND_Close();

	if(!moodi) return false;

	switch(moodi)
	{
	case 1:
		SDL_Quit();
		break;
	default: break;
	}

	moodi = 0;
	return true;
}

Grafiikka::~Grafiikka()
{
	if(moodi)
	{
		Sulje();
	}
}

bool Grafiikka::VapautaSprite(Sprite *sprite)
{
	if(!moodi) return false;

	switch(moodi)
	{
	case 1:
		if(sprite->sdl_data)
		{
			SDL_FreeSurface(sprite->sdl_data);
			sprite->sdl_data = 0;
		}
		break;
	default: break;
	}

	return true;
}

bool Grafiikka::LataaSprite(Sprite *sprite, const char *tiedostonimi)
{
	

	if(!moodi) return false;
	
	SDL_Surface *temp;
	switch(moodi)
	{
	case 1:
		temp = SDL_LoadBMP(tiedostonimi);
		if(temp == NULL) return false;

		sprite->sdl_data = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);

		sprite->width = sprite->sdl_data->w;
		sprite->height = sprite->sdl_data->h;
		break;

	default: break;
	}

	return true;

}

bool Grafiikka::LataaSprite2(Sprite *sprite, const char *tiedostonimi)
{
	std::ifstream tiedosto(tiedostonimi, std::ios_base::binary);
	if(!tiedosto) return false;
	int leveys, korkeus;
	unsigned char pituus;
	tiedosto.read((char*)&leveys,sizeof(unsigned int));
	tiedosto.read((char*)&korkeus,sizeof(unsigned int));
	unsigned char r,g,b;

	SDL_Surface *kuva = 0;
	Uint32 *pixelit_32;
	Uint16 *pixelit_16;
	int indeksi = 0;
	Uint32 pikseli32;
	Uint16 pikseli16;

	if(!moodi) return false;
	SDL_PixelFormat *fmt;

	int ax = 0, yy = 0;
	int y_kohta = 0;
	
	
	int p = 0;
	switch(moodi)
	{
	case 1:

		kuva = SDL_CreateRGBSurface(SDL_HWSURFACE, leveys, korkeus, bittia, 0, 0, 0, 0);

		if(kuva == NULL) return false;

		SDL_LockSurface(kuva);
		p = kuva->pitch;
		if(bittia == 32) pixelit_32 = (Uint32 *)kuva->pixels;
		else pixelit_16 = (Uint16 *)kuva->pixels;
		fmt = kuva->format;

		if(bittia == 32) while(yy < korkeus)
		{
			tiedosto.read((char*)&r,sizeof(unsigned char));
			tiedosto.read((char*)&g,sizeof(unsigned char));
			tiedosto.read((char*)&b,sizeof(unsigned char));

			pikseli32 = (r >> fmt->Rloss) << fmt->Rshift | (g >> fmt->Gloss) << fmt->Gshift | (b >> fmt->Bloss) << fmt->Bshift;

			tiedosto.read((char*)&pituus,sizeof(unsigned char));

			for(int index = 0; index < pituus; index++)
			{
				pixelit_32[y_kohta +ax] = pikseli32;

				ax++;
				if(ax == leveys) {yy++;ax = 0; y_kohta += p >> 2;}
			}
			indeksi += pituus;
		}
		else while(yy < korkeus)
		{
			tiedosto.read((char*)&r,sizeof(unsigned char));
			tiedosto.read((char*)&g,sizeof(unsigned char));
			tiedosto.read((char*)&b,sizeof(unsigned char));

			pikseli16 = (r >> fmt->Rloss) << fmt->Rshift | (g >> fmt->Gloss) << fmt->Gshift | (b >> fmt->Bloss) << fmt->Bshift;

			tiedosto.read((char*)&pituus,sizeof(unsigned char));

			for(int index = 0; index < pituus; index++)
			{
				pixelit_16[y_kohta +ax] = pikseli16;

				ax++;
				if(ax == leveys) {yy++;ax = 0; y_kohta += p >> 1;}
			}
		}

		SDL_UnlockSurface(kuva);

		
		sprite->sdl_data = kuva;
		break;

	default: break;
	}

	tiedosto.close();

	return true;

}

bool Grafiikka::TyhjennaRuutu()
{
	if(!moodi) return false;

	

	switch(moodi)
	{
	case 1:
		SDL_Rect kohde;
		kohde.x = 0;
		kohde.y = 0;
		kohde.w = x_reso;
		kohde.h = y_reso;
		
		if(zoom_lev != x_reso || zoom_kork != y_reso || rullaus_y > 0 || kolmiot) SDL_FillRect(valipinta, &kohde, SDL_MapRGB(sdl_screen->format, 0, 0, 0));
		else
		{
			kohde.x += heitto_x;
			kohde.y += heitto_y;
			
			SDL_FillRect(sdl_screen, &kohde, SDL_MapRGB(sdl_screen->format, 0, 0, 0));
		}

		break;
	default: break;
	}

	return true;
}

bool Grafiikka::Input()
{
	if(!moodi) return false;

	Uint8 hiirinapit;
	int indeksi;

	switch(moodi)
	{
		case 1:
			SDL_Event tapahtuma; // tapahtumat
			SDL_PollEvent(&tapahtuma);
			inputti.nappi = SDL_GetKeyState(NULL);
			
			
			unsigned char joysticknappi[24];
			
			if(padi)
			{
				int x = SDL_JoystickGetAxis(padi, 0);
				int y = SDL_JoystickGetAxis(padi, 1);
				
				if(x <= -4096) joysticknappi[0] = 1; else joysticknappi[0] = 0;
				if(x >= 4096) joysticknappi[1] = 1; else joysticknappi[1] = 0;
				
				if(y <= -4096) joysticknappi[2] = 1; else joysticknappi[2] = 0;
				if(y >= 4096) joysticknappi[3] = 1; else joysticknappi[3] = 0;

				for(indeksi = 0; indeksi < 20; indeksi++)
				{
					if(SDL_JoystickGetButton(padi, indeksi)) joysticknappi[indeksi+4] = 1;
					else joysticknappi[indeksi+4] = 0;
				}
				
				for(indeksi = 0; indeksi < 24; indeksi++)
				{
					if(joysticknappi[indeksi])
					{
						if(inputti.JOYSTICK_POHJASSA[indeksi] == 0)
						{
							inputti.JOYSTICK_POHJASSA[indeksi] = 1;
							inputti.JOYSTICK_PAINETTU[indeksi] = 1;
						} else inputti.JOYSTICK_PAINETTU[indeksi] = 0;
					}
					else
					{
						inputti.JOYSTICK_POHJASSA[indeksi] = 0;
						inputti.JOYSTICK_PAINETTU[indeksi] = 0;
					}
				}
				
			}

			
			for(indeksi = 0; indeksi < 1000; indeksi++)
			{
				if(inputti.nappi[indeksi])
				{
					if(inputti.NAPPI_POHJASSA[indeksi]  == 0)
					{
						inputti.NAPPI_POHJASSA[indeksi] = 1;
						inputti.NAPPI_PAINETTU[indeksi] = 1;
					}
					else inputti.NAPPI_PAINETTU[indeksi] = 0;
				}
				else
				{
					inputti.NAPPI_POHJASSA[indeksi] = 0;
					inputti.NAPPI_PAINETTU[indeksi] = 0;
				}
			}
			
			hiirinapit = SDL_GetRelativeMouseState(&(inputti.hiiri_x_siirto), &(inputti.hiiri_y_siirto));
			
			for(indeksi = 0; indeksi < 3; indeksi++)
			{
				if(hiirinapit & SDL_BUTTON(indeksi+1))
				{
					if(inputti.HIIRINAPPI_POHJASSA[indeksi]  == 0)
					{
						inputti.HIIRINAPPI_POHJASSA[indeksi] = 1;
						inputti.HIIRINAPPI_PAINETTU[indeksi] = 1;
						inputti.HIIRINAPPI_ESTO[indeksi] = 0;
					}
					else inputti.HIIRINAPPI_PAINETTU[indeksi] = 0;
				}
				else
				{
					inputti.HIIRINAPPI_POHJASSA[indeksi] = 0;
					inputti.HIIRINAPPI_PAINETTU[indeksi] = 0;
				}
			}
			
			/*			
			for(index = 0; index < 6; index++)
			{
				if(inputti.NAPPI_PAINETTU[glob_data->keyboard_indeksit[index]] ||
					((glob_data->saadot & 2) && padi && inputti.JOYSTICK_PAINETTU[glob_data->gamepad_indeksit[index]])
					) inputti.PAINETTU[index] = 1;
				else inputti.PAINETTU[index] = 0;
				
				if(inputti.NAPPI_POHJASSA[glob_data->keyboard_indeksit[index]] ||
					((glob_data->saadot & 2) && padi && inputti.JOYSTICK_POHJASSA[glob_data->gamepad_indeksit[index]])
					) inputti.POHJASSA[index] = 1;
				else inputti.POHJASSA[index] = 0;
			}
			*/
			break;

		default: break;
	}

	return true;
}

bool Grafiikka::nollaaInput()
{
	if(!moodi) return false;
	int indeksi;

	switch(moodi)
	{
		case 1:
			
			if(padi)
			{
				for(indeksi = 0; indeksi < 20; indeksi++)
				{
					inputti.JOYSTICK_PAINETTU[indeksi] = 0;
				}
				
			}

			
			for(indeksi = 0; indeksi < 1000; indeksi++)
			{
				inputti.NAPPI_PAINETTU[indeksi] = 0;
			}
			
			for(indeksi = 0; indeksi < 3; indeksi++)
			{
				inputti.HIIRINAPPI_PAINETTU[indeksi] = 0;
			}
			
					/*	
			for(index = 0; index < 6; index++)
			{
				inputti.PAINETTU[index] = 0;
			}
			*/
			break;

		default: break;
	}

	return true;
}

bool Grafiikka::PiirraSprite(Sprite *sprite, int x, int y, Rect *source, bool variavain,int alpha)
{
	if(!moodi) return false;

	int lev = 0;
	int kork = 0;
	int lahde_x = 0, lahde_y = 0;

	switch(moodi)
	{
	case 1:
		SDL_Rect sdl_source, sdl_dest;

		if(source)
		{
			lev = source->width;
			kork = source->height;
			lahde_x = source->x;
			lahde_y = source->y;
		}
		else
		{
			lev = sprite->width;
			kork = sprite->height;
			lahde_x = 0;
			lahde_y = 0;
		}

		if(x >= x_reso) return true;
		if(y >= y_reso) return true;
		if(x + lev <= 0) return true;
		if(y + kork <= 0) return true;

		if(x < 0)
		{
			lahde_x += -x;
			lev += x;
			x = 0;
		}
		if(y < 0)
		{
			lahde_y += -y;
			kork += y;
			y = 0;
		}

		if(x + lev > x_reso)
		{
			lev = x_reso - x;
		}

		if(y + kork > y_reso)
		{
			kork = y_reso -y;
		}

		sdl_source.x = lahde_x;
		sdl_source.y = lahde_y;
		sdl_source.w = lev;
		sdl_source.h = kork;

		
		sdl_dest.x = x;
		sdl_dest.y = y;
		sdl_dest.w = lev;
		sdl_dest.h = kork;

		if(!skaalaus)
		{
			sdl_dest.x += heitto_x;
			sdl_dest.y += heitto_y;
		}

		if(alpha)
		{
			SDL_SetAlpha(sprite->sdl_data, SDL_SRCALPHA, alpha);
		}

		if(variavain) SDL_SetColorKey(sprite->sdl_data, SDL_SRCCOLORKEY, SDL_MapRGB((sprite->sdl_data)->format,255,0,255));

		
		// kun rullataan tai zoomataan, k‰ytet‰‰n v‰lipintaa
		if(zoom_lev != x_reso || zoom_kork != y_reso || rullaus_y > 0 || kolmiot)
			SDL_BlitSurface(sprite->sdl_data, &sdl_source, valipinta, &sdl_dest);
		else
			SDL_BlitSurface(sprite->sdl_data, &sdl_source, sdl_screen, &sdl_dest);
		

		if(variavain) SDL_SetColorKey(sprite->sdl_data, 0, 0);

		if(alpha)
		{
			SDL_SetAlpha(sprite->sdl_data, SDL_SRCALPHA, 255);
		}

		break;
	default: break;
	}

	return true;
}

void Grafiikka::lataaBiisi(const char *tiedostonimi, Biisi *biisi)
{
	biisi->handle = FMUSIC_LoadSong(tiedostonimi);
}

void Grafiikka::vapautaBiisi(Biisi *biisi)
{
	FMUSIC_FreeSong(biisi->handle);
}

void Grafiikka::soitaBiisi(Biisi *biisi)
{
	FMUSIC_PlaySong(biisi->handle);
}

void Grafiikka::pysaytaBiisi(Biisi *biisi)
{
	FMUSIC_StopSong(biisi->handle);
}

void Grafiikka::lataaAani(const char *tiedostonimi, Aani *aani)
{
	aani->handle = FSOUND_Sample_Load(aaniindeksi, tiedostonimi, 0,0,0);
	aaniindeksi++;

	FSOUND_Sample_SetMode(aani->handle, FSOUND_LOOP_OFF);
};

void Grafiikka::vapautaAani(Aani *aani)
{
	FSOUND_Sample_Free(aani->handle);
};

void Grafiikka::soitaAani(Aani *aani)
{
	FSOUND_PlaySound(FSOUND_FREE,aani->handle);
	
};

bool Grafiikka::Alusta(int x, int y, int oikea_x, int oikea_y, int bit, int m, int h_x, int h_y, int s)
{

	FSOUND_Init (44100, 32, 0);
	
	aaniindeksi = 0;

	switch(m)
	{
	case 1: // SDL
		
		if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK/*| SDL_OPENGL*/) < 0) return false;
		sdl_screen = SDL_SetVideoMode(oikea_x, oikea_y, bit, SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF);
		if(sdl_screen == NULL) return false;
		SDL_ShowCursor(0);

		
		padi = 0;
		
		if(SDL_NumJoysticks()) padi = SDL_JoystickOpen(0);
		SDL_JoystickEventState(SDL_ENABLE);

		inputti.HIIRINAPPI_ESTO[0] = 0;
		inputti.HIIRINAPPI_ESTO[1] = 0;
		inputti.HIIRINAPPI_ESTO[2] = 0;


		valipinta = SDL_CreateRGBSurface(SDL_HWSURFACE, x, y, bit, 0, 0, 0, 0);
		buffer_16 = 0;
		buffer_32 = 0;
		if(bit == 16) buffer_16 = new Uint16[x*y];
		else buffer_32 = new Uint32[x*y];

		break;

	default: return false; break;
	}

	kolmiomaara = 0;

	kolmiot = 0;
	seuraava_kolmiot = 0;

	skaalaus = s;
	rullaus_y = 0;
	seuraava_rullaus_y = 0;
	x_oikea = oikea_x;
	y_oikea = oikea_y;

	heitto_x = h_x;
	heitto_y = h_y;
	x_reso = x;
	y_reso = y;

	zoom_x = 0;
	zoom_y = 0;
	zoom_lev = x_reso;
	zoom_kork = y_reso;

	seuraava_zoom_x = 0;
	seuraava_zoom_y = 0;
	seuraava_zoom_lev = x_reso;
	seuraava_zoom_kork = y_reso;

	moodi = m;
	bittia = bit;

	

	return true;
}


