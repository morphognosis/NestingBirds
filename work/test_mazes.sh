#!/bin/bash
# Generate results.
MIN_NUM_DOORS=1
MAX_NUM_DOORS=5
MIN_MAZE_INTERIOR_LENGTH=5
MAX_MAZE_INTERIOR_LENGTH=10
MIN_CONTEXT_MAZES=10
MAX_CONTEXT_MAZES=15
MIN_NUM_INDEPENDENT_MAZES=10
MAX_NUM_INDEPENDENT_MAZES=15

echo "numDoors,mazeInteriorLength,numContextMazes,numIndependentMazes,trial,source,trainSuccess,trainTotal,trainPct,testSuccess,testTotal,testPct"
for (( numDoors=${MIN_NUM_DOORS}; numDoors <= $MAX_NUM_DOORS; ++numDoors)); do
  for (( mazeInteriorLength=${MIN_MAZE_INTERIOR_LENGTH}; mazeInteriorLength <= $MAX_MAZE_INTERIOR_LENGTH; ++mazeInteriorLength)); do
    for (( numContextMazes=${MIN_CONTEXT_MAZES}; numContextMazes <= $MAX_CONTEXT_MAZES; ++numContextMazes)); do
      for (( numIndependentMazes=${MIN_NUM_INDEPENDENT_MAZES}; numIndependentMazes <= $MAX_NUM_INDEPENDENT_MAZES; ++numIndependentMazes)); do
        trials=10
       for (( i=0; i< $trials; ++i)); do
            randomSeed=`expr $i + 4517`
            results=`./maze.sh -batch -responseDriver metamorphNN -randomSeed $randomSeed -numDoors $numDoors -mazeInteriorLength $mazeInteriorLength -numContextMazes $numContextMazes -numIndependentMazes $numIndependentMazes | tail -2`
            morph_train_success=`echo $results | cut -d' ' -f4 | cut -d'/' -f1`
            morph_train_total=`echo $results | cut -d' ' -f4 | cut -d'/' -f2`
            morph_train_results=`echo $results | cut -d'(' -f2 | cut -d'.' -f1`
            morph_test_success=`echo $results | cut -d' ' -f9 | cut -d'/' -f1`
            morph_test_total=`echo $results | cut -d' ' -f9 | cut -d'/' -f2`
            morph_test_results=`echo $results | cut -d'(' -f3 | cut -d'.' -f1`
            echo ${numDoors},${mazeInteriorLength},${numContextMazes},${numIndependentMazes},${i},morph,${morph_train_success},${morph_train_total},${morph_train_results},${morph_test_success},${morph_test_total},${morph_test_results}
            results=`python maze_rnn.py 2>/dev/null | tail -2`
            rnn_train_success=`echo $results | cut -d' ' -f4 | cut -d'/' -f1`
            rnn_train_total=`echo $results | cut -d' ' -f4 | cut -d'/' -f2`
            rnn_train_results=`echo $results | cut -d'(' -f2 | cut -d'.' -f1`
            rnn_test_success=`echo $results | cut -d' ' -f9 | cut -d'/' -f1`
            rnn_test_total=`echo $results | cut -d' ' -f9 | cut -d'/' -f2`
            rnn_test_results=`echo $results | cut -d'(' -f3 | cut -d'.' -f1`
            echo ${numDoors},${mazeInteriorLength},${numContextMazes},${numIndependentMazes},${i},rnn,${rnn_train_success},${rnn_train_total},${rnn_train_results},${rnn_test_success},${rnn_test_total},${rnn_test_results}
        done
      done
    done
  done
done