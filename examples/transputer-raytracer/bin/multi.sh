#!/bin/sh
if [ $# -ne 1 -a $# -ne 2 ];
then
  echo "usage: multi.sh [-sl] <#processors>"
  exit 1
fi

while :
do
  case "$1" in
    -sl)
      SHLINK="-sl"
      shift 1
      ;;
    *)
      N=${1}
      break
      ;;
  esac
done

MAPFILE="raytrace${N}.map"
BTLFILE="raytrace${N}.btl"

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

nworkers=`expr ${N} - 1`
echo "Workers = ${nworkers}"

export SPYNET=${MAPFILE}

if [ 1 -le ${nworkers} ];
then
  for i in `seq 1 ${nworkers}`
  do
    echo "Starting worker${i}"
    t4 -s8 ${SHLINK} -sn ${i} -si &
    sleep 1
  done
fi

echo "Starting HOST"
t4 -s8 ${SHLINK} -sn 0 -sb ${BTLFILE}

# kill processors
pkill -9 -x t4 2>&1 >/dev/null
