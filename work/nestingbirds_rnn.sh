# Train RNN with dataset.
if [ $# -lt 1 ] || [ $# -gt 2 ]
then
   echo "Usage: nestingbirds_rnn.sh \"male or female\" [<runs>]"
   exit 1
fi
doruns=0
runs=1
if [ $# -eq 2 ]
then
  runs=$2
  doruns=1
fi
for run in $(seq $runs)
do
  suffix=""
  if [ $doruns -eq 1 ]
  then
    suffix=_$((run - 1))
  fi
  if [ "$1" = "male" ]
  then
    cat male_dataset${suffix}.csv | cut -d, -f1-43 > X.csv
    cat male_dataset${suffix}.csv | cut -d, -f44- > y.csv
  elif [ "$1" = "female" ]
  then
    cat female_dataset${suffix}.csv | cut -d, -f1-41 > X.csv
    cat female_dataset${suffix}.csv | cut -d, -f42- > y.csv
  else
    echo "Usage: nestingbirds_rnn.sh \"male or female\" [<runs>]"
    exit 1
  fi
  num_steps=`cat X.csv | wc -l`
  num_steps=$((num_steps - 1))
  num_features=`head -2 X.csv | tail -1 | sed -e 's/,//g' | wc -c`
  num_features=$((num_features - 1))
  echo "X_train_shape = [1,${num_steps},${num_features}]" > nestingbirds_dataset.py
  echo "X_train_seq = [" >> nestingbirds_dataset.py
  skip=1
  count=0
  while read -r line
  do
     if [ $skip -eq 1 ]
     then
        skip=0
     else
        echo -n $line >> nestingbirds_dataset.py
        count=$((count + 1))
        if [ $count -lt $num_steps ]
        then
           echo -n "," >> nestingbirds_dataset.py
        fi
        echo >> nestingbirds_dataset.py
     fi
  done < X.csv
  echo "]" >> nestingbirds_dataset.py
  num_steps=`cat y.csv | wc -l`
  num_steps=$((num_steps - 1))
  num_features=`head -2 y.csv | tail -1 | sed -e 's/,//g' | wc -c`
  num_features=$((num_features - 1))
  echo "y_train_shape = [1,${num_steps},${num_features}]" >> nestingbirds_dataset.py
  echo "y_train_seq = [" >> nestingbirds_dataset.py
  skip=1
  count=0
  while read -r line
  do
     if [ $skip -eq 1 ]
     then
        skip=0
     else
        echo -n $line >> nestingbirds_dataset.py
        count=$((count + 1))
        if [ $count -lt $num_steps ]
        then
           echo -n "," >> nestingbirds_dataset.py
        fi
        echo >> nestingbirds_dataset.py
     fi
  done < y.csv
  echo "]" >> nestingbirds_dataset.py
  rm X.csv y.csv
  python nestingbirds_rnn.py
done
exit 0

