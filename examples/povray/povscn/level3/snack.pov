// Persistence Of Vision raytracer version 1.0 sample file.

// By Tom Price

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0 50.0 -150.0>
   up <0.0 1.0 0.0>
   right <1.3333 0.0 0.0>
   look_at <0.0 0.0 0.0>
}


object {
   light_source { <50.0 150.0 -250.0>
      colour White
   }
}

object {
   light_source { <-50.0 150.0 -250.0>
      colour White
   }
}


/*The Sky*/
object {
   sphere { <0.0 -49000.0 -200.0> 50000.0 inverse }
   texture { 
      Bright_Blue_Sky
      0.05 
      scale <5000.0 1000.0 5000.0>
      ambient 0.7
      diffuse 0.0
   }
   colour SkyBlue
}

/* The wood grain tabletop */
object {
   intersection {
      plane { <1.0 0.0 0.0> 1.0 }
      plane { <1.0 0.0 0.0> -1.0 inverse }
      plane { <0.0 1.0 0.0> 1.0 }
      plane { <0.0 1.0 0.0> -1.0 inverse }
      plane { <0.0 0.0 1.0> 1.0 }
      plane { <0.0 0.0 1.0> -1.0 inverse }
   }
   texture {
      Pine_Wood
      scale <7.0 1.0 0.7>
      rotate <0.0 -30.0 0.0>
      ambient 0.1
      diffuse 0.5
      reflection 0.3
      brilliance 3.0
   }
   bounded_by {
      intersection {
         plane { <1.0 0.0 0.0> 1.01 }
         plane { <1.0 0.0 0.0> -1.01 inverse }
         plane { <0.0 1.0 0.0> 1.01 }
         plane { <0.0 1.0 0.0> -1.01 inverse }
         plane { <0.0 0.0 1.0> 1.01 }
         plane { <0.0 0.0 1.0> -1.01 inverse }
      }
   }
   scale <200.0 1.0 200.0>
   translate <0.0 -42.0 0.0>
}


/* a salami */
#declare
Salami = composite {
   /* the outside skin */
   object {
      union {
         intersection {
            quadric { Cylinder_X scale <1.0 20.0 20.0> }
            plane { <1.0 0.0 0.0> 10.0 }
            plane { <1.0 0.0 0.0> -10.0 inverse }
         }
         sphere { <-10.0 0.0 0.0> 20.0 }
      }
      texture {
         0.05
         colour red 0.5 green 0.2 blue 0.2
         ambient 0.1
         diffuse 0.8
         reflection 0.2
         brilliance 3.0
         phong 0.3
         phong_size 20.0
      }
      colour red 0.5 green 0.2 blue 0.2
   }
   object {
      intersection {
         quadric { Cylinder_X scale <1.0 19.0 19.0> }
         plane { <1.0 0.0 0.0> 10.01 }
         plane { <1.0 0.0 0.0> -10.0 inverse }
      }
      texture {
         0.1
         colour Pink
         ambient 0.3
         diffuse 0.7
      }
      colour Pink
   }
   bounded_by {
      intersection {
         quadric { Cylinder_X scale <1.0 20.01 20.01> }
         plane { <1.0 0.0 0.0> 10.02 }
         plane { <1.0 0.0 0.0> -30.01 inverse }
      }
   }
}

/* a salami slice */
#declare
Slice = composite {
   /* the outside skin */
   object {
      intersection {
         quadric { Cylinder_X scale <1.0 20.0 20.0> }
         plane { <1.0 0.0 0.0> 0.5 }
         plane { <1.0 0.0 0.0> -0.5 inverse }
      }
      texture {
         0.05
         colour red 0.5 green 0.2 blue 0.2
         ambient 0.1
         diffuse 0.8
         reflection 0.2
         brilliance 3.0
         phong 0.3
         phong_size 20.0
      }
      colour red 0.5 green 0.2 blue 0.2
   }
   object {
      intersection {
         quadric { Cylinder_X scale <1.0 19.0 19.0> }
         plane { <1.0 0.0 0.0> 0.51 }
         plane { <1.0 0.0 0.0> -0.51 inverse }
      }
      texture {
         0.1
         ambient 0.3
         diffuse 0.7
         colour Pink
      }
      colour Pink
   }
   bounded_by {
      intersection {
         quadric { Cylinder_X scale <1.0 20.01 20.01> }
         plane { <1.0 0.0 0.0> 0.52 }
         plane { <1.0 0.0 0.0> -0.52 inverse }
      }
   }
}


/* a wedge of cheese*/
#declare
Cheese = object {
   intersection {
      quadric { Cylinder_Y scale <10.0 1.0 10.0> }
      plane { <0.0 1.0 0.0> 20.0 }
      plane { <0.0 1.0 0.0> 0.0 inverse }
      plane { <1.0 0.0 0.0>  5.0 rotate <0.0 30.0 0.0> }
      plane { <1.0 0.0 0.0> -5.0 rotate <0.0 -30.0 0.0> inverse }
      quadric { Cylinder_X scale <1.0 1.0 1.5>
         rotate <0.0 -20.0 30>
         translate <0.0 10.0 0.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 2.0 1.5>
         rotate <0.0 20.0 -30>
         translate <0.0 12.0 -4.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 1.0 1.0>
         translate <0.0 15.0 -9.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 2.0 1.5>
         rotate <0.0 -30.0 -30.0>
         translate <0.0 15.0 5.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 1.5 1.5>
         rotate <0.0 0.0 -20.0>
         translate <0.0 7.0 -9.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 1.0 1.0>
         rotate <0.0 10.0 10.0>
         translate <0.0 10.0 -2.0>
         inverse }
      quadric { Cylinder_X scale <1.0 1.0 1.2>
         rotate <0.0 -10.0 0.0>
         translate <0.0 5.0 0.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 1.3 1.0>
         translate <0.0 3.0 -3.0>
         inverse
      }
   }
   texture {
      0.05
      colour red 1.0 green 0.8 blue 0.0
      ambient 0.2
      diffuse 0.8
   }
   colour red 1.0 green 0.8 blue 0.0
   bounded_by {
      intersection {
         quadric { Cylinder_Y scale <10.01 1.0 10.01> }
         plane { <0.0 1.0 0.0> 20.01 }
         plane { <0.0 1.0 0.0> -0.01 inverse }
      }
   }
}

/* a slice of cheese*/
#declare
CheeseSlice = object {
   intersection {
      quadric { Cylinder_Y scale <10.0 1.0 10.0> }
      plane { <0.0 1.0 0.0> 20.0 }
      plane { <0.0 1.0 0.0> 0.0 inverse }
      plane { <1.0 0.0 0.0>  0.2  }
      plane { <1.0 0.0 0.0> -0.2 inverse }
      quadric { Cylinder_X scale <1.0 1.0 1.5>
         rotate <0.0 -20.0 30>
         translate <0.0 10.0 0.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 2.0 1.5>
         rotate <0.0 20.0 -30>
         translate <0.0 12.0 -4.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 1.0 1.0>
         translate <0.0 15.0 -9.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 2.0 1.5>
         rotate <0.0 -30.0 -30.0>
         translate <0.0 15.0 5.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 1.5 1.5>
         rotate <0.0 0.0 -20.0>
         translate <0.0 7.0 -9.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 1.0 1.0>
         rotate <0.0 10.0 10.0>
         translate <0.0 10.0 -2.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 1.0 1.2>
         rotate <0.0 -10.0 0.0>
         translate <0.0 3.0 5.0>
         inverse
      }
      quadric { Cylinder_X scale <1.0 1.3 1.0>
         rotate <0.0 0.0 0.0>
         translate <0.0 2.0 2.0>
         inverse
      }
   }
   texture {
      0.05
      colour red 1.0 green 0.8 blue 0.0
      ambient 0.2
      diffuse 0.8
   }
   colour red 1.0 green 0.8 blue 0.0
   bounded_by {
      intersection {
         quadric { Cylinder_Y scale <10.01 1.0 10.01> }
         plane { <0.0 1.0 0.0> 20.01 }
         plane { <0.0 1.0 0.0> -0.01 inverse }
         plane { <1.0 0.0 0.0> 0.3 }
         plane { <1.0 0.0 0.0> -0.3 inverse }
      }
   }
}

/* An oval glass dish */
#declare
Dish = object {
   union {
      intersection {
         quadric { Ellipsoid scale <100.0 25.0 25.0> }
         quadric { Ellipsoid scale <95.0 24.0 24.0> inverse }
         plane { <0.0 1.0 0.0> 0.0 }
         plane { <0.0 1.0 0.0> -5.0 inverse }
      }
      intersection {
         quadric { Cylinder_Y scale <95.0 1.0 24.0> }
         plane { <0.0 1.0 0.0> -4.0 }
         plane { <0.0 1.0 0.0> -5.0 inverse }
      }
   }
   texture {
      colour Clear
      ambient 0.1
      diffuse 0.8
      refraction 0.95
      ior 1.5
      reflection 0.05
      brilliance 2.0
   }
   colour Clear
   bounded_by {
      intersection {
         quadric { Cylinder_Y scale <100.01 1.0 25.01> }
         plane { <0.0 1.0 0.0> 0.01 }
         plane { <0.0 1.0 0.0> -5.01 inverse }
      }
   }
}

/* a slice of bread */
#declare
BreadSlice = composite {
   object {
      union {
         intersection {
            plane { <1.0 0.0 0.0> 10.0 }
            plane { <1.0 0.0 0.0> -10.0 inverse }
            plane { <0.0 1.0 0.0> 1.0 }
            plane { <0.0 1.0 0.0> -1.0 inverse }
            plane { <0.0 0.0 1.0> 10.0 }
            plane { <0.0 0.0 1.0> -10.0 inverse }
         }
         intersection {
            quadric { Cylinder_Y scale <11.0 1.0 7.0>
               translate <0.0 0.0 10.0>
            }
            plane { <0.0 1.0 0.0> 1.0 }
            plane { <0.0 1.0 0.0> -1.0 inverse }
         }
      }
      texture {
         0.05
         colour red 0.4 green 0.3 blue 0.1
         ambient 0.3
         diffuse 0.7
         reflection 0.1
         brilliance 2.0
      }
      colour red 0.4 green 0.3 blue 0.1
   }

   object {
      union {
         intersection {
            plane { <1.0 0.0 0.0> 9.5 }
            plane { <1.0 0.0 0.0> -9.5 inverse }
            plane { <0.0 1.0 0.0> 1.01 }
            plane { <0.0 1.0 0.0> -1.01 inverse }
            plane { <0.0 0.0 1.0> 9.5 }
            plane { <0.0 0.0 1.0> -9.5 inverse }
         }
         intersection {
            quadric { Cylinder_Y scale <10.5 1.0 6.5>
               translate <0.0 0.0 10.0>
            }
            plane { <0.0 1.0 0.0> 1.01 }
            plane { <0.0 1.0 0.0> -1.01 inverse }
         }
      }
      texture {
         0.1
         colour red 0.7 green 0.6 blue 0.45
         ambient 0.3
         diffuse 0.7
      }
      colour red 0.7 green 0.6 blue 0.45
   }
   bounded_by {
      intersection {
         plane { <1.0 0.0 0.0> 10.1 }
         plane { <1.0 0.0 0.0> -10.1 inverse }
         plane { <0.0 1.0 0.0> 1.0 }
         plane { <0.0 1.0 0.0> -1.1 inverse }
         plane { <0.0 0.0 1.0> 17.1 }
         plane { <0.0 0.0 1.0> -10.1 inverse }
      }
   }
}	

/* Now to put the scene together*/
composite {
   object { Dish
      scale <0.7 1.0 1.2>
      translate <0.0 -36.0 -25.0>
   }

   composite { Salami
      rotate <0.0 35.0 0.0>
      translate <-30.0 -20.0 -20.0>
   }

   object { Cheese
      scale <2.0 2.0 2.0>
      rotate <0.0 25.0 0.0>
      translate <30.0 -40.0 -25.0>
   }
   translate <0.0 0.0 25.0>
   rotate <0.0 -15.0 0.0>
   translate <-35.0 0.0 20.0>
}

/* now a sandwich */
composite {
   composite { Slice
      rotate <0.0 90.0 0.0>
      rotate <90.0 0.0 0.0>
      translate <45.0 -33.0 -35.0>
   }

   object { CheeseSlice
      scale <1.0 2.0 2.0>
      rotate <0.0 -90.0 0.0>
      rotate <90.0 0.0 0.0>
      translate <0.0 0.0 -16.0>
      rotate <0.0 30.0 0.0>
      translate <45.0 -35.0 -35.0>
   }

   composite { BreadSlice
      scale <1.6 2.0 1.0>
      rotate <0.0 -150.0 0.0>
      translate <45.0 -38.0 -35.0>
   }

   composite { BreadSlice
      scale <1.6 2.0 1.0>
      rotate <0.0 -150.0 0.0>
      translate <45.0 -29.0 -35.0>
   }
   translate <0.0 15.0 10.0>
}

/*spotlight on the sandwich*/
object {
   intersection {
      quadric { Cylinder_Y scale <7.5 1.0 7.5> }
      quadric { Cylinder_Y scale <7.4 1.0 7.4> inverse }
      plane { <0.0 1.0 0.0> 50.0 }
      plane { <0.0 1.0 0.0> 0.0 inverse }
   }
   translate <45.0 100.0 -25.0>
   texture {
      colour White
      ambient 0.3
      diffuse 0.7
   }
   colour White
}

object {
   light_source { <45.0 145.0 -25.0>
      colour White
   }
}

composite { Slice
   rotate <0.0 90.0 0.0>
   rotate <65.0 0.0 0.0>
   translate <-30.0 25.0 45.0>
}

composite { Slice
   rotate <0.0 90.0 0.0>
   rotate <60.0 0.0 0.0>
   translate <35.0 25.0 25.0>
}

object { CheeseSlice
   scale <1.0 2.0 2.0>
   rotate <0.0 -90.0 0.0>
   rotate <50.0 0.0 0.0>
   translate <-20.0 35.0 30.0>
}

object { CheeseSlice
   scale <1.0 2.0 2.0>
   rotate <0.0 -90.0 0.0>
   rotate <70.0 0.0 0.0>
   translate <65.0 15.0 35.0>
}

composite { BreadSlice
   scale <1.6 2.0 1.0>
   rotate <-40.0 -60.0 0.0>
   translate <-60.0 25.0 35.0>
}

composite { BreadSlice
   scale <1.6 2.0 1.0>
   rotate <60.0 50.0 0.0>
   translate <70.0 0.0 30.0>
}
