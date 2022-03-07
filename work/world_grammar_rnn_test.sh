# Test world grammar RNN performance.

# Parameters:
minTerminals=5
incrTerminals=3
maxTerminals=11
minNonterminals=1
incrNonTerminals=3
maxNonterminals=10
minProductionRightHandSideLength=5
maxProductionRightHandSideLength=15
minTerminalProductionProbability=.2
incrTerminalProductionProbability=.2
maxTerminalProductionProbability=.8
minNonterminalExpansions=5
incrNonterminalsExpansions=5
maxNonterminalExpansions=15
minPaths=10
incrPaths=5
maxPaths=30

randomSeed=$RANDOM
echo "random seed =" $randomSeed

for terminals in $(seq $minTerminals $incrTerminals $maxTerminals)
do
  for nonTerminals in $(seq $minNonterminals $incrNonterminals $maxNonterminals)
  do
    for terminalProductionProbability in $(seq $minTerminalProductionProbability $incrTerminalProductionProbability $maxTerminalProductionProbability)
    do
      for numNonterminalExpansions in $(seq $minNonterminalExpansions $incrNonterminalExpansions $maxNonterminalExpansions)
      do
        for numPaths in $(seq $minPaths $incrPaths $maxPaths)
        do
             echo terminals = $terminals nonTerminals = $nonTerminals minProductionRightHandSideLength = $minProductionRightHandSideLength maxProductionRightHandSideLength = $maxProductionRightHandSideLength terminalProductionProbability = $terminalProductionProbability numNonterminalExpansions = $numNonterminalExpansions numPaths = $numPaths
             ./world_grammar.sh -generateGrammar -numTerminals $terminals -numNonterminals $nonTerminals -minProductionRightHandSideLength $minProductionRightHandSideLength -maxProductionRightHandSideLength $maxProductionRightHandSideLength -terminalProductionProbability $terminalProductionProbability -numNonterminalExpansions $numNonterminalExpansions -numPaths $numPaths -exportPathDataset
             python world_grammar_rnn.py
             cat world_grammar_rnn_results.json
        done
      done
    done
  done
done
