#!/bin/sh
if [ $# -ne 1 ];
then
  echo "usage: multi.sh <#processors>"
  exit 1
fi

MAPFILE="raytrace${1}.map"
BTLFILE="raytrace${1}.btl"

if [ ! -f ${MAPFILE} ];
then
  echo "${MAPFILE} does not exists!"
  exit 1
fi
if [ ! -f ${BTLFILE} ];
then
  echo "${BTLFILE} does not exists!"
  exit 1
fi

nworkers=`expr ${1} - 1`
echo "Workers = ${nworkers}"

export SPYNET=${MAPFILE}

if [ 1 -le ${nworkers} ];
then
  for i in `seq 1 ${nworkers}`
  do
    echo "Starting worker${i}"
    t4 -s8 -sn ${i} -si &
    sleep 1
  done
fi

echo "Starting HOST"
t4 -s8 -sn 0 -sb ${BTLFILE}

# kill processors
pkill -9 -x t4 2>&1 >/dev/null