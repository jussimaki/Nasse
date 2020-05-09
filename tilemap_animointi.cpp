#include "maaritykset.h"

bool ResetoiAnimaatiot(GLOBAALI *glob_data, int tileset)
{
	for(int index = 0; index < 3; index++)
	{
		Anim_tile **animaatiot = glob_data->tilesetit.at(tileset)->kerrokset[index]->animoidut;

		for(int tile = 0; tile < glob_data->tilesetit.at(tileset)->kerrokset[index]->tilemaara; tile++)
		{
			if(animaatiot[tile])
			{
				animaatiot[tile]->counter = 0;
				animaatiot[tile]->ruutu = 0;
			}
		}
	}

	return true;
}

bool AnimoiTilet(GLOBAALI *glob_data, int tileset)
{
	for(int index = 0; index < 3; index++)
	{
		Anim_tile **animaatiot = glob_data->tilesetit.at(tileset)->kerrokset[index]->animoidut;

		for(int tile = 0; tile < glob_data->tilesetit.at(tileset)->kerrokset[index]->tilemaara; tile++)
		{
			if(animaatiot[tile])
			{
				animaatiot[tile]->counter++;

				if(animaatiot[tile]->counter == animaatiot[tile]->nopeus)
				{
					animaatiot[tile]->counter = 0;

					animaatiot[tile]->ruutu++;
					if(animaatiot[tile]->ruutu == animaatiot[tile]->ruudut.size()+1)
						animaatiot[tile]->ruutu = 0;
				}
			}
		}
	}
	return true;
}