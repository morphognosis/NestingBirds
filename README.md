# Nesting birds.

Abstract:

An animal behavior problem in the form of a game is proposed that involves two cooperating birds, a male and female. 
The female builds a nest
into which she lays an egg. The male's job is to forage in a forest for food for both himself and the
female. In addition, the male must fetch stones from a nearby desert for the female to use as nesting material. 
The game is complete when the nest is built and an egg is laid in it. The game can be run in three modes: manual
(user-supplied responses), "auto pilot" (self-playing), and using the bird's brain (supplied by a player).

See: 
<br>http://tom.portegys.com/research.html#nestingbirds
<br>http://tom.portegys.com/research.html#mona2

Build with Eclipse project and export to bin/nestingbirds.jar to run with scripts in the work directory.

Graphical interface:
<pre>
Usage:
    java morphognosis.nestingbirds.NestingBirdsDisplay
      [-steps <steps> (default=single step)]
      [-responseDriver <autopilot | bird> (default=autopilot)]
      [-maleInitialFood <amount> (default=200)]
      [-femaleInitialFood <amount> (default=200)]
      [-maleFoodDuration <amount> (default=200)]
      [-femaleFoodDuration <amount> (default=200)]
      [-randomizeMaleFoodLevel (food level probabilistically increases 0-200 upon eating food)]
      [-randomizeFemaleFoodLevel (food level probabilistically increases 0-200 upon eating food)]
      [-writeMaleDataset (write dataset file: male_dataset.csv)]
      [-writeFemaleDataset (write dataset file: female_dataset.csv)]
      [-verbose <true | false> (default=false)]
      [-randomSeed <seed> (default=4517)]
      [-version]
</pre>
Batch interface:
<pre>
Usage:
    java morphognosis.nestingbirds.NestingBirds
      -steps <steps>
      [-runs <runs> (default=1)]
      [-responseDriver <autopilot | bird> (default=autopilot)]
      [-maleInitialFood <amount> (default=200)]
      [-femaleInitialFood <amount> (default=200)]
      [-maleFoodDuration <amount> (default=200)]
      [-femaleFoodDuration <amount> (default=200)]
      [-randomizeMaleFoodLevel (food level probabilistically increases 0-200 upon eating food)]
      [-randomizeFemaleFoodLevel (food level probabilistically increases 0-200 upon eating food)]
      [-writeMaleDataset (write dataset file: male_dataset_<run>.csv)]
      [-writeFemaleDataset (write dataset file: female_dataset_<run>.csv)]
      [-verbose <true | false> (default=true)]
      [-randomSeed <seed> (default=4517)]
      [-version]
Exit codes:
  0=success
  1=error
</pre>

Nesting birds environment:
<br>To build: 
<pre>build_nestingbirds.bat, build_nestingbirds.sh</pre>
<br>To run: 
<pre>run_nestingbirds.bat, run_nestingbirds.sh</pre>
<br>To create 3 dataset files (\<gender\>\_dataset\_\<run\>.csv) example:
<pre>run_nestingbirds_batch.sh -steps 1000 -runs 3 -writeMaleDataset -writeFemaleDataset</pre>
<br>To train and test RNN with 3 datasets (2 training and 1 testing) example:
<pre>nestingbirds_rnn.sh -gender male -num_datasets 3 -num_test_datasets 1</pre>
