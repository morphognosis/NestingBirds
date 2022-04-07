# Test world path TCN performance.

# Parameters:
minNonterminals=1
incrNonterminals=3
maxNonterminals=16
minNonterminalProductions=1
incrNonterminalProductions=2
maxNonterminalProductions=5
minProductionRightHandSideLength=1
incrProductionRightHandSideLength=2
maxProductionRightHandSideLength=5
minPaths=10
incrPaths=5
maxPaths=30
minPathExpansions=1
incrPathExpansions=1
maxPathExpansions=4
minKernelSize=2
incrKernelSize=2
maxKernelSize=4

randomSeed=$RANDOM
echo "random seed =" $randomSeed

echo numNonterminals,nonTerminalProductions,productionRightHandSideLength,numPaths,numPathExpansions,kernelSize,train_correct_paths,train_total_paths,train_prediction_errors,train_total_predictions,train_error_pct,test_correct_paths,test_total_paths,test_prediction_errors,test_total_predictions,test_error_pct > world_path_tcn_test_results.csv

for numNonterminals in $(seq $minNonterminals $incrNonterminals $maxNonterminals)
do
  for nonTerminalProductions in $(seq $minNonterminalProductions $incrNonterminalProductions $maxNonterminalProductions)
  do
    for productionRightHandSideLength in $(seq $minProductionRightHandSideLength $incrProductionRightHandSideLength $maxProductionRightHandSideLength)
    do
      for numPaths in $(seq $minPaths $incrPaths $maxPaths)
      do
        for numPathExpansions in $(seq $minPathExpansions $incrPathExpansions $maxPathExpansions)
        do
          for kernelSize in $(seq $minKernelSize $incrKernelSize $maxKernelSize)
          do
            ./world_grammar.sh -generateGrammar -numNonterminals $numNonterminals -minNonterminalProductions $nonTerminalProductions -maxNonterminalProductions $nonTerminalProductions -minProductionRightHandSideLength $productionRightHandSideLength -maxProductionRightHandSideLength $productionRightHandSideLength -numPaths $numPaths -numPathExpansions $numPathExpansions -exportPathTCNdataset
            python world_grammar_tcn.py --kernel_size $kernelSize
            echo ${numNonterminals},${nonTerminalProductions},${productionRightHandSideLength},${numPaths},${numPathExpansions},${kernelSize},$(cat world_path_tcn_results.json | cut -d'"' -f4,8,12,16,20,24,28,32,36,40 | sed 's/"/,/g') >> world_path_tcn_test_results.csv
          done
        done
      done
    done
  done
done
