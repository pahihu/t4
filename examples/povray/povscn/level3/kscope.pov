// Persistence Of Vision raytracer version 1.0 sample file.

// By Tom Price

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

camera {
   location <0.0 0.0 -100.0>
   up <0.0 1.0 0.0>
   right <1.3333 0.0 0.0>
   look_at <0.0 11.547005 0.0>
}

/*The Kaleidoscope Tube*/
composite {
   composite {
      object {
         union {
            triangle { <0.0  1.1547005 -100.0>
               <0.0  1.1547005  0.0>
               <-1.0 -0.5773502 0.0>
               inverse }

            triangle { <0.0  1.1547005 -100.0>
               <-1.0 -0.5773502 -100.0>
               <-1.0 -0.5773502 0.0>
               inverse }
         }
         texture { 0.05
            ambient 0.1
            diffuse 0.7
            colour White
            reflection 1.0
         }
      }

      object {
         union {
            triangle { <-0.001  1.1547005 -100.0>
               <-0.001  1.1547005  0.0>
               <1.0 -0.5773502  0.0>
               inverse }


            triangle { <-0.001  1.1547005 -100.0>
               <1.0 -0.5773502 -100.0>
               <1.0 -0.5773502 0.0>
               inverse }
         }
         texture { 0.05
            ambient 0.1
            diffuse 0.7
            colour White
            reflection 1.0
         }
      }

      object {
         union {
            triangle {
               <-1.0 -0.5773502 -100.0>
               <1.0  -0.5773502 -100.0>
               <-1.0 -0.5773502 0.0>
            }

            triangle {
               <1.0  -0.5773502 -100.0>
               <-1.0 -0.5773502 0.0>
               <1.0 -0.5773502 0.0>
            }
         }
         texture {
            0.05
            Bright_Blue_Sky
            ambient 0.5
            diffuse 0.5
         }
         colour Cyan
      }

      object {
         triangle { <-1.0 -0.5773502 0.0>
            <1.0 -0.5773502 0.0>
            <0.0  1.1547005  0.0>
         }
         colour red 0.5 green 0.5 blue 0.5 alpha 0.3
         texture {
            0.1
            ambient 0.3
            diffuse 0.7
            colour red 0.5 green 0.5 blue 0.5 alpha 0.3
         }
      }

      scale <10.0 10.0 1.0>
   }

   object {
      sphere { <-3.5 -3.0 -45.0> 3.0 }
      texture {
         0.05
         ambient 0.1
         diffuse 0.7
         colour Red alpha 0.8
         reflection 0.2 
         refraction 1.0
         ior 1.2
         brilliance 3.0
      }
   }

   object {
      sphere { <4.0 -3.0 -40.0> 2.5 }
      texture {
         0.05
         ambient 0.1
         diffuse 0.7
         colour Green alpha 0.8
         reflection 0.2 
         refraction 1.0
         ior 1.2
         brilliance 3.0
      }
   }

   object {
      sphere { <0.0 4.0 -30.0> 2.0 }
      texture {
         0.05
         ambient 0.1
         diffuse 0.7
         colour Magenta alpha 0.8
         reflection 0.2 
         refraction 1.0
         ior 1.2
         brilliance 3.0
      }
   }

   object {
      sphere { <0.0 -2.0 -20.0> 2.0 }
      texture {
         0.05
         ambient 0.1
         diffuse 0.7
         colour Yellow alpha 0.8
         reflection 0.2 
         refraction 1.0
         ior 1.2
         brilliance 3.0
      }
   }

   object {
      sphere { <2.0 -4.0 -70.0> 2.0 }
      texture { 0.05
         ambient 0.1
         diffuse 0.7
         colour Cyan alpha 0.8
         reflection 0.2 
         refraction 1.0
         ior 1.2
         brilliance 3.0
      }
   }

   object {
      triangle {
         <-1.0 -8.0 -50.0>
         < 0.0  1.0 -50.0>
         < 2.0 -3.0 -50.0>
      }

      texture {
         0.05
         ambient 0.3
         diffuse 0.7
         colour red 0.8 green 0.5 blue 0.3 alpha 0.9
      }
   }

   object {
      triangle {
         <1.0 7.0 -5.0>
         < -4.0  -1.0 -5.0>
         < 3.0 1.0 -5.0>
      }

      texture {
         0.05
         ambient 0.3
         diffuse 0.7
         colour red 0.2 green 0.9 blue 0.5 alpha 0.9
      }
   }

   object {
      triangle {
         <-8.0 -5.0 -80.0>
         < -4.0  -1.0 -80.0>
         < 0.0 -4.0 -80.0>
      }
      texture {
         0.05
         ambient 0.3
         diffuse 0.7
         colour red 0.7 green 0.7 blue 0.3 alpha 1.0
         refraction 0.9
         ior 1.0
      }
   }

   object {
      union {
         triangle {
            <0.0 0.0 0.0>
            <0.0 1.0 0.0>
            <1.0 0.0 0.0>
         }

         triangle {
            <1.0 0.0 0.0>
            <0.0 1.0 0.0>
            <1.0 1.0 0.0>
         }
      }
      rotate <20.0 45.0 -10.0>
      translate <-0.6 -0.5 -65.0>
      scale <10.0 10.0 1.0>
      texture {
         ambient 0.5
         diffuse 0.3  
         // Subsitute your own image here instead of sunset.ham
         //    image_map { <1.0 -1.0 0.0> iff "sunset.ham" once 
         //    interpolate 2.0
         //    }
         rotate <20.0 45.0 -10.0>
         translate <-0.6 -0.5 -65.0>
         scale <9.0 9.0 1.0>
         refraction 0.25
         ior 1.0
      }
   }

}

object {
   light_source { <0.0 0.0 20.0>
      colour White
   }
}
