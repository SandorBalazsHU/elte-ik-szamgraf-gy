#include <vector>
#include <iostream>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// standard
#include <iostream>
#include <sstream>

// A színteret reprezentáló névtér. 
// Ennek a struktúrának a segítségével könnyen bővíthetővév válik a szöveg.
// TODO: Később műveletekkel is bővítheő, pl.: eltolás. Később burkolás a vector-ra
// TODO: Az Ellipse kezelhetné a Color oszttályt is.
namespace Scene
{
	class Object
	{
	public:
		std::string type;

		Object(std::string type)
		{
			this->type = type;
		}
	};

	class Group: public Object
	{
	public:
		std::vector<Object*> storage;

		Group(std::string type = "Group") : Object(type)
		{

		}

		void add(Object* object)
		{
			this->storage.push_back(object);
		}
	};

	class Color: public Object
	{
	public:
		Uint8 r;
		Uint8 g;
		Uint8 b;

		Color(Uint8 r, Uint8 g, Uint8 b, std::string type = "Color") : Object(type)
		{
			this->r = r;
			this->g = g;
			this->b = b;
		}
	};

	class Line: public Object
	{
	public:
		int x1;
		int y1;
		int x2;
		int y2;

		Line(int x1, int y1, int x2, int y2, std::string type = "Line") : Object(type)
		{
			this->x1 = x1;
			this->y1 = y1;
			this->x2 = x2;
			this->y2 = y2;
		}
	};

	class Ellipse : public Line
	{
	public:
		bool fill;
		Uint8 clearR;
		Uint8 clearG;
		Uint8 clearB;

		Ellipse(int x1, int y1, int x2, int y2, std::string type = "Ellipse", bool fill = false, Uint8 clearR = 0, Uint8 clearG = 0, Uint8 clearB = 0) : Line(x1, y1, x2, y2, type)
		{
			this->fill = fill;
			this->clearR = clearR;
			this->clearG = clearG;
			this->clearB = clearB;
		}
	};
}

//Ellipszis rajzoló
void SDL_RenderDrawEllipse(SDL_Renderer* ren, int x1, int y1, int x2, int y2, bool fill = false, Uint8 clearR = 0, Uint8 clearG = 0, Uint8 clearB = 0)
{
	// Calculate the center of the ellipse
	int centerX = (x1 + x2) / 2;
	int centerY = (y1 + y2) / 2;

	// Calculate the radii
	int radiusX = std::abs(x2 - x1) / 2;
	int radiusY = std::abs(y2 - y1) / 2;

	// Loop through the screen and draw points within the ellipse
	for (int y = centerY - radiusY; y <= centerY + radiusY; ++y)
	{
		for (int x = centerX - radiusX; x <= centerX + radiusX; ++x)
		{
			// Check if the point is within the ellipse equation
			if (((x - centerX) * (x - centerX) * radiusY * radiusY +
				(y - centerY) * (y - centerY) * radiusX * radiusX) <=
				(radiusX * radiusX * radiusY * radiusY))
			{
				SDL_RenderDrawPoint(ren, x, y);
			}
		}
	}
	if (!fill)
	{
		SDL_SetRenderDrawColor(ren, clearR, clearG, clearB, 255);
		SDL_RenderDrawEllipse(ren, x1 + 1, y1 + 1, x2 - 1, y2 - 1, true);
	}
}

int main( int argc, char* args[] )
{
	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// Állítsuk be a hiba Logging függvényt.
	SDL_LogSetPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR);
	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjünk ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es termináljon a program
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[SDL initialization] Error during the SDL initialization: %s", SDL_GetError());
		return 1;
	}

	// Miután az SDL Init lefutott, kilépésnél fusson le az alrendszerek kikapcsolása.
	// Így akkor is lefut, ha valamilyen hiba folytán lépünk ki.
	std::atexit(SDL_Quit);

	// hozzuk létre az ablakunkat
	SDL_Window *win = nullptr;
	win = SDL_CreateWindow( "Hello SDL!",		// az ablak fejléce
							100,						// az ablak bal-felső sarkának kezdeti X koordinátája
							100,						// az ablak bal-felső sarkának kezdeti Y koordinátája
							800,						// ablak szélessége
							600,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok


	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
	if (win == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "[Window creation] Error during the SDL initialization: %s", SDL_GetError());
		return 1;
	}

	//
	// 3. lépés: hozzunk létre egy renderelőt, rajzolót
	//

	SDL_Renderer *ren = nullptr;
	ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendeljük hozzá a renderert
							  -1,  // melyik indexű renderert inicializáljuk
							  // a -1 a harmadik paraméterben meghatározott igényeinknek megfelelő első renderelőt jelenti
							  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az igényeink, azaz
	// hardveresen gyorsított és vsync-et beváró
	if (ren == nullptr)
	{
		SDL_LogError( SDL_LOG_CATEGORY_ERROR, "[Renderer creation] Error during the creation of an SDL renderer: %s", SDL_GetError() );
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}


	//
	// 3. lépés:  ------------------ 1.) Házi Feladat ------------------
	//

	SDL_SetRenderDrawColor(ren,	// melyik renderelőnek állítjuk be az aktuális rajzolási színét
							0,		// vörös intenzitás - 8 bites előjel nélküli egész szám
							0,		// zöld intenzitás - 8 bites előjel nélküli egész szám
							0,		// kék intenzitás - 8 bites előjel nélküli egész szám
							255);	// átlátszóság - 8 bites előjel nélküli egész szám

	SDL_RenderClear(ren);

	// aktuális rajzolási szín legyen fekete és rajzoljunk ki egy vonalat
	SDL_SetRenderDrawColor(	ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
							0,		// piros
							0,	// zöld
							0,		// kék
							255);	// átlátszatlanság

	//Színtér létrehozása
	// TODO: destruktorok
	Scene::Group* scene = new Scene::Group();

	Scene::Group* h = new Scene::Group();
	h->add(new Scene::Line(20, 20, 20, 100));
	h->add(new Scene::Line(20, 60, 60, 60));
	h->add(new Scene::Line(60, 20, 60, 100));
	scene->add(h);

	Scene::Group* e = new Scene::Group();
	e->add(new Scene::Line(85, 20, 85, 100));
	e->add(new Scene::Line(85, 20, 125, 20));
	e->add(new Scene::Line(85, 60, 125, 60));
	e->add(new Scene::Line(85, 100, 125, 100));
	scene->add(e);

	Scene::Group* l = new Scene::Group();
	l->add(new Scene::Line(150, 20, 150, 100));
	l->add(new Scene::Line(150, 100, 200, 100));
	scene->add(l);

	Scene::Group* l2 = new Scene::Group();
	l2->add(new Scene::Line(225, 20, 225, 100));
	l2->add(new Scene::Line(225, 100, 275, 100));
	scene->add(l2);

	scene->add(new Scene::Ellipse(295, 20, 345, 100));

	//Törlő színek hozzáadása (Pont annyi ahány betű van. Fontos mert nincs ellenőrzés)
	Scene::Group* colors = new Scene::Group();
	colors->add(new Scene::Color(255, 0, 0));
	colors->add(new Scene::Color(0, 255, 0));
	colors->add(new Scene::Color(0, 0, 255));
	colors->add(new Scene::Color(255, 255, 0));
	colors->add(new Scene::Color(255, 0, 255));

	//Írószín
	Scene::Color* writeColor = new Scene::Color(0, 0, 0);



	// TODO: Bővíteni kell típusok szerint.
	// TODO: Futhatna végtelen ciklusban renderciklus szerűen.
	// TODD: Hatékonyság nöelése, pointerek átgondolása.

	//Hozzáadott külső ciklus a lépésenkénti újrarajzolsához
	for (int k = 4; k >= 0; k--)
	{
		Scene::Color* currentClearColor = (Scene::Color*)colors->storage[k];
		SDL_SetRenderDrawColor(ren, currentClearColor->r, currentClearColor->g, currentClearColor->b, 255);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, writeColor->r, writeColor->g, writeColor->b, 255);
		for (size_t i = 0; i < scene->storage.size()-k; i++)
		{
			if (scene->storage[i]->type == "Group")
			{
				Scene::Group* currentGroup = (Scene::Group*)scene->storage[i];
				for (size_t j = 0; j < currentGroup->storage.size(); j++)
				{
					if (currentGroup->storage[j]->type == "Line")
					{
						Scene::Line* currentLine = (Scene::Line*)currentGroup->storage[j];
						SDL_RenderDrawLine(ren, currentLine->x1, currentLine->y1, currentLine->x2, currentLine->y2);
					}
				}
			}
			else
			{
				if (scene->storage[i]->type == "Ellipse")
				{
					Scene::Ellipse* currentEllipse = (Scene::Ellipse*)scene->storage[i];
					SDL_RenderDrawEllipse(ren, currentEllipse->x1, currentEllipse->y1, currentEllipse->x2, currentEllipse->y2, false, currentClearColor->r, currentClearColor->g, currentClearColor->b);
				}
			}
		}
		SDL_RenderPresent(ren);
		SDL_Delay(1000);
	}

	//
	// 4. lépés: lépjünk ki
	// 
	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	return 0;
}
