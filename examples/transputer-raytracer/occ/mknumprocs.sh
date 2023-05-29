for x in 1 2 3 4 5 6 8 16 24 32 40 48 64 80
do
  echo "using $x processors..."
  rm -f raytrace.cfs
  sed -e "s/NUMPROCS/$x/g" raytrace.cfs.temp > raytrace.cfs
  make clean
  make
  cp raytrace.btl ../bin/raytrace$x.btl
  cp raytrace.map ../bin/raytrace$x.map
done
