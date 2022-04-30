mode=sub
for k in 128,${mode} 256,${mode}
do
echo neurons = $k
for i in 10,5,${k} 10,10,${k} 20,5,${k} 20,10,${k}
do
echo prefix = $i
for j in $(ls world_composer_*nn*_test_results.csv)
do
echo file = $j
grep ^${i} $j | cut -d"," -f10 | awk '{sum += $0; count += 1} END {print sum/count}'
done
done
done
for k in 2,${mode} 4,${mode}
do
echo neurons = $k
for i in 10,5,${k} 10,10,${k} 20,5,${k} 20,10,${k}
do
echo prefix = $i
for j in $(ls world_composer_tcn_test_results.csv)
do
echo file = $j
grep ^${i} $j | cut -d"," -f10 | awk '{sum += $0; count += 1} END {print sum/count}'
done
done
done