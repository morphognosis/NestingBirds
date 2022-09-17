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

// The Mona nesting birds program

/*

This program illustrates how a neural-network can be used
to simulate a pair of nesting birds.  One of the birds builds
the nest; the other forages for food and nesting materials.

*/

#include "male.hpp"
#include "female.hpp"
#ifdef UNIX
#include <unistd.h>
#endif

// Type definitions.
typedef Environment::OUTPUT OUTPUT;
typedef Mona::RESPONSE RESPONSE;

// Parameters
const int NumSensors = 9;
const int MaxResponse = 15;
const int NumMaleNeeds = 4;
const int NumFemaleNeeds = 2;
const long DefaultRandomSeed = 15;
long RandomSeed = -1;

// Object declarations
Environment Environment;
Mona Male(NumSensors, MaxResponse, NumMaleNeeds);
class Male MaleAmi(&Male);
Mona Female(NumSensors, MaxResponse, NumFemaleNeeds);
class Female FemaleAmi(&Female);

// "Body" needs
void maleNeeds(OUTPUT);
void femaleNeeds(OUTPUT);

#ifdef ACTIVITY_TRACKING
char *Usemsg = "Usage: %s -cycles <run cycles> [-dump_frequency <dump frequency> -enablement_dump -drive_dump] [-random_seed <random seed>] | -version\n";
#else
char *Usemsg = "Usage: %s -cycles <run cycles> [-dump_frequency <dump frequency>] [-random_seed <random seed>] | -version\n";
#endif

int
main(int argc, char *argv[])
{
	OUTPUT output;
	RESPONSE response;
	int i,cycles,dumpFrequency;
	char buf[100];
#ifdef ACTIVITY_TRACKING
	bool enablementDump = false;
	bool driveDump = false;
#endif

	cycles = dumpFrequency = -1;
    for (i = 1; i < argc; i++)
    {
		if (strcmp(argv[i], "-cycles") == 0)
		{
			i++;
			if (cycles >= 0)
			{
				fprintf(stderr,Usemsg,argv[0]);
				exit(1);
			}
			if ((cycles = atoi(argv[i])) < 0)
			{
				fprintf(stderr,Usemsg,argv[0]);
				exit(1);
			}
			continue;
		}

		if (strcmp(argv[i], "-dump_frequency") == 0)
		{
			i++;
			if (dumpFrequency > 0)
			{
				fprintf(stderr,Usemsg,argv[0]);
				exit(1);
			}
			if ((dumpFrequency = atoi(argv[i])) <= 0)
			{
				fprintf(stderr,"%s: dump frequency must be > 0\n",argv[0]);
				exit(1);
			}
			continue;
		}

#ifdef ACTIVITY_TRACKING
		if (strcmp(argv[i], "-enablement_dump") == 0)
		{
			enablementDump = true;
			continue;
		}

		if (strcmp(argv[i], "-drive_dump") == 0)
		{
			driveDump = true;
			continue;
		}
#endif

		if (strcmp(argv[i], "-random_seed") == 0)
		{
			i++;
			if (RandomSeed != -1)
			{
				fprintf(stderr,Usemsg,argv[0]);
				exit(1);
			}
			if ((RandomSeed = atoi(argv[i])) < 0)
			{
				fprintf(stderr,Usemsg,argv[0]);
				exit(1);
			}
			continue;
		}

		if (strcmp(argv[i], "-version") == 0)
		{
			Mona::version();
			Environment.version();
			exit(0);
		}

		fprintf(stderr,Usemsg,argv[0]);
		exit(1);
	}
	if (cycles < 0)
	{
		fprintf(stderr,Usemsg,argv[0]);
		exit(1);
	}
	if (dumpFrequency > cycles)
	{
		fprintf(stderr,"%s: dump frequency must be <= cycles\n",argv[0]);
		exit(1);
	}
#ifdef ACTIVITY_TRACKING
	if (dumpFrequency > 0)
	{
		if (!enablementDump && !driveDump)
		{
			fprintf(stderr,"%s: must dump either enablement or drive\n",argv[0]);
			exit(1);
		}
	}
#endif
	if (RandomSeed == -1)
	{
		RandomSeed = DefaultRandomSeed;
	}
	srand(RandomSeed);

	if (dumpFrequency != -1)
	{
#ifndef ACTIVITY_TRACKING
		Male.dumpNetwork(Mona::GRAPH, "Male bird network", false);
		Female.dumpNetwork(Mona::GRAPH, "Female bird network", false);
#endif
	}
#ifdef VIEWER
		Environment.output(Environment::MALE, output);
		maleNeeds(output);
		cout << "%%__Viewer:male.needs=";
		cout << Male.getNeed(0) << ",";
		cout << Male.getNeed(1) << ",";
		cout << Male.getNeed(2) << ",";
		cout << Male.getNeed(3) << "\n";
		Environment.output(Environment::FEMALE, output);
		femaleNeeds(output);
		cout << "%%__Viewer:female.needs=";
		cout << Female.getNeed(0) << ",";
		cout << Female.getNeed(1) << "\n";
		cout << "%%__Viewer:break\n";
#endif

	for (i = 0; i < cycles; i++)
	{
#ifdef MONA_TRACE
		cout << "***Male activated***\n";
#endif
		Environment.output(Environment::MALE, output);
		maleNeeds(output);
#ifdef VIEWER
		cout << "%%__Viewer:male.needs=";
#endif
#ifdef MONA_TRACE
		cout << "Male needs:" << "\n";
#endif
#if VIEWER || MONA_TRACE
		cout << Male.getNeed(0) << ",";
		cout << Male.getNeed(1) << ",";
		cout << Male.getNeed(2) << ",";
		cout << Male.getNeed(3) << "\n";
#endif
		response = Male.cycle(MonAmi::outputToSensors(output));
		Environment.input(Environment::MALE, MonAmi::responseToInput(response));
#ifdef MONA_TRACE
		cout << "Male sensors: ( ";
		Environment.printOutput(output);
		cout << ")\n";
		cout << "Response: ( ";
		Environment.printInput(MonAmi::responseToInput(response));
		cout << " )\n";
#endif

#ifdef MONA_TRACE
		cout << "***Female activated***\n";
#endif
		Environment.output(Environment::FEMALE, output);
		femaleNeeds(output);
#ifdef VIEWER
		cout << "%%__Viewer:female.needs=";
#endif
#ifdef MONA_TRACE
		cout << "Female needs:" << "\n";
#endif
#if VIEWER || MONA_TRACE
		cout << Female.getNeed(0) << ",";
		cout << Female.getNeed(1) << "\n";
#endif
		response = Female.cycle(MonAmi::outputToSensors(output));
		Environment.input(Environment::FEMALE, MonAmi::responseToInput(response));
#ifdef MONA_TRACE
		cout << "Female sensors: ( ";
		Environment.printOutput(output);
		cout << ")\n";
		cout << "Response: ( ";
		Environment.printInput(MonAmi::responseToInput(response));
		cout << " )\n";
#endif

		if (dumpFrequency != -1 && (i+1)%dumpFrequency == 0)
		{
#ifdef ACTIVITY_TRACKING
			FILE *fp = fopen("male.txt", "w");
			sprintf(buf,"Male bird network - cycle = %d",i);
			Male.dumpNetwork(Mona::GRAPH, buf, enablementDump, driveDump, false, fp);
			fclose(fp);
			fp = fopen("female.txt", "w");
			sprintf(buf,"Female bird network - cycle = %d",i);
			Female.dumpNetwork(Mona::GRAPH, buf, enablementDump, driveDump, false, fp);
			fclose(fp);
#ifdef UNIX
			char buf[100];
			sprintf(buf, "dot -Tjpg -omale%d.jpg male.txt", i);
			system(buf);
			sprintf(buf, "dot -Tjpg -ofemale%d.jpg female.txt", i);
			system(buf);
#endif
			unlink("male.txt");
			unlink("female.txt");
#else
			sprintf(buf,"Male bird network - cycle = %d",i);
			Male.dumpNetwork(Mona::GRAPH, buf, false);
			sprintf(buf,"Female bird network - cycle = %d",i);
			Female.dumpNetwork(Mona::GRAPH, buf, false);
#endif
		}
	}

	exit(0);
}

// Male needs
void
maleNeeds(OUTPUT output)
{
	if (output.condition.wantsFood)
	{
		Male.setNeed(0, Male::FOOD_NEED);
	} else {
		Male.setNeed(0, 0.0);
	}
	if (output.mate.present)
	{
		if (output.mate.condition.wantsFood)
		{
			Male.setNeed(1, Male::MATE_FOOD_NEED);
		} else {
			Male.setNeed(1, 0.0);
		}
	}
	if (output.mate.present)
	{
		if (output.mate.condition.wantsStone)
		{
			Male.setNeed(2, Male::MATE_STONE_NEED);
		} else {
			Male.setNeed(2, 0.0);
		}
	}
	Male.setNeed(3, Male::CHECK_MATE_NEED);
}

// Female needs
void
femaleNeeds(OUTPUT output)
{
	if (output.condition.wantsFood)
	{
		Female.setNeed(0, Female::FOOD_NEED);
	} else {
		Female.setNeed(0, 0.0);
	}

	// Maintain need to check nest
	Female.setNeed(1, Female::EGG_NEED);
}
