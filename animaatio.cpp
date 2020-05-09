#include "maaritykset.h"

void piirraAnimaatio(Animaatio *anim, int x, int y, int *ruutu_counter, int *anim_ruutu, GLOBAALI *glob_data)
{
	Sprite *sprite = anim->osoitin;

	if((*anim_ruutu) >= anim->laatikot.size()) (*anim_ruutu) = 0;

	Rect laatikko = anim->laatikot.at(*anim_ruutu);
	int x_offset = anim->offsetit.at(*anim_ruutu).x;
	int y_offset = anim->offsetit.at(*anim_ruutu).y;

	glob_data->gfx->PiirraSprite(sprite, x-x_offset,
		y-y_offset,&laatikko,1,0);	

	//char *teksti;
	//number_to_string(*anim_ruutu, &teksti);
	//Tulosta(teksti, x,y,glob_data);
	//delete []teksti;
}

void animoi(Animaatio *anim, int *ruutu_counter, int *anim_ruutu)
{
	(*ruutu_counter)++;
	if((*ruutu_counter) >= anim->nopeus)
	{
		*ruutu_counter = 0;
		(*anim_ruutu)++;
	}

	if((*anim_ruutu) >= anim->laatikot.size())
			(*anim_ruutu) = 0;
}