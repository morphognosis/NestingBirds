# Test world grammar RNN performance.

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

randomSeed=$RANDOM
echo "random seed =" $randomSeed

echo numNonterminals,nonTerminalProductions,productionRightHandSideLength,numPaths,numPathExpansions,train_correct_paths,train_total_paths,train_prediction_errors,train_total_predictions,test_correct_paths,test_total_paths,test_prediction_errors,test_total_predictions > world_grammar_rnn_test_results.csv

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
           ./world_grammar.sh -generateGrammar -numNonterminals $numNonterminals -minNonterminalProductions $nonTerminalProductions -maxNonterminalProductions $nonTerminalProductions -minProductionRightHandSideLength $productionRightHandSideLength -maxProductionRightHandSideLength $productionRightHandSideLength -numPaths $numPaths -numPathExpansions $numPathExpansions -exportPathRNNdataset
           python world_grammar_rnn.py
           echo ${numNonterminals},${nonTerminalProductions},${productionRightHandSideLength},${numPaths},${numPathExpansions},$(cat world_grammar_rnn_results.json | cut -d'"' -f4,8,12,16,20,24,28,32 | sed 's/"/,/g') >> world_grammar_rnn_test_results.csv
        done
      done
    done
  done
done
