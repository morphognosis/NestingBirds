/*
 * This software is provided under the terms of the GNU General
 * Public License as published by the Free Software Foundation.
 *
 * Copyright (c) 2003 Tom Portegys, All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for NON-COMMERCIAL purposes and without
 * fee is hereby granted provided that this copyright notice
 * appears in all copies.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.
 */

// The Mona nesting birds environment functions

#include "environment.hpp"

// Version
void
Environment::version()
{
	const char *nestEnvironmentVersion = NEST_ENVIRONMENT_VERSION;
	cout << &nestEnvironmentVersion[5] << "\n";
}

// Parameter initialization.
const int Environment::FOOD_DURATION = 60;

// Construct nest environment
Environment::Environment()
{
	register int x,y;

	// locale map: grassland=0, forest=1, food=2, desert=3, stone=4
	static const int width=21, height=21;
	static const char locale[width][height] = {
		{ 0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,1,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,2,1,2,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,1,1,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,2,1,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,2,1,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,3,3,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,3,3,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,4,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,3,4,4,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,4,3,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,4,3,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,4,4,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,3,3,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,3,0,0,0 }
	};

	// Initialize world.
#ifdef VIEWER
	cout << "%%__Viewer:world.dimensions=" << width <<
		"," << height << "\n";
#endif
	this->width = width;
	this->height = height;
	this->world = new struct Cell *[width];
	for (x = 0; x < width; x++)
	{
		this->world[x] = new struct Cell[height];
		for (y = 0; y < height; y++)
		{
			switch(locale[x][y])
			{
			case 0:
				this->world[x][y].locale = GRASSLAND;
				this->world[x][y].object = NO_OBJECT;
				break;
			case 1:
				this->world[x][y].locale = FOREST;
#ifdef VIEWER
				cout << "%%__Viewer:world[" << x << "][" << y << "].locale=" << FOREST << "\n";
#endif
				this->world[x][y].object = NO_OBJECT;
				break;
			case 2:
				this->world[x][y].locale = FOREST;
#ifdef VIEWER
				cout << "%%__Viewer:world[" << x << "][" << y << "].locale=" << FOREST << "\n";
#endif
				this->world[x][y].object = FOOD;
#ifdef VIEWER
				cout << "%%__Viewer:world[" << x << "][" << y << "].object=" << FOOD << "\n";
#endif
				break;
			case 3:
				this->world[x][y].locale = DESERT;
#ifdef VIEWER
				cout << "%%__Viewer:world[" << x << "][" << y << "].locale=" << DESERT << "\n";
#endif
				this->world[x][y].object = NO_OBJECT;
				break;
			case 4:
				this->world[x][y].locale = DESERT;
#ifdef VIEWER
				cout << "%%__Viewer:world[" << x << "][" << y << "].locale=" << DESERT << "\n";
#endif
				this->world[x][y].object = STONE;
#ifdef VIEWER
				cout << "%%__Viewer:world[" << x << "][" << y << "].object=" << STONE << "\n";
#endif
				break;
			}
		}
	}

	// Initialize birds.
	this->bird[MALE].location.x = width/2;
	this->bird[MALE].location.y = height/2;
	this->bird[MALE].condition.wantsFood = false;
	this->bird[MALE].condition.wantsStone = false;
	this->bird[MALE].condition.hasObject = NO_OBJECT;
	this->bird[MALE].food = FOOD_DURATION;
	this->bird[MALE].orientation = NORTH;
#ifdef VIEWER
	cout << "%%__Viewer:male.location=" << this->bird[MALE].location.x << "," <<
		this->bird[MALE].location.y << "\n";
	cout << "%%__Viewer:male.condition=0,0," << NO_OBJECT << "\n";
	cout << "%%__Viewer:male.food=" << this->bird[MALE].food << "\n";
#endif
	this->bird[FEMALE].location.x = width/2;
	this->bird[FEMALE].location.y = height/2;
	this->bird[FEMALE].condition.wantsFood = true;
	this->bird[FEMALE].condition.wantsStone = false;
	this->bird[FEMALE].condition.hasObject = NO_OBJECT;
	this->bird[FEMALE].food = 0;
	this->bird[FEMALE].orientation = NORTH;
#ifdef VIEWER
	cout << "%%__Viewer:female.location=" << this->bird[FEMALE].location.x << "," <<
		this->bird[FEMALE].location.y << "\n";
	cout << "%%__Viewer:female.condition=0,0," << NO_OBJECT << "\n";
	cout << "%%__Viewer:female.food=" << this->bird[FEMALE].food << "\n";
#endif
}

// Environment input
void
Environment::input(GENDER gender, INPUT input)
{
	POINT *location;
	CONDITION *condition;
	OBJECT *object;
	register int i,j,x,y;
	static int disruptNest = 0;

	location = &(this->bird[gender].location);
	object = &(this->world[(int)(location->x)][(int)(location->y)].object);
	condition = &(this->bird[gender].condition);

	// Disrupt nest to motivate female to fix it.
	if (disruptNest == 1 && gender == FEMALE) disruptNest++;
	if (disruptNest == 2 && gender == FEMALE)
	{
		disruptNest++;

		// Remove stone from nest and drop on egg.
		this->world[(int)(location->x)][(int)(location->y-1)].object = NO_OBJECT;
		*object = STONE;
#ifdef VIEWER
		cout << "%%__Viewer:world[" << (int)(location->x) << "][" <<
			(int)(location->y) << "].object=" << STONE << "\n";
		cout << "%%__Viewer:world[" << (int)(location->x) << "][" <<
			(int)(location->y)-1 << "].object=" << NO_OBJECT << "\n";
		cout << "%%__Viewer:status=Stone falls on egg!\n";
#endif
	}

#ifdef VIEWER
	if (gender == MALE)
	{
		cout << "%%__Viewer:male.response=" << input << "\n";
	} else {
		cout << "%%__Viewer:female.response=" << input << "\n";
	}
#endif

	switch(input)
	{

	case DO_NOTHING:
		break;

	case EAT:
		if (condition->hasObject == FOOD)
		{
			condition->wantsFood = false;
			this->bird[gender].food = FOOD_DURATION;
			condition->hasObject = NO_OBJECT;
		}
		break;

	case GET:
		if (condition->hasObject != NO_OBJECT) break;
		condition->hasObject = *object;
		*object = NO_OBJECT;
#ifdef VIEWER
		cout << "%%__Viewer:world[" << (int)(location->x) << "][" <<
			(int)(location->y) << "].object=" << NO_OBJECT << "\n";
#endif
		break;

	case GO_TO_DESERT:
		stepTo((GENDER)DONT_CARE, (LOCALE)DESERT, (OBJECT)DONT_CARE, location);
		break;

	case GO_TO_FOREST:
		stepTo((GENDER)DONT_CARE, (LOCALE)FOREST, (OBJECT)DONT_CARE, location);
		break;

	case GO_TO_MATE:
		stepTo((GENDER)(((int)gender+1)%2), (LOCALE)DONT_CARE,
			(OBJECT)DONT_CARE, location);
		break;

	case LAY_EGG:
		if (*object != NO_OBJECT) break;
		*object = EGG;
		if (disruptNest == 0) disruptNest=1;
#ifdef VIEWER
		cout << "%%__Viewer:world[" << (int)(location->x) << "][" <<
			(int)(location->y) << "].object=" << EGG << "\n";
#endif
		break;

	case LOOK_FOR_FOOD:
		stepTo((GENDER)DONT_CARE, (LOCALE)DONT_CARE, FOOD, location);
		break;

	case LOOK_FOR_STONE:
		stepTo((GENDER)DONT_CARE, DESERT, STONE, location);
		break;

	case PUT:
		if (*object != NO_OBJECT) break;
		*object = condition->hasObject;
		condition->hasObject = NO_OBJECT;
#ifdef VIEWER
		cout << "%%__Viewer:world[" << (int)(location->x) << "][" <<
			(int)(location->y) << "].object=" << *object << "\n";
#endif
		break;

	case RECEIVE:
		if (condition->hasObject != NO_OBJECT) break;
		if (gender == MALE)
		{
			if (location->x != this->bird[FEMALE].location.x) break;
			if (location->y != this->bird[FEMALE].location.y) break;
			if (this->bird[FEMALE].condition.hasObject != NO_OBJECT)
			{
				condition->hasObject = this->bird[FEMALE].condition.hasObject;
				this->bird[FEMALE].condition.hasObject = NO_OBJECT;
			}
		} else {
			if (location->x != this->bird[MALE].location.x) break;
			if (location->y != this->bird[MALE].location.y) break;
			if (this->bird[MALE].condition.hasObject != NO_OBJECT)
			{
				condition->hasObject = this->bird[MALE].condition.hasObject;
				this->bird[MALE].condition.hasObject = NO_OBJECT;
			}
		}
		break;

	case STEP:
		switch(this->bird[gender].orientation)
		{
		case NORTH:
			if (this->bird[gender].location.y < this->height-1)
			{
				this->bird[gender].location.y++;
			}
			break;
		case SOUTH:
			if (this->bird[gender].location.y > 0)
			{
				this->bird[gender].location.y--;
			}
			break;
		case EAST:
			if (this->bird[gender].location.x < this->width-1)
			{
				this->bird[gender].location.x++;
			}
			break;
		case WEST:
			if (this->bird[gender].location.x > 0)
			{
				this->bird[gender].location.x--;
			}
			break;
		}
		break;

	case TOSS:
		if (condition->hasObject == NO_OBJECT) break;

		// Object lands randomly nearby.
		if ((i = this->height/2) > this->width/2) i = this->width/2;
		for (j = 5; j < i; j++)
		{
			x = (int)(rand()%(long)j);
			y = (int)(rand()%(long)j);
			if (x == 0 && y == 0) continue;
			if ((rand()%2) == 0) x = -x;
			if ((rand()%2) == 0) y = -y;
			x += (int)this->bird[gender].location.x;
			if (x < 0 || x >= this->width) continue;
			y += (int)this->bird[gender].location.y;
			if (y < 0 || y >= this->height) continue;
			if (this->world[x][y].object == NO_OBJECT)
			{
				this->world[x][y].object = condition->hasObject;
				condition->hasObject = NO_OBJECT;
#ifdef VIEWER
				cout << "%%__Viewer:world[" << x << "][" <<
					y << "].object=" << this->world[x][y].object << "\n";
#endif
				break;
			}
		}
		if (condition->hasObject != NO_OBJECT)
		{
			condition->hasObject = NO_OBJECT;  // vaporize
		}
		break;

	case TURN:
		switch(this->bird[gender].orientation)
		{
		case NORTH:
			this->bird[gender].orientation = EAST;
			break;
		case SOUTH:
			this->bird[gender].orientation = WEST;
			break;
		case EAST:
			this->bird[gender].orientation = SOUTH;
			break;
		case WEST:
			this->bird[gender].orientation = NORTH;
			break;
		}
		break;

	case WANT_STONE_SET:
		condition->wantsStone = true;
		break;

	case WANT_STONE_UNSET:
		condition->wantsStone = false;
		break;
	}

	// Digest food.
	if (this->bird[gender].food > 0) this->bird[gender].food--;
	if (this->bird[gender].food == 0) condition->wantsFood = true;

#ifdef VIEWER
	cout << "%%__Viewer:male.location=" << this->bird[MALE].location.x << "," <<
		this->bird[MALE].location.y << "\n";
	cout << "%%__Viewer:male.condition=" << this->bird[MALE].condition.wantsFood << "," <<
		condition->wantsStone << "," << this->bird[MALE].condition.hasObject << "\n";
	cout << "%%__Viewer:male.food=" << this->bird[MALE].food << "\n";
	cout << "%%__Viewer:female.location=" << this->bird[FEMALE].location.x << "," <<
		this->bird[FEMALE].location.y << "\n";
	cout << "%%__Viewer:female.condition=" << this->bird[FEMALE].condition.wantsFood << "," <<
		condition->wantsStone << "," << this->bird[FEMALE].condition.hasObject << "\n";
	cout << "%%__Viewer:female.food=" << this->bird[FEMALE].food << "\n";
	cout << "%%__Viewer:break\n";
#endif
}

// Step to various things
void
Environment::stepTo(GENDER gender, LOCALE locale, OBJECT object, POINT *location)
{
	int x, y, x2, y2;
	double d, d2;

	if (gender != (GENDER)DONT_CARE)
	{
		x = (int)this->bird[gender].location.x;
		y = (int)this->bird[gender].location.y;
		if (locale != (LOCALE)DONT_CARE &&
			locale != this->world[x][y].locale) return;
		if (object != (OBJECT)DONT_CARE &&
			object != this->world[x][y].object) return;
		if (this->bird[gender].location.x < location->x)
		{
			location->x--;
		} else if (this->bird[gender].location.x > location->x)
		{
		location->x++;
		}
		if (this->bird[gender].location.y < location->y)
		{
			location->y--;
		} else if (this->bird[gender].location.y > location->y)
		{
			location->y++;
		}
		return;
	}

	d2 = -1.0;
	for (x = 0; x < this->height; x++)
	{
		for (y = 0; y < this->width; y++)
		{
			if (locale != (LOCALE)DONT_CARE &&
				locale != this->world[x][y].locale) continue;
			if (object != (OBJECT)DONT_CARE &&
				object != this->world[x][y].object) continue;
			d = location->distance((double)x, (double)y,
				location->x, location->y);
			if (d2 < 0.0 || d < d2)
			{
				d2 = d;
				x2 = x;
				y2 = y;
			}
		}
	}
	if (d2 < 0.0) return;
	if (x2 < (int)location->x)
	{
		location->x--;
	} else if (x2 > (int)location->x)
	{
		location->x++;
	}
	if (y2 < (int)location->y)
	{
		location->y--;
	} else if (y2 > (int)location->y)
	{
		location->y++;
	}
}

// Environment output
void
Environment::output(Environment::GENDER gender, OUTPUT &out)
{
	int x, y;
#ifdef MONA_TRACE
	struct Cell *cell;

	cout << "Environment:\n";
	for (x = 0; x < this->width; x++) cout << "####";
	cout << "##\n";
	for (y = this->height-1; y >= 0; y--)
	{
		cout << "#";
		for (x = 0; x < this->width; x++)
		{
			cell = &(this->world[x][y]);
			switch(cell->locale)
			{
			case DESERT: cout << '_'; break;
			case FOREST: cout << '|'; break;
			case GRASSLAND: cout << ' '; break;
			}
			switch(cell->object)
			{
			case NO_OBJECT: cout << ' '; break;
			case EGG: cout << 'o'; break;
			case FOOD: cout << '+'; break;
			case STONE: cout << '*'; break;
			}
			if ((int)this->bird[MALE].location.x == x &&
				(int)this->bird[MALE].location.y == y)
			{
				cout << 'm';
			} else {
				cout << ' ';
			}
			if ((int)this->bird[FEMALE].location.x == x &&
				(int)this->bird[FEMALE].location.y == y)
			{
				cout << 'f';
			} else {
				cout << ' ';
			}
		}
		cout << "#\n";
	}
	for (x = 0; x < this->width; x++) cout << "####";
	cout << "##\n";
	cout << "Key: 'm'=male, 'f'=female, 'o'=egg, '+'=food, '*'=stone, '|'=forest, '_'=desert\n";
	cout << "Male location: " << this->bird[MALE].location.x << ","
		<< this->bird[MALE].location.y << "\n";
	cout << "Female location: " << this->bird[FEMALE].location.x << ","
		<< this->bird[FEMALE].location.y << "\n";
	cout << "Male condition:\n";
	if (this->bird[MALE].condition.wantsFood) cout << "\tWants food\n";
	if (this->bird[MALE].condition.wantsStone) cout << "\tWants stone\n";
	switch(this->bird[MALE].condition.hasObject)
	{
	case EGG: cout << "\tHas egg\n"; break;
	case FOOD: cout << "\tHas food\n"; break;
	case STONE: cout << "\tHas stone\n"; break;
	}
	cout << "\tFood left = " << this->bird[MALE].food << "\n";
	cout << "\tFacing ";
	switch(this->bird[MALE].orientation)
	{
	case NORTH: cout << "North\n"; break;
	case SOUTH: cout << "South\n"; break;
	case EAST: cout << "East\n"; break;
	case WEST: cout << "West\n"; break;
	}
	cout << "Female condition:\n";
	if (this->bird[FEMALE].condition.wantsFood) cout << "\tWants food\n";
	if (this->bird[FEMALE].condition.wantsStone) cout << "\tWants stone\n";
	switch(this->bird[FEMALE].condition.hasObject)
	{
	case EGG: cout << "\tHas egg\n"; break;
	case FOOD: cout << "\tHas food\n"; break;
	case STONE: cout << "\tHas stone\n"; break;
	}
	cout << "\tFood left = " << this->bird[FEMALE].food << "\n";
	cout << "\tFacing ";
	switch(this->bird[FEMALE].orientation)
	{
	case NORTH: cout << "North\n"; break;
	case SOUTH: cout << "South\n"; break;
	case EAST: cout << "East\n"; break;
	case WEST: cout << "West\n"; break;
	}
#endif

	out.mate.present = false;
	out.mate.condition.wantsFood = false;
	out.mate.condition.wantsStone = false;
	out.mate.condition.hasObject = NO_OBJECT;
	if (gender == MALE)
	{
		out.condition = this->bird[MALE].condition;
		x = (int)this->bird[MALE].location.x;
		y = (int)this->bird[MALE].location.y;
		out.locale = this->world[x][y].locale;
		out.object = this->world[x][y].object;
		if (x == this->bird[FEMALE].location.x && y == this->bird[FEMALE].location.y)
		{
			out.mate.present = true;
			out.mate.condition = this->bird[FEMALE].condition;
		}
	} else {
		out.condition = this->bird[FEMALE].condition;
		x = (int)this->bird[FEMALE].location.x;
		y = (int)this->bird[FEMALE].location.y;
		out.locale = this->world[x][y].locale;
		out.object = this->world[x][y].object;
		if (x == this->bird[MALE].location.x && y == this->bird[MALE].location.y)
		{
			out.mate.present = true;
			out.mate.condition = this->bird[MALE].condition;
		}
	}
}

// Print input to stdout
void
Environment::printInput(INPUT input)
{
	switch(input)
	{
	case DO_NOTHING: cout << "DO_NOTHING"; break;
	case EAT: cout << "EAT"; break;
	case GET: cout << "GET"; break;
	case GO_TO_DESERT: cout << "GO_TO_DESERT"; break;
	case GO_TO_FOREST: cout << "GO_TO_FOREST"; break;
	case GO_TO_MATE: cout << "GO_TO_MATE"; break;
	case LAY_EGG: cout << "LAY_EGG"; break;
	case LOOK_FOR_FOOD: cout << "LOOK_FOR_FOOD"; break;
	case LOOK_FOR_STONE: cout << "LOOK_FOR_STONE"; break;
	case PUT: cout << "PUT"; break;
	case RECEIVE: cout << "RECEIVE"; break;
	case TOSS: cout << "TOSS"; break;
	case TURN: cout << "TURN"; break;
	case STEP: cout << "STEP"; break;
	case WANT_STONE_SET: cout << "WANT_STONE_SET"; break;
	case WANT_STONE_UNSET: cout << "WANT_STONE_UNSET"; break;
	}
}

// Print output to stdout
void
Environment::printOutput(OUTPUT output)
{
	switch(output.locale)
	{
	case DESERT: cout << "DESERT "; break;
	case FOREST: cout << "FOREST "; break;
	case GRASSLAND: cout << "GRASSLAND "; break;
	}
	switch(output.object)
	{
	case NO_OBJECT: cout << "NO_OBJECT "; break;
	case EGG: cout << "EGG "; break;
	case FOOD: cout << "FOOD "; break;
	case STONE: cout << "STONE "; break;
	}
	cout << "CONDITION: ";
	if (output.condition.wantsFood) cout << "WANTS_FOOD ";
	if (output.condition.wantsStone) cout << "WANTS_STONE ";
	switch(output.condition.hasObject)
	{
	case EGG: cout << "HAS_EGG "; break;
	case FOOD: cout << "HAS_FOOD "; break;
	case STONE: cout << "HAS_STONE "; break;
	}
	cout << "MATE: ";
	if (output.mate.present)
	{
		cout << "PRESENT ";
		if (output.mate.condition.wantsFood) cout << "WANTS_FOOD ";
		if (output.mate.condition.wantsStone) cout << "WANTS_STONE ";
		switch(output.mate.condition.hasObject)
		{
		case EGG: cout << "HAS_EGG"; break;
		case FOOD: cout << "HAS_FOOD"; break;
		case STONE: cout << "HAS_STONE"; break;
		}
	} else cout << "NOT PRESENT";
}
