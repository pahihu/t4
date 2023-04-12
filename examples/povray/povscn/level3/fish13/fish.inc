// Persistence Of Vision raytracer version 1.0 sample file.

//----------- one fish include....3/13/92 M.Miller


/*------------ FISH SKIN-----------------------*/
#declare YourTexture = 
texture {
   image_map { <1 -1 0> gif "finskin2.gif"  }
   scale <100 60 1> translate <-50 -30 -1>
   //Brown_Agate
   //scale <50 30 27>
   ambient  0.2 diffuse 0.5 phong 1.0 phong_size 80
}

/*------------- CTDS INFO -----------------------*/
#declare Dot =
quadric {
   < 1.0  1.0  1.0>
   < 0.0  0.0  0.0>
   < 0.0  0.0  0.0>
   -1.0
}

#declare Connector_1 = 
object {
   quadric {
      < 0.0  1.0  1.0>
      < 0.0  0.0  0.0>
      < 0.0  0.0  0.0>
      -1.0
   }
   clipped_by {
      plane { < 1.0  0.0  0.0> 1.0  }
      plane { <-1.0  0.0  0.0> 0.0  }
   }
}

#declare Connector_2 =
quadric {
   < 1.0 -1.0  1.0>
   < 0.0  0.0  0.0>
   < 0.0  0.0  0.0>
   0.0
   rotate < 0.0  0.0  90.0>
}

/*------------ START THE BODY CURVES OF FISH-----------*/
#declare YourThing1 =
composite {
   object {
      union {
         difference {
            union {
               quadric {
                  Dot
                  scale     < 7.000000  7.000000  7.000000>
                  translate <-37.000010 -8.000000  0.000000>
               }

               quadric {
                  Dot
                  scale     < 7.600000  7.600000  7.600000>
                  translate <-35.005830 -8.066590  0.000000>
               }

               quadric {
                  Dot
                  scale     < 8.200000  8.200000  8.200000>
                  translate <-33.011630 -8.000770  0.000000>
               }

               quadric {
                  Dot
                  scale     < 8.800000  8.800000  8.800000>
                  translate <-31.026180 -7.802830  0.000000>
               }

               quadric {
                  Dot
                  scale     < 9.400000  9.400000  9.400000>
                  translate <-29.058240 -7.473640  0.000000>
               }

               quadric {
                  Dot
                  scale     < 10.000000  10.000000  10.000000>
                  translate <-27.116460 -7.014650  0.000000>
               }

               quadric {
                  Dot
                  scale     < 10.600000  10.600000  10.600000>
                  translate <-25.209400 -6.427880  0.000000>
               }

               quadric {
                  Dot
                  scale     < 11.200000  11.200000  11.200000>
                  translate <-23.345460 -5.715910  0.000000>
               }

               quadric {
                  Dot
                  scale     < 11.800000  11.800000  11.800000>
                  translate <-21.532840 -4.881880  0.000000>
               }


               quadric {
                  Dot
                  scale     < 12.400000  12.400000  12.400000>
                  translate <-19.779530 -3.929470  0.000000>
               }

               quadric {
                  Dot
                  scale     < 13.000000  13.000000  13.000000>
                  translate <-18.093250 -2.862860  0.000000>
               }

               quadric {
                  Dot
                  scale     < 12.720000  12.720000  12.720000>
                  translate <-16.763390 -1.988050  0.000000>
               }

               quadric {
                  Dot
                  scale     < 12.440000  12.440000  12.440000>
                  translate <-15.398940 -1.168230  0.000000>
               }

               quadric {
                  Dot
                  scale     < 12.160000  12.160000  12.160000>
                  translate <-14.002160 -0.404780  0.000000>
               }

               quadric {
                  Dot
                  scale     < 11.880000  11.880000  11.880000>
                  translate <-12.575390  0.301040  0.000000>
               }

               quadric {
                  Dot
                  scale     < 11.600000  11.600000  11.600000>
                  translate <-11.121010  0.948050  0.000000>
               }

               quadric {
                  Dot
                  scale     < 11.320000  11.320000  11.320000>
                  translate <-9.641440  1.535170  0.000000>
               }

               quadric {
                  Dot
                  scale     < 11.040000  11.040000  11.040000>
                  translate <-8.139140  2.061410  0.000000>
               }

               quadric {
                  Dot
                  scale     < 10.760000  10.760000  10.760000>
                  translate <-6.616620  2.525920  0.000000>
               }

               quadric {
                  Dot
                  scale     < 10.480000  10.480000  10.480000>
                  translate <-5.076400  2.927910  0.000000>
               }
            }

            //----------------- chops off front face & hollow mouth
            plane { < 1 0 0 > 0 translate <-30 -10 0> rotate <0 0 45>   }
            quadric { Ellipsoid scale <10 10 10> translate <-27 -7 0>  }
         }

         //---------------- add chin


         union {
            difference {
               quadric { Ellipsoid scale <11 11 11>  }
               plane { <0 1 0> 0 inverse translate <0 -7 0> rotate <0 0 23>  }
               translate <-30 -6 0>
            }
            //----------------- lower lip
            difference {
               intersection { Disk_Y scale <11 1.5 9> rotate <0 0 23> translate <-29 -14 0>  }
               quadric { Ellipsoid scale <8.5 8.5 8.5> translate <-31 -7 0>  }
            }
         }
      }
      texture { YourTexture  }

      bounded_by {
         quadric {
            Dot
            scale < 26.403494  26.403494  26.403494>
            translate <-26.866425 -5.054880  0.000000>
         }
      }
   }
}

#declare YourThing3 =
composite {
   object {
      quadric {
         Dot
         scale     < 10.200000  10.200000  10.200000>
         translate <-3.521070  3.266700  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 9.920000  9.920000  9.920000>
         translate <-1.953210  3.541750  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 9.640000  9.640000  9.640000>
         translate <-0.375430  3.752580  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 9.360000  9.360000  9.360000>
         translate < 1.209640  3.898860  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 9.080000  9.080000  9.080000>
         translate < 2.799360  3.980320  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 8.800000  8.800000  8.800000>
         translate < 4.391080  3.996850  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 8.520000  8.520000  8.520000>
         translate < 5.982150  3.948410  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 8.240000  8.240000  8.240000>
         translate < 7.569910  3.835070  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 7.960000  7.960000  7.960000>
         translate < 9.151730  3.657040  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 7.680000  7.680000  7.680000>
         translate < 10.724970  3.414610  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 7.400000  7.400000  7.400000>
         translate < 12.287000  3.108170  0.000000>
      }
      texture {
         YourTexture
      }
   }
   bounded_by {
      quadric {
         Dot
         scale < 24.277496  24.277496  24.277496>
         translate < 2.982965  3.266700  0.000000>
      }
   }

}
#declare YourThing4 =
composite {
   object {
      quadric {
         Dot
         scale     < 7.120000  7.120000  7.120000>
         translate < 13.835220  2.738250  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 6.840000  6.840000  6.840000>
         translate < 15.367060  2.305460  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 6.560000  6.560000  6.560000>
         translate < 16.879960  1.810510  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 6.280000  6.280000  6.280000>
         translate < 18.371410  1.254240  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 6.000000  6.000000  6.000000>
         translate < 19.838910  0.637570  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 5.750000  5.750000  5.750000>
         translate < 21.200120  0.000900  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 5.500000  5.500000  5.500000>
         translate < 22.535790 -0.687730  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 5.250000  5.250000  5.250000>
         translate < 23.843940 -1.427310  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 5.000000  5.000000  5.000000>
         translate < 25.122620 -2.216740  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 4.750000  4.750000  4.750000>
         translate < 26.369940 -3.054850  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 4.500000  4.500000  4.500000>
         translate < 27.584050 -3.940380  0.000000>
      }
      texture {
         YourTexture
      }
   }
   bounded_by {
      quadric {
         Dot
         scale < 21.771969  21.771969  21.771969>
         translate < 16.042025  0.708935  0.000000>
      }
   }
}

#declare YourThing5 =
composite {
   object {
      quadric {
         Dot
         scale     < 4.250000  4.250000  4.250000>
         translate < 28.763140 -4.872040  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 4.000000  4.000000  4.000000>
         translate < 29.905460 -5.848430  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 3.750000  3.750000  3.750000>
         translate < 31.009320 -6.868100  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 3.500000  3.500000  3.500000>
         translate < 32.073070 -7.929540  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 3.775000  3.775000  3.775000>
         translate < 30.657990 -7.043180  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 4.050000  4.050000  4.050000>
         translate < 29.205690 -6.219210  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 4.325000  4.325000  4.325000>
         translate < 27.718920 -5.459210  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 4.600000  4.600000  4.600000>
         translate < 26.200500 -4.764600  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 4.875000  4.875000  4.875000>
         translate < 24.653290 -4.136700  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 5.150000  5.150000  5.150000>
         translate < 23.080240 -3.576700  0.000000>
      }
      texture {
         YourTexture
      }
   }
   bounded_by {
      quadric {
         Dot
         scale < 21.587813  21.587813  21.587813>
         translate < 17.786535 -4.928120  0.000000>
      }
   }
}

#declare YourThing6 =
composite {
   object {
      quadric {
         Dot
         scale     < 5.425000  5.425000  5.425000>
         translate < 21.484310 -3.085670  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 5.700000  5.700000  5.700000>
         translate < 19.868540 -2.664520  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 5.975000  5.975000  5.975000>
         translate < 18.235970 -2.314070  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 6.250000  6.250000  6.250000>
         translate < 16.589700 -2.034960  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 6.525000  6.525000  6.525000>
         translate < 14.932850 -1.827740  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 6.800000  6.800000  6.800000>
         translate < 13.268560 -1.692790  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 7.075000  7.075000  7.075000>
         translate < 11.599970 -1.630370  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 7.350000  7.350000  7.350000>
         translate < 9.930240 -1.640590  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 7.625000  7.625000  7.625000>
         translate < 8.262540 -1.723440  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 7.900000  7.900000  7.900000>
         translate < 6.600020 -1.878760  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 8.175000  8.175000  8.175000>
         translate < 4.945830 -2.106260  0.000000>
      }
      texture {
         YourTexture
      }
   }
   bounded_by {
      quadric {
         Dot
         scale < 20.892566  20.892566  20.892566>
         translate < 11.840070 -2.106260  0.000000>
      }
   }
}

#declare YourThing7 =
composite {
   object {
      quadric {
         Dot
         scale     < 8.450000  8.450000  8.450000>
         translate < 3.303100 -2.405500  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 8.725000  8.725000  8.725000>
         translate < 1.674950 -2.775920  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 9.000000  9.000000  9.000000>
         translate < 0.064450 -3.216820  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 9.571430  9.571430  9.571430>
         translate <-2.337010 -4.575770  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 10.142860  10.142860  10.142860>
         translate <-4.935990 -5.502660  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 10.714280  10.714280  10.714280>
         translate <-7.655430 -5.969990  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 11.285710  11.285710  11.285710>
         translate <-10.414730 -5.963930  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 11.857140  11.857140  11.857140>
         translate <-13.132100 -5.484650  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 12.428570  12.428570  12.428570>
         translate <-15.726970 -4.546350  0.000000>
      }
      texture {
         YourTexture
      }
   }
   object {
      quadric {
         Dot
         scale     < 13.000000  13.000000  13.000000>
         translate <-18.122440 -3.176860  0.000000>
      }
      texture {
         YourTexture
      }
   }
   bounded_by {
      quadric {
         Dot
         scale < 31.365653  31.365653  31.365653>
         translate <-9.684670 -3.759325  0.000000>
      }
   }
}

/*----------- COMBINES BODY SECTION IN ONE COMPOSITE ----*/
#declare fish_body = 
composite {
   composite { YourThing1  }
   composite { YourThing3  }
   composite { YourThing4  }
   composite { YourThing5  }
   composite { YourThing6  }
   composite { YourThing7  }
   bounded_by {
      quadric {
         Dot
         scale < 49.061227  49.061227  49.061227>
         translate <-4.213470 -1.937545  0.000000>
      }
   }
}

/*----------- ENDS BODY ------------------------------*/

// ----------- 2 lower dangling fins


#declare spike1 =
composite {
   object {
      quadric {
         Dot
         scale     < 0.100000  0.100000  0.100000>
         translate <-5.000000 -7.000010  5.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2 
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.279490  }
         plane {     < 1.000000  0.000000  0.000000>  0.099818 inverse  }
      }
      translate <-0.099818  0.000000  0.000000>
      scale     < 16.543659  1.000000  1.000000>
      translate < 0.006034  0.000000  0.000000>
      rotate    < 0.000000  0.000000  69.224366>
      rotate    < 0.000000  109.091249  0.000000>
      translate <-5.000000 -7.000010  5.000000>
      texture {
         YourTexture
      }

   }
   object {
      quadric {
         Dot
         scale     < 0.280000  0.280000  0.280000>
         translate <-5.346110 -4.210700  4.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2 
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.459162  }
         plane {     < 1.000000  0.000000  0.000000>  0.279490 inverse  }
      }
      translate <-0.279490  0.000000  0.000000>
      scale     < 16.543617  1.000000  1.000000>
      translate < 0.016894  0.000000  0.000000>
      rotate    < 0.000000  0.000000  70.110802>
      rotate    < 0.000000  99.836716  0.000000>
      translate <-5.346110 -4.210700  4.000000>
      texture {
         YourTexture
      }

   }
   object {
      quadric {
         Dot
         scale     < 0.460000  0.460000  0.460000>
         translate <-5.519500 -1.405360  3.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2 
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.638834  }
         plane {     < 1.000000  0.000000  0.000000>  0.459162 inverse  }
      }
      translate <-0.459162  0.000000  0.000000>
      scale     < 16.543652  1.000000  1.000000>
      translate < 0.027755  0.000000  0.000000>
      rotate    < 0.000000  0.000000  70.415293>
      rotate    < 0.000000  90.000000  0.000000>
      translate <-5.519500 -1.405360  3.000000>
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Dot
         scale     < 0.640000  0.640000  0.640000>
         translate <-5.519500  1.405340  2.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2 
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.818506  }
         plane {     < 1.000000  0.000000  0.000000>  0.638834 inverse  }
      }
      translate <-0.638834  0.000000  0.000000>
      scale     < 16.543666  1.000000  1.000000>
      translate < 0.038615  0.000000  0.000000>
      rotate    < 0.000000  0.000000  70.110898>
      rotate    < 0.000000  80.163840  0.000000>
      translate <-5.519500  1.405340  2.000000>
      texture {
         YourTexture
      }

   }
   object {
      quadric {
         Dot
         scale     < 0.820000  0.820000  0.820000>
         translate <-5.346120  4.210690  1.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2 
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.998178  }
         plane {     < 1.000000  0.000000  0.000000>  0.818506 inverse  }
      }
      translate <-0.818506  0.000000  0.000000>
      scale     < 16.543614  1.000000  1.000000>
      translate < 0.049476  0.000000  0.000000>
      rotate    < 0.000000  0.000000  69.224239>
      rotate    < 0.000000  70.908240  0.000000>
      translate <-5.346120  4.210690  1.000000>
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Dot
         translate <-5.000000  6.999990  0.000000>
      }
      texture {
         YourTexture
      }
   }

   bounded_by {
      quadric {
         Dot
         scale < 9.035951  9.035951  9.035951>
         translate <-5.083060  0.449990  2.050000>
      }
   }
}

#declare spike2 =
composite {
   object {
      Connector_1
      scale     < 10.000000  1.000000  1.000000>
      rotate    < 0.000000  0.000000  0.000057>
      translate <-5.000000  6.999990  0.000000>
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Dot
         translate < 5.000000  7.000000  0.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.998178  }
         plane {     < 1.000000  0.000000  0.000000>  0.818506 inverse  }
      }
      translate <-0.998178  0.000000  0.000000>
      scale     < 16.543666  1.000000  1.000000>
      translate <-0.060336  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000 -69.224308>
      rotate    < 0.000000 -70.908240  0.000000>
      translate < 5.000000  7.000000  0.000000>
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Dot
         scale     < 0.820000  0.820000  0.820000>
         translate < 5.346120  4.210690  1.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.818506  }
         plane {     < 1.000000  0.000000  0.000000>  0.638834 inverse  }
      }
      translate <-0.818506  0.000000  0.000000>
      scale     < 16.543666  1.000000  1.000000>
      translate <-0.049475  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000 -70.110898>
      rotate    < 0.000000 -80.163840  0.000000>
      translate < 5.346120  4.210690  1.000000>
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Dot
         scale     < 0.640000  0.640000  0.640000>
         translate < 5.519500  1.405340  2.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.638834  }
         plane {     < 1.000000  0.000000  0.000000>  0.459162 inverse  }
      }
      translate <-0.638834  0.000000  0.000000>
      scale     < 16.543652  1.000000  1.000000>
      translate <-0.038615  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000 -70.415293>
      rotate    < 0.000000 -90.000000  0.000000>
      translate < 5.519500  1.405340  2.000000>
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Dot
         scale     < 0.460000  0.460000  0.460000>
         translate < 5.519500 -1.405360  3.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.459162  }
         plane {     < 1.000000  0.000000  0.000000>  0.279490 inverse  }
      }
      translate <-0.459162  0.000000  0.000000>
      scale     < 16.543617  1.000000  1.000000>
      translate <-0.027755  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000 -70.110802>
      rotate    < 0.000000 -99.836716  0.000000>
      translate < 5.519500 -1.405360  3.000000>
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Dot
         scale     < 0.280000  0.280000  0.280000>
         translate < 5.346110 -4.210700  4.000000>
      }
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Connector_2
      }
      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  0.279490  }
         plane {     < 1.000000  0.000000  0.000000>  0.099818 inverse  }
      }
      translate <-0.279490  0.000000  0.000000>
      scale     < 16.543659  1.000000  1.000000>
      translate <-0.016894  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000 -69.224366>
      rotate    < 0.000000 -109.091249  0.000000>
      translate < 5.346110 -4.210700  4.000000>
      texture {
         YourTexture
      }
   }

   object {
      quadric {
         Dot
         scale     < 0.100000  0.100000  0.100000>
         translate < 5.000000 -7.000010  5.000000>
      }
      texture {
         YourTexture
      }
   }

   bounded_by {
      quadric {
         Dot
         scale < 9.577604  9.577604  9.577604>
         translate < 3.083060  0.449995  2.050000>
      }
   }
}

/*-------------- COMBINE SPIKE -----------------------*/
#declare l_fin = 
composite {
   composite { spike1  }
   composite { spike2  }
   bounded_by {
      quadric {
         Dot
         scale < 11.235417  11.235417  11.235417>
         translate < 0.000000  0.449995  2.050000>
      }
   }
}



//**************************************************************
//----------------- Bass Fins...Spiked fingers, bloody fingers



/*-------------- MAKE POINTS --------------------------------*/
#declare bf1 =
object {
   quadric {
      Connector_2
   }
   clipped_by {
      plane {     < 1.000000  0.000000  0.000000>  0.995942  }
      plane {     < 1.000000  0.000000  0.000000>  0.099594 inverse  }
   }
   translate <-0.995942  0.000000  0.000000>
   scale     < 11.066020  1.000000  1.000000>
   translate <-0.090000  0.000000  0.000000>
   rotate    < 0.000000  180.000000  0.000000>
   rotate    < 0.000000  0.000000  90.000000>
   texture {
      YourTexture
   }

   bounded_by {
      quadric {
         Dot
         scale < 6.300081  6.300081  6.300081>
         translate < 0.000000  4.550000  0.000000>
      }
   }
}

#declare bf2 =
object {
   quadric {
      Connector_2
   }
   clipped_by {
      plane {     < 1.000000  0.000000  0.000000>  0.997018  }
      plane {     < 1.000000  0.000000  0.000000>  0.099702 inverse  }
   }
   translate <-0.997018  0.000000  0.000000>
   scale     < 12.919026  1.000000  1.000000>
   translate <-0.077174  0.000000  0.000000>
   rotate    < 0.000000  180.000000  0.000000>
   rotate    < 0.000000  0.000000  59.036243>
   translate < 3.000000  1.000000  0.000000>
   texture {
      YourTexture
   }

   bounded_by {
      quadric {
         Dot
         scale < 7.970637  7.970637  7.970637>
         translate < 4.550000  5.550000  0.000000>
      }
   }
}

#declare bf3 =
object {
   quadric {
      Connector_2
   }
   clipped_by {
      plane {     < 1.000000  0.000000  0.000000>  0.995439  }
      plane {     < 1.000000  0.000000  0.000000>  0.099544 inverse  }
   }
   translate <-0.995439  0.000000  0.000000>
   scale     < 10.434392  1.000000  1.000000>
   translate <-0.095400  0.000000  0.000000>
   rotate    < 0.000000  180.000000  0.000000>
   rotate    < 0.000000  0.000000  57.994617>
   translate < 9.000000  2.000000  0.000000>
   texture {
      YourTexture
   }

   bounded_by {
      quadric {
         Dot
         scale < 9.602502  9.602502  9.602502>
         translate < 7.050000  5.050000  0.000000>
      }
   }
}

#declare bf4 =
object {
   quadric {
      Connector_2
   }
   clipped_by {
      plane {     < 1.000000  0.000000  0.000000>  0.993750  }
      plane {     < 1.000000  0.000000  0.000000>  0.099375 inverse  }
   }
   translate <-0.993750  0.000000  0.000000>
   scale     < 8.902074  1.000000  1.000000>
   translate <-0.111631  0.000000  0.000000>
   rotate    < 0.000000  180.000000  0.000000>
   rotate    < 0.000000  0.000000  60.255119>
   translate < 13.000000  2.000000  0.000000>
   texture {
      YourTexture
   }

   bounded_by {
      quadric {
         Dot
         scale < 10.710465  10.710465  10.710465>
         translate < 8.550000  4.550000  0.000000>
      }
   }
}

#declare bf5 =
object {
   quadric {
      Connector_2
   }
   clipped_by {
      plane {     < 1.000000  0.000000  0.000000>  0.993338  }
      plane {     < 1.000000  0.000000  0.000000>  0.099334 inverse  }
   }
   translate <-0.993338  0.000000  0.000000>
   scale     < 8.620246  1.000000  1.000000>
   translate <-0.115233  0.000000  0.000000>
   rotate    < 0.000000  180.000000  0.000000>
   rotate    < 0.000000  0.000000  50.194429>
   translate < 17.000000  2.000000  0.000000>
   texture {
      YourTexture
   }

   bounded_by {
      quadric {
         Dot
         scale < 12.992346  12.992346  12.992346>
         translate < 11.050000  4.050000  0.000000>
      }
   }
}

#declare bf6 =
object {
   quadric {
      Connector_2
   }
   clipped_by {
      plane {     < 1.000000  0.000000  0.000000>  0.994512  }
      plane {     < 1.000000  0.000000  0.000000>  0.099451 inverse  }
   }
   translate <-0.994512  0.000000  0.000000>
   scale     < 9.505684  1.000000  1.000000>
   translate <-0.104623  0.000000  0.000000>
   rotate    < 0.000000  180.000000  0.000000>
   rotate    < 0.000000  0.000000  35.537678>
   translate < 20.000000  2.000000  0.000000>
   texture {
      YourTexture
   }

   bounded_by {
      quadric {
         Dot
         scale < 15.447267  15.447267  15.447267>
         translate < 13.550000  3.550000  0.000000>
      }
   }
}

#declare bf7 =
object {
   quadric {
      Connector_2
   }
   clipped_by {
      plane {     < 1.000000  0.000000  0.000000>  0.992993  }
      plane {     < 1.000000  0.000000  0.000000>  0.099299 inverse  }
   }
   translate <-0.992993  0.000000  0.000000>
   scale     < 8.402674  1.000000  1.000000>
   translate <-0.118176  0.000000  0.000000>
   rotate    < 0.000000  180.000000  0.000000>
   rotate    < 0.000000  0.000000  23.198591>
   translate < 23.000000  1.000000  0.000000>
   texture {
      YourTexture
   }

   bounded_by {
      quadric {
         Dot
         scale < 16.744045  16.744045  16.744045>
         translate < 15.050000  2.050000  0.000000>
      }
   }
}

/*-------------------- COMBINE ALL SPIKES ---------------*/
#declare pnts = 
composite {
   object { bf1  }
   object { bf2  }
   object { bf3  }      
   object { bf4  }
   object { bf5  }
   object { bf6  }
   object { bf7  }

   bounded_by {
      quadric {
         Dot
         scale < 18.386953  18.386953  18.386953>
         translate < 14.550000  5.050000  0.000000>
      }
   }
}



/*------------ THIN SKIN BETWEEN THE SPIKES------------------------------------*/
#declare fin_skin = 
object {
   difference {
      intersection { Cube scale <15 5 0.1> translate <15 5 0>  }
      quadric { Cylinder_Z scale <4 6 1> rotate <0 0 -32> translate <5 12 0>  }
      quadric { Cylinder_Z scale <2.5 3.5 1> rotate <0 0 -32> translate <11.5 11 0>  }
      quadric { Cylinder_Z scale <1.6 2.5 1> rotate <0 0 -31> translate <15.5 10 0>  }
      quadric { Cylinder_Z scale <2.5 3.5 1> rotate <0 0 -38> translate <20 9.5 0>  }
      quadric { Cylinder_Z scale <3 4.1 1> rotate <0 0 -40> translate <26 10 0>  }
      quadric { Cylinder_Z scale <3 4.5 1> rotate <0 0 -51> translate <30.5 7.5 0>  }
      quadric { Cylinder_Z scale <3 4 1> rotate <0 0 -39> translate <32 2 0>  }
   }
   texture { YourTexture  }
   bounded_by { 
      intersection { Cube scale <15 5 0.1> translate <15 5 0>  }
   }
}

#declare fin = composite {
   composite { pnts  }
   object { fin_skin  }
   bounded_by { 
      intersection { Cube scale <17 5 0.1> translate <15 5 0>  }
   }
}


#declare tail = object {
   difference {
      union {
         quadric { Ellipsoid scale <10 5 0.5> rotate <0 0 -14> translate <9 -4 0>  }
         quadric { Ellipsoid scale <10 5 0.5> rotate <0 0 -52> translate <7 -8 0>  }
      }
      plane { <0 1 0> 0 rotate <0 0 53> translate <14 -9 0>  }
   }

   bounded_by {
      intersection { Disk_Z scale <11 11 3> translate <9 -6 -1.5>  }
   }
}


//------------ upper lip

#declare u_lip = object {
   difference {
      quadric { Ellipsoid scale <11 11 11>  }
      quadric { Ellipsoid scale <10 18 20> rotate <0 0 30> translate <-9 -6 0>  }
      plane { <0 1 0> 0 rotate <0 0 -48> translate <1.6 0 0> inverse  }
   }
   //texture { YourTexture  }
   bounded_by {
      quadric { Ellipsoid scale <12 12 12>  }
   }
}



/*--------------- GLASS FISH EYE --------------*/
#declare eye = composite {
   //-------------- extra skin around eye
   object {
      quadric { Ellipsoid scale <5 5 3>  }
      texture { YourTexture  }
   }
   //-------------- color pupil
   object {
      quadric { Ellipsoid scale <4 4 4>  }
      texture { 
         image_map { <1 -1 0> gif "fisheye.gif"  }
         scale <8 8 1> translate <-4 -4 -1>
         ambient 0.2 phong 1.0  }
   }
   //-------------- glass sphere {
   object {
      quadric { Ellipsoid scale <5 5 5>  }
      texture { Glass  }
   }
}





/*------ BUILD THE FISH---------------*/

#declare fish = composite {
   composite { fish_body scale <1 1 0.6>   }
   composite { eye translate <-26 -2 -4>  }
   composite { eye translate <-26 -2 4>  }
   composite { pnts scale <1.3 1.7 0.4> translate <-17 7 0>  }
   object { fin_skin scale <1.3 1.7 0.4> translate <-17 7 0> texture { YourTexture  }  }
   composite { fin scale <0.7 1 0.7> rotate <180 0 0> rotate <0 0 20> translate <-6 -10 0>  }
   composite { l_fin rotate <0 90 0> scale <1 1 1> translate <-14 -20 0>  }
   object { tail translate <33 -8 0> texture { YourTexture } }
   object { u_lip rotate <0 0 5> scale <1 0.96  0.65 > translate <-32 -8.5 0> texture { YourTexture } }

   bounded_by {
      intersection { Disk_Z scale <60 40 20> translate <0 0  -10>  }
   }
}
