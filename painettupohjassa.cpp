#include "maaritykset.h"

bool painettu(int index, GLOBAALI *glob_data)
{
	InputData *inputti = glob_data->gfx->annaInput();

	if(inputti->NAPPI_PAINETTU[glob_data->keyboard_indeksit[index]] || 
		((glob_data->saadot & 2) && glob_data->gfx->padiKiinni() && inputti->JOYSTICK_PAINETTU[glob_data->gamepad_indeksit[index]])
	) return true;
	else return false;
}

bool pohjassa(int index, GLOBAALI *glob_data)
{
	InputData *inputti = glob_data->gfx->annaInput();

	if(inputti->NAPPI_POHJASSA[glob_data->keyboard_indeksit[index]] ||
		((glob_data->saadot & 2) && glob_data->gfx->padiKiinni() && inputti->JOYSTICK_POHJASSA[glob_data->gamepad_indeksit[index]])
	) return true;
	else return false;
}