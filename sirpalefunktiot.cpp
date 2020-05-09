#include "maaritykset.h"


const int sirpalesarakkeet = 31;
const int sirpalerivit = 23;
const int sivu = 20;
const int sirpalemaara = 2*sirpalesarakkeet*sirpalerivit;

int x_koord[sirpalemaara*3], y_koord[sirpalemaara*3], x_text[sirpalemaara*3], y_text[sirpalemaara*3];


struct Sirpale
{
	double x_keskus;
	double y_keskus;

	double x_nopeus;
	double y_nopeus;

	double rotaationopeus, rotaationopeus2;

	double x_kulmat[3], y_kulmat[3], z_kulmat[3];

	int x_text[3];
	int y_text[3];


};

Sirpale sirpaleet[sirpalemaara];


void kasitteleSirpaleet()
{
	for(int index = 0; index < sirpalemaara; index++)
	{
		Sirpale *os = &sirpaleet[index];

		os->x_keskus += os->x_nopeus;
		os->y_keskus += os->y_nopeus;

		os->y_nopeus += 0.1;

		for(int kulma = 0; kulma < 3; kulma++)
		{
			double nopeus = os->rotaationopeus;

			double uusi_x = os->x_kulmat[kulma]*cos(nopeus) - os->z_kulmat[kulma]*sin(nopeus);
			double uusi_z = os->z_kulmat[kulma]*cos(nopeus) + os->x_kulmat[kulma]*sin(nopeus);

			os->x_kulmat[kulma] = uusi_x;
			os->z_kulmat[kulma] = uusi_z;

			nopeus = os->rotaationopeus2;

			double uusi_y = os->y_kulmat[kulma]*cos(nopeus) - os->z_kulmat[kulma]*sin(nopeus);
			uusi_z = os->z_kulmat[kulma]*cos(nopeus) + os->y_kulmat[kulma]*sin(nopeus);

			os->y_kulmat[kulma] = uusi_y;
			os->z_kulmat[kulma] = uusi_z;
		}
	}

	for(index = 0; index < sirpalemaara; index++)
	{
		Sirpale *os = &sirpaleet[index];

		x_koord[index*3] = os->x_keskus+os->x_kulmat[0];
		y_koord[index*3] = os->y_keskus+os->y_kulmat[0];

		x_koord[index*3+1] = os->x_keskus+os->x_kulmat[1];
		y_koord[index*3+1] = os->y_keskus+os->y_kulmat[1];

		x_koord[index*3+2] = os->x_keskus+os->x_kulmat[2];
		y_koord[index*3+2] = os->y_keskus+os->y_kulmat[2];

	}
}

const kolmiorivit = 15, kolmiosarakkeet = 20;
const int kertoma = 32;
const int kolmiomaara = kolmiorivit*kolmiosarakkeet*2;
const int offset = 20;
const int heitto = 9;

void varina(GLOBAALI *glob_data)
{
	double  kulma = (double)glob_data->gfx->AnnaAika()/50;
	int index = 0;
	for(int rivi = 0; rivi < kolmiorivit; rivi++)
	{
		for(int sarake = 0; sarake < kolmiosarakkeet; sarake++)
		{
			x_koord[index*3] = sarake*kertoma;
			y_koord[index*3] = rivi *kertoma;

			x_koord[index*3+1] = (sarake+1)*kertoma -1;
			y_koord[index*3+1] = rivi*kertoma;

			x_koord[index*3+2] = sarake*32;
			y_koord[index*3+2] = (rivi+1)*kertoma -1;
			
			x_text[index*3] = heitto+(639 -2*heitto)*sarake/kolmiosarakkeet;
			y_text[index*3] = heitto+(479 -2*heitto)*rivi/kolmiorivit;

			x_text[index*3+1] = heitto+(639 -2*heitto)*(sarake+1)/kolmiosarakkeet;
			y_text[index*3+1] = heitto+(479 -2*heitto)*rivi/kolmiorivit;

			x_text[index*3+2] = heitto+(639 -2*heitto)*sarake/kolmiosarakkeet;
			y_text[index*3+2] = heitto+(479 -2*heitto)*(rivi+1)/kolmiorivit;
						
			for(int i = 0; i < 3; i++)
			{
				
				

				double etaisyys = sqrt(x_koord[index*3+i] * x_koord[index*3+i] + y_koord[index*3+i] * y_koord[index*3+i]);

				x_text[index*3+i] += heitto*sin(etaisyys*0.03 + kulma*3);
				y_text[index*3+i] += heitto*sin(etaisyys*0.03 + kulma*3);


			}

			index++;



			x_koord[index*3+0] = (sarake+1)*kertoma -1;
			y_koord[index*3+0] = rivi*kertoma;

			x_koord[index*3+1] = (sarake+1)*kertoma -1;
			y_koord[index*3+1] = (rivi+1)*kertoma -1;

			x_koord[index*3+2] = sarake*kertoma;
			y_koord[index*3+2] = (rivi+1)*kertoma -1;

			x_text[index*3] = heitto+(639 -2*heitto)*(sarake+1)/kolmiosarakkeet;
			y_text[index*3] = heitto+(479 -2*heitto)*rivi/kolmiorivit;

			x_text[index*3+1] = heitto+(639 -2*heitto)*(sarake+1)/kolmiosarakkeet;
			y_text[index*3+1] = heitto+(479 -2*heitto)*(rivi+1)/kolmiorivit;

			x_text[index*3+2] = heitto+(639 -2*heitto)*sarake/kolmiosarakkeet;
			y_text[index*3+2] = heitto+(479 -2*heitto)*(rivi+1)/kolmiorivit;

			for(i = 0; i < 3; i++)
			{
			
				double etaisyys = sqrt(x_koord[index*3+i] * x_koord[index*3+i] + y_koord[index*3+i] * y_koord[index*3+i]);

				x_text[index*3+i] += heitto*sin(etaisyys*0.03 + kulma*3);
				y_text[index*3+i] += heitto*sin(etaisyys*0.03 + kulma*3);
			}

			index++;
			
		}
	}
}

void alustaSirpaleet()
{
	int index = 0;

	for(int sarake = 0; sarake < sirpalesarakkeet; sarake++)
	{
		for(int rivi = 0; rivi < sirpalerivit; rivi++)
		{
			Sirpale *os = &sirpaleet[index];

			os->x_keskus = sarake*sivu + sivu/2;
			os->y_keskus = rivi*sivu + sivu/2;

			os->x_kulmat[0] = -sivu/2;
			os->y_kulmat[0] = -sivu/2;
			os->z_kulmat[0] = 0;

			os->x_kulmat[1] = sivu/2;
			os->y_kulmat[1] = -sivu/2;
			os->z_kulmat[1] = 0;

			os->x_kulmat[2] = -sivu/2;
			os->y_kulmat[2] = sivu/2;
			os->z_kulmat[2] = 0;

			os->x_text[0] = sarake*sivu;
			os->y_text[0] = rivi*sivu;

			os->x_text[1] = (sarake+1)*sivu;
			os->y_text[1] = rivi*sivu;

			os->x_text[2] = sarake*sivu;
			os->y_text[2] = (rivi+1)*sivu;




			os->x_nopeus = (double)(rand()%600) / 100 -3;
			os->y_nopeus = -(double)(rand()%300) / 100;

			os->rotaationopeus = (0.05)*(double)(rand()%1000)/(double)200;
			os->rotaationopeus2 = (0.1)*(double)(rand()%1000 + 100)/(double)200;
			

			index++;

			os = &sirpaleet[index];

			os->x_keskus = sarake*sivu + sivu/2;
			os->y_keskus = rivi*sivu + sivu/2;

			os->x_kulmat[0] = sivu/2;
			os->y_kulmat[0] = -sivu/2;
			os->z_kulmat[0] = 0;

			os->x_kulmat[1] = sivu/2;
			os->y_kulmat[1] = sivu/2;
			os->z_kulmat[1] = 0;

			os->x_kulmat[2] = -sivu/2;
			os->y_kulmat[2] = sivu/2;
			os->z_kulmat[2] = 0;

			os->x_text[0] = (sarake+1)*sivu;
			os->y_text[0] = rivi*sivu;

			os->x_text[1] = (sarake+1)*sivu;
			os->y_text[1] = (rivi+1)*sivu;

			os->x_text[2] = sarake*sivu;
			os->y_text[2] = (rivi+1)*sivu;

	

			os->x_nopeus = (double)(rand()%600) / 100 -3;
			os->y_nopeus = -(double)(rand()%300) / 100;

			os->rotaationopeus = (0.05)*(double)(rand()%1000)/(double)200;
			os->rotaationopeus2 = (0.1)*(double)(rand()%1000 + 100)/(double)200;




			index++;
		}
	}

	for(index = 0; index < sirpalemaara; index++)
	{
		Sirpale *os = &sirpaleet[index];

		x_koord[index*3] = os->x_keskus+os->x_kulmat[0];
		y_koord[index*3] = os->y_keskus+os->y_kulmat[0];

		x_koord[index*3+1] = os->x_keskus+os->x_kulmat[1];
		y_koord[index*3+1] = os->y_keskus+os->y_kulmat[1];

		x_koord[index*3+2] = os->x_keskus+os->x_kulmat[2];
		y_koord[index*3+2] = os->y_keskus+os->y_kulmat[2];

		x_text[index*3] = os->x_text[0];
		y_text[index*3] = os->y_text[0];

		x_text[index*3+1] = os->x_text[1];
		y_text[index*3+1] = os->y_text[1];

		x_text[index*3+2] = os->x_text[2];
		y_text[index*3+2] = os->y_text[2];

	}

}