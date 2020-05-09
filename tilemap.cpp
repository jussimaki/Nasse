#include "maaritykset.h"

void Tilemap::AsetaX(int x) {x_tilet = x;}
void Tilemap::AsetaY(int y) {y_tilet = y;}
int Tilemap::AnnaX() {return x_tilet;}
int Tilemap::AnnaY() {return y_tilet;}
unsigned char *Tilemap::AnnaData() {return data;}

void Tilemap::AsetaData(unsigned char *d)
{
	data = d;
}

void Tilemap::Alusta(int x, int y)
{
	x_tilet = x;
	y_tilet = y;

	data = new unsigned char[x*y];
	for(int index = 0; index < x*y; index++)
		data[index] = 0;
}


void Tilemap::Tuhoa()
{
	delete[] data;
	data = 0;
}

bool Tilemap::Piirra(int tileset, int kerros, int x_cam, int y_cam, GLOBAALI *glob_data, bool paallys)
{	
	if(kerros != 0 && paallys) return true;

	Sprite *sprite = glob_data->tilesetit.at(tileset)->kerrokset[kerros]->sprite;
	unsigned char *avaimet = 0;
	avaimet = glob_data->tilesetit.at(tileset)->kerrokset[kerros]->avaimet;

	int t_maara = glob_data->tilesetit.at(tileset)->kerrokset[kerros]->tilemaara;

	for(int looppi_x = 0; looppi_x < 20; looppi_x++)
	{
		for(int looppi_y = 0; looppi_y < 15; looppi_y++)
		{
			unsigned char tile = data[(looppi_y+y_cam)*x_tilet + looppi_x+x_cam];

			if(paallys)
				tile = glob_data->tilesetit.at(tileset)->kerrokset[kerros]->lapinakyvat[tile];

			if(tile && (tile < t_maara))
			{
				int kohde_x = looppi_x*32;
				int kohde_y = looppi_y*32;
				Rect lahde;
				lahde.x = 1+(tile%20)*33;
				lahde.y = 1+(int)(tile/20)*33;
				lahde.width = 32;
				lahde.height = 32;
		
				bool avain = 0;
				if(kerros != 2)
				{
					if(avaimet[tile] & 2) avain = 1; else avain = 0;
				

					if(avaimet[tile] & 4) glob_data->gfx->PiirraSprite(sprite, kohde_x, kohde_y, &lahde, avain, 128);
					else glob_data->gfx->PiirraSprite(sprite, kohde_x, kohde_y, &lahde, avain, 0);
				} else glob_data->gfx->PiirraSprite(sprite, kohde_x, kohde_y, &lahde, 0, 0);
			} 
		}
	}

	return true;
}

bool Tilemap::PiirraLeikepoyta(int tileset, int kerros, int x, int y, GLOBAALI *glob_data, bool paallys)
{
	
	if(kerros != 0 && paallys) return true;

	Sprite *sprite = glob_data->tilesetit.at(tileset)->kerrokset[kerros]->sprite;
	unsigned char *avaimet = 0;
	avaimet = glob_data->tilesetit.at(tileset)->kerrokset[kerros]->avaimet;

	for(int looppi_x = 0; looppi_x < x_tilet; looppi_x++)
	{
		for(int looppi_y = 0; looppi_y < y_tilet; looppi_y++)
		{
			int kohde_x = (looppi_x+x)*32;
			int kohde_y = (looppi_y+y)*32;

			unsigned char tile = data[looppi_x + looppi_y*x_tilet];

			if(paallys)
				tile = glob_data->tilesetit.at(tileset)->kerrokset[kerros]->lapinakyvat[tile];

			Rect lahde;
			lahde.x = 1+(tile%20)*33;
			lahde.y = 1+(int)(tile/20)*33;
			lahde.width = 32;
			lahde.height = 32;
			
			bool avain = 0;
			if(kerros != 2)
			{
				if(avaimet[tile] & 2) avain = 1; else avain = 0;
			
				if(avaimet[tile] & 4) glob_data->gfx->PiirraSprite(sprite, kohde_x, kohde_y, &lahde, avain, 64);
				else glob_data->gfx->PiirraSprite(sprite, kohde_x, kohde_y, &lahde, avain, 128);
			} else glob_data->gfx->PiirraSprite(sprite, kohde_x, kohde_y, &lahde, 0, 0);
		}
	}

	return true;
}


bool Tilemap::Tallenna(std::ofstream &tiedosto, int koodi)
{
	tiedosto.write((char*)&x_tilet, sizeof(int));
	tiedosto.write((char*)&y_tilet, sizeof(int));
	if(koodi < 2) tiedosto.write((char*)data, sizeof(unsigned char)*x_tilet*y_tilet);
	else
	{
		unsigned char laskuri = 0;
		int indeksi = 0;
		int rletila = 0;
		bool  check = 0;
		unsigned char tile = 0;

		while(indeksi < x_tilet*y_tilet)
		{
			switch(rletila)
			{
			case 0:
				tile = data[indeksi];
				check = 1;
				laskuri = 1;
				rletila = 1;
				break;

			case 1:
				if(data[indeksi] == tile)
				{
					laskuri++;
					if(laskuri == 127)
					{
						// kirjoita_data
						unsigned char maxx = 255;
						tiedosto.write((char*)&maxx,sizeof(unsigned char));
						tiedosto.write((char*)&tile,sizeof(unsigned char));


						check = 0;
						laskuri = 1;
						rletila = 0;
					}
				}
				else
				{
					// kirjoita data
					if(laskuri == 1)
					{
						if(tile <= 128)
						{
							tiedosto.write((char*)&tile,sizeof(unsigned char));
						}
						else
						{
							unsigned char yksitoisto = 129;
							tiedosto.write((char*)&yksitoisto,sizeof(unsigned char));
							tiedosto.write((char*)&tile,sizeof(unsigned char));
						}
					}
					else
					{
						unsigned char summa = 128 + laskuri;
						tiedosto.write((char*)&summa,sizeof(unsigned char));
						tiedosto.write((char*)&tile,sizeof(unsigned char));
					}
					tile = data[indeksi];
					check = 1;
					laskuri = 1;
				}
				break;
			}

			indeksi++;
		}

		if(check)
		{
			if(laskuri == 1)
			{
				if(tile <= 128)
				{
					tiedosto.write((char*)&tile,sizeof(unsigned char));
				}
				else
				{
					unsigned char yksitoisto = 129;
					tiedosto.write((char*)&yksitoisto,sizeof(unsigned char));
					tiedosto.write((char*)&tile,sizeof(unsigned char));
				}
			}
			else
			{
				unsigned char summa = 128 + laskuri;
				tiedosto.write((char*)&summa,sizeof(unsigned char));
				tiedosto.write((char*)&tile,sizeof(unsigned char));
			}
		}
	}

	return true;
}

bool Tilemap::Lataa(std::ifstream &tiedosto, GLOBAALI *glob_data, int tileset, int kerros, int koodi)
{
	int x = 0, y = 0;
	tiedosto.read((char*)&x, sizeof(int));
	tiedosto.read((char*)&y, sizeof(int));
	
	Alusta(x, y);

	if(koodi < 2)
	{
		tiedosto.read((char*)data, sizeof(unsigned char)*x_tilet*y_tilet);
	}
	else
	{
		int indeksi = 0;
		while(indeksi < x*y)
		{
			unsigned char tile, pituus;
			unsigned char luku;

			tiedosto.read((char*)&luku, sizeof(unsigned char));

			if(luku <= 128)
			{
				pituus = 1;
				tile = luku;
			}
			else
			{
				pituus = luku-128;
				tiedosto.read((char*)&tile, sizeof(unsigned char));
			}

			for(unsigned char index = 0; index < pituus; index++)
				data[indeksi+index] = tile;

			indeksi += pituus;

		}
	}

	
	int t_maara = glob_data->tilesetit.at(tileset)->kerrokset[kerros]->tilemaara;

	for(int index = 0; index < x_tilet*y_tilet; index++)
	{
		if(data[index] >= t_maara) data[index] = 0;
	}

	return true;
}

bool Tilemap::KopioiLeikepoydalle(Tilemap *leikepoyta, int alku_x, int alku_y, int loppu_x, int loppu_y)
{
	int final_alku_x, final_alku_y;
	int leveys, korkeus;
	
	if(loppu_x >= alku_x)
	{
		final_alku_x = alku_x;
		leveys = loppu_x-alku_x+1;
	}
	else
	{
		final_alku_x = loppu_x;
		leveys = alku_x - loppu_x +1;
	}

	if(loppu_y >= alku_y)
	{
		final_alku_y = alku_y;
		korkeus = loppu_y-alku_y+1;
	}
	else
	{
		final_alku_y = loppu_y;
		korkeus = alku_y - loppu_y +1;
	}

	leikepoyta->Alusta(leveys, korkeus);
	unsigned char *poyta = leikepoyta->AnnaData();
	
	int dest_index = 0;
	int source_index = final_alku_y*x_tilet+ final_alku_x;

	for(int looppi_y = 0; looppi_y < korkeus; looppi_y++)
	{
		memcpy((void*)&(poyta[dest_index]), (void*)&(data[source_index]), leveys*sizeof(unsigned char));

		dest_index += leveys;
		source_index += x_tilet;
	}

	return true;
}

bool Tilemap::OtaLeikepydalta(Tilemap *leikepoyta, int dest_x, int dest_y)
{
	int copy_width, copy_height;
	unsigned char *poyta = leikepoyta->AnnaData();

	int leveys = leikepoyta->AnnaX();
	int korkeus = leikepoyta->AnnaY();

	if(dest_x + leveys-1 >= x_tilet) copy_width = x_tilet - dest_x;
	else copy_width = leveys;

	if(dest_y + korkeus-1 >= y_tilet) copy_height = y_tilet - dest_y;
	else copy_height = korkeus;
	

	int dest_index = dest_y*x_tilet + dest_x;
	int source_index = 0;

		for(int y_looppi = 0; y_looppi < copy_height; y_looppi++)
		{
			memcpy((void*)&data[dest_index], (void*)&poyta[source_index], copy_width*sizeof(unsigned char));
			
			dest_index += x_tilet;
			source_index += leveys;
		}

	return 0;
}

bool Tilemap::x_siirto(int x)
{
	unsigned char* new_map = new unsigned char[x_tilet*y_tilet];

	memset(new_map,0,sizeof(unsigned char)*x_tilet*y_tilet);

	int dest_index, source_index;

	if(x >= 0)
	{
		dest_index = x;
		source_index = 0;

		for(int y = 0; y < y_tilet; y++)
		{
			memcpy(&(new_map[dest_index]), &(data[source_index]), (x_tilet-x)*sizeof(unsigned char));

			dest_index += x_tilet;
			source_index += x_tilet;
		}

	}
	else
	{
		dest_index = 0;
		source_index = -x;

		for(int y = 0; y < y_tilet; y++)
		{
			memcpy(&(new_map[dest_index]), &(data[source_index]), (x_tilet+x)*sizeof(char));

			dest_index += x_tilet;
			source_index += x_tilet;
		}
	}

	delete[] data;
	data = new_map;

	return true;
}

bool Tilemap::y_siirto(int y)
{
	unsigned char* new_map = new unsigned char[x_tilet*y_tilet];

	memset(new_map,0,sizeof(unsigned char)*x_tilet*y_tilet);

	int dest_index, source_index;

	if(y >= 0)
	{
		dest_index = y*x_tilet;
		source_index = 0;

		for(int looppi_y = 0; looppi_y < y_tilet-y; looppi_y++)
		{
			memcpy(&(new_map[dest_index]), &(data[source_index]), x_tilet*sizeof(unsigned char));

			dest_index += x_tilet;
			source_index += x_tilet;
		}

	}
	else
	{
		dest_index = 0;
		source_index = -y*x_tilet;

		for(int looppi_y = 0; looppi_y < y_tilet+y; looppi_y++)
		{
			memcpy(&(new_map[dest_index]), &(data[source_index]), x_tilet*sizeof(unsigned char));

			dest_index += x_tilet;
			source_index += x_tilet;
		}

	}

	delete[] data;
	data = new_map;

	return true;

}

bool Tilemap::MuutaKoko(int width, int height)
{
	int temp_x, temp_y;
	unsigned char *new_map;

	if(x_tilet > width) temp_x = width; else temp_x = x_tilet;
	if(y_tilet > height) temp_y = height; else temp_y = y_tilet;
	
	new_map = new unsigned char[width * height];
	memset(new_map, 0, width * height);

	for(int y = 0; y < temp_y; y++)
	{
		memcpy(new_map + (width * y), data + (x_tilet * y), temp_x);
	}

	delete[] data;
	data = new_map;

	x_tilet = width;
	y_tilet = height;

	return true;
}

void Tilemap::vasenTyonto(int x)
{
	for(int y = 0; y < y_tilet; y++)
	{
		for(int x_looppi = 0; x_looppi < x; x_looppi++)
		{
			data[x_tilet*y + x_looppi] = data[x_tilet*y + x_looppi+1];
		}

		data[x_tilet*y+x] = 0;
	}
}

void Tilemap::ylaTyonto(int y)
{
	for(int y_looppi = 0; y_looppi < y; y_looppi++)
	{
		memcpy(&data[x_tilet*y_looppi],&data[x_tilet*(y_looppi+1)],x_tilet*sizeof(char));
	}

	memset(&data[x_tilet*y],0,x_tilet*sizeof(char));
}

void Tilemap::ylaVeto(int y)
{
	for(int y_looppi = y; y_looppi < y_tilet-1; y_looppi++)
	{
		memcpy(&data[x_tilet*y_looppi],&data[x_tilet*(y_looppi+1)],x_tilet*sizeof(char));
	}

	memset(&data[x_tilet*(y_tilet-1)],0,x_tilet*sizeof(char));
}

void Tilemap::alaTyonto(int y)
{
	for(int y_looppi = y_tilet -1; y_looppi > y; y_looppi--)
	{
		memcpy(&data[x_tilet*y_looppi],&data[x_tilet*(y_looppi-1)],x_tilet*sizeof(char));
	}
	memset(&data[x_tilet*y],0,x_tilet*sizeof(char));
}

void Tilemap::alaVeto(int y)
{
	for(int y_looppi = y; y_looppi > 0; y_looppi--)
	{
		memcpy(&data[x_tilet*y_looppi],&data[x_tilet*(y_looppi-1)],x_tilet*sizeof(char));
	}
	memset(&data[0],0,x_tilet*sizeof(char));
}

void Tilemap::vasenVeto(int x)
{
	for(int y = 0; y < y_tilet; y++)
	{
		for(int x_looppi = x; x_looppi < x_tilet-1; x_looppi++)
		{
			data[x_tilet*y + x_looppi] = data[x_tilet*y + x_looppi+1];
		}

		data[x_tilet*y+x_tilet-1] = 0;
	}
}

void Tilemap::oikeaTyonto(int x)
{
	for(int y = 0; y < y_tilet; y++)
	{
		for(int x_looppi = x_tilet-1; x_looppi > x; x_looppi--)
		{
			data[x_tilet*y + x_looppi] = data[x_tilet*y + x_looppi-1];
		}

		data[x_tilet*y+x] = 0;
	}
}


void Tilemap::oikeaVeto(int x)
{
	for(int y = 0; y < y_tilet; y++)
	{
		for(int x_looppi = x; x_looppi > 0; x_looppi--)
		{
			data[x_tilet*y + x_looppi] = data[x_tilet*y + x_looppi-1];
		}

		data[x_tilet*y] = 0;
	}
}


int Tilemap::draw_scrolling_tilemap(GLOBAALI *glob_data, int x_cam, int y_cam, int tileset, bool paallys)
{
	Sprite *kuva = glob_data->tilesetit.at(tileset)->kerrokset[0]->sprite;
	int eka_x, eka_y;
	int x_tileja, y_tileja;
	int x_kohta = x_cam%32;
	int y_kohta = y_cam%32;

	eka_x = x_cam/32;
	if(x_cam%32 == 0) x_tileja = 20;
	else x_tileja = 21;

	eka_y = y_cam/32;
	if(y_cam%32 == 0) y_tileja = 15;
	else y_tileja = 16;

	unsigned char *mappi = data;
	unsigned char *tilemaski = glob_data->tilesetit.at(tileset)->kerrokset[0]->avaimet;

	int leveys = x_tilet;

	Anim_tile **animaatiotaulukko = glob_data->tilesetit.at(tileset)->kerrokset[0]->animoidut;
	int t_maara = glob_data->tilesetit.at(tileset)->kerrokset[0]->tilemaara;

/*
	bitti 0: 0 ei piirretä 1, piirretään
bitti 1: 0 peittää kokonaan ruudun, 1 avain käytössä
bitti 2: 0 ei läpinäkyvä, 1 läpinäkyvä
*/

	for(int looppi_x = 0; looppi_x < x_tileja; looppi_x++)
	{
		for(int looppi_y = 0; looppi_y < y_tileja; looppi_y++)
		{
			unsigned char tile = mappi[(looppi_y+eka_y)*leveys + looppi_x+eka_x];

			if(paallys) tile = glob_data->tilesetit.at(tileset)->kerrokset[0]->lapinakyvat[tile];
			
			if(!(tilemaski[tile] & 1)) continue;

			bool lapinakyvyys = 0;
			if(tilemaski[tile] & 4) lapinakyvyys = 1;

			if(tile && (tile < t_maara))
			{
				

				bool avain;
				if(tilemaski[tile] & 2) avain = 1; else avain = 0;
								
				
				int kohde_x = looppi_x*32 -x_kohta;
				int kohde_y = looppi_y*32 -y_kohta;
				Rect lahde;

				Anim_tile *anim = animaatiotaulukko[tile];

				if(anim)
				{
					int ruutu;
					if(anim->ruutu == 0) ruutu = tile;
					else ruutu = anim->ruudut.at(anim->ruutu-1);
					lahde.x = 1+(ruutu%20)*33;
					lahde.y = 1+(int)(ruutu/20)*33;
				}
				else
				{
					lahde.x = 1+(tile%20)*33;
					lahde.y = 1+(int)(tile/20)*33;
				}

				lahde.width = 32;
				lahde.height = 32;

				int *tileset_tiletyypit = glob_data->tilesetit.at(tileset)->kerrokset[0]->tyypit;
				unsigned char *tileset_lapinakyvat = glob_data->tilesetit.at(tileset)->kerrokset[0]->lapinakyvat;
				
				if(lapinakyvyys) 
				{
					glob_data->gfx->PiirraSprite(kuva,kohde_x, kohde_y, &lahde, avain, 128);	
				}
				else glob_data->gfx->PiirraSprite(kuva,kohde_x, kohde_y, &lahde, avain, 255);				
			}
		}
	}


	return 0;
}