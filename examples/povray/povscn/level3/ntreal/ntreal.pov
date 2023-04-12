// Persistence Of Vision raytracer version 1.0 sample file.

// By Drew Wells

// Title-"Not a Trace of Reality"

// -+ Compuserve Hall Of Fame award winner +- 

// This one is hard to describe and easy to look at. Have fun with it! 
// - Drew Wells cis 73767,1244 
// 11/29/90 



// Note - This one is a memory hog for pc's because of the twister 
//        If you can't run it, try editing out some of the twister's 
//        parts.  Requires #include file ntreal.inc

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare It = 
quadric { Ellipsoid scale <4.0 0.3 0.3> }

/* Camera/Viewer */

// This camera is different from any of the other sample scene files
// The camera is looking in the negative Z direction
// and right is in the negative X direction.
camera {
   direction <0.0 0.0  1.5>
   up  <0.0  1.0  0.0>
   right <-1.333 0.0 0.0>
   translate < -15.0  5.0  120.0>
   look_at <10.0 12.0 55.0>
}

#declare Purple_Clouds = texture {
   bozo
   turbulence 0.6
   colour_map {
      [0.0 0.5   colour red 0.9 green 0.5  blue 0.6
      colour red 0.4 green 0.0  blue 0.4]  
      [0.5 0.6   colour red 1.0 green 1.0  blue 1.0 alpha 1.0
      colour red 1.0 green 1.0  blue 1.0 alpha 1.0 ]
      [0.6 1.001 colour red 1.0 green 1.0  blue 1.0 alpha 1.0
      colour red 1.0 green 1.0  blue 1.0 alpha 1.0]
   }
}

#declare Sunset_Sky = texture {
   gradient <0.0 1.0 0.0>

   colour_map {   
      [0.0 0.4   colour red 0.8 green 0.0 blue 0.0
      colour red 0.4 green 0.0 blue 0.4]
      [0.4 0.6   colour red 0.4 green 0.0 blue 0.4
      colour red 0.0 green 0.0 blue 0.2]
      [0.6 1.001 colour red 0.0 green 0.0 blue 0.2
      colour red 0.0 green 0.0 blue 0.0]
   }
   scale <700.0 700.0 700.0>
}

#declare Twister = object {
   union {
#include "ntreal.inc"
   }
   bounded_by {
      sphere { <0.0 5.0 0.0> 6.0  }
   }
   texture {
      White_Wood
      0.05
      scale <3.0 3.0 3.0>
      ambient 0.1
      diffuse 0.99
   }
   color Blue
}

#declare Slice = quadric {
   Ellipsoid
   translate <0.0 0.0 3.0>
   scale <0.25 1.00 0.25>
}

#declare Thing = object {
   union {
      quadric { Slice }
      quadric { Slice rotate <0.0 -20.0  0.0> }
      quadric { Slice rotate <0.0 -40.0  0.0> }
      quadric { Slice rotate <0.0 -60.0  0.0> }
      quadric { Slice rotate <0.0 -80.0  0.0> }
      quadric { Slice rotate <0.0 -100.0 0.0> }
      quadric { Slice rotate <0.0 -120.0 0.0> }
      quadric { Slice rotate <0.0 -140.0 0.0> }
      quadric { Slice rotate <0.0 -160.0 0.0> }
      quadric { Slice rotate <0.0  180.0 0.0> }
      quadric { Slice rotate <0.0 -200.0 0.0> }
      quadric { Slice rotate <0.0 -220.0 0.0> }
      quadric { Slice rotate <0.0 -240.0 0.0> }
      quadric { Slice rotate <0.0 -260.0 0.0> }
      quadric { Slice rotate <0.0 -280.0 0.0> }
      quadric { Slice rotate <0.0 -300.0 0.0> }
      quadric { Slice rotate <0.0 -320.0 0.0> }
      quadric { Slice rotate <0.0 -340.0 0.0> }
   }
   bounded_by {
      quadric { Ellipsoid scale <6.7 6.7 6.7> }
   }            
   texture {
      color red 0.8 green 0.22 blue 0.1
      bumps 0.3
      scale < 0.1 0.1 0.1>
      ambient 0.1
      diffuse 0.9
      phong 0.75
      phong_size 30.0
   }
   color red 0.8 green 0.22 blue 0.1
}    

#declare Slice2 = sphere { <0.0 0.0 0.5> 0.1 }

#declare Thing2 =
object {
   union {
      sphere { Slice2 }
      sphere { Slice2 rotate <0.0 -20.0  0.0> }
      sphere { Slice2 rotate <0.0 -40.0  0.0> }
      sphere { Slice2 rotate <0.0 -60.0  0.0> }
      sphere { Slice2 rotate <0.0 -80.0  0.0> }
      sphere { Slice2 rotate <0.0 -100.0 0.0> }
      sphere { Slice2 rotate <0.0 -120.0 0.0> }
      sphere { Slice2 rotate <0.0 -140.0 0.0> }
      sphere { Slice2 rotate <0.0 -160.0 0.0> }
      sphere { Slice2 rotate <0.0  180.0 0.0> }
      sphere { Slice2 rotate <0.0 -200.0 0.0> }
      sphere { Slice2 rotate <0.0 -220.0 0.0> }
      sphere { Slice2 rotate <0.0 -240.0 0.0> }
      sphere { Slice2 rotate <0.0 -260.0 0.0> }
      sphere { Slice2 rotate <0.0 -280.0 0.0> }
      sphere { Slice2 rotate <0.0 -300.0 0.0> }
      sphere { Slice2 rotate <0.0 -320.0 0.0> }
      sphere { Slice2 rotate <0.0 -340.0 0.0> }
   }
   bounded_by {
      sphere { <0.0 0.0 0.0> 12 }
   }
   texture {
      color red 0.1 green 0.22 blue 0.8
      ambient 0.1
      diffuse 0.9
      phong 0.75
      phong_size 30.0
   }
   color red 0.1 green 0.22 blue 0.8
}    


/*******************************************/
/*******************************************/

object {
   Twister
   /* scale <4.7 8.0 4.0> */
   rotate <-15.0 30.0 0.0>
   translate <-16.0 7.7 61.5>    
}    
object {
   Twister
   /*scale <1.0 2.0 1.0> */
   rotate <-15.0 0.0 -10.0>
   translate <0.0 1.0 88.0>    
}    
object {
   Twister
   /*scale <4.5 8.0 4.0>*/
   rotate <0.0 45.0 0.0>
   translate <13.0 25.0 40.0>    
}    

object {
   Twister
   /*scale <4.5 8.0 4.0>*/
   rotate <-15.0 0.0 -10.0>
   translate <26.0 14.0 70.0>    
}    

/* Little Things */
object {
   Thing 
   scale <1.5 3.0 1.5>
   translate < -11.0 1.55 95.0> 
}    
object {
   Thing2 
   scale <6.0 6.0 6.0>
   rotate <-10.0 30.0 0.0>
   translate <-11.0 1.55 95.0> 
}    

object {
   Thing 
   scale <1.5 3.5 1.5>
   translate <-10.0 10.55 95.0> 
}    
object {
   Thing2 
   scale <10.0 10.0 10.0>
   rotate <0.0 0.0 -35.0>
   translate < -10.0 10.55 95.0>    
}    

object {
   Thing 
   scale <1.5 3.5 1.5>
   translate < -4.0 4.0 80.0>    
}    
object {
   Thing2 
   scale <6.0 6.0 6.0>
   rotate <-30.0 0.0 20.0>
   translate < -4.6 5.55 80.0> 
}    


object {
   Thing 
   scale <1.5 3.5 1.5>
   translate < 11.0 1.6 90.0>    
}
object {
   Thing2    
   scale <8.0 8.0 8.0>
   rotate <0.0 45.0 20.0>
   translate < 10.0 7.0 90.0>    
}    

/*cloud hills*/
object {
   quadric { Paraboloid_Y 
      scale<40.0 10.0 77.0>
   }       
   rotate <0.0 0.0 180.0>
   translate <0.0 21.0 -28.0>
   texture {
      Purple_Clouds
      scale < 5.0 5.0 5.0>
      ambient 0.5
      diffuse 0.9    
   }
   color red 0.5 green 0.6 blue 0.2
}

/*cloud hill to right*/
object {
   quadric { Paraboloid_Y 
      scale<30.0 10.0 40.0>
   }       
   rotate <0.0 0.0 180.0>
   translate <40.0 14.0 50.0>
   texture {
      Purple_Clouds
      scale < 7.0 5.0 5.0>
      ambient 0.5
      diffuse 0.9    
   }
   color red 0.6 green 0.6 blue 0.1
}

/*The Sun*/
object {
   light_source { <150.0 40.0 1200.0>
      colour White
   }
}

/*sky*/
object {
   sphere { <0.0 0.0 0.0> 2000.0 inverse }
   texture {
      Sunset_Sky
      translate <0.0 200.0 0.0>
      scale <1.2 1.2 1.2>
      ambient 0.6
      diffuse 0.0
   }
   color White
}    
