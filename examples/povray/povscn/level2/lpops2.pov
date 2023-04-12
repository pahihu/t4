// Persistence Of Vision raytracer version 1.0 sample file.

// By Tom Price



#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0 35.0 -150.0>
   up <0.0 1.0 0.0>
   right <1.3333 0.0 0.0>
   look_at <0.0 0.0 0.0>
}

fog {
   colour red 0.5 green 0.5 blue 0.5
   1000.0
}

/*The floor*/
object {
   plane { <0.0 1.0 0.0> -60.0 }
   texture { 
      0.05 
      checker colour Brown colour LightGray
      scale <40.0 40.0 40.0>
      translate <-5.0 0.0 0.0>
      ambient 0.2
      diffuse 0.7
      reflection 0.3
      phong 1.0
      phong_size 20.0
   }
   colour red 0.4 blue 0.4 green 0.4
}

/*The Sky*/
object {
   sphere { <0.0 -40000.0 0.0> 50000.0 }
   texture { 
      0.05 
      Bright_Blue_Sky
      scale <6000.0 3000.0 3000.0>
      rotate <0.0 -30.0 0.0>
      ambient 0.7
      diffuse 0.0
   }
   colour Blue
}

composite {
   object {
      intersection {
         sphere { <0.0 0.0 0.0> 40.0 }
         quadric { Cylinder_Z scale <20.0 20.0 1.0> inverse }
      }
      texture {
         0.05
         ambient 0.1
         diffuse 0.7
         colour red 0.6 green 0.6 blue 0.0 
         reflection 0.7
         brilliance 6.0
         specular 0.5
      }
   }

   composite {
      object {
         triangle {
            <-1.0 0.0 -0.5773502>
            < 1.0 0.0 -0.5773502>
            < 0.0 0.0  1.1547005>
         }
         texture {
            0.05
            colour Gold
            ambient 0.1
            diffuse 0.7
            reflection 0.4
            brilliance 6.0
            specular 0.5
         }
      }

      object {
         triangle {
            <-1.0 0.0 -0.5773502>
            < 0.0 0.0  1.1547005>
            < 0.0 1.6329931  0.0>
         }
         texture {
            0.05
            colour Gold
            ambient 0.1
            diffuse 0.7
            reflection 0.4
            brilliance 6.0
            specular 0.5
         }
      }

      object {
         triangle {
            < 1.0 0.0 -0.5773502>
            < 0.0 0.0  1.1547005>
            < 0.0 1.6329931  0.0>
         }
         texture {
            0.05
            colour Gold
            ambient 0.1
            diffuse 0.7
            reflection 0.4
            brilliance 6.0
            specular 0.5
         }
      }

      object {
         triangle {
            <-1.0 0.0 -0.5773502>
            < 1.0 0.0 -0.5773502>
            < 0.0 1.6329931  0.0>
         }
         texture {
            0.05
            colour red 0.5 green 0.4 blue 0.0
            ambient 0.4
            diffuse 0.6
            reflection 0.2
            brilliance 6.0
         }
      }

      translate <0.0 -0.4082886 0.0>
      scale <15.0 15.0 15.0>
      rotate <-45.0 -50.0 10.0>
   }

   object {
      intersection {
         quadric { Cylinder_Y scale <7.0 1.0 7.0> }
         plane { <0.0 1.0 0.0> -20.0 }
      }
      texture {
         0.05
         Pine_Wood
         scale <10.0 100.0 10.0> 
         ambient 0.1
         diffuse 0.7
         reflection 0.5
         specular 0.5
      }
   }
   translate <10.0 20.0 60.0>
}

composite {
   object {
      sphere { <0.0 0.0 0.0> 20.0 }
      texture {
         Glass
         0.05
      }
   }          

   object {
      intersection {
         quadric { Cylinder_Y scale <3.0 1.0 3.0> }
         plane { <0.0 1.0 0.0> 0.0 }
      }
      texture {
         0.05
         Pine_Wood
         scale <10.0 50.0 10.0>  
         ambient 0.1
         diffuse 0.7
         reflection 0.5
         specular 0.5
      }
   }
   translate <-40.0 -5.0 -0.0>
}

composite {
   object {
      sphere { <0.0 0.0 0.0> 15.0 }
      texture {
         0.05
         colour red 0.0 green 0.6 blue 0.3 
         ambient 0.1
         diffuse 0.5
         reflection 0.5
         brilliance 3.0
         specular 0.1
      }
   }

   object {
      intersection {
         quadric { Cylinder_Y scale <3.0 1.0 3.0> }
         plane { <0.0 1.0 0.0> 0.0 }
      }
      texture {
         0.05
         Pine_Wood
         scale <10.0 100.0 10.0> 
         ambient 0.1
         diffuse 0.7
         reflection 0.5
         specular 0.5
      }
   }
   translate <50.0 10.0 -30>
}

object {
   sphere { <250.0 25.0 350.0> 30.0 }
   texture {
      Glass
      0.05
   }
}      

composite {
   object {
      sphere { <0.0 0.0 0.0> 40.0 }
      texture { 
         /*         imagemap <1.0 -1.0 0.0> gif "city01.gif" once */
         color Blue
         scale <75.0 75.0 75.0>
         translate <-127.5 -7.5 100.0> 
         ambient 0.1
         diffuse 0.5
         colour red 0.5 green 0.5 blue 0.5
         reflection 0.2
         /*         refraction 0.4*/
         /*         ior 1.2       */
         phong 0.1
         phong_size 10
      }
   }          

   object {
      intersection {
         quadric { Cylinder_Y scale <6.0 1.0 6.0> }
         plane { <0.0 1.0 0.0> -20.0 }
      }
      texture {
         0.05
         Pine_Wood
         scale <10.0 50.0 10.0>        
         ambient 0.1
         diffuse 0.7
         reflection 0.5
         specular 0.5
      }
   }
   translate <-100.0 30.0 100.0>
}

object {
   light_source { <-100.0 100.0 -200.0>
      colour White
   }
}

