# Nesting birds.

Abstract:

An animal behavior problem is proposed that involves two cooperating birds, a male and female.
The female builds a nest into which she lays an egg. The male's job is to forage in a forest for food for both himself
and the female. In addition, the male must fetch stones from a nearby desert for the female to use as nesting material.
Their task is complete when the nest is built and an egg is laid in it. It can be run in two modes: manual
(user-supplied responses) or birds, using a Mona neural network replay.

See:
<br>http://tom.portegys.com/research.html#nestingbirds
<br>http://tom.portegys.com/research.html#mona2

Import Eclipse project, build and run with scripts in the work directory.

Build nestingbirds.jar and nestingbirds executable in bin directory:
<pre>build_nestingbirds.sh, build_nestingbirds.bat</pre>

For the bird driver, first train/test birds and create a replay file. See src/main/mona/Readme.txt.

Run graphical interface:
<pre>
run_nestingbirds.sh, run_nestingbirds.bat

Usage:
    java morphognosis.nestingbirds.NestingBirds
      [-steps <steps> (default=single step)]
      [-responseDriver <manual | bird> (default=bird)]
      [-replayFile <replay file name> (for bird driver)]
      [-writeMaleDataset (write dataset file: male_dataset.csv)]
      [-writeFemaleDataset (write dataset file: female_dataset.csv)]
      [-verbose <true | false> (default=false)]
      [-version]
</pre>

Dataset preparation for external training and testing:

To create 3 dataset files (\<gender\>\_dataset\_\<run\>.csv) example:
<pre>run_nestingbirds_batch.sh -steps 1000 -runs 3 -writeMaleDataset -writeFemaleDataset</pre>

To train and test Morphognosis NN with 3 datasets (2 training and 1 testing) example:
<pre>nestingbirds_nn.sh -gender male -num_datasets 3 -num_test_datasets 1</pre>

To train and test RNN with 3 datasets (2 training and 1 testing) example:
<pre>nestingbirds_rnn.sh -gender male -num_datasets 3 -num_test_datasets 1</pre>
