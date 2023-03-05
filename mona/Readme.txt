Nesting birds with the Mona neural network.

To build:

UNIX:
Execute 'make' to make executables.

Windows:
Start Microsoft Visual Studio using solution.

Directories:

src:
- mona: mona neural network.
- common files.
- nestingbirds: nesting birds.
bin: executables.

Commands:

nestingbirds: run nestingbirds program.

Usage:

    nestingbirds
      -trainMale <male save file name> | -testMale <male load file name>
      -trainFemale <female save file name> | -testFemale <female load file name>
      -steps <steps>
      [-maleInitialFood <amount> (default=200]
      [-maleFoodDuration <amount> (default=200)]
      [-maleRandomizeFoodLevel (food level probabilistically increases 0-200 upon eating food)]
      [-maleMouseNeed <amount> (default=0.5)]
      [-maleFemaleMouseNeed <amount> (default=0.4)]
      [-maleStoneNeed <amount> (default=0.3)]
      [-maleFemaleStoneNeed <amount> (default=0.2)]
      [-femaleInitialFood <amount> (default=75)]
      [-femaleFoodDuration <amount> (default75)]
      [-femaleRandomizeFoodLevel (food level probabilistically increases 0-75 upon eating food)]
      [-femaleMouseNeed <amount> (default=0.3)]
      [-femaleStoneNeed <amount> (default=0.1)]
      [-femaleLayEggNeed <amount> (default=0.2)]
      [-femaleBroodEggNeed <amount> (default=0.4)]
      [-verbose <true | false> (default=true)]
      [-randomSeed <seed> (default=4517)]
      [-version]

