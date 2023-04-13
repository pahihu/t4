// Persistence Of Vision raytracer version 1.0 sample file.


/*--------------- THIS DATA PRODUCES A SHELL W/SLIMMY SNAIL -----------*/


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
      plane { < 1.0  0.0  0.0> 1.0 }
      plane { <-1.0  0.0  0.0> 0.0 }
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


/*--------------------- FOLLOWING DECLARES COLORS FOR SHELL & SNAIL----*/

/*----THE BALL JOINTS CONNECTORS---*/
#declare snail_skin = texture {
   color color red 0.733 green 0.486 blue 0.447 
   phong 0.7 phong_size 30  
   ambient 0.15 diffuse 0.9
}



/*----THE LONG CONNECTORS---*/
#declare snail_bone = texture {
   gradient <1 0 0>
   colour_map {
      [0.000 0.244   colour red  0.733  green  0.486  blue  0.447  alpha  0.000
      colour red  0.545  green  0.467  blue  0.612  alpha  0.000]
      [0.244 0.395   colour red  0.545  green  0.467  blue  0.612  alpha  0.000
      colour red  0.816  green  0.612  blue  0.565  alpha  0.000]
      [0.395 0.580   colour red  0.816  green  0.612  blue  0.565  alpha  0.000
      colour red  0.643  green  0.498  blue  0.569  alpha  0.000]
      [0.580 1.001   colour red  0.643  green  0.498  blue  0.569  alpha  0.000
      colour red  0.733  green  0.486  blue  0.447  alpha  0.000]
   }
   scale <0.3 20 0.3>
   ambient 0.15
   diffuse 0.9
}


/*----------------THE_SNAIL's BODY TEXTURE ------------*/
#declare slime = texture {

   gradient <1 0 0>
   colour_map {
      [0.000 0.179   colour red  0.482  green  0.349  blue  0.353  alpha  0.000
      colour red  0.322  green  0.180  blue  0.278  alpha  0.000]
      [0.179 0.368   colour red  0.322  green  0.180  blue  0.278  alpha  0.000
      colour red  0.365  green  0.294  blue  0.396  alpha  0.000]
      [0.368 0.538   colour red  0.365  green  0.294  blue  0.396  alpha  0.000
      colour red  0.322  green  0.180  blue  0.278  alpha  0.000]
      [0.538 0.701   colour red  0.322  green  0.180  blue  0.278  alpha  0.000
      colour red  0.365  green  0.294  blue  0.318  alpha  0.000]
      [0.701 0.846   colour red  0.365  green  0.294  blue  0.318  alpha  0.000
      colour red  0.322  green  0.180  blue  0.278  alpha  0.000]
      [0.846 1.001   colour red  0.322  green  0.180  blue  0.278  alpha  0.000
      colour red  0.482  green  0.349  blue  0.353  alpha  0.000]
   }
}


#declare bone2 = texture { 
   color Red alpha 1.0 
   phong 0.7 phong_size 30 }


/*-------- antennae color -------------------------------------*/
#declare antler_color = texture { 
   colour red  0.322  green  0.180  blue  0.278
   bumps 0.4 frequency 5
   phong 1.0 }



/*------ THE START OF SEA SHELL -----------------------*/

/*-------------- LOCATION OF Rim on SHEEL -------------*/


#declare snail = composite {

   object { quadric { Ellipsoid }
      texture { color red 0.714 green 0.447 blue 0.408 
         ambient 0.15 phong 1.0 }
      scale <21 21 10> translate <21.5 0 0>
   }


   object {
      quadric {
         Connector_2 } texture { snail_bone } 
      texture { bone2  } 

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  19.971935 }
         plane {     < 1.000000  0.000000  0.000000>  19.642398 inverse }
      }
      translate <-19.971935  0.000000  0.000000>
      scale     < 18.856352  1.000000  1.000000>
      translate <-1.059162  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  4.602336>
      rotate    < 0.000000  102.281736  0.000000>
      translate < 20.000000  0.000000  0.000000>
   }


   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 19.670000  19.670000  19.670000>
      translate < 18.678750  0.500000 -6.069094>
   }


   object {
      quadric {
         Connector_2 } texture { snail_bone } 
      texture { bone2 }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  19.641377 }
         plane {     < 1.000000  0.000000  0.000000>  19.311857 inverse }
      }
      translate <-19.641377  0.000000  0.000000>
      scale     < 18.516243  1.000000  1.000000>
      translate <-1.060765  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  4.686815>
      rotate    < 0.000000  120.342307  0.000000>
      translate < 18.678750  0.500000 -6.069094>
   }


   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 19.340000  19.340000  19.340000>
      translate < 15.597848  1.000000 -11.332500>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  19.310796 }
         plane {     < 1.000000  0.000000  0.000000>  18.981295 inverse }
      }
      translate <-19.310796  0.000000  0.000000>
      scale     < 18.176180  1.000000  1.000000>
      translate <-1.062423  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  4.774441>
      rotate    < 0.000000  138.405161  0.000000>
      translate < 15.597848  1.000000 -11.332500>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 19.010000  19.010000  19.010000>
      translate < 11.120897  1.500000 -15.306602>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  18.980192 }
         plane {     < 1.000000  0.000000  0.000000>  18.650710 inverse }
      }
      translate <-18.980192  0.000000  0.000000>
      scale     < 17.836164  1.000000  1.000000>
      translate <-1.064141  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  4.865392>
      rotate    < 0.000000  156.470421  0.000000>
      translate < 11.120897  1.500000 -15.306602>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 18.680000  18.680000  18.680000>
      translate < 5.735355  2.000000 -17.651609>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  18.649563 }
         plane {     < 1.000000  0.000000  0.000000>  18.320101 inverse }
      }
      translate <-18.649563  0.000000  0.000000>
      scale     < 17.496197  1.000000  1.000000>
      translate <-1.065921  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  4.959860>
      rotate    < 0.000000  174.538218  0.000000>
      translate < 5.735355  2.000000 -17.651609>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 18.350000  18.350000  18.350000>
      translate <-0.000000  2.500000 -18.200000>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  18.318908 }
         plane {     < 1.000000  0.000000  0.000000>  17.989467 inverse }
      }
      translate <-18.318908  0.000000  0.000000>
      scale     < 17.156285  1.000000  1.000000>
      translate <-1.067767  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  5.058051>
      rotate    < 0.000000 -167.391278  0.000000>
      translate <-0.000000  2.500000 -18.200000>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 18.020000  18.020000  18.020000>
      translate <-5.512863  3.000000 -16.966848>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  17.988223 }
         plane {     < 1.000000  0.000000  0.000000>  17.658805 inverse }
      }
      translate <-17.988223  0.000000  0.000000>
      scale     < 16.816430  1.000000  1.000000>
      translate <-1.069681  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  5.160189>
      rotate    < 0.000000 -149.317919  0.000000>
      translate <-5.512863  3.000000 -16.966848>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 17.690000  17.690000  17.690000>
      translate <-10.274486  3.500000 -14.141617>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  17.657509 }
         plane {     < 1.000000  0.000000  0.000000>  17.328115 inverse }
      }
      translate <-17.657509  0.000000  0.000000>
      scale     < 16.476635  1.000000  1.000000>
      translate <-1.071670  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  5.266517>
      rotate    < 0.000000 -131.241520  0.000000>
      translate <-10.274486  3.500000 -14.141617>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 17.360000  17.360000  17.360000>
      translate <-13.850371  4.000000 -10.062884>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  17.326762 }
         plane {     < 1.000000  0.000000  0.000000>  16.997394 inverse }
      }
      translate <-17.326762  0.000000  0.000000>
      scale     < 16.136905  1.000000  1.000000>
      translate <-1.073735  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  5.377295>
      rotate    < 0.000000 -113.161873  0.000000>
      translate <-13.850371  4.000000 -10.062884>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 17.030000  17.030000  17.030000>
      translate <-15.939707  4.500000 -5.179125>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  16.995981 }
         plane {     < 1.000000  0.000000  0.000000>  16.666640 inverse }
      }
      translate <-16.995981  0.000000  0.000000>
      scale     < 15.797242  1.000000  1.000000>
      translate <-1.075883  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  5.492809>
      rotate    < 0.000000 -95.078799  0.000000>
      translate <-15.939707  4.500000 -5.179125>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 16.700000  16.700000  16.700000>
      translate <-16.400000  5.000000  0.000000>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  16.665163 }
         plane {     < 1.000000  0.000000  0.000000>  16.335852 inverse }
      }
      translate <-16.665163  0.000000  0.000000>
      scale     < 15.457652  1.000000  1.000000>
      translate <-1.078117  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  5.613365>
      rotate    < 0.000000 -76.992054  0.000000>
      translate <-16.400000  5.000000  0.000000>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 16.370000  16.370000  16.370000>
      translate <-15.254947  5.500000  4.956633>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  16.334306 }
         plane {     < 1.000000  0.000000  0.000000>  16.005025 inverse }
      }
      translate <-16.334306  0.000000  0.000000>
      scale     < 15.118139  1.000000  1.000000>
      translate <-1.080444  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  5.739299>
      rotate    < 0.000000 -58.901369  0.000000>
      translate <-15.254947  5.500000  4.956633>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 16.040000  16.040000  16.040000>
      translate <-12.685386  6.000000  9.216473>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  16.003406 }
         plane {     < 1.000000  0.000000  0.000000>  15.674159 inverse }
      }
      translate <-16.003406  0.000000  0.000000>
      scale     < 14.778706  1.000000  1.000000>
      translate <-1.082869  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  5.870980>
      rotate    < 0.000000 -40.806519  0.000000>
      translate <-12.685386  6.000000  9.216473>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 15.710000  15.710000  15.710000>
      translate <-9.004870  6.500000  12.394140>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  15.672460 }
         plane {     < 1.000000  0.000000  0.000000>  15.343249 inverse }
      }
      translate <-15.672460  0.000000  0.000000>
      scale     < 14.439367  1.000000  1.000000>
      translate <-1.085398  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  6.008802>
      rotate    < 0.000000 -22.707174  0.000000>
      translate <-9.004870  6.500000  12.394140>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 15.380000  15.380000  15.380000>
      translate <-4.622894  7.000000  14.227805>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  15.341466 }
         plane {     < 1.000000  0.000000  0.000000>  15.012293 inverse }
      }
      translate <-15.341466  0.000000  0.000000>
      scale     < 14.100120  1.000000  1.000000>
      translate <-1.088038  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  6.153207>
      rotate    < 0.000000 -4.603027  0.000000>
      translate <-4.622894  7.000000  14.227805>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 15.050000  15.050000  15.050000>
      translate < 0.000000  7.500000  14.600000>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  15.010419 }
         plane {     < 1.000000  0.000000  0.000000>  14.681286 inverse }
      }
      translate <-15.010419  0.000000  0.000000>
      scale     < 13.760977  1.000000  1.000000>
      translate <-1.090796  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  6.304672>
      rotate    < 0.000000  13.506302  0.000000>
      translate < 0.000000  7.500000  14.600000>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 14.720000  14.720000  14.720000>
      translate < 4.400402  8.000000  13.543045>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  14.679314 }
         plane {     < 1.000000  0.000000  0.000000>  14.350226 inverse }
      }
      translate <-14.679314  0.000000  0.000000>
      scale     < 13.421942  1.000000  1.000000>
      translate <-1.093680  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  6.463725>
      rotate    < 0.000000  31.621196  0.000000>
      translate < 4.400402  8.000000  13.543045>
   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 14.390000  14.390000  14.390000>
      translate < 8.158459  8.500000  11.229156>
   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  14.348148 }
         plane {     < 1.000000  0.000000  0.000000>  14.019108 inverse }
      }
      translate <-14.348148  0.000000  0.000000>
      scale     < 13.083027  1.000000  1.000000>
      translate <-1.096699  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  6.630945>
      rotate    < 0.000000  49.742072  0.000000>
      translate < 8.158459  8.500000  11.229156>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 14.060000  14.060000  14.060000>
      translate < 10.937910  9.000000  7.946857>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  14.016915 }
         plane {     < 1.000000  0.000000  0.000000>  13.687926 inverse }
      }
      translate <-14.016915  0.000000  0.000000>
      scale     < 12.744239  1.000000  1.000000>
      translate <-1.099863  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  6.806973>
      rotate    < 0.000000  67.869444  0.000000>
      translate < 10.937910  9.000000  7.946857>          


   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 13.730000  13.730000  13.730000>
      translate < 12.515904  9.500000  4.066664>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  13.685609 }
         plane {     < 1.000000  0.000000  0.000000>  13.356676 inverse }
      }
      translate <-13.685609  0.000000  0.000000>
      scale     < 12.405591  1.000000  1.000000>
      translate <-1.103181  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  6.992516>
      rotate    < 0.000000  86.003826  0.000000>
      translate < 12.515904  9.500000  4.066664>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 13.400000  13.400000  13.400000>
      translate < 12.800000  10.000000 -0.000000>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  13.354224 }
         plane {     < 1.000000  0.000000  0.000000>  13.025351 inverse }
      }
      translate <-13.354224  0.000000  0.000000>
      scale     < 12.067089  1.000000  1.000000>
      translate <-1.106665  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  7.188362>
      rotate    < 0.000000  104.145808  0.000000>
      translate < 12.800000  10.000000 -0.000000>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 13.070000  13.070000  13.070000>
      translate < 11.831143  10.500000 -3.844171>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  13.022752 }
         plane {     < 1.000000  0.000000  0.000000>  12.693945 inverse }
      }
      translate <-13.022752  0.000000  0.000000>
      scale     < 11.728756  1.000000  1.000000>
      translate <-1.110327  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  7.395380>
      rotate    < 0.000000  122.296057  0.000000>
      translate < 11.831143  10.500000 -3.844171>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 12.740000  12.740000  12.740000>
      translate < 9.772925  11.000000 -7.100446>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  12.691186 }
         plane {     < 1.000000  0.000000  0.000000>  12.362450 inverse }
      }
      translate <-12.691186  0.000000  0.000000>
      scale     < 11.390594  1.000000  1.000000>
      translate <-1.114181  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  7.614551>
      rotate    < 0.000000  140.455326  0.000000>
      translate < 9.772925  11.000000 -7.100446>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 12.410000  12.410000  12.410000>
      translate < 6.888843  11.500000 -9.481679>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  12.359516 }
         plane {     < 1.000000  0.000000  0.000000>  12.030858 inverse }
      }
      translate <-12.359516  0.000000  0.000000>
      scale     < 11.052630  1.000000  1.000000>
      translate <-1.118242  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  7.846957>
      rotate    < 0.000000  158.624429  0.000000>
      translate < 6.888843  11.500000 -9.481679>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 12.080000  12.080000  12.080000>
      translate < 3.510433  12.000000 -10.804002>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  12.027732 }
         plane {     < 1.000000  0.000000  0.000000>  11.699160 inverse }
      }
      translate <-12.027732  0.000000  0.000000>
      scale     < 10.714877  1.000000  1.000000>
      translate <-1.122526  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  8.093823>
      rotate    < 0.000000  176.804323  0.000000>
      translate < 3.510433  12.000000 -10.804002>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 11.750000  11.750000  11.750000>
      translate <-0.000000  12.500000 -11.000000>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  11.695822 }
         plane {     < 1.000000  0.000000  0.000000>  11.367344 inverse }
      }
      translate <-11.695822  0.000000  0.000000>
      scale     < 10.377358  1.000000  1.000000>
      translate <-1.127052  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  8.356523>
      rotate    < 0.000000 -165.003937  0.000000>
      translate <-0.000000  12.500000 -11.000000>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 11.420000  11.420000  11.420000>
      translate <-3.287941  13.000000 -10.119241>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  11.363773 }
         plane {     < 1.000000  0.000000  0.000000>  11.035398 inverse }
      }
      translate <-11.363773  0.000000  0.000000>
      scale     < 10.040091  1.000000  1.000000>
      translate <-1.131840  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  8.636612>
      rotate    < 0.000000 -146.799161  0.000000>
      translate <-3.287941  13.000000 -10.119241>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 11.090000  11.090000  11.090000>
      translate <-6.042432  13.500000 -8.316695>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  11.031570 }
         plane {     < 1.000000  0.000000  0.000000>  10.703309 inverse }
      }
      translate <-11.031570  0.000000  0.000000>
      scale     < 9.703114  1.000000  1.000000>
      translate <-1.136910  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  8.935839>
      rotate    < 0.000000 -128.579963  0.000000>
      translate <-6.042432  13.500000 -8.316695>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 10.760000  10.760000  10.760000>
      translate <-8.025449  14.000000 -5.830830>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  10.699195 }
         plane {     < 1.000000  0.000000  0.000000>  10.371060 inverse }
      }
      translate <-10.699195  0.000000  0.000000>
      scale     < 9.366447  1.000000  1.000000>
      translate <-1.142290  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  9.256214>
      rotate    < 0.000000 -110.344734  0.000000>
      translate <-8.025449  14.000000 -5.830830>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 10.430000  10.430000  10.430000>
      translate <-9.092100  14.500000 -2.954202>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  10.366628 }
         plane {     < 1.000000  0.000000  0.000000>  10.038633 inverse }
      }
      translate <-10.366628  0.000000  0.000000>
      scale     < 9.030126  1.000000  1.000000>
      translate <-1.148005  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  9.600018>
      rotate    < 0.000000 -92.091755  0.000000>
      translate <-9.092100  14.500000 -2.954202>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 10.100000  10.100000  10.100000>
      translate <-9.200000  15.000000  0.000000>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  10.033847 }
         plane {     < 1.000000  0.000000  0.000000>  9.706008 inverse }
      }
      translate <-10.033847  0.000000  0.000000>
      scale     < 8.694197  1.000000  1.000000>
      translate <-1.154085  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  9.969862>
      rotate    < 0.000000 -73.818862  0.000000>
      translate <-9.200000  15.000000  0.000000>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 9.770000  9.770000  9.770000>
      translate <-8.407340  15.500000  2.731710>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  9.700824 }
         plane {     < 1.000000  0.000000  0.000000>  9.373161 inverse }
      }
      translate <-9.700824  0.000000  0.000000>
      scale     < 8.358707  1.000000  1.000000>
      translate <-1.160565  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  10.368759>
      rotate    < 0.000000 -55.523638  0.000000>
      translate <-8.407340  15.500000  2.731710>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 9.440000  9.440000  9.440000>
      translate <-6.860464  16.000000  4.984419>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  9.367528 }
         plane {     < 1.000000  0.000000  0.000000>  9.040062 inverse }
      }
      translate <-9.367528  0.000000  0.000000>
      scale     < 8.023703  1.000000  1.000000>
      translate <-1.167482  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  10.800200>
      rotate    < 0.000000 -37.203272  0.000000>
      translate <-6.860464  16.000000  4.984419>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 9.110000  9.110000  9.110000>
      translate <-4.772816  16.500000  6.569218>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  9.033923 }
         plane {     < 1.000000  0.000000  0.000000>  8.706679 inverse }
      }
      translate <-9.033923  0.000000  0.000000>
      scale     < 7.689255  1.000000  1.000000>
      translate <-1.174876  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  11.268229>
      rotate    < 0.000000 -18.854426  0.000000>
      translate <-4.772816  16.500000  6.569218>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 8.780000  8.780000  8.780000>
      translate <-2.397972  17.000000  7.380199>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  8.699966 }
         plane {     < 1.000000  0.000000  0.000000>  8.372974 inverse }
      }
      translate <-8.699966  0.000000  0.000000>
      scale     < 7.355440  1.000000  1.000000>
      translate <-1.182793  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  11.777573>
      rotate    < 0.000000 -0.473103  0.000000>
      translate <-2.397972  17.000000  7.380199>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 8.450000  8.450000  8.450000>
      translate < 0.000000  17.500000  7.400000>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  8.365605 }
         plane {     < 1.000000  0.000000  0.000000>  8.038901 inverse }
      }
      translate <-8.365605  0.000000  0.000000>
      scale     < 7.022345  1.000000  1.000000>
      translate <-1.191284  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  12.333789>
      rotate    < 0.000000  17.945328  0.000000>
      translate < 0.000000  17.500000  7.400000>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 8.120000  8.120000  8.120000>
      translate < 2.175480  18.000000  6.695438>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  8.030780 }
         plane {     < 1.000000  0.000000  0.000000>  7.704406 inverse }
      }
      translate <-8.030780  0.000000  0.000000>
      scale     < 6.690073  1.000000  1.000000>
      translate <-1.200402  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  12.943441>
      rotate    < 0.000000  36.406587  0.000000>
      translate < 2.175480  18.000000  6.695438>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 7.790000  7.790000  7.790000>
      translate < 3.926405  18.500000  5.404234>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  7.695421 }
         plane {     < 1.000000  0.000000  0.000000>  7.369427 inverse }
      }
      translate <-7.695421  0.000000  0.000000>
      scale     < 6.358767  1.000000  1.000000>
      translate <-1.210206  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  13.614295>
      rotate    < 0.000000  54.917483  0.000000>
      translate < 3.926405  18.500000  5.404234>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 7.460000  7.460000  7.460000>
      translate < 5.112987  19.000000  3.714803>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  7.359439 }
         plane {     < 1.000000  0.000000  0.000000>  7.033888 inverse }
      }
      translate <-7.359439  0.000000  0.000000>
      scale     < 6.028575  1.000000  1.000000>
      translate <-1.220759  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  14.355670>
      rotate    < 0.000000  73.486397  0.000000>
      translate < 5.112987  19.000000  3.714803>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 7.130000  7.130000  7.130000>
      translate < 5.668297  19.500000  1.841741>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  7.022732 }
         plane {     < 1.000000  0.000000  0.000000>  6.697697 inverse }
      }
      translate <-7.022732  0.000000  0.000000>
      scale     < 5.699689  1.000000  1.000000>
      translate <-1.232126  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  15.178751>
      rotate    < 0.000000  92.123718  0.000000>
      translate < 5.668297  19.500000  1.841741>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 6.800000  6.800000  6.800000>
      translate < 5.600000  20.000000 -0.000000>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  6.685174 }
         plane {     < 1.000000  0.000000  0.000000>  6.360747 inverse }
      }
      translate <-6.685174  0.000000  0.000000>
      scale     < 5.372357  1.000000  1.000000>
      translate <-1.244365  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  16.097039>
      rotate    < 0.000000  110.842321  0.000000>
      translate < 5.600000  20.000000 -0.000000>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 6.470000  6.470000  6.470000>
      translate < 4.983536  20.500000 -1.619249>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  6.346614 }
         plane {     < 1.000000  0.000000  0.000000>  6.022907 inverse }
      }
      translate <-6.346614  0.000000  0.000000>
      scale     < 5.046876  1.000000  1.000000>
      translate <-1.257533  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  17.126987>
      rotate    < 0.000000  129.658554  0.000000>
      translate < 4.983536  20.500000 -1.619249>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 6.140000  6.140000  6.140000>
      translate < 3.948003  21.000000 -2.868392>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  6.006869 }
         plane {     < 1.000000  0.000000  0.000000>  5.684024 inverse }
      }
      translate <-6.006869  0.000000  0.000000>
      scale     < 4.723632  1.000000  1.000000>
      translate <-1.271663  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  18.288684>
      rotate    < 0.000000  148.593386  0.000000>
      translate < 3.948003  21.000000 -2.868392>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 5.810000  5.810000  5.810000>
      translate < 2.656789  21.500000 -3.656757>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  5.665718 }
         plane {     < 1.000000  0.000000  0.000000>  5.343913 inverse }
      }
      translate <-5.665718  0.000000  0.000000>
      scale     < 4.403111  1.000000  1.000000>
      translate <-1.286754  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  19.606835>
      rotate    < 0.000000  167.674035  0.000000>
      translate < 2.656789  21.500000 -3.656757>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 5.480000  5.480000  5.480000>
      translate < 1.285511  22.000000 -3.956395>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  5.322903 }
         plane {     < 1.000000  0.000000  0.000000>  5.002363 inverse }
      }
      translate <-5.322903  0.000000  0.000000>
      scale     < 4.085966  1.000000  1.000000>
      translate <-1.302728  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  21.111749>
      rotate    < 0.000000 -173.063496  0.000000>
      translate < 1.285511  22.000000 -3.956395>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 5.150000  5.150000  5.150000>
      translate <-0.000000  22.500000 -3.800000>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  4.978122 }
         plane {     < 1.000000  0.000000  0.000000>  4.659135 inverse }
      }
      translate <-4.978122  0.000000  0.000000>
      scale     < 3.773035  1.000000  1.000000>
      translate <-1.319395  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  22.840766>
      rotate    < 0.000000 -153.570633  0.000000>
      translate <-0.000000  22.500000 -3.800000>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 4.820000  4.820000  4.820000>
      translate <-1.063018  23.000000 -3.271634>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  4.631047 }
         plane {     < 1.000000  0.000000  0.000000>  4.313984 inverse }
      }
      translate <-4.631047  0.000000  0.000000>
      scale     < 3.465471  1.000000  1.000000>
      translate <-1.336340  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  24.839270>
      rotate    < 0.000000 -133.780866  0.000000>
      translate <-1.063018  23.000000 -3.271634>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 4.490000  4.490000  4.490000>
      translate <-1.810379  23.500000 -2.491772>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  4.281360 }
         plane {     < 1.000000  0.000000  0.000000>  3.966695 inverse }
      }
      translate <-4.281360  0.000000  0.000000>
      scale     < 3.164827  1.000000  1.000000>
      translate <-1.352794  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  27.161540>
      rotate    < 0.000000 -113.600358  0.000000>
      translate <-1.810379  23.500000 -2.491772>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 4.160000  4.160000  4.160000>
      translate <-2.200526  24.000000 -1.598776>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  3.928854 }
         plane {     < 1.000000  0.000000  0.000000>  3.617190 inverse }
      }
      translate <-3.928854  0.000000  0.000000>
      scale     < 2.873290  1.000000  1.000000>
      translate <-1.367371  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  29.869268>
      rotate    < 0.000000 -92.894757  0.000000>
      translate <-2.200526  24.000000 -1.598776>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 3.830000  3.830000  3.830000>
      translate <-2.244493  24.500000 -0.729280>

   }

   object {
      quadric {
         Connector_2 } texture { snail_bone } texture { bone2 
      }

      clipped_by {
         plane {     < 1.000000  0.000000  0.000000>  3.573633 }
         plane {     < 1.000000  0.000000  0.000000>  3.265723 inverse }
      }
      translate <-3.573633  0.000000  0.000000>
      scale     < 2.593922  1.000000  1.000000>
      translate <-1.377695  0.000000  0.000000>
      rotate    < 0.000000  180.000000  0.000000>
      rotate    < 0.000000  0.000000  33.025855>
      rotate    < 0.000000 -71.466140  0.000000>
      translate <-2.244493  24.500000 -0.729280>

   }

   object {
      quadric {
         Dot } texture { snail_skin }
      scale     < 3.500000  3.500000  3.500000>
      translate <-2.000000  25.000000  0.000000>

   }

   bounded_by {
      quadric {
         Dot
         scale < 60.445726  60.445726  60.445726>
         translate < 3.450000  4.250000 -3.450000>
      }
   }

}


/*------------------ THE END OF THE SEA SHELL ---------------------------*/


/*------------- THE SLIMMY BOBY ATTACHED TO THE SHELL -----*/

#declare slime_body = object {

   union {

      quadric { Dot
         scale     < 19.750000  19.750000  19.750000>
      }              



      difference { quadric {  Connector_2
         }                    

         plane {     < 1.000000  0.000000  0.000000>  18.974120 inverse }
         plane {     < 1.000000  0.000000  0.000000>  17.052691 }
         translate <-18.974120  0.000000  0.000000>
         scale     < 3.461575  1.000000  1.000000>
         translate <-5.481355  0.000000  0.000000>
         rotate    < 0.000000  180.000000  0.000000>
         rotate    < 0.000000  0.000000 -2.385944>
      }                      

      quadric { Dot
         scale     < 17.750000  17.750000  17.750000>
         translate < 7.200000 -0.300000  0.000000>
      }              


      difference { quadric { Connector_2
         }                    

         plane {     < 1.000000  0.000000  0.000000>  17.745302 inverse }
         plane {     < 1.000000  0.000000  0.000000>  17.495368 }
         translate <-17.745302  0.000000  0.000000>
         scale     < 43.454344  1.000000  1.000000>
         translate <-0.408367  0.000000  0.000000>
         rotate    < 0.000000  180.000000  0.000000>
         rotate    < 0.000000  0.000000 -6.340192>
         translate < 7.200000 -0.300000  0.000000>
      }


      quadric { Dot
         scale     < 17.500000  17.500000  17.500000>
         translate < 18.000000 -1.500000  0.000000>
      }              


      difference { quadric { Connector_2
         }                    

         plane {     < 1.000000  0.000000  0.000000>  17.320524 inverse }
         plane {     < 1.000000  0.000000  0.000000>  15.835908 }

         translate <-17.320524  0.000000  0.000000>
         scale     < 6.928524  1.000000  1.000000>
         translate <-2.499887  0.000000  0.000000>
         rotate    < 0.000000  180.000000  0.000000>
         rotate    < 0.000000  0.000000 -0.545658>
         translate < 18.000000 -1.500000  0.000000>
      }


      quadric { Dot
         scale     < 16.000000  16.000000  16.000000>
         translate < 28.500000 -1.600000  0.000000>
      }              


      difference { quadric { Connector_2
         }                    

         plane {     < 1.000000  0.000000  0.000000>  15.993736 inverse }
         plane {     < 1.000000  0.000000  0.000000>  15.493932 }

         translate <-15.993736  0.000000  0.000000>
         scale     < 35.727860  1.000000  1.000000>
         translate <-0.447654  0.000000  0.000000>
         rotate    < 0.000000  180.000000  0.000000>
         rotate    < 0.000000  0.000000  9.989407>
         translate < 28.500000 -1.600000  0.000000>
      }


      quadric { Dot
         scale     < 15.500000  15.500000  15.500000>
         translate < 46.100000  1.500000  0.000000>
      }              


      difference { quadric { Connector_2
         }                    

         plane {     < 1.000000  0.000000  0.000000>  15.215923 inverse }
         plane {     < 1.000000  0.000000  0.000000>  12.565407 }

         translate <-15.215923  0.000000  0.000000>
         scale     < 5.151078  1.000000  1.000000>
         translate <-2.953930  0.000000  0.000000>
         rotate    < 0.000000  180.000000  0.000000>
         rotate    < 0.000000  0.000000  26.855115>
         rotate    < 0.000000  4.537773  0.000000>
         translate < 46.100000  1.500000  0.000000>
      }


      quadric { Dot
         scale     < 12.800000  12.800000  12.800000>
         translate < 58.700000  7.900000 -1.000000>
      }              


      difference { quadric { Connector_2
         }                    

         plane {     < 1.000000  0.000000  0.000000>  12.656484 inverse }
         plane {     < 1.000000  0.000000  0.000000>  10.876666 }

         translate <-12.656484  0.000000  0.000000>
         scale     < 6.621607  1.000000  1.000000>
         translate <-1.911392  0.000000  0.000000>
         rotate    < 0.000000  180.000000  0.000000>
         rotate    < 0.000000  0.000000  42.219572>
         rotate    < 0.000000  9.673355  0.000000>
         translate < 58.700000  7.900000 -1.000000>
      }


      quadric { Dot
         scale     < 11.000000  11.000000  11.000000>
         translate < 67.500000  16.000000 -2.500000>
      }              

      difference { quadric { Connector_2
         }                    

         plane {     < 1.000000  0.000000  0.000000>  10.929158 inverse }
         plane {     < 1.000000  0.000000  0.000000>  9.438819 }

         translate <-10.929158  0.000000  0.000000>
         scale     < 8.768631  1.000000  1.000000>
         translate <-1.246393  0.000000  0.000000>
         rotate    < 0.000000  180.000000  0.000000>
         rotate    < 0.000000  0.000000  65.021660>
         rotate    < 0.000000  26.565051  0.000000>
         translate < 67.500000  16.000000 -2.500000>
      }


      quadric { Dot
         scale     < 9.500000  9.500000  9.500000>
         translate < 72.500000  28.000000 -5.000000>
      }              


      difference { quadric { Connector_2 
         }                    

         plane {     < 1.000000  0.000000  0.000000>  10.467692 inverse }
         plane {     < 1.000000  0.000000  0.000000>  9.470769 }

         translate <-9.470769  0.000000  0.000000>
         scale     < 12.718097  1.000000  1.000000>
         translate < 0.744669  0.000000  0.000000>
         rotate    < 0.000000  0.000000  64.348141>
         rotate    < 0.000000  84.805571  0.000000>
         translate < 72.500000  28.000000 -5.000000>
      }


      quadric { Dot
         scale     < 10.500000  10.500000  10.500000>
         translate < 73.000000  39.500000 -10.500000>
      }              


      difference { quadric { Connector_2
         }                    

         plane {     < 1.000000  0.000000  0.000000>  9.613702 inverse }
         plane {     < 1.000000  0.000000  0.000000>  6.180237 }

         translate <-9.613702  0.000000  0.000000>
         scale     < 2.276957  1.000000  1.000000>
         translate <-4.222171  0.000000  0.000000>
         rotate    < 0.000000  180.000000  0.000000>
         rotate    < 0.000000  0.000000  46.816209>
         rotate    < 0.000000  54.566686  0.000000>
         translate < 73.000000  39.500000 -10.500000>
      }


      quadric { Dot
         scale     < 6.750000  6.750000  6.750000>
         translate < 76.700000  46.300000 -15.700000>
      }              

   }
   texture { slime rotate <0 0 0> scale <80 10 80> }
   texture { color Orange alpha 0.9 phong 1.0 phong_size 20 bumps 0.6 frequency 20 }

   bounded_by {
      quadric { Dot
         scale < 73.257941  73.257941  73.257941>
         translate < 31.875000  16.650000 -1.350000>
      }              
   }

}

/*---------------------- END OF SLIMMY BODY ---------------*/


/*----------- MAKE SNAIL ANTLERS ---------------------------*/
#declare antlers = object {
   union {
      difference {
         intersection { UCone scale <3 40 3> }
         plane { <0 1 0> -10 inverse }
      }
      quadric { Ellipsoid scale <3 3 3> translate <0 -10 0> }
   }
   texture { slime rotate <0 0 0> scale <8 1 8> }
   texture { color Orange alpha 0.9 phong 1.0 phong_size 20 bumps 0.6 frequency 20 }
   translate <0 40 0>
}


#declare eyeb = object {
   quadric { Ellipsoid scale <2 1.5 2> }
   texture { color Black phong 1.0 }
   translate <0 32.2 0> }



/*----------------------COMPOSITE THE SNAIL-------------------------*/
#declare body = composite {
   object { slime_body rotate <0 -90 0> }
   object { antlers rotate <0 0 35> rotate <-35 0 0> translate <13.6 45 70> }
   object { antlers rotate <0 0 -45> rotate <-25 0 0> translate <17.0 45 70> }

   object { eyeb rotate <0 0 35> rotate <-35 0 0> translate <13.6 45 70> }
   object { eyeb rotate <0 0 -45> rotate <-25 0 0> translate <17 45 70> }

}

#declare thesnail = composite {
   composite { body rotate <0 0 0>  translate <26 0 3> }
   composite { snail scale <1.12 1.1 1.1> rotate <0 0 0> }
}

/*----------- THE TOTAL SNAIN / I AM SNAIL-SEE ME CRAWL----*/
/*COMPOSITE THESNAIL ROTATE <0 0 0> END_COMPOSITE*/
