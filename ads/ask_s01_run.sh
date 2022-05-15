#!/usr/bin/bash
cd fimi;

for dataset in retail
do

	for sp in 10 20 30 40 50 60 70 80 90
	do
		echo $dataset - $sp;
		len=$(cat $dataset.dat | wc -l);
		s=$(( ($sp * $len) / 100 ));
		echo patricia;
		time PATRICIAMINE/fim_all $dataset.dat $s $dataset_results;
		echo lcm
		time lcm/fim_all $dataset.dat $s $dataset_results;
	done
done
