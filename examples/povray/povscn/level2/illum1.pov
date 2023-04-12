// Persistence Of Vision raytracer version 1.0 sample file.

// By Tom Price
// Lots of refraction make this a slow one to trace, but worth it! - dmf

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
   plane { <0.0 1.0 0.0> -60.0 }
   texture {
      0.01
      Pine_Wood
      scale <4.0 1.0 2.0>
      rotate <0.0 45.0 0.0>
      ambient 0.15
      diffuse 0.8
      brilliance 3.0
   }
}

object {
   quadric { Cylinder_X }
   scale <5.0 5.0 5.0>
   translate <0.0 -60.0 120.0>
   rotate <0.0 -40.0 0.0>
   texture {
      0.01
      colour Brown
      ambient 0.15
      diffuse 0.8
      reflection 0.4
   }
}

object {
   quadric { Cylinder_X }
   scale <5.0 5.0 5.0>
   translate <0.0 -60.0 120.0>
   rotate <0.0 40.0 0.0>
   texture {
      0.01
      colour Brown
      ambient 0.15
      diffuse 0.8
      /*         reflection 0.4	*/
   }
}

object {
   plane { <0.0 0.0 1.0> -240.0 }
   texture {
      0.01
      colour Gray
      ambient 0.15
      diffuse 0.8
      brilliance 3.0
   }
}

object {
   plane { <0.0 0.0 1.0> 120.0 }
   rotate <0.0 -40.0 0.0>
   texture {
      0.01
      colour Gray
      ambient 0.15
      diffuse 0.8
      reflection 0.75
      brilliance 3.0
   }
}

object {
   plane { <0.0 0.0 1.0> 120.0 }
   rotate <0.0 40.0 0.0>
   texture {
      0.01
      colour Gray
      ambient 0.15
      diffuse 0.8
      reflection 0.75
      brilliance 3.0
   }
}

object {
   sphere { <0.0 32.659862 0.0> 20.0 }

   texture { colour Clear }
   texture {
      0.01
      colour White alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }
}

object {
   sphere { <0.0 0.0 -23.09401> 20.0 }
   rotate <0.0 -15.0 0.0>

   texture { colour Clear }
   texture {
      0.01
      colour Red alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }

}

object {
   sphere { <0.0 0.0 -23.09401> 20.0 }
   rotate <0.0 -135.0 0.0>
   texture { colour Clear }
   texture {
      0.01
      colour Green alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }
}

object {
   sphere { <0.0 0.0 -23.09401> 20.0 }
   rotate <0.0 105.0 0.0>

   texture { colour Clear }
   texture {
      0.01
      colour Blue alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -23.09401> 20.0 }
   rotate <0.0 -75.0 0.0>

   texture { colour Clear }
   texture {
      0.01
      colour Cyan alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -23.09401> 20.0 }
   rotate <0.0 -195.0 0.0>

   texture { colour Clear }
   texture {
      0.01
      colour Magenta alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -23.09401> 20.0 }
   rotate <0.0 45.0 0.0>

   texture { colour Clear }
   texture {
      0.01
      colour Yellow alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -46.18802> 20.0 }
   rotate <0.0 -15.0 0.0>

   texture { colour Clear }
   texture {
      0.01
      colour DimGray alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -46.18802> 20.0 }
   rotate <0.0 -135.0 0.0>

   texture { colour Clear }
   texture {
      0.01
      colour Turquoise alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }
}

object {
   sphere { <0.0 -32.659862 -46.18802> 20.0 }
   rotate <0.0 105.0 0.0>

   texture { colour Clear }
   texture {
      0.01
      colour Tan alpha 0.90
      ambient 0.7
      diffuse 0.3
      reflection 0.5
      refraction 1.0
      ior 1.5
      brilliance 5.0
   }
}

object {
   light_source { <0.0 0.0 0.0>
      colour White
   }
}
