struct Offset
{
	int x,y;
};

struct Animaatio
{
	Sprite *osoitin;
	int nopeus;
	std::vector <Rect> laatikot;
	std::vector <Offset> offsetit;
};
struct GLOBAALI;
void piirraAnimaatio(Animaatio *anim, int x, int y, int *ruutu_counter, int *anim_ruutu, GLOBAALI *glob_data);
void animoi(Animaatio *anim, int *ruutu_counter, int *anim_ruutu);