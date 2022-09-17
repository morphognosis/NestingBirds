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

// The male bird.

#include "male.hpp"

// Needs.
const Mona::NEED Male::FOOD_NEED = 100.0;
const Mona::NEED Male::MATE_FOOD_NEED = 50.0;
const Mona::NEED Male::MATE_STONE_NEED = 10.0;
const Mona::NEED Male::CHECK_MATE_NEED = 5.0;

// Build Mona instance for male bird.
Male::Male(Mona *mona)
{
	Mona::Receptor *anywhere, *desert, *forest,
		*foodOnGround, *stoneOnGround, *noObject,
		*gotFood, *gotStone,
		*mateHasFood, *mateFoodOK, *mateHasStone, *mateStoneOK,
		*foodOK, *matePresent;
	Mona::Motor *doNothing, *eat, *get, *goToDesert, *goToForest,
		*goToMate, *lookForFood, *lookForStone, *toss;
	Mona::Mediator *eatFood, *foodEaten, *getFood, *foodGotten,
		*findFood, *foodFound,
		*findForest, *forestFound,
		*enableFindFood, *disableFindFood,
		*checkMate, *mateChecked,
		*bringFoodToMate, *giveFoodToMate,
		*mateTakesFood, *mateAcceptsFood,
		*getStone, *stoneGotten,
		*findStone, *stoneFound,
		*findDesert, *desertFound,
		*enableFindStone, *disableFindStone,
		*enableGetStone, *disableGetStone,
		*bringStoneToMate, *giveStoneToMate,
		*mateTakesStone, *mateAcceptsStone,
		*getFoodDisablesGetStone, *getStoneDisablesGetFood,
		*tossObject, *objectTossed,
		*tossEnablesGetFood, *tossDisablesFindStone,
		*tossDisablesFindFood;
	Mona::SENSOR *s;
	VALUE_SET goals;

	assert(mona != NULL);
	this->mona = mona;

	// Needs.
	mona->initNeed(0, 0.0, "Need for food");
	mona->initNeed(1, 0.0, "Mate need for food");
	mona->initNeed(2, 0.0, "Mate need for stone");
	mona->initNeed(3, 0.0, "Need to check mate");

	// Receptors:
	// setSensors(locale, object, condition.wantsFood, condition.wantsStone,
	//  condition.hasObject, mate.present, mate.condition.wantsFood,
	//  mate.condition.wantsStone, mate.condition.hasObject)
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	anywhere = this->mona->newReceptor(s, "Anywhere");
	s = MonAmi::setSensors(DESERT, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	desert = this->mona->newReceptor(s, "Desert");
	s = MonAmi::setSensors(FOREST, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	forest = this->mona->newReceptor(s, "Forest");
	s = MonAmi::setSensors(DONT_CARE, FOOD, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	foodOnGround = this->mona->newReceptor(s, "Food on ground");
	s = MonAmi::setSensors(DONT_CARE, STONE, DONT_CARE, DONT_CARE, DONT_CARE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	stoneOnGround = this->mona->newReceptor(s, "Stone on ground");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, FOOD,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	gotFood = this->mona->newReceptor(s, "Got food");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, STONE,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	gotStone = this->mona->newReceptor(s, "Got stone");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, NO_OBJECT,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	noObject = this->mona->newReceptor(s, "No object");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
		TRUE, DONT_CARE, DONT_CARE, FOOD);
	mateHasFood = this->mona->newReceptor(s, "Mate has food");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
		TRUE, FALSE, DONT_CARE, DONT_CARE);
	mateFoodOK = this->mona->newReceptor(s, "Mate food OK");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
		TRUE, DONT_CARE, DONT_CARE, STONE);
	mateHasStone = this->mona->newReceptor(s, "Mate has stone");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
		TRUE, DONT_CARE, FALSE, DONT_CARE);
	mateStoneOK = this->mona->newReceptor(s, "Mate stone OK");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, FALSE, DONT_CARE, NO_OBJECT,
		DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
	foodOK = this->mona->newReceptor(s, "Food OK");
	s = MonAmi::setSensors(DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
		TRUE, DONT_CARE, DONT_CARE, DONT_CARE);
	matePresent = this->mona->newReceptor(s, "Mate present");

	// Motors:
	doNothing = this->mona->newMotor(DO_NOTHING, "Do nothing");
	eat = this->mona->newMotor(EAT, "Eat");
	get = this->mona->newMotor(GET, "Get");
	goToDesert = this->mona->newMotor(GO_TO_DESERT, "Go to desert");
	goToForest = this->mona->newMotor(GO_TO_FOREST, "Go to forest");
	goToMate = this->mona->newMotor(GO_TO_MATE, "Go to mate");
	lookForFood = this->mona->newMotor(LOOK_FOR_FOOD, "Look for food");
	lookForStone = this->mona->newMotor(LOOK_FOR_STONE, "Look for stone");
	toss = this->mona->newMotor(TOSS, "Toss");

	// Mediators:

	// getFood: (foodOnGround -> get)
	getFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Get food");
	getFood->addEvent(Mona::CAUSE, (Mona::Neuron *)foodOnGround);
	getFood->addEvent(Mona::EFFECT, (Mona::Neuron *)get);

	// foodGotten: (getFood -> gotFood)
	foodGotten = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Food gotten");
	foodGotten->addEvent(Mona::CAUSE, (Mona::Neuron *)getFood);
	foodGotten->addEvent(Mona::EFFECT, (Mona::Neuron *)gotFood);

	// eatFood: (foodGotten -> eat)
	eatFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Eat food");
	eatFood->addEvent(Mona::CAUSE, (Mona::Neuron *)foodGotten);
	eatFood->addEvent(Mona::EFFECT, (Mona::Neuron *)eat);

	// foodEaten: (eatFood -> foodOK)
	foodEaten = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Food eaten");
	foodEaten->addEvent(Mona::CAUSE, (Mona::Neuron *)eatFood);
	foodEaten->addEvent(Mona::EFFECT, (Mona::Neuron *)foodOK);

	// findFood: -(forest -> lookForFood)
	findFood = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, "Find food");
	findFood->addEvent(Mona::CAUSE, (Mona::Neuron *)forest);
	findFood->addEvent(Mona::EFFECT, (Mona::Neuron *)lookForFood);

	// foodFound: (findFood -> foodOnGround)
	foodFound = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Food found");
	foodFound->addEvent(Mona::CAUSE, (Mona::Neuron *)findFood);
	foodFound->addEvent(Mona::EFFECT, (Mona::Neuron *)foodOnGround);
	foodFound->setRepeater(true);

	// findForest: (anywhere -> goToForest)
	findForest = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Find forest");
	findForest->addEvent(Mona::CAUSE, (Mona::Neuron *)anywhere);
	findForest->addEvent(Mona::EFFECT, (Mona::Neuron *)goToForest);

	// forestFound: (findForest -> forest)
	forestFound = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Forest found");
	forestFound->addEvent(Mona::CAUSE, (Mona::Neuron *)findForest);
	forestFound->addEvent(Mona::EFFECT, (Mona::Neuron *)forest);
	forestFound->setRepeater(true);

	// enableFindFood: (forestFound -> findFood)
	enableFindFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Enable find food");
	enableFindFood->addEvent(Mona::CAUSE, (Mona::Neuron *)forestFound);
	enableFindFood->addEvent(Mona::EFFECT, (Mona::Neuron *)findFood);

	// disableFindFood: (foodFound -> -findFood)
	disableFindFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, false, "Disable find food");
	disableFindFood->addEvent(Mona::CAUSE, (Mona::Neuron *)foodFound);
	disableFindFood->addEvent(Mona::EFFECT, (Mona::Neuron *)findFood);

	// getStone: -(stoneOnGround -> get)
	getStone = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, "Get stone");
	getStone->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneOnGround);
	getStone->addEvent(Mona::EFFECT, (Mona::Neuron *)get);

	// stoneGotten: (getStone -> gotStone)
	stoneGotten = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Stone gotten");
	stoneGotten->addEvent(Mona::CAUSE, (Mona::Neuron *)getStone);
	stoneGotten->addEvent(Mona::EFFECT, (Mona::Neuron *)gotStone);

	// findStone: -(desert -> lookForStone)
	findStone = this->mona->newMediator(Mona::MIN_ENABLEMENT, true, "Find stone");
	findStone->addEvent(Mona::CAUSE, (Mona::Neuron *)desert);
	findStone->addEvent(Mona::EFFECT, (Mona::Neuron *)lookForStone);

	// stoneFound: (findStone -> stoneOnGround)
	stoneFound = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Stone found");
	stoneFound->addEvent(Mona::CAUSE, (Mona::Neuron *)findStone);
	stoneFound->addEvent(Mona::EFFECT, (Mona::Neuron *)stoneOnGround);
	stoneFound->setRepeater(true);

	// findDesert: (anywhere -> goToDesert)
	findDesert = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Find desert");
	findDesert->addEvent(Mona::CAUSE, (Mona::Neuron *)anywhere);
	findDesert->addEvent(Mona::EFFECT, (Mona::Neuron *)goToDesert);

	// desertFound: (findDesert -> desert)
	desertFound = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Desert found");
	desertFound->addEvent(Mona::CAUSE, (Mona::Neuron *)findDesert);
	desertFound->addEvent(Mona::EFFECT, (Mona::Neuron *)desert);
	desertFound->setRepeater(true);

	// enableGetStone: (stoneFound -> getStone)
	enableGetStone = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Enable get stone");
	enableGetStone->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneFound);
	enableGetStone->addEvent(Mona::EFFECT, (Mona::Neuron *)getStone);

	// disableGetStone: (stoneGotten -> -getStone)
	disableGetStone = this->mona->newMediator(Mona::MAX_ENABLEMENT, false, "Disable get stone");
	disableGetStone->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneGotten);
	disableGetStone->addEvent(Mona::EFFECT, (Mona::Neuron *)getStone);

	// enableFindStone: (desertFound -> findStone)
	enableFindStone = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Enable find stone");
	enableFindStone->addEvent(Mona::CAUSE, (Mona::Neuron *)desertFound);
	enableFindStone->addEvent(Mona::EFFECT, (Mona::Neuron *)findStone);

	// disableFindStone: (stoneFound -> -findStone)
	disableFindStone = this->mona->newMediator(Mona::MAX_ENABLEMENT, false, "Disable find stone");
	disableFindStone->addEvent(Mona::CAUSE, (Mona::Neuron *)stoneFound);
	disableFindStone->addEvent(Mona::EFFECT, (Mona::Neuron *)findStone);

	// bringStoneToMate: (gotStone -> goToMate)
	bringStoneToMate = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Bring stone to mate");
	bringStoneToMate->addEvent(Mona::CAUSE, (Mona::Neuron *)gotStone);
	bringStoneToMate->addEvent(Mona::EFFECT, (Mona::Neuron *)goToMate);

	// giveStoneToMate: (bringStoneToMate -> mateHasStone)
	giveStoneToMate = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Give stone to mate");
	giveStoneToMate->addEvent(Mona::CAUSE, (Mona::Neuron *)bringStoneToMate);
	giveStoneToMate->addEvent(Mona::EFFECT, (Mona::Neuron *)mateHasStone);
	giveStoneToMate->setRepeater(true);

	// mateTakesStone: (giveStoneToMate -> doNothing)
	mateTakesStone = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Mate takes stone");
	mateTakesStone->addEvent(Mona::CAUSE, (Mona::Neuron *)giveStoneToMate);
	mateTakesStone->addEvent(Mona::EFFECT, (Mona::Neuron *)doNothing);

	// mateAcceptsStone: (mateTakesStone -> mateStoneOK)
	mateAcceptsStone = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Mate accepts stone");
	mateAcceptsStone->addEvent(Mona::CAUSE, (Mona::Neuron *)mateTakesStone);
	mateAcceptsStone->addEvent(Mona::EFFECT, (Mona::Neuron *)mateStoneOK);

	// getFoodDisablesGetStone: (getFood -> -getStone)
	getFoodDisablesGetStone = this->mona->newMediator(Mona::MAX_ENABLEMENT, false, "Get food disables get stone");
	getFoodDisablesGetStone->addEvent(Mona::CAUSE, (Mona::Neuron *)getFood);
	getFoodDisablesGetStone->addEvent(Mona::EFFECT, (Mona::Neuron *)getStone);

	// getStoneDisablesGetFood: (getStone -> -getFood)
	getStoneDisablesGetFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, false, "Get stone disables get food");
	getStoneDisablesGetFood->addEvent(Mona::CAUSE, (Mona::Neuron *)getStone);
	getStoneDisablesGetFood->addEvent(Mona::EFFECT, (Mona::Neuron *)getFood);

	// tossObject: (anywhere -> toss)
	tossObject = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Toss object");
	tossObject->addEvent(Mona::CAUSE, (Mona::Neuron *)anywhere);
	tossObject->addEvent(Mona::EFFECT, (Mona::Neuron *)toss);

	// objectTossed: (tossObject -> noObject)
	objectTossed = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Toss object");
	objectTossed->addEvent(Mona::CAUSE, (Mona::Neuron *)tossObject);
	objectTossed->addEvent(Mona::EFFECT, (Mona::Neuron *)noObject);

	// tossEnablesGetFood: (tossObject -> getFood)
	tossEnablesGetFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Toss enables get food");
	tossEnablesGetFood->addEvent(Mona::CAUSE, (Mona::Neuron *)tossObject);
	tossEnablesGetFood->addEvent(Mona::EFFECT, (Mona::Neuron *)getFood);

	// tossDisablesFindStone: (tossObject -> -findStone)
	tossDisablesFindStone = this->mona->newMediator(Mona::MAX_ENABLEMENT, false, "Toss disables find stone");
	tossDisablesFindStone->addEvent(Mona::CAUSE, (Mona::Neuron *)tossObject);
	tossDisablesFindStone->addEvent(Mona::EFFECT, (Mona::Neuron *)findStone);

	// tossDisablesFindFood: (tossObject -> -findFood)
	tossDisablesFindFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, false, "Toss disables find food");
	tossDisablesFindFood->addEvent(Mona::CAUSE, (Mona::Neuron *)tossObject);
	tossDisablesFindFood->addEvent(Mona::EFFECT, (Mona::Neuron *)findFood);

	// bringFoodToMate: (gotFood -> goToMate)
	bringFoodToMate = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Bring food to mate");
	bringFoodToMate->addEvent(Mona::CAUSE, (Mona::Neuron *)gotFood);
	bringFoodToMate->addEvent(Mona::EFFECT, (Mona::Neuron *)goToMate);

	// giveFoodToMate: (bringFoodToMate -> mateHasFood)
	giveFoodToMate = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Give food to mate");
	giveFoodToMate->addEvent(Mona::CAUSE, (Mona::Neuron *)bringFoodToMate);
	giveFoodToMate->addEvent(Mona::EFFECT, (Mona::Neuron *)mateHasFood);
	giveFoodToMate->setRepeater(true);

	// mateTakesFood: (giveFoodToMate -> doNothing)
	mateTakesFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Mate takes food");
	mateTakesFood->addEvent(Mona::CAUSE, (Mona::Neuron *)giveFoodToMate);
	mateTakesFood->addEvent(Mona::EFFECT, (Mona::Neuron *)doNothing);

	// mateAcceptsFood: (mateTakesFood -> mateFoodOK)
	mateAcceptsFood = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Mate accepts food");
	mateAcceptsFood->addEvent(Mona::CAUSE, (Mona::Neuron *)mateTakesFood);
	mateAcceptsFood->addEvent(Mona::EFFECT, (Mona::Neuron *)mateFoodOK);

	// checkMate: (anywhere -> goToMate -> matePresent)
	checkMate = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Check mate");
	checkMate->addEvent(Mona::CAUSE, (Mona::Neuron *)anywhere);
	checkMate->addEvent(Mona::EFFECT, (Mona::Neuron *)goToMate);

	// mateChecked: (checkMate -> matePresent)
	mateChecked = this->mona->newMediator(Mona::MAX_ENABLEMENT, true, "Mate checked");
	mateChecked->addEvent(Mona::CAUSE, (Mona::Neuron *)checkMate);
	mateChecked->addEvent(Mona::EFFECT, (Mona::Neuron *)matePresent);
	mateChecked->setRepeater(true);

	// Goals:
	goals.alloc(mona->numNeeds);
	goals.set(0, FOOD_NEED);
	foodOK->goals.setGoals(&goals, 1.0);
	goals.set(0, 0.0);
	goals.set(1, MATE_FOOD_NEED);
	mateFoodOK->goals.setGoals(&goals, 1.0);
	goals.set(1, 0.0);
	goals.set(2, MATE_STONE_NEED);
	mateStoneOK->goals.setGoals(&goals, 1.0);
	goals.set(2, 0.0);
	goals.set(3, CHECK_MATE_NEED);
	matePresent->goals.setGoals(&goals, 1.0);
}
