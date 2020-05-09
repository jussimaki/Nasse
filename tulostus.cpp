#include "maaritykset.h"

int Tulosta(char *teksti, int x, int y, GLOBAALI *glob_data)
{
	int indeksi = 0;

	while(1)
	{
		if(teksti[indeksi] == '\0') break;
		else
		{
			if(teksti[indeksi] >= 32 && teksti[indeksi] <= 126)
			{
				Rect source;
				
	
				source.x = kirjaintaulukko[(teksti[indeksi] -32)*4];
				source.y = kirjaintaulukko[(teksti[indeksi] -32)*4 + 1];
				source.width = kirjaintaulukko[(teksti[indeksi] -32)*4 + 2];
				source.height = kirjaintaulukko[(teksti[indeksi] -32)*4 + 3];
	
				glob_data->gfx->PiirraSprite(glob_data->kirjaimet, x, y, &source,1,0);
				x += kirjaintaulukko[(teksti[indeksi] -32)*4 + 2];
			}
			indeksi++;
		}
	}

	return x;
}