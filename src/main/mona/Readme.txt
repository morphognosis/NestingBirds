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
../../../bin: nestingbirds and nestingbirds_rnn executables.

Commands:

nestingbirds: run nestingbirds.

Usage:
    nestingbirds
      -trainMale | -testMale
      -trainFemale | -testFemale
      -steps <steps>
      [-saveMaleFile <save file name>]
      [-loadMaleFile <load file name>]
      [-saveFemaleFile <save file name>]
      [-loadFemaleFile <load file name>]
      [-maleInitialFood <amount> (default=200]
      [-maleFoodDuration <amount> (default=200)]
      [-maleRandomizeFoodLevel (food level probabilistically increases 0-200 upon eating mouse)]
      [-maleMouseNeed <amount> (default=1.0)]
      [-maleFemaleMouseNeed <amount> (default=1.0)]
      [-maleFemaleStoneNeed <amount> (default=1.0)]
      [-maleAttendFemaleNeed <amount> (default=1.0)]
      [-femaleInitialFood <amount> (default=0)]
      [-femaleFoodDuration <amount> (default=100)]
      [-femaleRandomizeFoodLevel (food level probabilistically increases 0-100 upon eating mouse)]
      [-femaleMouseNeed <amount> (default=0.2)]
      [-femaleLayEggNeed <amount> (default=0.1)]
      [-femaleBroodEggNeed <amount> (default=0.3)]
      [-randomSeed <seed> (default=4517)]
      [-mouseMoveProbability <probability> (default=0.1)]
      [-writeBehaviorFile <behavior file name> (json)]
      [-verbose <true | false> (default=true)]
      [-version]
      
nestingbirds_rnn: train and test nestingbirds with RNN.

Usage:
    nestingbirds_rnn
      -steps <steps>
      -trainRandomSeeds <random seeds> (comma-separated list)
      [-testRandomSeed <random seed> [-dynamic (world generates sensory inputs from predicted responses)]]
      [-verbose <true | false> (default=true)]
      
Results: 
nestingbirds_rnn_male_train_results.json
nestingbirds_rnn_male_test_results.json
nestingbirds_rnn_female_train_results.json
nestingbirds_rnn_female_test_results.json
