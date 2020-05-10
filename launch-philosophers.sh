#! /bin/bash
for((i=0;i<$1;i++)); do
	./dining-p $1 $i &
done