#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "Module.h"
#include "Scene.h";
#include "PerfTimer.h"
#include "SDL\include\SDL_rect.h"

//this enum will manage all the transitions, using it to indicate which type of transition we're using
enum TransitionType {
	none,
	fade_to_black,
	fade_to_white,
	wipe,
	zoom,
	curtain
};

enum Scenes;

class Transitions : public Module
{
public:
	//main functions
	Transitions();
	~Transitions();

	bool FadeToBlackEffect(bool fadeIn, float frames = 60);

	bool Start();
	bool PostUpdate();


public:
	//this enum indicates the situation of the transition/effect we are in
	enum fade_step
	{
		none,
		entering,
		exiting
	} ongoingstep = fade_step::none;


	bool Transition(TransitionType type, Module* module_offp, Module* module_onp, float time = 2, float target_scalep = 2);

	//the following calculates the percentatge of the transition that is done
	float LerpValue(float percent, float start, float end);

	//for the zoom camera transition, it sets the camera scale that we will get at the final of the zoom transition
	void SetTargetScale(int target_scalep);


private:

	enum FadeToBlackStep
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	}
	currentStep = FadeToBlackStep::NONE;

	TransitionType tranistionType = TransitionType::none;

	//desactivate and activate the modules
	Module* module_on = nullptr;
	Module* module_off = nullptr;

	//used for calculating the percentage of screen doing the effect of the transition
	//we calculate it using the time of the transition and position of our rects that we create during it
	float percentatge = 0;
	float globaltime = 0;
	PerfTimer timer;

	//used for the renders and the transitions functions
	SDL_Rect screen;
	SDL_Rect WipeTransRect;
	SDL_Rect CurtainTransRect;
	SDL_Rect CurtainTransRect2;



	//for the zoom transition
	int original_width;
	int original_height;
	int final_width;
	int final_height;
	int current_width;
	int current_height;
	float current_scale = 1;
	float target_scale;
	float normal_scale = 1;
	float percentatge2 = 0;
	float percent3 = 0;

public:
	bool isFading = false;
private:
	uint32 frameCount;
	uint32 maxFadeFrames;
};

#endif //__MODULEFADETOBLACK_H__