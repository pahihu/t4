// Persistence Of Vision raytracer version 1.0 sample file.

// This scene file makes a good starting point for developing tests and
// new scene files.  It is also used as the benchmark for the timing tests.

#include "colors.inc"

camera {
   location <0.0  3.0  -10.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 2 0>
}

// Light source
object {
   light_source {
      <10 20 -30>
      color White
   }
}

// Sky sphere 
object {
   sphere { <0 0 0> 10000 }
   texture {
      ambient 1.0  // Bath it in so much "light" that it doesn't have any shadows
      diffuse 0.7
      color blue 1
      brilliance 2
   }
}

// Floor plane 
object {
   plane { <0.0 1.0 0.0> 0.0 }
   colour White
   texture {
      colour NeonBlue
      ambient 0.15
      diffuse 0.8
   }
}

// Sphere object 
object {
   sphere { <0 3 0> 3 }
   texture {
      ambient 0.2
      diffuse 0.7
      color Orange
      phong 1
      phong_size 80
      brilliance 2
   }
}
