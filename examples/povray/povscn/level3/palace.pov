// Persistence Of Vision raytracer version 1.0 sample file.


// Two rows of multi-colored pillars lead to a chrome hemisphere 
// reflecting a blinding sunrise. A nice looking cliche.
// - Drew Wells 1990 
//   cis 73767,1244 

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

// Camera 
camera {
   location <0.0  20.0 -100.0>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.333333333 0.0 0.0>
}


// Ground 
object {
   plane { < 0.0 1.0 0.0 > -10. }
   texture {
      White_Marble
      scale <10.0 10.0 10.0>
      ambient 0.3
      diffuse 0.7
   }
   color White
}

// Sky/Ceiling 
object {
   quadric { Ellipsoid 
      scale     < 200.0 100.0 200.0 >
   }
   texture {
      bozo
      turbulence 0.5
      colour_map { /* Sky Colors */
         [0.0 0.5   colour red 0.25 green 0.25 blue 0.5
         colour red 0.25 green 0.25 blue 0.5]
         [0.5 0.6   colour red 0.25 green 0.25 blue 0.5
         colour red 0.7 green 0.7 blue 0.7]
         [0.6 1.001 colour red 0.7 green 0.7 blue 0.7
         colour red 0.3 green 0.3 blue 0.3]
      }
      scale <50.0 50.0 50.0>
      translate <200.0 400.0 100.0>
      ambient 0.6
      diffuse 0.7
   }
   color White
}

// Mirrored sphere { to reflect sun 
// Diffuse value controls aura effect 
object {
   sphere { < 0.0 0.0 110.0 > 40.0 }
   texture { // Mirror
      color White
      ambient 0.0
      diffuse 0.3
      reflection 1.0
      brilliance 3.0
      specular 1.0
      roughness 0.01
   }
   color White
}

// Two rows of pillars - Grouped two per object { for easy manipulation 
object {
   union {
      quadric {
         Cylinder_Y
         scale <6.0 1.0 3.0>
         translate <-60.0 0.0 0.0>
      }
      quadric {
         Cylinder_Y
         scale <6.0 1.0 3.0>
         translate <60.0 0.0 0.0>
      }
      quadric {
         Cylinder_Y
         scale <6.0 1.0 3.0>
         translate <-60.0 0.0 180.0>
      }
      quadric {
         Cylinder_Y
         scale <6.0 1.0 3.0>
         translate <60.0 0.0 180.0>
      }
      quadric {
         Cylinder_Y
         scale <6.0 1.0 3.0>
         translate <-60.0 0.0 60.0>
      }
      quadric {
         Cylinder_Y
         scale <6.0 1.0 3.0>
         translate <60.0 0.0 60.0>
      }
   }
   texture {
      Blue_Agate
      scale <10.0 10.0 10.0>
      ambient 0.2
      diffuse 0.7
      brilliance 3.0
      specular 1.0
      roughness 0.01
   }
   color Blue
}

object {
   union {
      quadric {
         Cylinder_Y
         scale <6.0 1.0 3.0>
         translate <-60.0 0.0 30.0>
      }
      quadric {
         Cylinder_Y
         scale <6.0 1.0 3.0>
         translate <60.0 0.0 30.0>
      }
   }
   texture {
      gradient < 1.0 1.0 0.0 >
      colour_map {
         [0.00 0.25  colour red 0.0 green 0.0 blue 1.0
         colour red 0.7 green 0.3 blue 0.0]
         [0.25 0.75  colour red 1.0 green 0.0 blue 1.0
         colour red 0.8 green 0.4 blue 1.0]
         [0.75 1.001 colour red 0.0 green 0.3 blue 0.8
         colour red 0.7 green 0.3 blue 0.0]
      }
      scale <30.0 30.0 30.0>
      translate <30.0 -30.0 0.0>
      ambient 0.2
      diffuse 0.7
      brilliance 3.0
      specular 1.0
      roughness 0.01
   }
   color White
}

object {
   intersection {
      union {
         quadric {
            Cylinder_Y
            scale <6.0 1.0 3.0>
            translate <-60.0 0.0 60.0>
         }
         quadric {
            Cylinder_Y
            scale <6.0 1.0 3.0>
            translate <60.0 0.0 60.0>
         }
      }
      plane { < 0.0 1.0 0.0 > 250.0 }
   }
   texture {
      Blue_Agate
      scale <10.0 10.0 10.0>
      specular 1.0
      roughness 0.01
   }
   color Blue
}

object {
   intersection {
      union {
         quadric {
            Cylinder_Y
            scale <6.0 1.0 3.0>
            translate <-60.0 0.0 90.0>
         }
         quadric {
            Cylinder_Y
            scale <6.0 1.0 3.0>
            translate <60.0 0.0 90.0>
         }
      }
      plane { < 0.0 1.0 0.0 > 250.0 }
   }
   texture {
      gradient < 1.0 1.0 0.0 >
      colour_map {
         [0.00 0.25  colour red 0.0 green 0.8 blue 0.4
         colour red 0.2 green 0.3 blue 0.0]
         [0.25 0.75  colour red 0.0 green 0.0 blue 0.8
         colour red 0.1 green 0.6 blue 1.0]
         [0.75 1.001 colour red 0.0 green 0.3 blue 0.4
         colour red 0.8 green 0.8 blue 0.0]
      }
      scale <30.0 30.0 30.0>
      translate <30.0 -30.0 0.0>
      ambient 0.2
      diffuse 0.7
      brilliance 3.0
      specular 1.0
      roughness 0.01
   }
   color White
}

object {
   intersection {
      union {
         quadric {
            Cylinder_Y
            scale <6.0 1.0 3.0>
            translate <-60.0 0.0 120.0>
         }
         quadric {
            Cylinder_Y
            scale <6.0 1.0 3.0>
            translate <60.0 0.0 120.0>
         }
      }
      plane { < 0.0 1.0 0.0 > 250.0 }
   }
   texture {
      Blue_Agate
      scale <10.0 10.0 10.0>
      specular 1.0
      roughness 0.01
   }
   color Blue
}

object {
   intersection {
      union {
         quadric {
            Cylinder_Y
            scale <6.0 1.0 3.0>
            translate <-60.0 0.0 150.0>
         }
         quadric {
            Cylinder_Y
            scale <6.0 1.0 3.0>
            translate <60.0 0.0 150.0>
         }
      }
      plane { < 0.0 1.0 0.0 > 250.0 }
   }
   texture {
      gradient < 1.0 1.0 0.0 >
      colour_map {
         [0.00 0.25  colour red 0.0 green 0.8 blue 0.4
         colour red 0.2 green 0.3 blue 0.0]
         [0.25 0.75  colour red 0.0 green 0.0 blue 0.8
         colour red 0.1 green 0.6 blue 1.0]
         [0.75 1.001 colour red 0.0 green 0.3 blue 0.4
         colour red 0.8 green 0.8 blue 0.0]
      }
      scale <30.0 30.0 30.0>
      translate <30.0 -30.0 0.0>
      ambient 0.2
      diffuse 0.7
      brilliance 3.0
      specular 1.0
      roughness 0.01
   }
   color White
}

// The Blinding Sun 
object {
   light_source { < 0.0 50.0 -100.0 >
      colour White
   }
}
