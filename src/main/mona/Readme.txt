Nesting birds with the Mona neural network.

Mona 6.0 includes an implemenation of place neurons, in the form of specialized motor neurons,
which allow improved spatial navigation.

To build:

UNIX:
Execute 'make' to make executables.

Windows:
Start Microsoft Visual Studio using solution.

Directories:

mona: mona neural network.
common files.
nestingbirds: nesting birds.
../../../bin: nestingbirds executable.

Commands:

nestingbirds

Usage:
    nestingbirds
      -trainMale <male save file name> | -testMale <male load file name>
      -trainFemale <female save file name> | -testFemale <female load file name>
      -steps <steps>
      [-maleInitialFood <amount> (default=200]
      [-maleFoodDuration <amount> (default=200)]
      [-maleRandomizeFoodLevel (food level probabilistically increases 0-200 upon eating food)]
      [-maleMouseNeed <amount> (default=1.0)]
      [-maleFemaleMouseNeed <amount> (default=1.0)]
      [-maleFemaleStoneNeed <amount> (default=1.0)]
      [-maleAttendFemaleNeed <amount> (default=1.0)]
      [-femaleInitialFood <amount> (default=0)]
      [-femaleFoodDuration <amount> (default=100)]
      [-femaleRandomizeFoodLevel (food level probabilistically increases 0-100 upon eating food)]
      [-femaleMouseNeed <amount> (default=0.2)]
      [-femaleLayEggNeed <amount> (default=0.1)]
      [-femaleBroodEggNeed <amount> (default=0.3)]
      [-verbose <true | false> (default=true)]
      [-randomSeed <seed> (default=4517)]
      [-version]
    
