// Persistence Of Vision raytracer version 1.0 sample file.

// By Drew Wells

// Title-"Tomb" 
// a haunted tomb on a hill, gravestones, rusty fence, pumpkin patch. 
// - Drew Wells cis 73767,1244 
// 11/90 rev 7/92
#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "stones.inc"

/* Camera */
camera {
   direction <0.0 0.0  1.5>
   up  <0.0  1.0  0.0>
   right <-1.333 0.0 0.0>
   translate < -15.0  5.0  120.0>
   look_at <10.0 12.0 55.0>
}
#declare TombTex = texture { Stone8 ambient .1 scale <0.5 0.5 0.5> }
#declare HeadTex = texture { Stone3  }
#declare ColTex  = texture { Stone13 phong .3 scale <.2 .2 .2>  }
#declare Rust = texture { Stone1 }


#declare Yellow_Clouds = texture {
bozo
turbulence 0.6
colour_map {
   [0.0 0.3   colour red 0.9 green 0.5  blue 0.3
   colour red 0.6 green 0.4  blue 0.0]  
   [0.3 0.6   colour red 0.6 green 0.4  blue 0.0
   colour red 0.4 green 0.4  blue 0.0 alpha 1.0]  
   [0.6 0.8   colour red 0.4 green 0.4  blue 0.0 alpha 1.0
   colour Clear]  
   [0.8 1.001 colour Clear
   colour Clear]
}
}

#declare Sunset_Sky = texture {
gradient <0.0 1.0 0.0>

colour_map {
[0.0 0.4  /*blue*/
colour red 0.8 green 0.0 blue 0.0
colour red 0.4 green 0.0 blue 0.4]

[0.4 0.6  colour red 0.4 green 0.0 blue 0.4
/*white*/
colour red 0.0 green 0.0 blue 0.2]
[0.6 1.001 colour red 0.0 green 0.0 blue 0.2
/*grey*/
colour red 0.0 green 0.0 blue 0.1]
}
scale <700.0 700.0 700.0>
}


/*************************************************/
/* Define objects for use in scene               */
/*************************************************/

#declare Cross =
object {
union {
/* Tried to make celtic cross, but vga resolution is too coarse */
/* intersection
     quadric Cylinder_Z
        scale <0.4 0.4 0.4>
        translate <0.0 0.45 0.0>
     end_quadric
     plane <0.0 0.0 -1.0>  0.02 end_plane
     plane <0.0  0.0 1.0> 0.02 end_plane
     end_intersection */

/*vertical part*/
intersection {
   /*top & bottom*/
   plane { <0.0 1.0 0.0> 1.0 }
   plane { <0.0 -1.0 0.0> 2.0 }
   /*front & back*/
   plane { <0.0 0.0 1.0> 0.12 }
   plane { <0.0 0.0 -1.0> 0.12 }
   /*left & right*/
   plane { <1.0 0.0 0.0> 0.15 }
   plane { <-1.0 0.0 0.0> 0.15 }
}

/*horizontal part*/
intersection {
   /*top & bottom*/
   plane { <0.0 1.0 0.0> 0.6 }
   plane { <0.0 -1.0 0.0> -0.3 }
   /*front & back*/
   plane { <0.0 0.0 1.0> 0.12 }
   plane { <0.0 0.0 -1.0> 0.12 }
   /*left & right walls*/
   plane { <1.0 0.0 0.0> 0.7 }
   plane { <-1.0 0.0 0.0> 0.7 }
}
}
bounded_by { box { <-.8 -2.1 -.2>  <.8 1.1 .8> } }

texture {
HeadTex
scale <0.1 0.1 0.1>
ambient 0.1
diffuse 0.7   
}
colour Blue
}

#declare Headstone =
object {
union {
intersection {      
   quadric { Cylinder_Z }
   plane { <0.0 0.0 -1.0>  0.1 }
   plane { <0.0  0.0 1.0> 0.1 }
}            
intersection {
   /*top & bottom*/ 
   plane { <0.0 1.0 0.0> 0.0 }
   plane { <0.0 -1.0 0.0> 2.0 }
   /*front & back*/
   plane { <0.0 0.0 1.0> 0.1 }
   plane { <0.0 0.0 -1.0> 0.1 }
   /*left & right */
   plane { <1.0 0.0 0.0> 1.0 }
   plane { <-1.0 0.0 0.0> 1.0 }
}
}
texture { 
HeadTex
scale<0.1 0.2 0.1>
ambient 0.1
diffuse 0.7   
}
colour Blue
}


#declare Pole = quadric { Cylinder_Y
scale <0.1 1.0 0.1>
}


#declare Xpole = quadric { Cylinder_X
scale <0.1 0.1 0.1>
}

/* Rusty iron gate & fence - object should have been composite to avoid the */
/*                           "carved from one piece" look. */
#declare Gate = object {
union {
intersection {
quadric { Xpole  }
plane { <1.0 0.0 0.0> 8.0 }
plane { <-1.0 0.0 0.0> 2.0 }
translate <0.0 6.5 0.0>
}
intersection {
quadric { Xpole  }
plane { <1.0 0.0 0.0> 8.0 }
plane { <-1.0 0.0 0.0> 2.0 }
translate <0.0 1.9 0.0>
}
intersection {
quadric { Pole  }
plane { <0.0 1.0 0.0> 7.5 }
plane { <0.0 -1.0 0.0> 0.0 }
translate <1.0 0.0 0.0>
}
sphere { <1.0 7.5 0.0> 0.3 }
intersection {
quadric { Pole }
plane { <0.0 1.0 0.0> 7.75 }
plane { <0.0 -1.0 0.0> 0.2 }
translate <2.5 0.0 0.0>
}
sphere { <2.5 7.75 0.0> 0.3 }
intersection {
quadric { Pole }
plane { <0.0 1.0 0.0> 8.0 }
plane { <0.0 -1.0 0.0> 0.2 }
translate <4.0 0.0 0.0>
}
sphere { <4.0 8.0 0.0> 0.3 }
intersection {
quadric { Pole  }
plane { <0.0 1.0 0.0> 7.75 }
plane { <0.0 -1.0 0.0> 0.2 }
translate<5.5 0.0 0.0>
}
sphere { <5.5 7.75 0.0> 0.3 }
intersection {
quadric { Pole }
plane { <0.0 1.0 0.0> 7.5 }
plane { <0.0 -1.0 0.0> 0.2 }
translate <7.0 0.0 0.0>
}
sphere { <7.0 7.5 0.0> 0.3 }
}
bounded_by {
intersection {
plane { <0.0 1.0 0.0> 9.0 }
plane { <0.0 -1.0 0.0> 0.0 }
plane { <0.0 0.0 1.0> 1.0 }
plane { <0.0 0.0 -1.0> 1.0 }
plane { <1.0 0.0 0.0> 8.0 }
plane { <-1.0 0.0 0.0> 0.0 }
}
}
texture { 
Rust 
ambient 0.2
diffuse 0.9   
}    
colour Blue
}

/* a rectangular pad to serve as a footing for the column */
#declare BasePad =
object {
intersection {
plane { <0.0 1.0 0.0> 0.25 }
plane { <0.0 -1.0 0.0> 0.25 }
plane { <0.0 0.0 1.0> 1.2 }
plane { <0.0 0.0 -1.0> 1.2 }
plane { <1.0 0.0 0.0> 1.2 }
plane { <-1.0 0.0 0.0> 1.2 }
}

colour red 0.6 green 0.6 blue 0.4
}

#declare ColTop =
object { 
union {
box { <-1.5 -.1 -1.5> <1.5 .2 1.5> }
box { <-1.25 .2 -1.25> <1.25 .4 1.25> }
box { <-1 .4 -1> <1 .6 1> }
}
color red .6 green .6 blue .4
}
#declare Beam = quadric { Cylinder_Y  }

#declare Beam2 = 
quadric { Cylinder_Y
inverse
scale     <.3 1 .3>
translate <1.4 0 0>
}

#declare Beam3 =
object {
intersection {
quadric { Beam  scale <1.2 1 1.2> }
quadric { Beam2 }
quadric { Beam2 rotate <0  -45 0> }
quadric { Beam2 rotate <0  -90 0> }
quadric { Beam2 rotate <0 -135 0> }
quadric { Beam2 rotate <0  180 0> }
quadric { Beam2 rotate <0   45 0> }
quadric { Beam2 rotate <0   90 0> }
quadric { Beam2 rotate <0  135 0> }
plane { <0 1 0>  8 }
plane { <0 -1 0> 0 }
}
color red .8 green 0 blue .0
}

#declare Column = composite {
object { Beam3   texture { ColTex } }
//object { BasePad texture { ColTex phong 0 } translate <0 0 0> }
object { ColTop texture { ColTex phong 0 } translate <0 8 0> }
}


#declare Tomb = object {
union {
difference {
box { <-10 -5 -7.5> <10 10 7.5> } // bulk
box { <-3 -5 -7.0> <3 7 7.6> } // door
}
/* Foundation */
box { <-11 -2 -10.5> <11 -1 10.5> }
box { <-12 -3 -11>   <12 -2 11> }
box { <-13 -4 -12>   <13 -3 12> }
box { <-14 -5 -13>   <14 -4 13> }
}
bounded_by { box { <-14.1 -5.1 -13.1> <14.1 10.1 18.6> } }

}

#declare InnerBeams = object {
union {
intersection {
quadric { Beam }
plane { <0.0 1.0 0.0> 8.0 }
plane { <0.0 -1.0 0.0> 2.0 }
translate < 5.0 0.0 7.5>
}
intersection {
quadric { Beam }
plane { <0.0 1.0 0.0> 8.0 }
plane { <0.0 -1.0 0.0> 2.0 }
translate < -5.0 0.0 7.5>
}
}
colour Blue
}


#declare Pointy = object {
union {
/*pointy part*/
intersection {
plane { <0 -1 0> 1 }

plane { <0 0 -1> 1 }
plane { <0 0 1> 1 rotate <0 27 0> rotate <40 0 0> }
plane { <0 0 1> 1 rotate <0 -27 0> rotate <40 0 0> }
plane { <1 0 0> 0 rotate <0 0 70> }
plane { <-1 0 0> 0 rotate <0 0 -70> }
scale <3 4 1>
translate < 0 12 8.5>
}
}
color Blue
}


#declare CornerBeams = object {
union {
intersection {
quadric { Beam scale <1.5 1.0 1.5>}
plane { <0.0 1.0 0.0> 10.0 }
plane { <0.0 -1.0 0.0> 2.0 }
translate < 10.0 0.0 7.5>
}
intersection {      
quadric { Beam scale <1.5 1.0 1.5> }
plane { <0.0 1.0 0.0> 10.0 }
plane { <0.0 -1.0 0.0> 2.0 }
translate < -10.0 0.0 7.5>
}
sphere { < -10.0 10.0 7.5> 1.5 }
sphere { <  10.0 10.0 7.5> 1.5 }
}
colour Blue
}

/* Ghost in tomb doorway */
#declare Figure = object {
intersection {
quadric { Beam scale <2.3 1.0 2.3> }
plane { <0.0 1.0 0.0> 8.0 }
plane { <0.0 -1.0 0.0> 2.0 }
translate < 0.0 0.0 4.3>
}
texture {
granite
/* Use any ghost image you like for the tomb doorway or comment */
/* out the Figure in TombAll */
/*imagemap <-1.0 1.0 0.0> gif "ghost.gif" once 
      interpolate 2.0
     end_map*/
scale < 6.5 8.0 6.5 >
translate < -1.0 7.5 4.3 >
ambient 0.10
diffuse 0.3
}
colour Black
}

/* TombAll is the completed tomb */
#declare TombAll = composite {
object { Tomb texture { TombTex } }
object { InnerBeams texture { Stone21 } }
object { Pointy texture { Stone4 } }
object { CornerBeams texture { Stone18 scale <.5 .5 .5> } }   
}

/* Pumpkin parts */
#declare Stem = object {
intersection {
quadric { Pole  }
plane { <0.0 1.0 0.0> 0.04 rotate <0.0 -10.0 -10.0> }
plane { <0.0 -1.0 0.0> 1.0 }
translate<0.0 0.3 0.0>
}
texture {
bumps 0.5
color red 0.04 green 0.33 blue 0.05
scale < 0.2 0.2 0.2>
ambient 0.1
diffuse 0.9   
}
color red 0.04 green 0.33 blue 0.05
}

#declare Slice = quadric {
Ellipsoid
translate <0.0 0.0 0.5>
scale <0.28 0.30 0.40>
}

#declare Pumpkin = object {
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
quadric { Slice rotate <0.0 -180.0 0.0> }
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
quadric { Ellipsoid scale <1.5 1.5 1.5> }
}
texture {
bumps 0.3
color red 0.5 green 0.22 blue 0.1
scale < 0.1 0.1 0.1>
ambient 0.1
diffuse 0.9
phong 0.75
phong_size 30.0
}
color red 0.5 green 0.22 blue 0.1
}


/*******************************************/
/* Scene description     */
/*******************************************/

object {
Cross
scale <1.7 2.0 1.0>
rotate <-15.0 0.0 0.0>
translate <-16.0 7.7 61.5>    
}

object {
Headstone
scale <1.0 1.0 1.0>
rotate <15.0 0.0 0.0>
translate <-14.0 4.0 76.0>
}

object {
Headstone
scale <1.0 1.0 1.0>
translate <12.0 4.0 74.0>
}

object {
Headstone
scale <1.0 1.0 1.0>
rotate <-10.0 0.0 0.0>
translate <18.0 8.0 70.0>
}

object {
Cross
scale <1.5 2.0 1.0>
rotate <-15.0 0.0 0.0>
translate <17.0 10.0 60.0>
}

object {
Cross
scale <1.5 2.0 1.0>
rotate <-15.0 0.0 20.0>
translate <26.0 10.0 70.0>
}

object {
Cross
scale <1.5 2.0 1.0>
rotate <-15.0 0.0 -10.0>
translate <31.0 10.0 78.0>
}


/* Little Pumpkin Patch */
object {
Pumpkin
scale <1.5 2.0 1.5>
translate < 5.0 1.55 95.0>
}
object {
Stem
scale <1.5 2.5 1.5>
translate < 5.0 1.55 95.0>
}
object {
Pumpkin
scale <1.5 2.5 1.5>
translate < 11.0 1.6 90.0>
}
object {
Stem
scale <1.5 2.5 1.5>
translate < 11.0 1.6 90.0>
}


/* Ghastly Tomb!*/
composite {
TombAll
rotate <7.0 15.0 0.0>
translate <0.0 16.0 38.0>
}

/*left entry gate*/
object {
Gate
translate <-9.0 0.0 88.0>
}

/*right entry gate*/
object {
Gate
rotate <-10.0 -160.0 0.0>
translate <7.0 0.0 87.5>
}

/*Use gate object to make left & right fence*/
object {
Gate
translate <-19.0 0.0 88.0>
}
object {
Gate
translate <-31.0 0.0 88.0>
}
object {
Gate
translate <9.0 0.0 88.0>
}
object {
Gate
translate <19.0 0.0 88.0>
}

/* Columns to hold the fence and gates up */
composite { Column translate <-20.0 0.0 88.0> }
composite { Column translate <-10.0 0.0 88.0> }
composite { Column translate < 8.0 0.0 88.0> }
composite { Column translate < 18.0 0.0 88.0> }

/*hill under tomb*/
object {
quadric { Paraboloid_Y
scale<40.0 10.0 77.0>
}
rotate <0.0 0.0 180.0>
translate <0.0 21.0 -28.0>
texture {
bumps 0.8
color red 0.5 green 0.6 blue 0.2
scale < 5.0 5.0 5.0>
ambient 0.1
diffuse 0.7    
}                                         
color red 0.5 green 0.6 blue 0.2
}

/*hill to right of tomb*/
object {
quadric { Paraboloid_Y
scale<30.0 10.0 40.0>
}
rotate <0.0 0.0 180.0>
translate <40.0 14.0 50.0>
texture {
bumps 0.8
color red 0.6 green 0.6 blue 0.1
scale < 7.0 5.0 5.0>
ambient 0.1
diffuse 0.7
}
color red 0.6 green 0.6 blue 0.1
}

/* Ground */
object {
plane { <0.0 1.0 0.0> 1.0 }
texture {
bumps 0.7
color red 0.6 green 0.6 blue 0.1
scale < 1.0 1.0 1.0>
ambient 0.1
diffuse 0.7
}
color red 0.6 green 0.6 blue 0.1
}

/*The Sun*/
object {
light_source { <150.0 30.0 1200.0>
color red 1 green .8 blue .65
}
}

/* Sky - gradient sunset*/
object {
sphere { <0.0 0.0 0.0> 2000.0 inverse }
texture {
Sunset_Sky
translate <0.0 200.0 0.0>
scale <1.2 1.2 1.2>
ambient 0.6
diffuse 0.0
}
color Green
}

/* Clouds - uses a sky texture with the sky portion defined as transparent */
/*          so the gradient behind it is visible  */
object {
sphere { <0.0 0.0 0.0> 1997.0 inverse }
texture {
Yellow_Clouds
scale <1000.0 30.0 100.0>
ambient 0.7
diffuse 0.0
}
color Green
}
