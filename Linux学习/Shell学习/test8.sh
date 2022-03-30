#!/bin/bash 
for i in "$*"
do 
  echo "I am $i"
done 
for i in "$@"
do 
  echo "I am $i"
done
