#include "maaritykset.h"

void soitaGlobaaliBiisi(GLOBAALI *glob_data, Biisi *biisi)
{
	if(!glob_data->soivabiisi)
	{
		glob_data->soivabiisi = biisi;
		glob_data->gfx->soitaBiisi(biisi);
	}
	else if(glob_data->soivabiisi != biisi)
	{
		glob_data->gfx->pysaytaBiisi(glob_data->soivabiisi);
		glob_data->soivabiisi = biisi;
		glob_data->gfx->soitaBiisi(biisi);
	}
};

void pysaytaGlobaaliBiisi(GLOBAALI *glob_data)
{
	if(glob_data->soivabiisi)
	{
		glob_data->gfx->pysaytaBiisi(glob_data->soivabiisi);
		glob_data->soivabiisi = 0;
	}
}