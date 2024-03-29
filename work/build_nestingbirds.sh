#!/bin/bash
javac -classpath "../lib/*" -d . ../src/main/java/morphognosis/nestingbirds/*.java
cp ../res/images/male.gif morphognosis/nestingbirds
cp ../res/images/female.gif morphognosis/nestingbirds
cp ../res/images/egg.gif morphognosis/nestingbirds
cp ../res/images/cactus.gif morphognosis/nestingbirds
cp ../res/images/tree.gif morphognosis/nestingbirds
cp ../res/images/stone.gif morphognosis/nestingbirds
cp ../res/images/mouse.gif morphognosis/nestingbirds
cp ../src/main/nn/nestingbirds_nn.py .
cp ../src/main/nn/nestingbirds_rnn.py .
jar cvfm ../bin/nestingbirds.jar nestingbirds.mf morphognosis nestingbirds_nn.py nestingbirds_rnn.py
(cd ../src/main/mona;make)

