// Persistence Of Vision raytracer version 1.0 sample file.
// Demonstrates clipped_by

#include "colors.inc"           // Standard colors library
#include "textures.inc"         // LOTS of neat textures.  Lots of NEW textures.

camera {
   location <2.0  4  -4>
   direction <0.0 0.0  1.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0 0 0>
}


// Light source
#declare Grayscale = 0.25
#declare AmbientLight = color red Grayscale green Grayscale blue Grayscale

object { light_source { <-20 30 -100> color White  }}

object { light_source { <50 50 15> color AmbientLight }}


// A hollow sphere using a clipping plane.  This sphere has no "thickness"
// to its walls,  no matter what the scaling.
object {
   sphere { <0 0 0> 1 }
   clipped_by {
      plane { <0 1 0> 0.25 }
   }

   texture {
      Phong_Glossy
      ambient 0.2
      gradient <1 1 1>
      // Notice the -1 to +1 color range.  This is just to demonstrate
      // that this is a valid form for a color map.  It's not really
      // neccessary in this case to do it this way, but thought I'd
      // use it for instructional value.
      color_map {
         [ -1.0  0.0 color Yellow color Cyan ]
         [  0.0  1.0 color Cyan color Magenta]
      }
      scale <0.1  0.1  0.1>
   }
   bounded_by {
      sphere { <0 0 0> 1.001 }
   }
   translate <-2 1 0>
}

// Hollow sphere done with intersection.
object {
   intersection {
      sphere { <0 0 0> 1 }               // outer wall
      sphere { <0 0 0> 0.85 inverse }     // inner wall
      plane { <0 1 0> 0.25 }              // top surface
   }
   texture {
      Phong_Glossy
      ambient 0.2
      leopard
      color_map {
         [0.0   0.10 color Yellow color Red ]
         [0.10  0.98 color Red color Blue ]
         [0.98  1.00 color Magenta color Yellow  ]
      }
      //        scale <0.05  1.0  0.05>
      scale <0.025  1.0  0.025>
   }
   bounded_by {
      sphere { <0 0 0> 1.001 }
   }
   translate <2 1 0>
}

// Flat-topped sphere/plane intersection
object {
   intersection {
      sphere { <0 0 0> 1 }               // outer wall
      plane { <0 1 0> 0.25 }             // top surface
   }
   texture {
      Phong_Glossy
      ambient 0.2
      onion
      turbulence 10                      // try with 0 turb, too!
      octaves 2
      color_map {
         [0.0   0.30 color Yellow color Orange ]
         [0.30  0.90 color Orange color Magenta ]
         [0.90  1.00 color Blue color Green    ]
      }
   }
   bounded_by {
      sphere { <0 0 0> 1.001 }
   }
   translate <0 1 -1>
}

