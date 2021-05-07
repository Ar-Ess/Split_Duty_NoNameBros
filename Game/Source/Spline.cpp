#include "Spline.h"

void Spline::CreateSplines(pugi::xml_node& node)
{
	splinesList.Clear();
	for (pugi::xml_node sp = node.child("Spline"); sp != nullptr; sp = sp.next_sibling("Spline"))
	{
		sSpline spline;
		spline.selectedPoint = sp.attribute("SelectedPoint").as_int();
		spline.loop = sp.attribute("loop").as_bool();

		for (pugi::xml_node pt = sp.child("Point"); pt != nullptr; pt = pt.next_sibling("Point"))
		{
			spline.points.push_back(fPoint{ pt.attribute("x").as_float(), pt.attribute("y").as_float() });
		}
		splinesList.Add(spline);
	}
}

void Spline::DrawSpline(uint i)
{
	for (float t = 0.0f; t < splinesList[i].points.size(); t+=0.005f)
	{
		fPoint pos = splinesList[i].GetSplinePoint(t, true);
		SDL_SetRenderDrawColor(app->render->renderer, red.r, red.g, red.g, red.a);
		SDL_RenderDrawPoint(app->render->renderer, pos.x, pos.y);
	}
}

void Spline::DrawSplineControlPoints(uint i)
{
	SDL_Rect r{ 0,0,10,10 };
	for (int j = 0; j < splinesList[i].points.size(); ++j)
	{
		r.x = splinesList[i].points[j].x - r.w / 2;
		r.y = splinesList[i].points[j].y - r.h / 2;

		if (j == splinesList[i].selectedPoint)
			app->render->DrawRectangle(r, green);
		else
			app->render->DrawRectangle(r, red);
	}
}

void Spline::HandleInput(uint i)
{
	if (app->input->GetKey(SDL_SCANCODE_X) == KEY_UP)
	{
		splinesList[i].selectedPoint++;
		if (splinesList[i].selectedPoint >= splinesList[i].points.size())
			splinesList[i].selectedPoint = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_UP)
	{
		splinesList[i].selectedPoint--;
		if (splinesList[i].selectedPoint < 0)
			splinesList[i].selectedPoint = splinesList[i].points.size() - 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		splinesList[i].points[splinesList[i].selectedPoint].x -= 10.0f;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		splinesList[i].points[splinesList[i].selectedPoint].x += 10.0f;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		splinesList[i].points[splinesList[i].selectedPoint].y -= 10.0f;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		splinesList[i].points[splinesList[i].selectedPoint].y += 10.0f;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		fMarker -= 5.0f;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		fMarker += 5.0f;

	/*if (fMarker >= (float)path.points.size())
		fMarker -= (float)path.points.size();

	if (fMarker < 0.0f)
		fMarker += (float)path.points.size();*/

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		SaveSplines();

	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		pugi::xml_document load;
		LoadSplines(load);
	}
}

void Spline::Clear()
{
	for(int i = 0; i < splinesList.Count(); ++i)
		splinesList[i].points.clear();
}

pugi::xml_node Spline::LoadSplineConfig(pugi::xml_document& configfile)
{
	pugi::xml_node ret;
	pugi::xml_parse_result result = configfile.load_file("Splines.xml");

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", "Splines.xml", result.description());
	else ret = configfile.child("SplineSetUp");

	return ret;
}

void Spline::SaveSplines()
{
	pugi::xml_document save;
	pugi::xml_node root = save.append_child("SplineSetUp");
	pugi::xml_node sp;
	pugi::xml_node pt;

	for (ListItem<sSpline>* it = splinesList.start; it != nullptr; it = it->next)
	{
		sp = root.append_child("Spline");
		sp.append_attribute("SelectedPoint").set_value(it->data.selectedPoint);
		sp.append_attribute("loop").set_value(it->data.loop);

		for (int i = 0; i < it->data.points.size(); ++i)
		{
			pt = sp.append_child("Point");
			pt.append_attribute("x").set_value(it->data.points[i].x);
			pt.append_attribute("y").set_value(it->data.points[i].y);
		}
	}
	save.save_file("Splines.xml");
}

void Spline::LoadSplines(pugi::xml_document& configfile)
{
	pugi::xml_node setter = LoadSplineConfig(configfile);
	CreateSplines(setter);
}
