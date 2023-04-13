// Persistence Of Vision Raytracer version 1.0 sample file.

// Include file for DRUMS.POV by Dan Farmer

#declare Brass = colour red 0.88 green 0.84 blue 0.10
#declare ChromeBlue = colour red 0.84 green 0.89 blue 1.00

#declare BrassTexture = texture {
   reflection 0.15
   brilliance 10.0
   metallic
   phong 0.75
   phong_size 50
}

#declare Chrome = texture { Mirror }

#declare VeryShiny = texture {
   ambient 0.05  diffuse 0.75
   phong 1.0
   phong_size 100.0
   brilliance 5
   reflection 0.5
}

#declare X_Tube = intersection {
   quadric { Cylinder_X  }
   quadric { Cylinder_X scale <0.97 0.97 0.97 > inverse  }
   plane { <1.0 0.0 0.0> -1.0 inverse  }
   plane { <1.0 0.0 0.0>  1.0  }
}

#declare Cymbal = object {                          /* Overall radius of 6 units */
   union {
      quadric { Ellipsoid                             /* Dome */
         scale <2.0 0.75 2.0 >
         translate <0.0 1.0 0.0>
      }
      quadric { Ellipsoid 
         scale <6.0 1.0 6.0 >
      }
   }       /* end union */
   clipped_by {
      plane { <0.0 1.0 0.0> 0.25 inverse  }
   }       /* end clip */

   color Brass
   texture {
      BrassTexture
      color Brass
   }
   bounded_by {
      sphere { <0 0 0> 6.0  }
   }
}

#declare Tensioner1 =  union {
   intersection {
      Disk_X
      translate <-0.5 0 0>     // old-style _Disk
      scale <0.5 0.04 0.02>
   }
   intersection {
      quadric { Ellipsoid scale <0.25 0.1 0.06>}
      plane { <1 0 0> 0 }
   }
}

#declare Tensioner = object {
   union {
      union { Tensioner1 translate <0.5 0 0>}
      union { Tensioner1 rotate <0 0 180> translate <-0.5 0 0> }
   }
   translate <0.0 0.0 1.03>      /* Position for rotation around drum cylinder */
   color green 1.0
   texture { Chrome
      color ChromeBlue
   }
}


#declare Drum = composite {            
   object {                                       /* Top (and rear) Drumhead */
      quadric { Ellipsoid  scale <0.001 1.0 1.0> }
      translate <0.75 0.0 0.0>
      color Blue
      texture {
         ambient 0.1
         diffuse 0.7
         specular 0.2
         roughness 0.01
         color White
      }
   }


   object {                                       /* Chrome edge band #1 */
      intersection { X_Tube}
      scale <0.05 1.025 1.0>
      translate <-0.8 0.0 0.0>
      color Yellow
      texture { Chrome
         color ChromeBlue
      }
   }
   object {                                       /* Drum body */
      intersection {
         quadric { Cylinder_X
            texture {
               VeryShiny
               // color red 0.80 blue 0.10
               color White
            }
         }
         quadric { Cylinder_X                   /* Inside of the drum cylinder */
            scale <0.97 0.97 0.97 > inverse
            texture {
               color Tan
               ambient 0.05
               diffuse 0.5
            }
         }
         plane { <1.0 0.0 0.0> -1.0 inverse     /* Backside edge plane */
            texture { Chrome
               color ChromeBlue
            }
         }
         plane { <1.0 0.0 0.0>  1.0             /* Front edge plane */
            texture { Chrome
               color ChromeBlue
            }
         }
      }
      scale <0.8 1.0 1.0>
      color red 1.0
   }

   object {                                       /* Chrome edge band #2 */
      intersection { X_Tube  }
      scale <0.05 1.025 1.0>
      translate <+0.8 0.0 0.0>
      color Magenta
      texture { Chrome
         color ChromeBlue
      }
   }

   /* Now a bunch of (10) tensioners */

   object { Tensioner rotate <000.0 0.0 0.0>  }
   object { Tensioner rotate <036.0 0.0 0.0>  }
   object { Tensioner rotate <072.0 0.0 0.0>  }
   object { Tensioner rotate <108.0 0.0 0.0>  }
   object { Tensioner rotate <144.0 0.0 0.0>  }
   object { Tensioner rotate <180.0 0.0 0.0>  }
   object { Tensioner rotate <216.0 0.0 0.0>  }
   object { Tensioner rotate <252.0 0.0 0.0>  }
   object { Tensioner rotate <288.0 0.0 0.0>  }
   object { Tensioner rotate <324.0 0.0 0.0>  }

   bounded_by {
      intersection {
         quadric { Cylinder_X  scale <1.25 1.25 1.25>}
         plane { <1.0 0.0 0.0> -1.25 inverse }
         plane { <1.0 0.0 0.0>  1.25 }
      }
   }

}

#declare BassDrum = composite {
   composite {Drum}
   object {             /* Front Drumhead for Bass */
      quadric { Ellipsoid  scale <0.001 1.0 1.0> }
      translate <-0.75 0.0 0.0>
      color Blue
      texture {
         ambient 0.2
         diffuse 0.7
         specular 0.5
         roughness 0.05
         color White alpha 0.25
      }
   }
   scale <8.0 10.0 10.0>
}

/********************** not used at this time
DECLARE SnareDrum = COMPOSITE Drum
    ROTATE <0.0 0.0 90.0>
    SCALE <5.0 4.0 5.0>
END_COMPOSITE
***********************/

#declare TomTom = composite { Drum
   rotate <0.0 0.0 90.0>
   scale <5.0  5.0 5.0>
}

/* Combine a base and a pair of tomtoms into a half-set */
/* Object is still centered at 0,0 at this point */
#declare HalfSet = composite {

   composite { BassDrum      /* Right bass SCALE <8.0 10.0 10.0> */
      rotate <0.0 -90.0 0.0>                   /* Turn it on its end */
   }

   composite { TomTom  /* SCALE <5.0  5.0 5.0> */
      rotate <+30.0 0.0 0.0>                   /* Lean it towards drummer */
      translate <0.0 16.0 0.0>                 /* Raise it up */
      translate <-7.0 0.0 0.0>                 /* Move it left */
   }

   composite { TomTom  /* SCALE <5.0  5.0 5.0> */
      rotate <+30.0 0.0 0.0>                   /* Lean it towards drummer */
      translate <0.0 16.0 0.0>                 /* Raise it up */
      translate <+7.0 0.0 0.0>                 /* Move it right */
   }

   object {          /* tomtom bracket */
      union {
         intersection { Disk_X                  /* Horizontal bar */
            translate <-0.5 0 0>
            scale <4 0.25 0.25>            /* Fits between tomtoms */
            translate <0.0 16.0 0.0 >       /* Raise to top of vertical bar*/
         }

         quadric { Ellipsoid translate <-4 16 0> scale <0.5 1 1> }
         quadric { Ellipsoid translate < 4 16 0> scale <0.5 1 1> }

         intersection { Disk_Y                  /* Vertical bar */
            scale <0.30 6.0 0.30>
            translate <0.0 10.0 0.0 >
         }
         quadric { Ellipsoid scale <1.25 0.5 1.25> translate <0 10 0>}
      }
      color Cyan
      texture { Mirror
         color White
      }
   }
}
