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

#ifndef __MONAMI__
#define __MONAMI__

// The Mona nesting birds utility definitions

#include "../mona/mona.hpp"
#include "environment.hpp"

class MonAmi
{

public:

	// Shorthands for environment symbols
	typedef Environment::INPUT INPUT;
	typedef Environment::OUTPUT OUTPUT;
	static const INPUT DO_NOTHING;
	static const INPUT EAT;
	static const INPUT GET;
	static const INPUT GO_TO_DESERT;
	static const INPUT GO_TO_FOREST;
	static const INPUT GO_TO_MATE;
	static const INPUT LAY_EGG;
	static const INPUT LOOK_FOR_FOOD;
	static const INPUT LOOK_FOR_STONE;
	static const INPUT PUT;
	static const INPUT RECEIVE;
	static const INPUT STEP;
	static const INPUT TOSS;
	static const INPUT TURN;
	static const INPUT WANT_STONE_SET;
	static const INPUT WANT_STONE_UNSET;
	typedef Environment::LOCALE LOCALE;
	static const LOCALE DESERT;
	static const LOCALE FOREST;
	static const LOCALE GRASSLAND;
	typedef Environment::OBJECT OBJECT;
	static const OBJECT NO_OBJECT;
	static const OBJECT EGG;
	static const OBJECT FOOD;
	static const OBJECT STONE;
	typedef Environment::CONDITION CONDITION;

	// Mona/Environment translators
	static Mona::SENSOR *outputToSensors(Environment::OUTPUT);
	static Environment::INPUT responseToInput(Mona::RESPONSE);

	// Set sensors
	// (locale, object, condition.wantsFood, condition.wantsStone,
	//  condition.hasObject, mate.present, mate.condition.wantsFood,
	//  mate.condition.wantsStone, mate.condition.hasObject)
	static Mona::SENSOR *setSensors(int, int, int, int, int, int, int, int, int);

protected:

	class Mona *mona;
};
#endif
