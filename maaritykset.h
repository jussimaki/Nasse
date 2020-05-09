#include "SDL.h"
#include <windows.h>
#include <windowsx.h> 
#include <iostream>
#include <string.h>
#include <math.h>
#include <fmod.h>
#include <assert.h>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

#define bmp_formaatti 0
#define binaarimuoto 1

#define ei_frameskippeja 1

#include "definet.h"
#include "grafiikka.h"
#include "animointi.h"
#include "globaalit_tyypit.h"
#include "tulostus.h"
#include "tilemaaritykset.h"
#include "tilemap.h"
#include "tilemap_animointi.h"
#include "collision_detection.h"
#include "editoriobjekti.h"
#include "listanhallinta.h"
#include "objektimaaritys.h"
#include "kuvapankki.h"
#include "editorilayer.h"
#include "komentorivi.h"
#include "editorisektori.h"
#include "ikkunaluokka.h"
#include "peliobjekti.h"
#include "luokat.h"
#include "tyypit.h"
#include "poiminta.h"
#include "quadtree.h"
#include "tilemap_piirto.h"
#include "pelisektori.h"
#include "pelitaso.h"
#include "editor.h"
#include "kartta.h"
#include "peli.h"
#include "valikot.h"
#include "valikko.h"
#include "biisinsoitto.h"
#include "painettupohjassa.h"
#include "funktiot.h"
#include "sirpalefunktiot.h"
