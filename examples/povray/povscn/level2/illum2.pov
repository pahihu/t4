// Persistence Of Vision raytracer version 1.0 sample file.

// By Tom Price

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0 30.0 -200.0>
   direction <0.0 -0.2 1.0>
   up <0.0 1.0 0.0>
   right <1.333 0.0 0.0>
}

object {
   intersection {
      plane { <0.0 0.0 1.0> 1.0 }
      plane { <0.0 0.0 1.0> -1.0 inverse }
      plane { <0.0 1.0 0.0> -60.0 }
   }

   scale <1.0 1.0 200.0>

   texture {
      0.01
      Apocalypse
      scale <50.0 10.0 20.0>
      rotate <0.0 30.0 0.0>
      ambient 0.3
      diffuse 0.7
      reflection 0.3
      brilliance 3.0
   }
}

object {
   intersection {
      sphere { <0.0 0.0 0.0> 10000.0 }
      sphere { <0.0 0.0 0.0> 9999.0 inverse }
   }

   translate <0.0 -9500.0 0.0>

   texture {
      0.05
      Bright_Blue_Sky
      scale <400.0 50.0 800.0>
      rotate <0.0 -45.0 0.0>
      ambient 0.3
      diffuse 0.5
   }
}

object {
   sphere { <0.0 32.659862 0.0> 20.0 }

   texture {
      0.01
      colour White
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   sphere { <0.0 0.0 -23.09401> 20.0 }

   rotate <0.0 -15.0 0.0>
   texture {
      0.01
      colour Red
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   sphere { <0.0 0.0 -23.09401> 20.0 }

   rotate <0.0 -135.0 0.0>
   texture {
      0.01
      colour Green
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   sphere { <0.0 0.0 -23.09401> 20.0 }

   rotate <0.0 105.0 0.0>
   texture {
      0.01
      colour Blue
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -23.09401> 20.0 }

   rotate <0.0 -75.0 0.0>
   texture {
      0.01
      colour Cyan
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -23.09401> 20.0 }

   rotate <0.0 -195.0 0.0>
   texture {
      0.01
      colour Magenta
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -23.09401> 20.0 }

   rotate <0.0 45.0 0.0>
   texture {
      0.01
      colour Yellow
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -46.18802> 20.0 }

   rotate <0.0 -15.0 0.0>
   texture {
      0.01
      colour DimGray
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -46.18802> 20.0 }

   rotate <0.0 -135.0 0.0>
   texture {
      0.01
      colour Turquoise
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -46.18802> 20.0 }

   rotate <0.0 105.0 0.0>
   texture {
      0.01
      colour Tan
      ambient 0.11
      diffuse 0.22
      reflection 0.7
      brilliance 5.0
   }
}

object {
   union {
      sphere { <0.0 0.0 0.0> 75.0
         texture {
            colour Orange
            ambient 1.0
            diffuse 0.0
         }
      }

      light_source { <0 0 0> colour Orange }
   }

   translate <0 9800 0>
   rotate <-5.0 0.0 2.0>
   translate <0.0 -11000.0 0.0>

   colour Orange
}

object {
   union {
      sphere { <0.0 0.0 0.0> 20.0 }
      light_source { <0 0 0> color White }
   }
   translate <60.0 160.0 -200.0>
   colour White
}

object {
   union {
      sphere { <0.0 0.0 0.0> 20.0 }
      light_source { <0 0 0> color White }
   }
   translate <-60.0 160.0 -200.0>
   colour White
}
