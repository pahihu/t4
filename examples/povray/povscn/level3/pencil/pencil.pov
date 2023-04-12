// Persistence Of Vision Raytracer version 1.0 sample file.

// By Jorge Arreguin
/*
  NOTE: Requires PENCIL_.GIF (320x200x256)
  This scene file makes a pencil along y axis 
  with the label "Persistence Of Vision"

  By:  Jorge Arreguin

  Translation to English and dkb version 2.10 by Aaron a. Collins with a bit
  of help from David on the imagemap registration.

  note - unless this is rendered at a minimum of 640 x whatever, the logo
         is quite unreadable.

  The Pencil - major export of Faber, Pennsylvania, due to the plentiful
  presence of pencilwood trees and eraser-root plants, and the nearby
  graphite mines.
                                                (John Landis - Animal House)
*/

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare PencilPaint =
texture {
   color red 1.0 green 0.6666 blue 0.33333
}

camera {
   location <0.0 -70.0 0.0>
   direction <0.0 2.5 0.0>
   up <0.0 0.0 1.0>
   right <1.333333 0.0 0.0>
   rotate < -20.0 0.0 -140.0 >
   translate < 0.0 0.0 2.0 >
}

object {
   light_source { <30.0 30.0 55.0>
      colour White
   }
}

object {
   light_source { <-40.0 -40.0 35.0>
      colour Grey
   }
}


composite {

   /*----------------------------- parte metalica ----------------------*/
   /*                             (that metal part)                     */
   composite {
      object {
         union {
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > translate <0.0 0.0 2.0 > }
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > translate <0.0 0.0 1.0 > }
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > }
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > translate <0.0 0.0 -1.0 > }
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > translate <0.0 0.0 -2.0 > }
         }
         translate < 0.0 0.0 38.5 >
         texture {
            color Gold
            reflection 0.3
         }
      }

      object {
         union {
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > translate <0.0 0.0 2.0 > }
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > translate <0.0 0.0 1.0 > }
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > }
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > translate <0.0 0.0 -1.0 > }
            quadric { Ellipsoid scale < 4.0 4.0 1.5 > translate <0.0 0.0 -2.0 > }
         }
         translate < 0.0 0.0 30.5 >
         texture {
            color Gold
            reflection 0.3
         }
      }

      object {
         intersection { Disk_Z }
         scale < 3.5 3.5 3.0 >
         translate < 0.0 0.0 34.0 >
         texture {
            color Brown
            reflection 0.25
         }
      }

      bounded_by {
         sphere { <0.0 0.0 34.0> 9.0 }
      }
   }

   /*---------------------------------- borrador ----------------------------*/
   /*                (the rubber eraser, from eraser-root plants)            */

   composite {
      object {
         intersection { Disk_Z }
         scale < 3.5 3.5 3.0 >
         translate < 0.0 0.0 41.0 >
         texture {
            color Pink
         }
      }
      object { sphere { < 0.0 0.0 44.0 > 3.5 }
         texture {
            color Pink
         }
      }
   }

   /*---------------------------------- cuerpo de madera ---------------------*/
   /*                                 (body of pencilwood)                    */
   object {
      difference {
         intersection {
            plane { < 0.0 1.0 0.0 > 3.031
               texture { PencilPaint }
               texture {
                  image_map { <-1.0 0.0 1.0> gif "pencil_.gif" // 320x200
                     alpha 0 1.0      /* colour 0 is the "key" or transparent colour */
                     once
                  }
                  scale <15.0 1.0 15.0>
                  translate <-12.2 0.0 12.0 >
               }
               /* Now for the complicated image mapping:
  Currently, the pencil is standing up in the z axis:
                   z

             eraser  ^ *vp
                     | |   
                     | |
                     | |----/ y
                     |     /
                     |    /
                     |   /
                     |  /
                     | /
             point   |/
                     -------------->
                                   x

  We want the image to be mapped as follows (viewed from the +ve y axis):

          Right
          ---- z=28.0
          |  |
          |..|
          |..|
          |..|
          |ee|
          |ce|
      Top |ar| Bottom
          |rf|
          |t |
          | s|
          |b'|
          |Kt|
          |di|
          ---- z=12.7
          Left

          ^  ^
          |  |
    x=1.75   x=-1.75

 The image map above gives (viewed from the y axis):

                   ^ z
          1,1 Right|
             ------|
             |     |
             |     |
         Top |     |Bottom
             |     |
             |     |
         <----------
          x   Left     

  Which is at least in the right orientation.

  Now, we must scale the image so the letters are the right size. */

               //         scale <15.0 1.0 15.0>  /* Never use 0 for any scale value */

               /* Now, we align the top left of the picture to the proper point on the
  pencil.  This is a bit tricky because the top left of the picture is now
  at x=15 z=0.  We have to move it to about x=-12, z=12 because the letters
  don't start at the very top of the image. */

               //         translate <-12.20 0 12>

               /* As you may have guessed, this still took a lot of trial and error to get it
  right, but some analysis of the picture before hand saved a lot of time. */
               //      }
            }
            plane { < 0.0 1.0 0.0 > 3.031
               rotate < 0.0 0.0 60.0 >
               texture { PencilPaint }
            }
            plane { < 0.0 1.0 0.0 > 3.031
               rotate < 0.0 0.0 120.0 >
               texture { PencilPaint }
            }
            plane { < 0.0 1.0 0.0 > 3.031
               rotate < 0.0 0.0 180.0 >
               texture { PencilPaint }
            }
            plane { < 0.0 1.0 0.0 > 3.031
               rotate < 0.0 0.0 240.0 >
               texture { PencilPaint }
            }
            plane { < 0.0 1.0 0.0 > 3.031
               rotate < 0.0 0.0 300.0 >
               texture { PencilPaint }
            }
            plane { < 0.0 0.0 1.0 > 28.0 }
            plane { < 0.0 0.0 1.0 > 3.629 inverse }
         }
         quadric { QCone_Z inverse scale < 0.275558 0.275558 1.0 > }
      }
      texture {
         Pine_Wood
         turbulence 0.1
         scale <1.5 1.5 1.5>
         rotate <0.0 90.0 0.0>
         translate <30.0 0.0 0.0>
      }
   }

   /*---------------------------- punta de grafito ------------------------------*/
   /*                            (point of graphite)                             */

   object {
      intersection {
         quadric { QCone_Z scale < 0.275558 0.275558 1.0 > }
         plane { < 0.0 0.0 1.0 > 3.629 }
         plane { < 0.0 0.0 1.0 > 0.001 inverse }
      }
      texture {
         color Black
         reflection 0.25
         phong 1.0
         phong_size 20
      }
   }

   rotate < 90.0 0.0 0.0 >
   translate < 0.0 22.0 3.5 >
}

/*-------------------------- plano de horizonte --------------------------*/
/*                           (plane of horizon)                           */

object {
   plane { < 0.0 0.0 1.0 > 0.0 }
   texture {
      color Green
      reflection 0.25
   }
}
