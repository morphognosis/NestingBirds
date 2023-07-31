Nesting birds with the Mona neural network.

Mona 6.0 includes an implemenation of place neurons, in the form of specialized motor neurons,
which allow improved spatial navigation.

To build:

UNIX:
build_nestinbirds.sh, build_nestingbirds.bat in work directory.

Windows:
Start Microsoft Visual Studio using solutions.

Directories:

src/main/mona:
mona: mona neural network.
common files.
nestingbirds: nesting birds.
sensor_discriminator: sensor discrimination.

bin:
nestingbirds, nestingbirds_rnn executables.

work:
scripts

Commands:

run_nestingbirds.sh, run_nestingbirds.bat

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
      
run_nestingbirds_rnn.sh, run_nestingbirds_rnn.bat: train and test nestingbirds with RNN.

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

sensor_discriminator.py: discriminate input sensors by detecting signals that are important to the output.

Usage:
python sensor_discriminator.py 
       --sensor_input_file <input file name>
       --sensor_output_file <output file name>
       [--significant_output_delta <delta>]
       [--network_hidden_dimension <hidden layer dimension>]
       [--learning_rate <learning rate>]
       [--epochs <training epochs>]
       [--random_seed <random seed>]
       [--verbose <true | false>]
