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

#ifndef __MALE__
#define __MALE__

// The male bird definitions

#include "monami.hpp"

class Male : public MonAmi
{

public:

	// Needs.
	static const Mona::NEED FOOD_NEED;
	static const Mona::NEED MATE_FOOD_NEED;
	static const Mona::NEED MATE_STONE_NEED;
	static const Mona::NEED CHECK_MATE_NEED;

	// Constructor
	Male(Mona *mona);
};
#endif
