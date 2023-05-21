# Train and test birds with RNN
for i in {1..5}
do
./run_nestingbirds_rnn.sh -trainRandomSeeds ${i} -testRandomSeed ${i} -steps 1000 > test_rnn_${i}_${i}.txt
cp nestingbirds_rnn_male_train_results.json nestingbirds_rnn_male_train_results_${i}_${i}.json
cp nestingbirds_rnn_male_test_results.json nestingbirds_rnn_male_test_results_${i}_${i}.json
cp nestingbirds_rnn_female_train_results.json nestingbirds_rnn_female_train_results_${i}_${i}.json
cp nestingbirds_rnn_female_test_results.json nestingbirds_rnn_female_test_results_${i}_${i}.json
done
for i in {1..5}
do
j=$((i + 1))
./run_nestingbirds_rnn.sh -trainRandomSeeds ${j} -testRandomSeed ${i} -steps 1000 > test_rnn_${j}_${i}.txt
cp nestingbirds_rnn_male_train_results.json nestingbirds_rnn_male_train_results_${j}_${i}.json
cp nestingbirds_rnn_male_test_results.json nestingbirds_rnn_male_test_results_${j}_${i}.json
cp nestingbirds_rnn_female_train_results.json nestingbirds_rnn_female_train_results_${j}_${i}.json
cp nestingbirds_rnn_female_test_results.json nestingbirds_rnn_female_test_results_${j}_${i}.json
done
for i in {1..5}
do
j=$((i + 1))
k=$((j + 1))
./run_nestingbirds_rnn.sh -trainRandomSeeds ${j},${k} -testRandomSeed ${i} -steps 1000 > test_rnn_${j}_${k}_${i}.txt
cp nestingbirds_rnn_male_train_results.json nestingbirds_rnn_male_train_results_${j}_${k}_${i}.json
cp nestingbirds_rnn_male_test_results.json nestingbirds_rnn_male_test_results_${j}_${k}_${i}.json
cp nestingbirds_rnn_female_train_results.json nestingbirds_rnn_female_train_results_${j}_${k}_${i}.json
cp nestingbirds_rnn_female_test_results.json nestingbirds_rnn_female_test_results_${j}_${k}_${i}.json
done
for i in {1..5}
do
j=$((i + 1))
k=$((j + 1))
n=$((k + 1))
./run_nestingbirds_rnn.sh -trainRandomSeeds ${j},${k},${n} -testRandomSeed ${i} -steps 1000 > test_rnn_${j}_${k}_${n}_${i}.txt
cp nestingbirds_rnn_male_train_results.json nestingbirds_rnn_male_train_results_${j}_${k}_${n}_${i}.json
cp nestingbirds_rnn_male_test_results.json nestingbirds_rnn_male_test_results_${j}_${k}_${n}_${i}.json
cp nestingbirds_rnn_female_train_results.json nestingbirds_rnn_female_train_results_${j}_${k}_${n}_${i}.json
cp nestingbirds_rnn_female_test_results.json nestingbirds_rnn_female_test_results_${j}_${k}_${n}_${i}.json
done
for i in {1..5}
do
j=$((i + 1))
k=$((j + 1))
n=$((k + 1))
p=$((n + 1))
./run_nestingbirds_rnn.sh -trainRandomSeeds ${j},${k},${n},${p} -testRandomSeed ${i} -steps 1000 > test_rnn_${j}_${k}_${n}_${p}_${i}.txt
cp nestingbirds_rnn_male_train_results.json nestingbirds_rnn_male_train_results_${j}_${k}_${n}_${p}_${i}.json
cp nestingbirds_rnn_male_test_results.json nestingbirds_rnn_male_test_results_${j}_${k}_${n}_${p}_${i}.json
cp nestingbirds_rnn_female_train_results.json nestingbirds_rnn_female_train_results_${j}_${k}_${n}_${p}_${i}.json
cp nestingbirds_rnn_female_test_results.json nestingbirds_rnn_female_test_results_${j}_${k}_${n}_${p}_${i}.json
done
for i in {1..5}
do
j=$((i + 1))
k=$((j + 1))
n=$((k + 1))
p=$((n + 1))
q=$((p + 1))
./run_nestingbirds_rnn.sh -trainRandomSeeds ${j},${k},${n},${p},${q} -testRandomSeed ${i} -steps 1000 > test_rnn_${j}_${k}_${n}_${p}_${q}_${i}.txt
cp nestingbirds_rnn_male_train_results.json nestingbirds_rnn_male_train_results_${j}_${k}_${n}_${p}_${q}_${i}.json
cp nestingbirds_rnn_male_test_results.json nestingbirds_rnn_male_test_results_${j}_${k}_${n}_${p}_${q}_${i}.json
cp nestingbirds_rnn_female_train_results.json nestingbirds_rnn_female_train_results_${j}_${k}_${n}_${p}_${q}_${i}.json
cp nestingbirds_rnn_female_test_results.json nestingbirds_rnn_female_test_results_${j}_${k}_${n}_${p}_${q}_${i}.json
done