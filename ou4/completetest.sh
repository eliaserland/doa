#!/bin/bash

max_index=20000;
name=time;

# table2
for n in {1..5}; 
do ./tabletest -n -t 1000 >> ${name}_table2.txt; 
done

for (( i = 2000; i <= max_index; i = i + 2000 ));
do
for n in {1..5};
do ./tabletest -n -t $i >> ${name}_table2.txt;
done
done 

# mtftable
for n in {1..5}; 
do ./mtftabletest -n -t 1000 >> ${name}_mtf.txt; 
done

for (( i = 2000; i <= max_index; i = i + 2000 ));
do
for n in {1..5};
do ./mtftabletest -n -t $i >> ${name}_mtf.txt;
done
done 

# arraytable
for n in {1..5}; 
do ./arraytabletest -n -t 1000 >> ${name}_array.txt; 
done

for (( i = 2000; i <= max_index; i = i + 2000 ));
do
for n in {1..5};
do ./arraytabletest -n -t $i >> ${name}_array.txt;
done
done 

# hashtable
for n in {1..5}; 
do ./hashtabletest -n -t 1000 >> ${name}_hash.txt; 
done

for (( i = 2000; i <= max_index; i = i + 2000 ));
do
for n in {1..5};
do ./hashtabletest -n -t $i >> ${name}_hash.txt;
done
done 


