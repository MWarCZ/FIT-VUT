#!/bin/bash

ipcs | grep $USER | cut -d' ' -f2 | head -n1 | while read line; do 
	ipcrm -m "$line" ; 
done;

ipcs | grep $USER | cut -d' ' -f2 | while read line; do
	ipcrm -s "$line" ;
done;

rm /tmp/xvalka05_ios_proj2

echo "OK";
ipcs | grep "xvalka05"

