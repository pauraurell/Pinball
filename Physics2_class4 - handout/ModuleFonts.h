#include "Module.h"
#include "Globals.h"
#include "p2Point.h"


#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Fonts
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};

class ModuleFonts : public Module
{
public:
	Fonts	 fonts[MAX_FONTS];
	ModuleFonts(Application* app, bool start_enabled = true);
	virtual ~ModuleFonts();

	// Load Font
	int Load(const char* texture_path, const char* characters, uint rows, uint w, uint h, uint rc);
	void UnLoad(int font_id);

	// Create a surface from text
	void BlitText(int x, int y, int bmp_font_id, const char* text) const;

private:


	SDL_Texture* tex;

};