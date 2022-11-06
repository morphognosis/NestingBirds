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

Nesting birds environment:
<br>To build: build_nestingbirds.bat, build_nestingbirds.sh
<br>To run: run_nestingbirds.bat, run_nestingbirds.sh
<br>To create 3 dataset files (\<gender\>_dataset_\<run\>.csv) example:
<pre>run_nestingbirds_batch.sh -steps 1000 -runs 3 -writeMaleDataset -writeFemaleDataset</pre>
<br>To train and test RNN with 3 datasets (2 training and 1 testing) example:
<pre>nestingbirds_rnn.sh -gender male -num_datasets 3 -num_test_datasets 1</pre>
