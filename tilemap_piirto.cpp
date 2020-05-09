#include "maaritykset.h"


/*
	bitti 0: 0 ei piirretä 1, piirretään
bitti 1: 0 peittää kokonaan ruudun, 1 avain käytössä
bitti 2: 0 ei läpinäkyvä, 1 läpinäkyvä
*/


int poista_tausta(char *etu, char *tausta, int etu_x_cam, int etu_y_cam, int tausta_x_cam, int tausta_y_cam)
{
	int rivi;
	int x;

	if(etu_x_cam % 32 == tausta_x_cam % 32)
	{
		if(etu_y_cam % 32 == tausta_y_cam % 32)
		{
			for(rivi = 0; rivi < 15; rivi++)
			{
				for(x  = 0; x < 20; x++)
				{
					if(!(etu[21*rivi+x] & 6) && (etu[21*rivi+x] & 1)) tausta[21*rivi+x] = 0;
				}
			}
		}
		else
		{
			int y_plus;
			if(tausta_y_cam % 32 > etu_y_cam % 32) y_plus = 1; else y_plus = 0;

			for(rivi = y_plus; rivi < 16; rivi++)
			{
				for(x = 0; x < 20; x++)
				{
					if(!(etu[21*(rivi-y_plus) + x] & 6) && (etu[21*(rivi-y_plus) + x] & 1) &&
						!(etu[21*(rivi-y_plus+1) + x] & 6) && (etu[21*(rivi-y_plus+1) + x] & 1))
					{
						tausta[21*rivi+x] = 0;
					}
	
				}
			}
		}
	}
	else
	{
		if(etu_y_cam % 32 == tausta_y_cam % 32)
		{
			int x_plus;
			if(tausta_x_cam % 32 > etu_x_cam % 32) x_plus = 1; else x_plus = 0;

			for(rivi = 0; rivi < 15; rivi++)
			{
				for(x = x_plus; x < 21; x++)
				{
					if(!(etu[21*rivi + x-x_plus] &6) && (etu[21*rivi + x-x_plus] &1) &&
						!(etu[21*rivi + x-x_plus+1] &6) && (etu[21*rivi + x-x_plus+1] &1))
					{
						tausta[21*rivi+x] = 0;
					}
	
				}
			}
			
		}
		else
		{
			int x_plus, y_plus;
			if(tausta_x_cam % 32 > etu_x_cam % 32) x_plus = 1; else x_plus = 0;
			if(tausta_y_cam % 32 > etu_y_cam % 32) y_plus = 1; else y_plus = 0;
			
			int x_tasa, y_tasa;
			if(tausta_x_cam % 32 == 0) x_tasa = 1; else x_tasa = 0;
			if(tausta_y_cam % 32 == 0) y_tasa = 1; else y_tasa = 0;
			x_tasa = 0; y_tasa = 0;

			if(y_plus)
			for(x = x_plus; x < 21-x_tasa; x++)
			{
				if(!(etu[x-x_plus] &6) && (etu[x-x_plus] &1) &&
						!(etu[x-x_plus+1] & 6) && (etu[x-x_plus+1] & 1)) tausta[x] = 0;
			}
	
			if(x_plus)
			for(rivi = y_plus; rivi < 16-y_tasa; rivi++)
			{
				if(!(etu[21*(rivi-y_plus)] &6) && (etu[21*(rivi-y_plus)] &1)  &&
						!(etu[21*(rivi-y_plus+1)] &6) && (etu[21*(rivi-y_plus+1)] &1))
						tausta[21*rivi] = 0;
			}

			for(rivi = y_plus; rivi < 16-y_tasa; rivi++)
			{
				for(x = x_plus; x < 21-x_tasa; x++)
				{
					if(!(etu[21*(rivi-y_plus) + x-x_plus] &6) && (etu[21*(rivi-y_plus) + x-x_plus] &1) &&
						!(etu[21*(rivi-y_plus) + x-x_plus+1] &6)  && (etu[21*(rivi-y_plus) + x-x_plus+1] &1) &&
						!(etu[21*(rivi-y_plus+1) + x-x_plus] &6)  && (etu[21*(rivi-y_plus+1) + x-x_plus] &1) &&
						!(etu[21*(rivi-y_plus+1) + x-x_plus+1] &6) && (etu[21*(rivi-y_plus+1) + x-x_plus+1] &1))
					{
						tausta[21*rivi+x] = 0;
					}
	
				}
			}
	
		}
	}
	
	return 0;
}





