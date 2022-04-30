# Test world composer performance on time consolidation.

if [ "$1" = "" ]
then
   echo "Usage: world_composer_test_time_consolidation.sh <number of runs>"
   exit 1
fi
runs=$1

# Parameters:
minBasePathLength=10
incrBasePathLength=10
maxBasePathLength=20
minModularPathLength=2
maxModularPathLength=5
minNumModularPaths=5
incrNumModularPaths=5
maxNumModularPaths=10
minNumNeurons=128
incrNumNeurons=128
maxNumNeurons=256
minKernelSize=2
incrKernelSize=2
maxKernelSize=4

echo basePathLength,numModularPaths,numNeurons,pathModificationType,train_prediction_errors,train_total_predictions,train_error_pct,test_prediction_errors,test_total_predictions,test_error_pct > world_composer_nn_dilate_normalize_consolitate_2_test_results.csv
echo basePathLength,numModularPaths,numNeurons,pathModificationType,train_prediction_errors,train_total_predictions,train_error_pct,test_prediction_errors,test_total_predictions,test_error_pct > world_composer_nn_dilate_normalize_consolitate_3_test_results.csv

for basePathLength in $(seq $minBasePathLength $incrBasePathLength $maxBasePathLength)
do
 for numModularPaths in $(seq $minNumModularPaths $incrNumModularPaths $maxNumModularPaths)
 do
  for numNeurons in $(seq $minNumNeurons $incrNumNeurons $maxNumNeurons)
  do
    for i in $(seq $runs)
    do
     numInsertionTestPaths=$(( $basePathLength / 2 ))
     numSubstitutionTestPaths=$(( $basePathLength / 2 ))
     numDeletionTestPaths=$(( $basePathLength / 2 ))
     randomSeed=$RANDOM

     ./world_composer.sh -basePathLength $basePathLength -minModularPathLength $minModularPathLength -maxModularPathLength $maxModularPathLength -numModularPaths $numModularPaths -numInsertionTestPaths $numInsertionTestPaths -numSubstitutionTestPaths 0 -numDeletionTestPaths 0 -dilateEvents normalize -consolidateTime 2 -randomSeed $randomSeed
     python world_path_nn.py --neurons $numNeurons
     echo ${basePathLength},${numModularPaths},${numNeurons},insert,$(cat world_path_nn_results.json | cut -d'"' -f4,8,12,16,20,24 | sed 's/"/,/g') >> world_composer_nn_dilate_normalize_consolitate_2_test_results.csv
     ./world_composer.sh -basePathLength $basePathLength -minModularPathLength $minModularPathLength -maxModularPathLength $maxModularPathLength -numModularPaths $numModularPaths -numInsertionTestPaths $numInsertionTestPaths -numSubstitutionTestPaths 0 -numDeletionTestPaths 0 -dilateEvents normalize -consolidateTime 3 -randomSeed $randomSeed
     python world_path_nn.py --neurons $numNeurons
     echo ${basePathLength},${numModularPaths},${numNeurons},insert,$(cat world_path_nn_results.json | cut -d'"' -f4,8,12,16,20,24 | sed 's/"/,/g') >> world_composer_nn_dilate_normalize_consolitate_3_test_results.csv

     ./world_composer.sh -basePathLength $basePathLength -minModularPathLength $minModularPathLength -maxModularPathLength $maxModularPathLength -numModularPaths $numModularPaths -numInsertionTestPaths 0 -numSubstitutionTestPaths $numSubstitutionTestPaths -numDeletionTestPaths 0 -dilateEvents normalize -consolidateTime 2 -randomSeed $randomSeed
     python world_path_nn.py --neurons $numNeurons
     echo ${basePathLength},${numModularPaths},${numNeurons},substitute,$(cat world_path_nn_results.json | cut -d'"' -f4,8,12,16,20,24 | sed 's/"/,/g') >> world_composer_nn_dilate_normalize_consolitate_2_test_results.csv
     ./world_composer.sh -basePathLength $basePathLength -minModularPathLength $minModularPathLength -maxModularPathLength $maxModularPathLength -numModularPaths $numModularPaths -numInsertionTestPaths 0 -numSubstitutionTestPaths $numSubstitutionTestPaths -numDeletionTestPaths 0 -dilateEvents normalize -consolidateTime 3 -randomSeed $randomSeed
     python world_path_nn.py --neurons $numNeurons
     echo ${basePathLength},${numModularPaths},${numNeurons},substitute,$(cat world_path_nn_results.json | cut -d'"' -f4,8,12,16,20,24 | sed 's/"/,/g') >> world_composer_nn_dilate_normalize_consolitate_3_test_results.csv

     ./world_composer.sh -basePathLength $basePathLength -minModularPathLength $minModularPathLength -maxModularPathLength $maxModularPathLength -numModularPaths $numModularPaths -numInsertionTestPaths 0 -numSubstitutionTestPaths 0 -numDeletionTestPaths $numDeletionTestPaths -dilateEvents normalize -consolidateTime 2 -randomSeed $randomSeed
     python world_path_nn.py --neurons $numNeurons
     echo ${basePathLength},${numModularPaths},${numNeurons},delete,$(cat world_path_nn_results.json | cut -d'"' -f4,8,12,16,20,24 | sed 's/"/,/g') >> world_composer_nn_dilate_normalize_consolitate_2_test_results.csv
     ./world_composer.sh -basePathLength $basePathLength -minModularPathLength $minModularPathLength -maxModularPathLength $maxModularPathLength -numModularPaths $numModularPaths -numInsertionTestPaths 0 -numSubstitutionTestPaths 0 -numDeletionTestPaths $numDeletionTestPaths -dilateEvents normalize -consolidateTime 3 -randomSeed $randomSeed
     python world_path_nn.py --neurons $numNeurons
     echo ${basePathLength},${numModularPaths},${numNeurons},delete,$(cat world_path_nn_results.json | cut -d'"' -f4,8,12,16,20,24 | sed 's/"/,/g') >> world_composer_nn_dilate_normalize_consolitate_3_test_results.csv

     ./world_composer.sh -basePathLength $basePathLength -minModularPathLength $minModularPathLength -maxModularPathLength $maxModularPathLength -numModularPaths $numModularPaths -numInsertionTestPaths $numInsertionTestPaths -numSubstitutionTestPaths $numSubstitutionTestPaths -numDeletionTestPaths $numDeletionTestPaths -dilateEvents normalize -consolidateTime 2 -randomSeed $randomSeed
     python world_path_nn.py --neurons $numNeurons
     echo ${basePathLength},${numModularPaths},${numNeurons},all,$(cat world_path_nn_results.json | cut -d'"' -f4,8,12,16,20,24 | sed 's/"/,/g') >> world_composer_nn_dilate_normalize_consolitate_2_test_results.csv
     ./world_composer.sh -basePathLength $basePathLength -minModularPathLength $minModularPathLength -maxModularPathLength $maxModularPathLength -numModularPaths $numModularPaths -numInsertionTestPaths $numInsertionTestPaths -numSubstitutionTestPaths $numSubstitutionTestPaths -numDeletionTestPaths $numDeletionTestPaths -dilateEvents normalize -consolidateTime 3 -randomSeed $randomSeed
     python world_path_nn.py --neurons $numNeurons
     echo ${basePathLength},${numModularPaths},${numNeurons},all,$(cat world_path_nn_results.json | cut -d'"' -f4,8,12,16,20,24 | sed 's/"/,/g') >> world_composer_nn_dilate_normalize_consolitate_3_test_results.csv

    done
  done
 done
done

exit 0

