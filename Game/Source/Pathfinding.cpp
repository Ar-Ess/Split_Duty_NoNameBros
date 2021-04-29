#include "App.h"
#include "PathFinding.h"

#include "Defs.h"
#include "Log.h"

PathFinding* PathFinding::instance = nullptr;

PathFinding* PathFinding::GetInstance()
{
	if (instance == nullptr) instance = new PathFinding();
	return instance;
}

PathFinding::PathFinding() : map(NULL), width(0), height(0)
{
	//name.Create("pathfinding");
}

PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
	RELEASE(instance);
}

bool PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	RELEASE_ARRAY(map);

	return true;
}

void PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
const ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
uint PathNode::FindWalkableAdjacents(PathFinding *pathf, PathList& listToFill) const
{
	iPoint cell;
	uint before = listToFill.list.Count();

	// north
	cell.Create(pos.x, pos.y + 1);
	if(pathf->IsWalkable(cell)) listToFill.list.Add(PathNode(-1, -1, cell, this));

	// south
	cell.Create(pos.x, pos.y - 1);
	if(pathf->IsWalkable(cell)) listToFill.list.Add(PathNode(-1, -1, cell, this));

	// east
	cell.Create(pos.x + 1, pos.y);
	if(pathf->IsWalkable(cell)) listToFill.list.Add(PathNode(-1, -1, cell, this));

	// west
	cell.Create(pos.x - 1, pos.y);
	if(pathf->IsWalkable(cell)) listToFill.list.Add(PathNode(-1, -1, cell, this));

	return listToFill.list.Count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score. Cost So Far + Heuristic
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1
int PathFinding::CreatePath(DynArray<iPoint>& path, const iPoint& origin, const iPoint& destination)
{
	// L12b: TODO 1: if origin or destination are not walkable, return -1
	if (!IsWalkable(origin) || !IsWalkable(destination)) return NULL;

	// L12b: TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list
	PathList open;
	PathList close;
	open.list.Add(PathNode(0, origin.DistanceTo(destination), origin, nullptr));
	while (open.list.Count() != 0)
	{
		close.list.Add(open.GetNodeLowestScore()->data);
		open.list.Del(open.GetNodeLowestScore());

		if (close.list.end->data.pos == destination)
		{
			int counter = 1;
			PathNode backtrack = close.list.end->data;
			path.PushBack(backtrack.pos);
			do
			{
				counter++;
				backtrack = close.Find(backtrack.parent->pos)->data;
				path.PushBack(backtrack.pos);
			} while (backtrack.parent != nullptr);
			path.Flip();
			return counter;
		}

		PathList adjNodes;
		close.list.end->data.FindWalkableAdjacents(this, adjNodes);
		for (ListItem<PathNode>* i = adjNodes.list.start; i != NULL; i = i->next)
		{
			if (close.Find(i->data.pos) != NULL)
			{
				continue;
			}
			else if (open.Find(i->data.pos) != NULL)
			{
				PathNode tmp = open.Find(i->data.pos)->data;
				i->data.CalculateF(destination);
				if (i->data.g < tmp.g)
				{
					tmp.parent = i->data.parent;
				}
			}
			else
			{
				i->data.CalculateF(destination);
				open.list.Add(i->data);
			}
		}
		adjNodes.list.Clear();
	}

	return NULL;
}

