#ifndef _GUISTRING_H_
#define _GUISTRING_H_

#include "GuiControl.h"
#include "App.h"
#include "FontTTF.h"

class SString;
struct _TTF_Font;
struct SDL_Texture;

class GuiString : public GuiControl
{
public:

	GuiString(uint32 id, SDL_Rect bounds, const char* text, _TTF_Font* textFont);
	GuiString();

	virtual ~GuiString();

public:

	void Draw();
	void SetString(const char* newText, SDL_Color color = { 255,255,255,255 }, int endline = 850);
	void SetTextFont(_TTF_Font* textFont);
	void CenterAlign();
	void UnLoadTextTexture();

	void CenterDialogue();
	void NodePlacing();	

private:
	_TTF_Font* textFont = nullptr;
	SDL_Texture* textTexture = nullptr;
};

#endif // !_GUISTRING_H_
