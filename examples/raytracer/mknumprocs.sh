for x in 3 4 6 10 11 18 34
do
  echo "using $x processors..."
  rm -f raytraceT8.pgm
  sed -e "s/NUMPROCS/$x/g" raytraceT8.pgm.temp > raytraceT8.pgm
  make clean
  make
  cp raytraceT8.btl raytrace$x.btl
  cp raytraceT8.map raytrace$x.map
done
