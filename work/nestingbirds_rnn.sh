# Train RNN with dataset.
if [ $# -ne 1 ]
then
   echo "Specify male or female"
   exit 1
fi
if [ "$1" = "male" ]
then
  cat male_dataset.csv | cut -d, -f1-43 > X.csv
  cat male_dataset.csv | cut -d, -f44- > y.csv
elif [ "$1" = "female" ]
then
  cat female_dataset.csv | cut -d, -f1-41 > X.csv
  cat female_dataset.csv | cut -d, -f42- > y.csv
else
  echo "Specify male or female"
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
exit 0

