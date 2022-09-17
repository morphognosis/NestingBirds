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

// The Mona nesting birds utility functions

#include "monami.hpp"

// Constant initialization.
const MonAmi::INPUT MonAmi::DO_NOTHING = Environment::DO_NOTHING;
const MonAmi::INPUT MonAmi::EAT = Environment::EAT;
const MonAmi::INPUT MonAmi::GET = Environment::GET;
const MonAmi::INPUT MonAmi::GO_TO_DESERT = Environment::GO_TO_DESERT;
const MonAmi::INPUT MonAmi::GO_TO_FOREST = Environment::GO_TO_FOREST;
const MonAmi::INPUT MonAmi::GO_TO_MATE = Environment::GO_TO_MATE;
const MonAmi::INPUT MonAmi::LAY_EGG = Environment::LAY_EGG;
const MonAmi::INPUT MonAmi::LOOK_FOR_FOOD = Environment::LOOK_FOR_FOOD;
const MonAmi::INPUT MonAmi::LOOK_FOR_STONE = Environment::LOOK_FOR_STONE;
const MonAmi::INPUT MonAmi::PUT = Environment::PUT;
const MonAmi::INPUT MonAmi::RECEIVE = Environment::RECEIVE;
const MonAmi::INPUT MonAmi::STEP = Environment::STEP;
const MonAmi::INPUT MonAmi::TOSS = Environment::TOSS;
const MonAmi::INPUT MonAmi::TURN = Environment::TURN;
const MonAmi::INPUT MonAmi::WANT_STONE_SET = Environment::WANT_STONE_SET;
const MonAmi::INPUT MonAmi::WANT_STONE_UNSET = Environment::WANT_STONE_UNSET;
const MonAmi::LOCALE MonAmi::DESERT = Environment::DESERT;
const MonAmi::LOCALE MonAmi::FOREST = Environment::FOREST;
const MonAmi::LOCALE MonAmi::GRASSLAND = Environment::GRASSLAND;
const MonAmi::OBJECT MonAmi::NO_OBJECT = Environment::NO_OBJECT;
const MonAmi::OBJECT MonAmi::EGG = Environment::EGG;
const MonAmi::OBJECT MonAmi::FOOD = Environment::FOOD;
const MonAmi::OBJECT MonAmi::STONE = Environment::STONE;

// Mona/translators
Mona::SENSOR *
MonAmi::outputToSensors(OUTPUT output)
{
	return(setSensors(output.locale, output.object,
		output.condition.wantsFood, output.condition.wantsStone,
		output.condition.hasObject, output.mate.present,
		output.mate.condition.wantsFood, output.mate.condition.wantsStone,
		output.mate.condition.hasObject));
}

Environment::INPUT
MonAmi::responseToInput(Mona::RESPONSE response)
{
	switch(response)
	{
	case 0: return(DO_NOTHING);
	case 1: return(EAT);
	case 2: return(GET);
	case 3: return(GO_TO_DESERT);
	case 4: return(GO_TO_FOREST);
	case 5: return(GO_TO_MATE);
	case 6: return(LAY_EGG);
	case 7: return(LOOK_FOR_FOOD);
	case 8: return(LOOK_FOR_STONE);
	case 9: return(PUT);
	case 10: return(RECEIVE);
	case 11: return(STEP);
	case 12: return(TOSS);
	case 13: return(TURN);
	case 14: return(WANT_STONE_SET);
	case 15: return(WANT_STONE_UNSET);
	default: assert(0);
	}
	return(DO_NOTHING);
}

// Set sensors
Mona::SENSOR *
MonAmi::setSensors(int s0, int s1, int s2, int s3,
	int s4, int s5, int s6, int s7, int s8)
{
	static Mona::SENSOR sensors[9];

	sensors[0] = s0;	// locale
	sensors[1] = s1;	// object
	sensors[2] = s2;	// condition.wantsFood
	sensors[3] = s3;	// condition.wantsStone
	sensors[4] = s4;	// condition.hasObject
	sensors[5] = s5;	// mate.present
	sensors[6] = s6;	// mate.condition.wantsFood
	sensors[7] = s7;	// mate.condition.wantsStone
	sensors[8] = s8;	// mate.condition.hasObject

	return(sensors);
}
