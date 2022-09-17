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
 
#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

// The Mona nesting birds environment definitions

#ifdef UNIX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#else
#include <cstdio>
#include <cstdlib>
#include <string>
#endif
#include <iostream>
#include <assert.h>
#include "../common/common.h"
#include "../common/point.hpp"
using namespace std;

#define NEST_ENVIRONMENT_VERSION "@(#) Mona nesting birds environment - version 1.0"

class Environment
{

public:

	// Version
	void version();

	// Inputs
	enum INPUT
	{
		DO_NOTHING=0,
		EAT=1,
		GET=2,
		GO_TO_DESERT=3,
		GO_TO_FOREST=4,
		GO_TO_MATE=5,
		LAY_EGG=6,
		LOOK_FOR_FOOD=7,
		LOOK_FOR_STONE=8,
		PUT=9,
		RECEIVE=10,
		STEP=11,
		TOSS=12,
		TURN=13,
		WANT_STONE_SET=14,
		WANT_STONE_UNSET=15
	};

	// Output
	enum LOCALE
	{
		DESERT=0,
		FOREST=1,
		GRASSLAND=2
	};
	enum OBJECT
	{
		NO_OBJECT=0,
		EGG=1,
		FOOD=2,
		STONE=3
	};
	typedef struct Condition
	{
		bool wantsFood;
		bool wantsStone;
		OBJECT hasObject;
	} CONDITION;
	typedef struct Mate
	{
		bool present;
		CONDITION condition;
	} MATE;
	typedef struct Output
	{
		LOCALE locale;
		OBJECT object;
		CONDITION condition;
		MATE mate;
	} OUTPUT;

	// Set output
	static void setOutput(OUTPUT *out, LOCALE locale,
		OBJECT object, CONDITION condition, MATE mate)
	{
		out->locale = locale;
		out->object = object;
		out->condition = condition;
		out->mate = mate;
	}

	// Bird genders
	enum GENDER { MALE=0, FEMALE=1 };

	// Constructor
	Environment();

	// Functions
	void input(GENDER, INPUT);
	void output(GENDER, OUTPUT&);

	// Print input/output to stdout
	void printInput(INPUT);
	void printOutput(OUTPUT);

	// World
	struct Cell
	{
		LOCALE locale;
		OBJECT object;
	} **world;
	int width;	// dimensions
	int height;

	// Food duration
	static const int FOOD_DURATION;

	// Orientation
	enum ORIENTATION { NORTH, SOUTH, EAST, WEST };

	// Bird information, male and female
	typedef struct Bird
	{
		POINT location;		// location
		CONDITION condition;	// condition
		int food;		// food remaining
		ORIENTATION orientation;
	} BIRD;
	BIRD bird[2];

	// Step to various things
	void stepTo(GENDER, LOCALE, OBJECT, POINT *);
};
#endif
