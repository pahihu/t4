// Persistence Of Vision raytracer version 1.0 sample file.

// By Jorge Arreguin

/* This would have won the Intl. Obfuscated Raytrace Code Contest, So I */
/* reformatted it while converting it :-)  All kidding */
/* aside, nice job, Jorge!  -  Aaron A. Collins */
// Note: This file is designed in "right-handed" coordinates: z=height y=depth
// Anybody up to adding bounding shapes??
// NOTE: This file has "Disk" primitives that may be different than those
// in shapes.inc!

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

camera {
   location <0.0 -80.0 0.0>
   direction <0.0 1.0 0.0>
   up <0.0 0.0 1.0>
   right <1.333333 0.0 0.0>
   translate < 25.0 0.0 5.0 >
   rotate < -30.0 0.0 30.0 >
}

object {
   light_source { <44.514 0.0 13.5>
      color White
   }
}

object {
   light_source { <-60.514 0.0 160.5>
      color red 0.7 green 0.7 blue 0.7
   }
}

object {
   sphere { < 0.0 0.0 0.0 > 5.0 }
   texture { Brass_Texture }
}

object {
   sphere { < 38.161 0.0 17.197 > 1.0 }
   texture { Brass_Texture }
}

object {
   sphere { < 9.307 0.0 30.288 > 1.0 }
   texture { Brass_Texture }
}

object {
   intersection { Disk_X }
   scale < 31.685 1.0 1.0 >
   rotate < 0.0 -72.9193 0.0 >
   texture { Brass_Texture }
}

object { intersection { Disk_X }
   scale < 31.685 1.0 1.0 >
   rotate < 0.0 -155.5969 0.0 >
   translate < 38.161 0.0 17.197 >
   texture { Brass_Texture }
}

object {
   intersection { Disk_X }
   scale < 3.2 0.5 0.5 >
   translate < 38.161 0.0 17.197 >
   texture { Brass_Texture }
}

object {
   intersection {
      quadric {
         QCone_Z
         scale < 1.0 1.0 1.0 >
         translate < 44.514 0.0 17.685 >
      }
      quadric {
         QCone_Z
         scale < 1.0 1.0 1.0 >
         translate < 44.514 0.0 16.685 >
         inverse
      }
      plane { < 0.0 0.0 1.0 > 7.685 inverse }
      plane { < 0.0 0.0 1.0 > 13.695 }
   }
   texture { Brass_Texture }
}

object {
   intersection {
      quadric {
         QCone_Z
         scale < 0.2 0.2 1.0 >
         translate < 44.514 0.0 33.496 >
      }
      quadric {
         QCone_Z
         scale < 0.2 0.2 1.0 >
         translate < 44.514 0.0 32.496 >
         inverse
      }
      plane { < 0.0 0.0 1.0 > 13.695 inverse }
      plane { < 0.0 0.0 1.0 > 23.496 }
   }
   texture { Brass_Texture }
}

object {
   plane { < 0.0 0.0 1.0 > -4.0 }
   texture {
      color Red
      ambient 0.4
      diffuse 0.6
      brilliance 8.0
      reflection 0.8
      phong 1.0
      phong_size 30.0
   }
}
