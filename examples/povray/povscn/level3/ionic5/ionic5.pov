// Persistence of Vision Raytracer Version 1.0
//   This data includes 1 Wall, 1 large 2nd floor column,
//   & 2 smaller ionic colums.

//   b-snake.dat is the lower ctds twist that goes around the 
//   door openings.
//   s-head3.dat is the head & top twist of creature
//   turn.dat is ctds data that creates the ionic turned capitals.
//   panther.dat is csg of panther figure.

//  Modified for use in stereo pair. Two of the GIF
//  images have been replaced with single colors, and a third column
//  was added to the bottom row.
//
//  Three cameras are included, the original, and two for stereo.


#include "colors.inc"
#include "shapes.inc"
#include "marble.inc"
#include "b-snake.inc"
#include "turn.inc"
#include "panther.inc"
#include "s-head3.inc"

#declare pink = color red 1.0 green 0.5 blue 0.5


/* original camera */
camera {
   location <-50 80 -220>
   direction <0 0 1.5>
   up <0 1 0> right <1.333 0 0>
}

/* camera for left eye, render at 768 x 480 or similar aspect ratio */
/*
camera {
    location <-61 80 -220>
    direction <0 0 1.4>
    up <0 1 0> right <1.6 0 0>
}
*/

/* camera for right eye, render at 768 x 480 or similar aspect ratio */
/*
camera {
    location <-39 80 -220>
    direction <0 0 1.4>
    up <0 1 0> right <1.6 0 0>
}
*/

/*-------------- WORLD WALLS ---------------------------------*/
object {
   sphere { <0 0 0> 50000 }
   texture {
      color MidnightBlue
      ambient 1.0
      diffuse 0.0
   }
}

/*---------------LIGHT #1------------------------------------*/
object { light_source { <700 150 -500> color White    } }

/*--------------LIGHT #2-------------------------------------*/
object { light_source { <-4000 100 -1000> color White  } }


/**********************************************************************/

/*-----------BASE OF COLUMN A / TOP FLOOR-------------------*/
#declare base_a = object {
   union {
      quadric { Ellipsoid scale <13 5.5 13>  }
      quadric { Ellipsoid scale <11 3 11> translate <0 4 0>  }
      intersection { Disk_Y scale <10 4 10> translate <0 5 0>  }
   }

   texture {
      marble1
      scale <9 6 4>
      ambient 0.5
      diffuse 1
   }
   texture {
      marble4
      scale <7 12 9>
      diffuse 1.0
      rotate <0 0 -40>
      phong 0.6
      phong_size 50
   }

   rotate <0 40 0>
   bounded_by {
      intersection { Disk_Y scale
	 <13.5 16 13.5>
	 translate <0 -4 0>
      }
   }
}

/*-----------MOTIF TILE WORK--------------------*/
#declare motif1 = object {
   intersection { Disk_Y
      scale <9.5 7 9.5>
   }
   texture {
      image_map { <1.0 -1.0 0> gif "congo4.gif"  }
      scale <19 7 1>
      translate <-9.5 0 -1>
      ambient 0.3
      diffuse 0.9
      phong 0.6
   }
}

/*----------- COLUMN A -------------------------------------------------*/
#declare column_a = object {
   intersection { Disk_Y
      scale <9.3 50 9.3>
   }
   texture { 0.1
      marble1
      ambient 0.2
      diffuse 0.8
      scale <16 4 4>
   }
   texture {
      marble2
      scale <10 6 5>
      rotate <0 0 -30>
      diffuse 1.0
   }
   texture {
      marble4
      phong 0.6
      phong_size 45
      scale <8 15 5>
      rotate <0 0 50>
   }
   rotate <0 10 0>
}



/*-----------COMBINE COLUMN & BASE --------------------------------*/
#declare top_column = composite {
   object { base_a }
   object { motif1 translate <0 9 0> }
   object { column_a translate <0 12 0> }
   bounded_by {
      intersection { Disk_Y
	 scale <13.5 50 13.5>
	 translate <0 -3 0>
      }
   }
}


/*--------------------TOP  FLOOR---------------------------------------*/
#declare floor = object {
   intersection {  Cube
      scale <100 1 50>
   }
   texture {
      color Salmon
      ambient 0.2
      diffuse 0.5
      phong 1.0
      phong_size 10
   }
}

/*----------- ROUND CORNER ----------------------------*/
#declare sp1 = quadric { Ellipsoid scale <2 2 2> }

#declare round_cap = object {
   union {
      quadric { sp1 translate <-100 0 -50> }
      quadric { sp1 translate <-100 0  50> }
      quadric { sp1 translate <100 0  -50> }
      quadric { sp1 translate <100 0   50>  }
      intersection { Disk_X scale <200 2 2> translate <-100 0 -50> }
      intersection { Disk_Z scale <2 2 100> translate <-100 0 -50> }
   }

   texture {
      marble1
      scale <50 10 40>
      ambient 0.4
      diffuse 0.9
   }
   texture {
      marble2
      scale <40 5 20>
      rotate <0 0 40>
   }
   texture {
      marble4
      scale <20 20 20>
      rotate <0 0 -40>
      phong 0.6 phong_size 20
   }

   bounded_by {
      intersection { Cube
	 scale <103 25 53>
	 translate <0 0 0>
      }
   }
}

/*--------------- ONE DENTILE ----------------------------------*/
#declare d1 = intersection { Cube scale <1 1.5 2> }

#declare cornice = object {
   union {
      intersection { Cube scale <103 0.5 53> translate <0 2.5 0> }
      intersection { Cube scale <103 1 53>   translate <0 -2 0>  }
      intersection { Cube scale <101 2 51>   translate <0 0 0>   }
   }

   texture {
      marble1
      scale <10 6 4>
      ambient 0.4
      diffuse 1.0
   }
   texture {
      marble2
      scale <7 6 4>
      rotate <0 0 40>
   }
   texture {
      marble4
      scale <3 6 5>
      rotate <0 0 -40>
      phong 0.6
      phong_size 20
   }

   bounded_by {
      intersection { Cube
	 scale <104 3.5 54>
	 translate <0 0 0>
      }
   }
}

/*-------------------- ROW OF DENTILES ---------------*/
#declare dentile_a = object {
   union {
      intersection { d1 translate <-50 0 0> }
      intersection { d1 translate <-45 0 0> }
      intersection { d1 translate <-40 0 0> }
      intersection { d1 translate <-35 0 0> }
      intersection { d1 translate <-30 0 0> }
      intersection { d1 translate <-25 0 0> }
      intersection { d1 translate <-20 0 0> }
      intersection { d1 translate <-15 0 0> }
      intersection { d1 translate <-10 0 0> }
      intersection { d1 translate < -5 0 0> }
      intersection { d1 translate <  0 0 0> }
      intersection { d1 translate < 50 0 0> }
      intersection { d1 translate < 45 0 0> }
      intersection { d1 translate < 40 0 0> }
      intersection { d1 translate < 35 0 0> }
      intersection { d1 translate < 30 0 0> }
      intersection { d1 translate < 25 0 0> }
      intersection { d1 translate < 20 0 0> }
      intersection { d1 translate < 15 0 0> }
      intersection { d1 translate < 10 0 0> }
      intersection { d1 translate <  5 0 0> }
   }

   texture {
      marble1
      scale <10 6 4>
      ambient 0.4
      diffuse 0.9
   }
   texture {
      marble2
      scale <7 6 4>
      rotate <0 0 40>
   }
   texture {
      marble4
      scale <3 6 4>
      rotate <0 0 -40>
      phong 0.6
      phong_size 20
   }


   bounded_by {
      intersection { Cube
	 scale <60 2 2>
	 translate <0 0 0>
      }
   }
}

/*--------------  COMBINE CORNICE, FLOOR, CAP & DENTILES -----------*/
#declare top_floor = composite {
   object { floor     translate <0 9 0>     }
   object { round_cap translate <0 7 0>     }
   object { cornice   translate <0 3 0>     }
   object { dentile_a translate <-50 4 -52> }
   object { dentile_a translate < 50 4 -52>  }

   bounded_by {
      intersection { Cube
	 scale <105 8 57>
	 translate <0 8 0>
      }
   }
}

/*------------------MAKE AN IONIC CAPIAL---------------------------*/

#declare turn = composite { s1 rotate <-90 0 0>  }

/*--------------- 1/2 SECTION OF CAP ------------------------------*/
#declare cap_a = object {
   union {
      quadric { Ellipsoid 
	 scale <7 2 7>
	 translate <0 1 0>
      }
      intersection { Disk_Y
	 scale <6 5 6>
	 translate <0 2 0>
      }
      quadric { Ellipsoid 
	 scale <8 3 8>
	 translate <0 8 0>
      }
      intersection { Disk_Y
	 scale <8 2 8>
	 translate <0 8 0>
      }
   }

   texture {
      marble1
      ambient 0.4
      diffuse 1.0
      scale <5 13 4>
   }
   texture {
      marble4
      diffuse 1.0
      phong 0.6
      phong_size 20
      scale <5 7 3>
      rotate <0 0 50>
   }

   bounded_by {
      intersection { Disk_Y
	 scale <8.5 11 8.5>
	 translate <0 -0.5 0>
      }
   }
}

/*-------------- THE OTHER 1/2-----------------------------*/
#declare cap_b = object {
   union {
      intersection { Disk_Y scale <1.2 8 1.2> translate <1 0 -4>  }
      intersection { Disk_Y scale <1.2 8 1.2> translate <-1 0 -4> }

      intersection { Disk_Y scale <1.2 4 1.2> translate <-2 0 -4> }
      intersection { Disk_Y scale <1.2 4 1.2> translate <2 0 -4>  }

      intersection { Cube scale <10 2 8> translate <0 10 2>       }
      intersection { Cube scale <7 4 4> translate <0 4 0>  }
      intersection { Disk_Z scale <4.5 4.5 8> translate <-6 4 -4> }
      intersection { Disk_Z scale <4.5 4.5 8> translate <6 4 -4>  }
   }

   texture {
      marble1
      ambient 0.4
      diffuse 1.0
      scale <5 13 4>
   }
   texture {
      marble4
      diffuse 1.0
      phong 0.6
      phong_size 20
      scale <5 7 3>
      rotate <0 0 50>
   }

   bounded_by {
      intersection { Cube
	 scale <10.5 7 10.5>
	 translate <0 7 0>
      }
   }
}


/*-----------2 HALFS & 2 TURNS MAKE A WHOLE ------------------------*/
#declare capital = composite {
   object { cap_a translate <0 0 0>  }
   object { cap_b translate <0 10 0>  }
   composite { turn translate <-6 14 -4>  }
   composite { turn rotate <0 180 0> translate <6 14 -4>  }
   bounded_by {
      intersection { Cube
	 scale <11 13 11>
	 translate <0 13 0>
      }
   }
}

/*--------------MAKE COLUMN-------------------*/


/*-----------MOTIF TILE WORK--------------------*/
#declare motif2 = object {
   intersection { Disk_Y scale <6.5 8 6.5>  }
   texture {
      image_map {  <1.0 -1.0 0> gif "congo4.gif"  }
      scale <13 8 1>
      translate <-6.5 0 -1>
      ambient 0.3
      diffuse 0.9
      phong 1.0
   }
}

#declare column_b = object {
   intersection { Disk_Y
      scale <6 54 6>
   }
   texture {  0.1
      marble1
      ambient 0.4
      diffuse 0.8
      scale <9 30 18>
   }
   texture {
      marble2
      scale <10 18 5>
      rotate <0 0 -30>
      diffuse 1.0
   }
   texture {
      marble4
      diffuse 0.9
      phong 0.8
      phong_size 45
      scale <4 15 5>
      rotate <0 0 50>
   }
}


/*-----------THE COMPLETE BOTTOM COLUMN---------------------------------*/
#declare bottom_column = composite {
   composite { capital translate <0 58 0> }
   object    { motif2 translate <0 50 0>  }
   object    { column_b translate <0 0 0> }
   bounded_by {
      intersection { Disk_Y
	 scale <12 85 12>
	 translate <0 0 0>
      }
   }
}

/*---------------------START LOWER WALLS-----------------------------*/

/*----------- ABOVE DOOR SECTION OF WALL -----------------------------*/
#declare brick_wall = object {
   intersection { Cube scale <30 6 3>
   }

   texture {
      marble1
      ambient 0.2
      diffuse 0.9
      scale <20 30 4>
   }
   texture {
      marble2
      scale <20 9 5>
      diffuse 1.0
      rotate <0 0 -30>
   }
   texture {
      marble4
      phong 0.6
      phong_size 25
      scale <18 18 3>
      rotate <0 0 50>
   }
}

/*-------------- ARCH SECTION OF WALL --------------------------------*/
#declare top_wall = object {
   difference {
      intersection { Cube       scale <30 10 3> translate <0 0 0.01> }
      quadric      { Cylinder_Z scale <13 13 1> translate <0 -10 0>  }
   }

   texture {
      marble1
      ambient 0.2
      diffuse 0.9
      scale <30 20 4>
   }
   texture {
      marble2
      scale <26 16 5>
      diffuse 1.0
      rotate <0 0 -30>
   }
   texture {
      marble4
      phong 0.6
      phong_size 25
      scale <15 27 2>
      rotate <0 0 50>
   }
   bounded_by {
      intersection { Cube
	 scale <30 10 2>
	 translate <0 0 0>
      }
   }
}


/*------------------ TRIM ---------------------------------------------*/
#declare molding_2 = object {
   union {
      difference {
	 intersection { Disk_Z     scale <16 16 12> translate <0 0 -6>  }
	 quadric {      Cylinder_Z scale <13 13 1>  }
      }
      intersection { Cube scale <10 2 6> translate <-21 -2 0>  }
      intersection { Cube scale <10 2 6> translate <21 -2 0>  }
      intersection { Cube scale <31 1 6> translate <0 17 0>  }
   }

   texture {
      marble1
      ambient 0.5
      diffuse 1.0
      scale <15 3 4>
   }
   texture {
      marble4
      phong 0.8
      phong_size 15
      ambient 0.6
      scale <15 7 6>
      rotate <0 0 50>
   }
   bounded_by {
      intersection { Cube
	 scale <31 13 4>
	 translate <0 9 0>
      }
   }
}

#declare motif3 = object {
   intersection { Cube scale <9.5 5 3.1>  }
   texture {
      image_map { <1.0 -1.0 0> gif "congo4.gif"  }
      scale <19 10 1>
      translate <-9.5 -5 -1>
      ambient 0.3
      diffuse 0.9
      phong 1.0
   }
}

/*----------WALLS FLANKING DOOR WAY -----------------------------------*/
#declare low_wall = object {
   intersection { Cube scale <9 22 3>  }

   texture {
      marble1
      ambient 0.3
      diffuse 0.9
      scale <15 20 7>
   }
   texture {
      marble2
      scale <10 25 3>
      diffuse 1.0
      rotate <0 0 -30>
   }
   texture {
      marble4
      phong 0.8
      phong_size 25
      ambient 0.5
      scale <8 16 3>
      rotate <0 0 50>
   }
}

/*----------COMPOSITE THE WALL------------------------------*/

#declare wall = composite {
   object { low_wall translate <-21 22 0>  }
   object { low_wall translate <21 22 0>  }
   object { motif3 translate <-21 42 0>  }
   object { motif3 translate <21 42 0>  }
   object { molding_2 translate <0 50 0>  }
   object { top_wall translate <0 60 0>  }
   object { brick_wall translate <0 74 0>  }
   bounded_by {
      intersection { Cube
	 scale <32 41 4>
	 translate <0 41 0>
      }
   }
}


/*--------CREATE INSIDE BACK WALL-----------------------*/
object {
   intersection { Cube scale <95 55 1>  }

   texture {
      marble1
      ambient 0.2
      diffuse 0.9
      scale <40 30 5>
   }
   texture {
      marble2
      scale <30 60 3>
      rotate <0 0 -30>
   }
   texture {
      marble4
      phong 0.8
      phong_size 25
      scale <30 20 3>
      rotate <0 0 50>
   }
   translate <0 15 10>
}


/*----------*** PUT UP THE WALLS ****--------------------*/

composite { top_column  rotate <0 10 0>  scale <1.9 1.9 1.9> translate <-30 94 -25> }
composite { top_floor   translate <-10 80 0> }
   composite { bottom_column scale <1.3 1 1.3> translate <-100 0 -40>  }
   composite { bottom_column scale <1.3 1 1.3> translate <-30 0 -40>  }
   composite { bottom_column scale <1.3 1 1.3> translate <40 0 -40>     }
   composite { wall translate <5 0 -32>    }
   composite { wall translate <-65 0 -32>  }


   /*---------------------BACKDROP---------------------------------*/
   object {
      intersection { Cube
	 scale <2000 1500 1>
      }

      texture {
	 color Maroon
	 // image_map {  <1.0 -1.0 0> gif "sky1.gif"  }    // Optional sky map
	 // scale <4000 3000 2> translate <-2000 -1500 -2>
	 ambient 0.2
	 diffuse 0.6
      }


      texture {
	 gradient <0 1 0>
	 color_map {
	    [0.0 0.3 color Salmon color BlueViolet alpha 0.3]
	    [0.3 1.0 color BlueViolet alpha 0.3 color Black alpha 0.2]
	 }
	 scale <4000 3000 1>
	 translate <0 -1500 0>
	 ambient 1.0
	 diffuse 0.0
      }
      scale <1 1 1>
      translate <0 0 2000>
   }




   /*------------------- CREATURE & MOTHER-IN-LAW -------------------------*/

   composite { panther
      rotate <0 -20 0>
      scale <1.2 1.2 1.2>
      translate <-70 89.5 -10>
   }

   composite { lizard
      scale <1 0.9 1>
      rotate <0 0 -10>
      translate <-33 16 -20>
   }

#declare snake = composite {
      composite { top  }
      composite { head 
	 scale <1.9 1.6 1.6>
	 rotate <0 -90 -90>
	 translate <23 0 -13>
      }
      rotate <-90 -180 -90>
   }

   composite { snake  translate <-4 87 -68>  }
   // end-of-file
