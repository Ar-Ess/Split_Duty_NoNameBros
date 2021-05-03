#include "Spline.h"

void Spline::CreateSplines(pugi::xml_node& node)
{
	splinesList.Clear();
	for (pugi::xml_node sp = node.child("Spline"); sp != nullptr; sp = sp.next_sibling("Spline"))
	{
		Spline spline;
		spline.selectedPoint = sp.attribute("SelectedPoint").as_int();

		for (pugi::xml_node pt = sp.child("Point"); pt != nullptr; pt = pt.next_sibling("Point"))
		{
			spline.path.points.push_back({ pt.attribute("x").as_float(), pt.attribute("y").as_float() });
		}
		splinesList.Add(spline);
	}
}

void Spline::DrawSpline()
{
	for (float t = 0.0f; t < this->path.points.size(); t+=0.005f)
	{
		fPoint pos = this->path.GetSplinePoint(t, true);
		SDL_SetRenderDrawColor(app->render->renderer, red.r, red.g, red.g, red.a);
		SDL_RenderDrawPoint(app->render->renderer, pos.x, pos.y);
	}
}

void Spline::DrawSplineControlPoints()
{
	SDL_Rect r{ 0,0,10,10 };
	for (int i = 0; i < this->path.points.size(); i++)
	{
		r.x = this->path.points[i].x - r.w / 2;
		r.y = this->path.points[i].y - r.h / 2;

		if (i == selectedPoint)
			app->render->DrawRectangle(r, green);
		else
			app->render->DrawRectangle(r, red);
	}
}

void Spline::HandleInput()
{
	if (app->input->GetKey(SDL_SCANCODE_X) == KEY_UP)
	{
		selectedPoint++;
		if (selectedPoint >= this->path.points.size())
			selectedPoint = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_UP)
	{
		selectedPoint--;
		if (selectedPoint < 0)
			selectedPoint = this->path.points.size() - 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		this->path.points[selectedPoint].x -= 10.0f;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		this->path.points[selectedPoint].x += 10.0f;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		this->path.points[selectedPoint].y -= 10.0f;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		this->path.points[selectedPoint].y += 10.0f;

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
	path.points.clear();
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

	for (ListItem<Spline>* it = splinesList.start; it != nullptr; it = it->next)
	{
		sp = root.append_child("Spline");
		sp.append_attribute("SelectedPoint").set_value(selectedPoint);

		for (int i = 0; i < it->data.path.points.size(); ++i)
		{
			pt = sp.append_child("Point");
			pt.append_attribute("x").set_value(it->data.path.points[i].x);
			pt.append_attribute("y").set_value(it->data.path.points[i].y);
		}
	}

	save.save_file("Splines.xml");
	save.reset();
}

void Spline::LoadSplines(pugi::xml_document& configfile)
{
	pugi::xml_node setter = LoadSplineConfig(configfile);
	CreateSplines(setter);
}
