for prefix in 10,5 10,10 20,5 20,10
do
echo basePathLength,numModularPaths = $prefix
for mod in insert substitute delete all
do
echo modification = $mod
for file in $(ls world_composer_*nn*_test_results.csv)
do
f=$(basename $file .csv)
f=${f/_test_results/}
f=$(echo $f | cut -c16-)
echo -n $f ": "
nn128=$(grep ^${prefix},128,${mod} $file | cut -d"," -f10 | awk '{sum += $0; count += 1} END {print sum/count}')
nn256=$(grep ^${prefix},256,${mod} $file | cut -d"," -f10 | awk '{sum += $0; count += 1} END {print sum/count}')
python -c "print('128 neurons:', ${nn128}, '256 neurons:', ${nn256}, 'delta: ', ${nn128} - ${nn256})"
done
done
done
for prefix in 10,5 10,10 20,5 20,10
do
echo basePathLength,numModularPaths = $prefix
for mod in insert substitute delete all
do
echo modification = $mod
for file in $(ls world_composer_tcn_test_results.csv)
do
f=$(basename $file .csv)
f=${f/_test_results/}
f=$(echo $f | cut -c16-)
echo -n $f ": "
k2=$(grep ^${prefix},2,${mod} $file | cut -d"," -f10 | awk '{sum += $0; count += 1} END {print sum/count}')
k4=$(grep ^${prefix},4,${mod} $file | cut -d"," -f10 | awk '{sum += $0; count += 1} END {print sum/count}')
python -c "print('kernel 2:', ${k2}, 'kernel 4:', ${k4}, 'delta: ', ${k2} - ${k4})"
done
done
done