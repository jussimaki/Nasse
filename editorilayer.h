class Editorilayer
{
private:
	Tilemap *mappi;
	Tilemap *leikepoyta;
	int nopeus;
	int x_cam, y_cam;
	bool skrollaus;
	int scroll_xv, scroll_yv;
	int scroll_x, scroll_y;
public:
	Editorilayer(): mappi(0), nopeus(0), x_cam(0), y_cam(0){}
	int AnnaXcam();
	int AnnaYcam();
	Tilemap *annamappi();
	Tilemap *annaleikepoyta();
	bool PiirraTilemap(int tileset, int kerros, GLOBAALI *glob_data);
	bool Hiiriskrollaus(GLOBAALI *glob_data);
	void KorjaaKamera();
	bool Alusta();
	bool Tallenna(std::ofstream &tiedosto, int koodi);
	bool Lataa(std::ifstream &tiedosto,GLOBAALI *glob_data,int tileset,int kerros, int koodi);
	bool Tuhoa();
	void AsetaCam(int x, int y) {x_cam = x; y_cam = y;}
};


