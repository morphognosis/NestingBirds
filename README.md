# Nesting birds.

Abstract:

An animal behavior problem involving two cooperating birds, a male and female.
The female builds a nest into which she lays an egg. The male's job is to forage in a forest for food for both himself
and the female. In addition, the male must fetch stones from a nearby desert for the female to use as nesting material.
Their task is complete when the nest is built and an egg is laid in it. The bird animations are replayed from
training and testing the birds in the environment.

Paper:
<br>https://www.biorxiv.org/content/10.1101/2023.05.30.542884v1
<br>http://tom.portegys.com/research.html#nestingbirds

Import Eclipse project, build and run with scripts in the work directory.

Build nestingbirds.jar and nestingbirds executables:
<pre>build_nestingbirds.sh, build_nestingbirds.bat</pre>

Train/test birds and create behavior files. See src/main/mona/Readme.txt.
<pre>run_nestingbirds.sh, run_nestingbirds.bat</pre>

Run graphical interface:
<pre>
run_nestingbirds_replay.sh, run_nestingbirds_replay.bat

Usage: java morphognosis.nestingbirds.NestingBirdsReplay -behaviorFile file_name
</pre>

Train/test RNN:
<pre>
run_nestingbirds_rnn.sh, run_nestingbirds_rnn.bat
</pre>

Original nesting birds:
<br>http://tom.portegys.com/research.html#mona2
<br>Download and run interactive demo: http://tom.portegys.com/research/NestViewer/NestViewer.zip
