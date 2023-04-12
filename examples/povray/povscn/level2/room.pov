// Persistence Of Vision raytracer version 1.0 sample file.

// By Drew Wells

// Room.pov - Empty room created for cis Comart Raytracing Group Project

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

/* Camera Facing North*/
camera {
   direction <0.0 0.0  1.5>
   up  <0.0  1.0  0.0>
   right <1.333333 0.0 0.0>
   translate < 10.0  5.0  -30.0>
   look_at <0.0 5.0 0.0>
}

/*************************************************/
/* Define objects for use in scene               */
/* Your object should be DECLAREd here           */
/*************************************************/

#declare North_Wall = object { 
   plane { <0.0 0.0 1.0> 10.0 }
   texture { 
      granite
      scale <1.0 20.0 1.0>
   }
   color Yellow
}
/* South Wall commented out so camera can see in*/
#declare South_Wall = object {
   plane { <0.0 0.0 1.0> -10.0 }
   texture { 
      Cherry_Wood
      0.05
      scale <10.0 10.0 10.0>
   }
   color Red
}

/* Not visible with current viewpoint*/
/* But could be useful for light coming through window etc. */
#declare East_Wall = object {
   plane { <1.0 0.0 0.0> 15.0 }
   texture {
      Jade
      scale <10.0 10.0 10.0>
   }
   color Green  
}
#declare West_Wall = object {
   plane { <1.0 0.0 0.0> -15.0 }
   texture { 
      White_Wood
      0.05
      scale <15.0 20.0 3.0>
   }
   color Blue
}
#declare Ceiling = object {
   plane { <0.0 1.0 0.0> 15.0 }
   texture { 
      Red_Marble
      scale <10.0 10.0 10.0>
   }
   color White
} 
#declare Floor = object {
   plane { <0.0 1.0 0.0> 0.0 }
   texture { 
      Pine_Wood
      0.05
      scale <40.0 3.0 3.0>
   }
   color Violet
}

/*******************************************/
/* Scene description                       */
/*******************************************/
object {
   North_Wall
}
/* Comment out South Wall so camera can see in */
/*object
  South_Wall
end_object*/
object {
   East_Wall
}
object {
   West_Wall
}
object {
   Ceiling
}
object {
   Floor
}

/* Colored spheres to help visualize coordinates*/
object { /*North*/
   sphere { <0.0 7.0 9.0> 1.0 }
   texture { color Yellow }
   color Yellow
}
/* South sphere not visible in example gif*/
/*object {South}
 sphere <0.0 5.0 -10.0> 1.0 end_sphere
 texture color CRed end_texture
 color CRed
end_object*/
object { /*East*/
   sphere { <10.0 7.0 0.0> 1.0 }
   texture { color Green }
   color Green
}
object { /*West*/
   sphere { <-14.0 7.0 0.0> 1.0 }
   texture { color Blue }
   color Blue
}
object { /*Up*/
   sphere { <0.0 14.0 0.0> 1.0 }
   texture { color White }
   color White
}
object { /*Down*/
   sphere { <0.0 1.0 0.0> 1.0 }
   texture { color Violet }
   color Violet
}


/*Crude Spot light - feel free to add more light sources.*/
composite {
   object {
      intersection {
         sphere { <0.0 0.0 0.0> 2.0 inverse }
         sphere { <0.0 0.0 0.0> 2.1 }
         plane {  <0.0 0.0 1.0> 1.2 }
      }
      texture {
         Brown_Agate
         scale <3.0 3.0 3.0>
         specular 1.0
      }
      rotate <45.0 -40.0 0.0>
      translate <10.0 10.0 -8.0>
   }

   object {
      light_source { <10.0 10.0 -8.0>
         colour White
      }
   }
}

/* Far light source to light where spot doesn't */   
object {
   light_source { <-10.0 10.0 -40.0>
      colour White
   }
}
