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

// The female bird.

#include "female.hpp"

// Needs.
const Mona::NEED Female::FOOD_NEED = 100.0;
const Mona::NEED Female::EGG_NEED = 5.0;

// Build Mona instance for female bird.
Female::Female(Mona *mona)
{
	Mona::Receptor *anywhere, *emptyGround, *stoneOnGround,
		*haveStone, *mateHasStone, *readyToLayEgg, *eggOnGround,
		*mateHasFood, *gotFood, *foodOK;
	Mona::Motor *eat, *get, *layEgg, *put, *receive,
		*step, *toss, *turn, *wantStoneSet, *wantStoneUnset;
	Mona::Mediator *eatFood, *foodEaten,
		*receiveFood, *foodReceived,
		*eggInNest[50], *buildNest[30],
		*clearNest[30], *layEggInNest, *eggLaidInNest;
	Mona::SENSOR *s;
	VALUE_SET goals;
	int i;
	char *description;

	assert(mona != NULL);
	this->mona = mona;

	// Needs.
	mona->initNeed(0, 0.0, "Need for food");
	mona->initNeed(1, 0.0, "Need for egg in nest");

	// Receptors:
	// setSensors(locale, object, condition.wantsFood, condition.wantsStone,
	//  condition.hasObject, mate.present, mate.condition.wantsFood,
	//  mate.condition.wantsStone, mate.condition.hasObject)
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	anywhere = this->mona->newReceptor(s, "Anywhere");
	s = MonAmi::setSensors(GRASSLAND, NO_OBJECT, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	emptyGround = this->mona->newReceptor(s, "Empty ground");
	s = MonAmi::setSensors(GRASSLAND, STONE, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	stoneOnGround = this->mona->newReceptor(s, "Stone on ground");
	s = MonAmi::setSensors(GRASSLAND, NO_OBJECT, DONT_CARE, DONT_CARE, NO_OBJECT,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	readyToLayEgg = this->mona->newReceptor(s, "Ready to lay egg");
	s = MonAmi::setSensors(GRASSLAND, EGG, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	eggOnGround = this->mona->newReceptor(s, "Egg on ground");
	s = MonAmi::setSensors(GRASSLAND, NO_OBJECT, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, STONE);
	mateHasStone = this->mona->newReceptor(s, "Have stone");
	s = MonAmi::setSensors(GRASSLAND, NO_OBJECT, DONT_CARE, DONT_CARE, STONE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	haveStone = this->mona->newReceptor(s, "Have stone");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, NO_OBJECT,
		TRUE, DONT_CARE, DONT_CARE, FOOD);
	mateHasFood = this->mona->newReceptor(s, "Mate has food");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, FOOD,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	gotFood = this->mona->newReceptor(s, "Got food");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, FALSE, DONT_CARE, NO_OBJECT,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	foodOK = this->mona->newReceptor(s, "Food OK");

	// Motors:
	eat = this->mona->newMotor(EAT, "Eat");
	get = this->mona->newMotor(GET, "Get");
	layEgg = this->mona->newMotor(LAY_EGG, "Lay egg");
	put = this->mona->newMotor(PUT, "Put");
	receive = this->mona->newMotor(RECEIVE, "Receive");
	step = this->mona->newMotor(STEP, "Step");
	toss = this->mona->newMotor(TOSS, "Toss");
	turn = this->mona->newMotor(TURN, "Turn");
	wantStoneSet = this->mona->newMotor(WANT_STONE_SET, "Want stone set");
	wantStoneUnset = this->mona->newMotor(WANT_STONE_UNSET, "Want stone unset");

	// Mediators:

	// eatFood: (gotFood -> eat)
	eatFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Eat food");
	eatFood->addEvent(Mona::CAUSE, (Mona::Neuron *)gotFood);
	eatFood->addEvent(Mona::EFFECT, (Mona::Neuron *)eat);

	// foodEaten: (eatFood -> foodOK)
	foodEaten = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Food eaten");
	foodEaten->addEvent(Mona::CAUSE, (Mona::Neuron *)eatFood);
	foodEaten->addEvent(Mona::EFFECT, (Mona::Neuron *)foodOK);

	// receiveFood: (mateHasFood -> receive)
	receiveFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Receive food");
	receiveFood->addEvent(Mona::CAUSE, (Mona::Neuron *)mateHasFood);
	receiveFood->addEvent(Mona::EFFECT, (Mona::Neuron *)receive);

	// foodReceived: (receiveFood -> gotFood)
	foodReceived = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Food received");
	foodReceived->addEvent(Mona::CAUSE, (Mona::Neuron *)receiveFood);
	foodReceived->addEvent(Mona::EFFECT, (Mona::Neuron *)gotFood);

	// eggInNest: egg in nest

	// Prepare to check nest.
	i = 0;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)turn);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)anywhere);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Move to center and lay egg.
	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)eggOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)turn);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Build SE corner.
	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)turn);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Build south side.
	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Build SW corner.
	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)turn);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Build west side.
	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Build NW corner.
	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)turn);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Build north side.
	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Build NE corner.
	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)turn);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Build east side.
	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)step);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	i++;
	description = new char[50];
	sprintf(description, "Egg in nest[%d]", i);
	eggInNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	eggInNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	eggInNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)eggInNest[i-1]);

	// Set long wait parameter since building nest is lengthy.
	for (i = 0; i <= 30; i++)
	{
		eggInNest[i]->timedWagerWeights[0] = 0.0;
		eggInNest[i]->timedWagerWeights[1] = 1.0;
	}

	// buildNest: (emptyGround -> wantStoneSet -> mateHasStone ->
	//	receive -> haveStone -> wantStoneUnset -> haveStone ->
	//	put -> stoneOnGround)
	i = 0;
	description = new char[50];
	sprintf(description, "Build nest[%d]", i);
	buildNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	buildNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)emptyGround);
	buildNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)wantStoneSet);
	buildNest[i]->setRepeater(true);

	i++;
	description = new char[50];
	sprintf(description, "Build nest[%d]", i);
	buildNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	buildNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)buildNest[i-1]);
	buildNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)mateHasStone);
	buildNest[i]->setRepeater(true);

	i++;
	description = new char[50];
	sprintf(description, "Build nest[%d]", i);
	buildNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	buildNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)buildNest[i-1]);
	buildNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)receive);
	buildNest[i]->setRepeater(true);

	i++;
	description = new char[50];
	sprintf(description, "Build nest[%d]", i);
	buildNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	buildNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)buildNest[i-1]);
	buildNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)haveStone);
	buildNest[i]->setRepeater(true);

	i++;
	description = new char[50];
	sprintf(description, "Build nest[%d]", i);
	buildNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	buildNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)buildNest[i-1]);
	buildNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)wantStoneUnset);
	buildNest[i]->setRepeater(true);

	i++;
	description = new char[50];
	sprintf(description, "Build nest[%d]", i);
	buildNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	buildNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)buildNest[i-1]);
	buildNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)haveStone);
	buildNest[i]->setRepeater(true);

	i++;
	description = new char[50];
	sprintf(description, "Build nest[%d]", i);
	buildNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	buildNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)buildNest[i-1]);
	buildNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)put);
	buildNest[i]->setRepeater(true);

	i++;
	description = new char[50];
	sprintf(description, "Build nest[%d]", i);
	buildNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	buildNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)buildNest[i-1]);
	buildNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)stoneOnGround);

	// clearNest: (stoneOnGround -> get -> haveStone -> toss -> readyToLayEgg)
	i = 0;
	description = new char[50];
	sprintf(description, "Clear nest[%d]", i);
	clearNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	clearNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	clearNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)get);

	i++;
	description = new char[50];
	sprintf(description, "Clear nest[%d]", i);
	clearNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	clearNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)clearNest[i-1]);
	clearNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)haveStone);

	i++;
	description = new char[50];
	sprintf(description, "Clear nest[%d]", i);
	clearNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	clearNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)clearNest[i-1]);
	clearNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)toss);

	i++;
	description = new char[50];
	sprintf(description, "Clear nest[%d]", i);
	clearNest[i] = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, description);
	clearNest[i]->addEvent(Mona::CAUSE, (Mona::Neuron *)clearNest[i-1]);
	clearNest[i]->addEvent(Mona::EFFECT, (Mona::Neuron *)readyToLayEgg);

	// layEggInNest: (readyToLayEgg -> layEgg)
	layEggInNest = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Lay egg in nest");
	layEggInNest->addEvent(Mona::CAUSE, (Mona::Neuron *)readyToLayEgg);
	layEggInNest->addEvent(Mona::EFFECT, (Mona::Neuron *)layEgg);

	// eggLaidInNest: (layEggInNest -> eggOnGround)
	eggLaidInNest = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Egg laid in nest");
	eggLaidInNest->addEvent(Mona::CAUSE, (Mona::Neuron *)layEggInNest);
	eggLaidInNest->addEvent(Mona::EFFECT, (Mona::Neuron *)eggOnGround);

	// Goals:
	goals.alloc(mona->numNeeds);
	goals.set(0, FOOD_NEED);
	foodOK->goals.setGoals(&goals, 1.0);
	goals.set(0, 0.0);
	goals.set(1, EGG_NEED);
	eggInNest[30]->goals.setGoals(&goals, 1.0);
}
