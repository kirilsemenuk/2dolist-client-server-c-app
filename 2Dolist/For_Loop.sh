#!/bin/bash
for value in {1..50}
do
    ./cli -n bee$value 'is gay'
    
done

echo All done    