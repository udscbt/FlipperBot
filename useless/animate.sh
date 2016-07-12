#!/bin/bash
while [ 1 ]
do
  for frame in `cat anim.dat`
  do
    echo -n $frame > /dev/ttyACM0
    sleep 0.2
  done
done
