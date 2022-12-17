#!/bin/bash
# Train and test RNN with datasets.
usage="Usage: nestingbirds_rnn.sh -gender <\"male or female\"> [-num_datasets <number>] [-num_test_datasets <number>] [-num_neurons <neurons>] [-num_epochs <epochs>]"
gender=""
num_datasets=1
num_test_datasets=0
num_neurons=128
num_epochs=500
while [ "$#" -ne 0 ] ; do 
    if [ "$1" = "-gender" ] ; then
      shift
      gender=$1
    elif [ "$1" = "-num_datasets" ]
    then
      shift
      num_datasets=$1
    elif [ "$1" = "-num_test_datasets" ]
    then
      shift
      num_test_datasets=$1
    elif [ "$1" = "-num_neurons" ]
    then
      shift
      num_neurons=$1
    elif [ "$1" = "-num_epochs" ]
    then
      shift
      num_epochs=$1
    elif [ "$1" = "-help" ] || [ "$1" = "-h" ] || [ "$1" = "-?" ]
    then
      echo $usage
      exit 0
    else
      echo $usage
      exit 1
    fi
    shift
done
if [ "$gender" = "" ]
then
   echo $usage
   exit 1
fi
if [ "$gender" != "male" ] && [ "$gender" != "female" ]
then
   echo $usage
   exit 1
fi
if [ $num_test_datasets -ge "$num_datasets" ]
then
   echo "number of datasets (${num_datasets}) must be greater than number of testing datasets (${num_test_datasets}) "
   exit 1
fi

# Prepare training set.
num_train_sequences=$((num_datasets - num_test_datasets))
sequence_length=""
X_num_features=""
y_num_features=""
> X_train.csv
> y_train.csv
first=1
for dataset_num in $(seq $num_train_sequences)
do
  suffix=_$((dataset_num - 1))
  if [ "$gender" = "male" ]
  then
    cat male_dataset${suffix}.csv | sed 1d | cut -d, -f1-43 >> X_train.csv
    cat male_dataset${suffix}.csv | sed 1d | cut -d, -f44- >> y_train.csv
  elif [ "$gender" = "female" ]
  then
    cat female_dataset${suffix}.csv | sed 1d | cut -d, -f1-41 >> X_train.csv
    cat female_dataset${suffix}.csv | sed 1d | cut -d, -f42- >> y_train.csv
  fi
  if [ $first = 1 ]
  then
    first=0
    X_num_features=`head -1 X_train.csv | sed -e 's/,//g' | wc -c`
    X_num_features=$((X_num_features - 1))
    y_num_features=`head -1 y_train.csv | sed -e 's/,//g' | wc -c`
    y_num_features=$((y_num_features - 1))
    sequence_length=`cat X_train.csv | wc -l`
  fi
done
echo "X_train_shape = [${num_train_sequences},${sequence_length},${X_num_features}]" > nestingbirds_rnn_dataset.py
echo "X_train_seq = [" >> nestingbirds_rnn_dataset.py
count=0
num_steps=`cat X_train.csv | wc -l`
while read -r line
do
  echo -n $line >> nestingbirds_rnn_dataset.py
  count=$((count + 1))
  if [ $count -lt $num_steps ]
  then
     echo -n "," >> nestingbirds_rnn_dataset.py
  fi
  echo >> nestingbirds_rnn_dataset.py
done < X_train.csv
echo "]" >> nestingbirds_rnn_dataset.py
echo "y_train_shape = [${num_train_sequences},${sequence_length},${y_num_features}]" >> nestingbirds_rnn_dataset.py
echo "y_train_seq = [" >> nestingbirds_rnn_dataset.py
count=0
while read -r line
do
  echo -n $line >> nestingbirds_rnn_dataset.py
  count=$((count + 1))
  if [ $count -lt $num_steps ]
  then
     echo -n "," >> nestingbirds_rnn_dataset.py
  fi
  echo >> nestingbirds_rnn_dataset.py
done < y_train.csv
echo "]" >> nestingbirds_rnn_dataset.py
rm X_train.csv y_train.csv

# Prepare test set.
> X_test.csv
> y_test.csv
first=1
for dataset_num in $(seq $num_test_datasets)
do
  suffix=_$((dataset_num + $num_train_sequences - 1))
  if [ "$gender" = "male" ]
  then
    cat male_dataset${suffix}.csv | sed 1d | cut -d, -f1-43 >> X_test.csv
    cat male_dataset${suffix}.csv | sed 1d | cut -d, -f44- >> y_test.csv
  elif [ "$gender" = "female" ]
  then
    cat female_dataset${suffix}.csv | sed 1d | cut -d, -f1-41 >> X_test.csv
    cat female_dataset${suffix}.csv | sed 1d | cut -d, -f42- >> y_test.csv
  fi
  if [ $first = 1 ]
  then
    first=0
    sequence_length=`cat X_test.csv | wc -l`
  fi
done
echo "X_test_shape = [${num_test_datasets},${sequence_length},${X_num_features}]" >> nestingbirds_rnn_dataset.py
echo "X_test_seq = [" >> nestingbirds_rnn_dataset.py
count=0
num_steps=`cat X_test.csv | wc -l`
while read -r line
do
  echo -n $line >> nestingbirds_rnn_dataset.py
  count=$((count + 1))
  if [ $count -lt $num_steps ]
  then
     echo -n "," >> nestingbirds_rnn_dataset.py
  fi
  echo >> nestingbirds_rnn_dataset.py
done < X_test.csv
echo "]" >> nestingbirds_rnn_dataset.py
echo "y_test_shape = [${num_test_datasets},${sequence_length},${y_num_features}]" >> nestingbirds_rnn_dataset.py
echo "y_test_seq = [" >> nestingbirds_rnn_dataset.py
count=0
while read -r line
do
  echo -n $line >> nestingbirds_rnn_dataset.py
  count=$((count + 1))
  if [ $count -lt $num_steps ]
  then
     echo -n "," >> nestingbirds_rnn_dataset.py
  fi
  echo >> nestingbirds_rnn_dataset.py
done < y_test.csv
echo "]" >> nestingbirds_rnn_dataset.py
rm X_test.csv y_test.csv

# Run RNN.
python nestingbirds_rnn.py --neurons $num_neurons --epochs $num_epochs
exit 0

