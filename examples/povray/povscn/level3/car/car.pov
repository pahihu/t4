// Persistence Of Vision raytracer version 1.0 sample file.

// By Jorge Arreguin

/*
 car.dat  POV-Ray Scene             February 27, '91

 By Jorge Arreguin
    i. Allende # 611
    Cortazar , Gto.
    Mexico  cp 38300
    
    Renders a Futuristic Car, making a diferent use of the imagemap
    NOTES: Requires "car_inc.gif" for imagemap. 540x350x16 color
    May require protected mode version of POV-Ray to run.  If you can't
    run POV-Ray, and you've already got all TSR's removed, then try
    scaling the imagemap down using PICLAB and changing the scaling in
    the texture block accordingly.  The results won't be as nice, but
    at least you'll be able to render it!

    To reduce by 1/2 from PICLAB:

            gload "car_inc.gif"   //The windshield glass
            unmap
            rescale 0.5
            map
            gsave "car_inc.gif"

     Then, in the texture block, immediatly after the image_map block,
     add a scale "<2 2 2>" command.


    This image is designed using "right-hand" coordinates... z = height
    and y = depth.

*/

/*-----------------------------declaraciones------------------------*/

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare JA_Hyperboloid = quadric {
   <  1.0  -1.0  1.0>
   <  0.0   0.0  0.0>
   <  0.0   0.0  0.0>
   -0.6
}

#declare Near_Black = colour red 0.1 green 0.1 blue 0.1

#declare Llanta = intersection {
   quadric { Ellipsoid scale < 1.0 1.0 1.0 > }
   quadric { JA_Hyperboloid scale < 0.7 0.3 0.7 > inverse }
}

#declare Polvera = object {
   quadric { Ellipsoid scale < 0.8 0.2 0.8 > }
   texture {
      color Gray
      reflection 0.3
      phong 0.8
      phong_size 10.0
   }
   color Gray
}

camera {
   location <0.0  -33.0  0.0>
   direction <0.0 2.0  0.0>
   up  <0.0  0.0  1.0>
   right <1.33333 0.0 0.0>
   rotate < -30.0 0.0 30.0 >
}

object {
   light_source { <-10.0  -30.0  50.0>
      colour White
   }
}

object {  
   light_source { <40.0  -40.0  45.0>
      colour White
   }
}

/*--------------------------------------- cuerpo --------------------*/

object {
   intersection {
      quadric {
         Ellipsoid
         scale < 10.0 10.0 4.0 >
      }
      plane { < 1.0 0.0 0.0 > 0.0
         rotate < 0.0 -16.2379 0.0 >
         translate < -5.0 0.0 0.0 >
         inverse
      }
      plane { < 0.0 0.0 1.0 > 0.01 }
      plane { < 0.0 0.0 1.0 > 0.005 inverse }
      quadric {
         Cylinder_X
         scale < 1.0 5.0 5.0 >
      }
   }
   bounded_by { sphere { < 0.0 0.0 0.0 > 10.0 } }
   texture {
      color Blue
      reflection 0.4
   }
   color Blue
}

object {
   union {
      intersection {
         quadric {
            Ellipsoid
            scale < 10.0 10.0 4.0 >
         }
         plane { < 1.0 0.0 0.0 > 0.0
            rotate < 0.0 -16.2379 0.0 >
            translate < -5.0 0.0 0.0 >
            inverse
         }
         plane { < 0.0 0.0 1.0 > 0.01 inverse }
         quadric {
            Cylinder_X
            scale < 1.0 5.0 5.0 >
         }
      }
      intersection {
         quadric {
            Ellipsoid
            scale < 3.352 8.380 3.352 >
            rotate < 0.0 -16.2379 0.0 >
            translate < -5.0 0.0 0.0 >
         }
         plane { < 1.0 0.0 0.0 > 0.0
            rotate < 0.0 -16.2379 0.0 >
            translate < -5.0 0.0 0.0 >
         }
         plane { < 1.0 0.0 0.0 > 0.0
            rotate < 0.0 -64.0 0.0 >
            translate < -5.0 0.0 0.0 >
            inverse
         }
         plane { < 0.0 0.0 1.0 > 0.01 inverse }
         quadric {
            Cylinder_X
            scale < 1.0 5.0 5.0 >
         }
      }
   }
   bounded_by { sphere { < 0.0 0.0 0.0 > 10.0 } }
   texture {
      image_map { < 1.0 -1.0 0.0 > gif "car_inc.gif"
         interpolate 2.0
         once 
      }
      scale < 18.6 10.0 1.0 >
      translate < -8.38 -5.0 0.0 >
      phong 0.8
      phong_size 20.0
      reflection 0.4
   }
   color Blue
}

/*------------------------------- llantas ------------------------------*/

object {
   intersection {
      Llanta
      scale < 1.907 1.0 1.907 >
      translate < -4.0 -5.0 0.907 >
   }
   texture {
      color Near_Black
      phong 0.8
      phong_size 40.0
   }
   color Near_Black
}

object {
   intersection {
      Llanta
      scale < 1.907 1.0 1.907 >
      translate < -4.0 5.0 0.907 >
   }
   texture {
      color Near_Black
      phong 0.8
      phong_size 40.0
   }
   color Near_Black
}

object {
   intersection {
      Llanta
      scale < 1.435 1.0 1.435 >
      translate < 6.5 -5.0 0.435 >
   }
   texture {
      color Near_Black
      phong 0.8
      phong_size 40.0
   }
   color Near_Black
}


object {
   intersection {
      Llanta
      scale < 1.435 1.0 1.435 >
      translate < 6.5 5.0 0.435 >
   }
   texture {
      color Near_Black
      phong 0.8
      phong_size 40.0
   }
   color Near_Black
}

/*---------------------------- polveras -------------------------*/

object {
   Polvera
   scale < 1.3 1.0 1.3 >
   translate < 6.5 5.0 0.435 >
}

object {
   Polvera
   scale < 1.3 1.0 1.3 >
   translate < 6.5 -5.0 0.435 >
}

object {
   Polvera
   scale < 1.8 1.0 1.8 >
   translate < -4.0 5.0 0.907 >
}

object {
   Polvera
   scale < 1.8 1.0 1.8 >
   translate < -4.0 -5.0 0.907 >
}

/*------------------------------ piso y bardas -----------------------------*/

object {
   plane { < 0.0 0.0 -1.0 > 1.0 }
   texture {
      checker color Brown color Khaki
      rotate < -90.0 0.0 0.0 >
      reflection 0.5
   }
   color Maroon
}

object {
   plane { < -1.0 0.0 0.0 > 11.0 }
   texture {
      color Maroon
      reflection 0.5
   }
   color Maroon
}

object {
   plane { < 0.0 1.0 0.0 > 7.0 }
   texture {
      color Maroon
      reflection 0.5
   }
   color Maroon
}
