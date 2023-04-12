// Persistence Of Vision raytracer version 1.0 sample file.

// By Dan Farmer
//    Minneapolis, mn

//   skyvase.pov
//   Vase made with Hyperboloid and sphere {, sitting on a hexagonal
//   marble column.  Take note of the color and surface characteristics
//   of the gold band around the vase.  It seems to be a successful
//   combination for gold or brass.
//
// Contains a Disk_Y object which may have changed in shapes.dat


#include "shapes.inc"
#include "shapes2.inc"
#include "colors.inc"
#include "textures.inc"

#declare DMF_Hyperboloid = quadric {  /* Like Hyperboloid_Y, but more curvy */
   <1.0 -1.0  1.0>
   <0.0  0.0  0.0>
   <0.0  0.0  0.0>
   -0.5
}

camera {
   location <0.0  28.0  -200.0>
   direction <0.0 0.0  2.0>
   up  <0.0  1.0  0.0>
   right <1.33333 0.0 0.0>
   look_at <0.0  -12.0 0.0>
}

/* Light behind viewer postion (pseudo-ambient light) */
object {  
   light_source { <100.0  500.0  -500.0>
      colour White
   }
}

composite {
   object {

      union {
         intersection {
            plane { <0 1 0> 0.7 }
            quadric { DMF_Hyperboloid scale <0.75 1.25 0.75> }
            quadric { DMF_Hyperboloid scale <0.70 1.25 0.70> inverse }
            plane { <0.0 1.0 0.0> -1.0 inverse }
         }
         quadric { Ellipsoid scale <1.6 0.75 1.6 > translate <0 -1.15 0> }
      }

      scale <20 25 20>
      texture {
         Bright_Blue_Sky
         turbulence 0.3
         scale <8.0 4.0 4.0>
         rotate <0 0 15>
         ambient 0.1   diffuse 0.75
         phong 1 phong_size 100
         reflection 0.35
      }
      color Green
   }

   object {  /* Gold ridge around sphere portion of vase*/
      quadric { Ellipsoid 
         scale <1.6 0.75 1.6 >
         translate <0 -7.0 0>
         scale <20.5 4.0  20.5 >
      }
      texture {
         Metal
         color OldGold
      }
      color OldGold
   }

   bounded_by {
      intersection {
         Disk_Y
         translate <0.0 -0.5 0.0> // Remove for new Disk_Y definition
         scale <34 100 34>
      }
   }

}

object {  /* Stand for the vase */
   intersection { Hexagon
      rotate <0.0 0.0 -90.0>	/* Stand it on end (vertical)*/
      rotate<0.0 -45.0 0.0>	/* Turn it to a pleasing angle */
      scale<40 25 40>
      translate<0 -70 0>
   }
   texture {
      Sapphire_Agate
      scale <10.0 10.0 10.0>
      ambient 0.2
      diffuse 0.75
      reflection 0.85
   }
   color red 1.0          /*Color is only for debugging w/o texture*/
}

object {
   union {
      plane { <0 0 1> 50  rotate <0 -45 0> }
      plane { <0 0 1> 50  rotate <0 +45 0> }
   }
   texture {
      color DimGray
      ambient 0.2
      diffuse 0.75
      reflection 0.5
   }
   color Gray
}
