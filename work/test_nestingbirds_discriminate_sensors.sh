# Train and test birds.
./run_nestingbirds.sh -trainMale -saveMaleFile male.net -trainFemale -saveFemaleFile female.net -maleRandomizeFoodLevel -femaleRandomizeFoodLevel -randomSeed 1 -steps 1000 -discriminateSensors > train.txt
./run_nestingbirds.sh -testMale -loadMaleFile male.net -testFemale -loadFemaleFile female.net -maleRandomizeFoodLevel -femaleRandomizeFoodLevel -randomSeed 1 -steps 1000 -writeBehaviorFile test_behavior.json > test.txt

