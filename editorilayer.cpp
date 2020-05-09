#include "maaritykset.h"

int Editorilayer::AnnaXcam(){return x_cam;}
int Editorilayer::AnnaYcam(){return y_cam;}
Tilemap *Editorilayer::annamappi(){return mappi;}
Tilemap *Editorilayer::annaleikepoyta(){return leikepoyta;}

bool Editorilayer::Hiiriskrollaus(GLOBAALI *glob_data)
{
	InputData *input = glob_data->gfx->annaInput();

	if(input->HIIRINAPPI_PAINETTU[KESKI]) {skrollaus = 1;  scroll_xv = 0; scroll_yv = 0; scroll_x = 0; scroll_y = 0;}
	if(!(input->HIIRINAPPI_POHJASSA[KESKI])) {skrollaus = 0;}

	if(skrollaus)
	{
		scroll_xv += input->hiiri_x_siirto;
		scroll_yv += input->hiiri_y_siirto;

		scroll_x += scroll_xv;
		scroll_y += scroll_yv;

		if(scroll_x > 250) {scroll_x = 0; x_cam++;}
		if(scroll_x < -250) {scroll_x = 0; x_cam--;}

		if(scroll_y > 250) {scroll_y = 0; y_cam++;}
		if(scroll_y < -250) {scroll_y = 0; y_cam--;}
	}

	return true;
}

void Editorilayer::KorjaaKamera()
{
	if(x_cam < 0) x_cam = 0;
	if(y_cam < 0) y_cam = 0;
	if(x_cam + 20 > mappi->AnnaX()) x_cam = mappi->AnnaX()-20;
	if(y_cam + 15 > mappi->AnnaY()) y_cam = mappi->AnnaY()-15;
}

bool Editorilayer::Tallenna(std::ofstream &tiedosto, int koodi)
{
	tiedosto.write((char*)&nopeus, sizeof(int));
	mappi->Tallenna(tiedosto,koodi);
	return true;
}

bool Editorilayer::Lataa(std::ifstream &tiedosto, GLOBAALI *glob_data,int tileset, int kerros, int koodi)
{
	tiedosto.read((char*)&nopeus, sizeof(int));
	mappi = new Tilemap;
	mappi->Lataa(tiedosto,glob_data,tileset,kerros,koodi);
	
	leikepoyta = new Tilemap;
	leikepoyta->Alusta(1,1);

	return true;
}

bool Editorilayer::PiirraTilemap(int tileset, int kerros, GLOBAALI *glob_data)
{
	mappi->Piirra(tileset, kerros, x_cam, y_cam, glob_data,0);
		mappi->Piirra(tileset, kerros, x_cam, y_cam, glob_data,1);
	return true;
}

bool Editorilayer::Alusta()
{
	nopeus = 0;
	x_cam = 0;
	y_cam = 0;

	mappi = new Tilemap;
	mappi->Alusta(20,15);

	leikepoyta = new Tilemap;
	leikepoyta->Alusta(1,1);

	return true;
}

bool Editorilayer::Tuhoa()
{
	mappi->Tuhoa();
	delete mappi;
	leikepoyta->Tuhoa();
	delete leikepoyta;
	return true;
}