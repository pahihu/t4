// Persistence Of Vision raytracer version 1.0 sample file.

// By  Tom Price
// Requires "rough.gif" 320x200x256, but feel free to substitute any
// image that you wish.  (It goes in a picture frame on the desk)

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare
RedPencil = composite {
   object {
      intersection {
         quadric { Cylinder_Y scale <0.5 1.0 0.5> }
         plane { <0.0 1.0 0.0> 30.0 }
         plane { <0.0 1.0 0.0> 0.0 inverse }
      }
      texture {
         0.05
         ambient 0.3
         diffuse 0.7
         colour red 1.0
      }
   }

   object {
      intersection {
         quadric { Cylinder_Y scale <0.5 1.0 0.5> }
         plane { <0.0 1.0 0.0> 32.0 }
         plane { <0.0 1.0 0.0> 30.0 inverse }
      }
      texture {
         0.05
         ambient 0.3
         diffuse 0.7
         colour Tan
      }
   }
}

#declare
GreenPencil = composite {
   object {
      intersection {
         quadric { Cylinder_Y scale <0.5 1.0 0.5> }
         plane { <0.0 1.0 0.0> 30.0 }
         plane { <0.0 1.0 0.0> 0.0 inverse }
      }
      texture {
         0.05
         ambient 0.3
         diffuse 0.7
         colour green 1.0
      }
   }
   object {
      intersection {
         quadric { Cylinder_Y scale <0.5 1.0 0.5> }
         plane { <0.0 1.0 0.0> 32.0 }
         plane { <0.0 1.0 0.0> 30.0 inverse }
      }
      texture {
         0.05
         ambient 0.3
         diffuse 0.7
         colour Tan
      }
   }
}

#declare
BluePencil = composite {
   object {
      intersection {
         quadric { Cylinder_Y scale <0.5 1.0 0.5> }
         plane { <0.0 1.0 0.0> 30.0 }
         plane { <0.0 1.0 0.0> 0.0 inverse }
      }
      texture {
         0.05
         ambient 0.3
         diffuse 0.7
         colour blue 1.0
      }
   }
   object {
      intersection {
         quadric { Cylinder_Y scale <0.5 1.0 0.5> }
         plane { <0.0 1.0 0.0> 32.0 }
         plane { <0.0 1.0 0.0> 30.0 inverse }
      }
      texture {
         0.05
         ambient 0.3
         diffuse 0.7
         colour Tan
      }
   }
}

camera {
   location <0.0 40.0 -150.0>
   up <0.0 1.0 0.0>
   right <1.3333 0.0 0.0>
   look_at <0.0 0.0 0.0>
}

/*The back wall*/

object {
   plane { <0.0 0.0 1.0> 200.0 }
   texture {
      0.1
      ambient 0.3
      diffuse 0.7
      colour LightGray
   }
   colour LightGray
}

/*The ceiling*/
object {
   plane { <0.0 1.0 0.0> 500.0 }
   texture {
      0.1
      ambient 0.3
      diffuse 0.7
      colour White
   }
}


/*The desk surface*/

object {
   intersection {
      plane { <0.0 1.0 0.0> 2.0 }
      plane { <0.0 1.0 0.0> -2.0 inverse }
      plane { <0.0 0.0 1.0> 100.0 }
      plane { <0.0 0.0 1.0> -100.0 inverse }
      plane { <1.0 0.0 0.0> 125.0 }
      plane { <1.0 0.0 0.0> -125.0 inverse }
   }
   translate <0.0 -20.0 0.0>
   texture {
      0.05
      ambient 0.4
      diffuse 0.6
      Dark_Wood
      scale <5.0 1.0 1.0>
      reflection 0.2
      brilliance 3.0
   }
}

/*Paperwork*/
object {
   union {
      triangle {
         <0.0 0.0 0.0>
         <8.5 0.0 0.0>
         <0.0 0.0 -11.0>
      }
      triangle {
         <0.0 0.0 -11.0>
         <8.5 0.0 -11.0>
         <8.5 0.0 0.0>
      }
   }
   scale <3.0 1.0 3.0>
   rotate <0.0 -30.0 0.0>
   translate <-20.0 -17.9 -40.0>

   texture {
      0.05
      ambient 0.1
      diffuse 0.4
      colour red 0.5 green 0.5 blue 0.3
   }
   colour red 0.5 green 0.5 blue 0.3
}

/*a glass paperweight*/
object {
   intersection {
      sphere { <0.0 -5.0 0.0> 10.0 }
      plane { <0.0 1.0 0.0> 0.0 inverse }
   }
   translate <0.0 -17.8 -35.0>
   texture {
      ambient 0.1
      diffuse 0.6
      colour red 0.3 green 0.5 blue 0.3 alpha 1.0
      reflection 0.1
      refraction 1.0
      ior 1.5
      brilliance 2.0
   }
}

/*The desk lamp*/

composite {
   object {
      intersection {
         quadric { Cylinder_Y scale <3.0 1.0 3.0> }
         plane { <0.0 1.0 0.0> 40.0 }
         plane { <0.0 1.0 0.0> -18.0 inverse }
      }
      texture {
         Shiny
         0.05
         ambient 0.2
         diffuse 0.7
         colour red 0.7 green 0.6 blue 0.1
      }
   }

   object {
      intersection {
         quadric { Cylinder_Y scale <25.0 1.0 25.0> }
         plane { <0.0 1.0 0.0> 2.0 }
         plane { <0.0 1.0 0.0> -2.0 inverse }
      }
      translate <0.0 -16.0 -5.0>
      texture {
         0.05
         ambient 0.3
         diffuse 0.6
         colour red 0.5 green 0.4 blue 0.1
         reflection 0.4
         brilliance 4.0
         bumps 0.1
      }
   }

   object {
      intersection {
         quadric { Cylinder_X scale <1.0 10.0 10.0> }
         quadric { Cylinder_X scale <1.0 9.95 9.95> inverse }
         plane { <0.0 1.0 0.0> 0.0 inverse }
         plane { <1.0 0.0 0.0> -30.0 inverse }
         plane { <1.0 0.0 0.0> 30.0 }
      }
      translate <0.0 35.0 -13.0>
      texture {
         Shiny
         0.05
         ambient 0.5
         diffuse 0.5
         colour DarkGreen 
         reflection 0.3
         brilliance 4.0
      }
   }

   object {
      union {
         intersection {
            sphere { <-30.0 35.0 -13.0> 10.0 }
            sphere { <-30.0 35.0 -13.0> 9.95 inverse }
            plane { <0.0 1.0 0.0> 35.0 inverse }
            plane { <1.0 0.0 0.0> -30.0 }
         }
         intersection {
            plane { <0.0 1.0 0.0> 35.0 inverse }
            plane { <1.0 0.0 0.0> 30.0 inverse }
            sphere { <30.0 35.0 -13.0> 10.0 }
            sphere { <30.0 35.0 -13.0> 9.95 inverse }
         }
      }
      texture {
         Shiny
         0.05
         ambient 0.2
         diffuse 0.7
         colour red 0.7 green 0.6 blue 0.1
      }
   }


   rotate <0.0 35.0 0.0>
   translate <50.0 0.0 30.0>

}

/*The fluorescent tube inside the lamp*/

object {
   union {
      intersection {
         quadric { Cylinder_X }
         plane { <1.0 0.0 0.0> -25.0 inverse }
         plane { <1.0 0.0 0.0>  25.0 }
      }
      light_source { <0 0 0> color White }
   }
   translate <0.0 43.0 -10.0>
   rotate <0.0 35.0 0.0>
   translate <50.0 0.0 30.0>
}

/*The Picture itself*/

composite {
   object {
      intersection {
         plane { <1.0 0.0 0.0> 1.0 }
         plane { <1.0 0.0 0.0> -1.0 inverse }
         plane { <0.0 1.0 0.0> 1.0 }
         plane { <0.0 1.0 0.0> -1.0 inverse }
         plane { <0.0 0.0 1.0> 1.0 }
         plane { <0.0 0.0 1.0> -1.0 inverse }
      }
      translate <1.0 1.0 1.0>
      scale <20.0 15.0 1.0>
      texture {
         ambient 0.05
         diffuse 0.9
         image_map { <1.0 -1.0 0.0> gif "rough.gif" once 
            interpolate 2.0
         }
         scale <40.0 30.0 1.0>
         scale <1.5 1.5 1.0>
      }
   }

   /* The picture frame */
   object {
      union {
         intersection {
            union {
               quadric { Cylinder_Y scale <1.0 1.0 1.0> translate <41.0 0.0 0.0> }
               quadric { Cylinder_Y scale <1.0 1.0 1.0> translate <-1.0 0.0 0.0> }
            }
            plane { <0.0 1.0 0.0> 31.0 }
            plane { <0.0 1.0 0.0> -1.0 inverse }
         }
         intersection {
            union {
               quadric { Cylinder_X scale <1.0 1.0 1.0> translate <0.0 31.0 0.0> }
               quadric { Cylinder_X scale <1.0 1.0 1.0> translate <0.0 -1.0 0.0> }
            }
            plane { <1.0 0.0 0.0> 41.0 }
            plane { <1.0 0.0 0.0> -1.0 inverse }
         }
         sphere { <-1.0 -1.0 0.0> 1.0 }
         sphere { <-1.0 31.0 0.0> 1.0 }
         sphere { <41.0 -1.0 0.0> 1.0 }
         sphere { <41.0 31.0 0.0> 1.0 }
      }
      texture {
         0.05
         Shiny
         colour red 0.6 green 0.5 blue 0.1
         ambient 0.3
         diffuse 0.7
      }
      colour red 0.6 green 0.5 blue 0.1
   }

   scale <1.5 1.5 1.5>
   rotate <10.0 -35.0 0.0>
   translate <-65.0 -15.0 -25.0>

}

/*The pencil holder*/
composite {
   object {
      intersection {
         quadric { Cylinder_Y scale <5.0 1.0 5.0> }
         quadric { Cylinder_Y scale <4.8 1.0 4.8> inverse }
         plane { <0.0 1.0 0.0> 0.0 inverse }
         plane { <0.0 1.0 0.0> 15.0 rotate <-45.0 0.0 0.0> }
      }
      texture {
         0.05
         Shiny
         ambient 0.3
         diffuse 0.7
         colour red 0.7 green 0.6 blue 0.1
         reflection 0.4
         brilliance 6.0
      }
   }
   composite { RedPencil
      rotate <0.0 0.0 -2.0>
      translate <1.0 0.0 1.0>
   }
   composite { GreenPencil
      rotate <0.0 0.0 2.0>
      translate <-1.0 3.0 0.0>
   }
   composite { BluePencil
      rotate <-2.0 0.0 3.0>
      translate <0.0 -2.0 -1.0>
   }
   bounded_by {
      intersection {
         quadric { Cylinder_Y scale <6.0 1.0 6.0> }
         plane { <0.0 1.0 0.0> 36.0 }
         plane { <0.0 1.0 0.0> -4.0 inverse }
      }
   }

   rotate <0.0 45.0 0.0>
   translate <70.0 -18.0 -20.0>

}

/*The light source*/

object {
   light_source { <20.0 100.0 -200.0>
      colour White
   }
}
