// Persistence Of Vision raytracer version 1.0 sample file.

// non-Standard shapes #include file
// 

#declare Ellipsoid = quadric {
   <1.0 1.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   -1.0
}

#declare light = object {
   light_source { <0 0 0>
      color White
   }
}




#declare USphere =
intersection { quadric { Ellipsoid scale <1 1 1> }
   plane { <0 1 0> 0 inverse }
}


#declare DSphere =
intersection { quadric { Ellipsoid scale <1 1 1> }
   plane { <0 -1 0> 0 inverse }
}



#declare Cylinder_X = quadric {
   <0.0 1.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   -1.0
}


#declare Cylinder_Y = quadric {
   <1.0 0.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   -1.0
}


#declare Cylinder_Z = quadric {
   <1.0 1.0 0.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   -1.0
}


#declare QCone_X = quadric {
   <-1.0 1.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   0.0
}


#declare QCone_Y = quadric {
   <1.0 -1.0 1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   0.0
}


#declare QCone_Z = quadric {
   <1.0 1.0 -1.0>
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   0.0
}

#declare DCone = /*---------CONE POINT DOWN /POINT at 000--------*/
intersection {
   quadric { QCone_Y scale <1 1 1> }
   plane { <0 -1 0> 0 }
   plane { <0 1 0> 1  }
}

#declare UCone = /*---------CONE POINT UP /POINT at 000--------*/
intersection {
   quadric { QCone_Y scale <1 1 1> }
   plane { <0 1 0> 0   }
   plane { <0 1 0> -1 inverse }
}

#declare BiCone = /*----2 CONES POINTS TOGETHER /POINT at 000---*/
intersection {
   quadric { QCone_Y scale <1 1 1> }
   plane { <0 1 0> 1   }
   plane { <0 1 0> -1 inverse }
}



#declare Plane_YZ = quadric {
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   <1.0 0.0 0.0>
   0.0
}


#declare Plane_XZ = quadric {
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   <0.0 1.0 0.0>
   0.0
}


#declare Plane_XY = quadric {
   <0.0 0.0 0.0>
   <0.0 0.0 0.0>
   <0.0 0.0 1.0>
   0.0
}


/* y^2 + z^2 - x = 0 */
#declare Paraboloid_X = quadric {
   <0.0  1.0  1.0>
   <0.0  0.0  0.0>
   <-1.0 0.0  0.0>
   0.0
}


/* x^2 + z^2 - y = 0 */
#declare Paraboloid_Y = quadric {
   <1.0  0.0  1.0>
   <0.0  0.0  0.0>
   <0.0 -1.0  0.0>
   0.0
}


/* x^2 + y^2 - z = 0 */
#declare Paraboloid_Z = quadric {
   <1.0  1.0  0.0>
   <0.0  0.0  0.0>
   <0.0  0.0 -1.0>
   0.0
}


/* y - x^2 + z^2 = 0 */
#declare Hyperboloid = quadric {
   < -1.0  0.0  1.0>
   <  0.0  0.0  0.0>
   <  0.0  1.0  0.0>
   0.0
}


#declare Hyperboloid_Y = quadric {  /* Vertical hyperboloid */
   <1.0 -1.0  1.0>          /*                      */
   <0.0  0.0  0.0>          /*            \   /     */
   <0.0  0.0  0.0>          /* Like this:  ) (      */
   -1.0                     /*            /   \     */
}


#declare Cube = intersection {
   plane { <0.0 0.0 1.0> 1.0 }
   plane { <0.0 0.0 -1.0> 1.0 }
   plane { <0.0 1.0 0.0> 1.0 }
   plane { <0.0 -1.0 0.0> 1.0 }
   plane { <1.0 0.0 0.0> 1.0 }
   plane { <-1.0 0.0 0.0> 1.0 }
}


#declare Tetrahedron = intersection {
   plane { <0.0 -1.0 0.0> 1.0 }
   plane { <0.0 0.0 1.0> 1.0 rotate <-30.0 0.0 0.0> }
   plane { <1.0 0.0 0.0> 1.0 rotate <0.0 30.0 30.0> }
   plane { <-1.0 0.0 0.0> 1.0 rotate <0.0 -30.0 -30.0> }
}


#declare Disk_X = intersection {       /* Capped cylinder, Length in x axis */
   quadric { Cylinder_X }
   plane { <1.0 0.0 0.0> 0.0 inverse }
   plane { <1.0 0.0 0.0> 1.0 }
}


#declare Disk_Y = intersection {       /* Capped cylinder, Length in y axis */
   quadric { Cylinder_Y }
   plane { <0.0 1.0 0.0> 0.0 inverse }
   plane { <0.0 1.0 0.0> 1.0 }
}


#declare Disk_Z = intersection {       /* Capped cylinder, Length in z axis */
   quadric { Cylinder_Z }
   plane { <0.0 0.0 1.0> 0.0 inverse }
   plane { <0.0 0.0 1.0> 1.0 }
}


#declare Hexagon = intersection {	        /* Hexagonal Solid, axis along x */
   plane { <0.0 0.0 1.0> 1.0 }   /* Rotate 90 in z axis to stand up */
   plane { <0.0 0.0 1.0> 1.0 rotate <60.0 0.0 0.0> }
   plane { <0.0 0.0 1.0> 1.0 rotate <120.0 0.0 0.0> }
   plane { <0.0 0.0 1.0> 1.0 rotate <180.0 0.0 0.0> }
   plane { <0.0 0.0 1.0> 1.0 rotate <240.0 0.0 0.0> }
   plane { <0.0 0.0 1.0> 1.0 rotate <300.0 0.0 0.0> }
   plane { <1.0 0.0 0.0> 1.0 }
   plane { <1.0 0.0 0.0> -1.0 inverse }
}

#declare Rhomboid =			/* Three Dimensional 4-Sided Diamond */
intersection {
   plane { <-1.0  0.0  0.0>  1.0 rotate <0.0 0.0 -30.0> }
   plane { < 1.0  0.0  0.0>  1.0 rotate <0.0 0.0 -30.0> }
   plane { < 0.0  0.0  1.0>  1.0 }
   plane { < 0.0  0.0 -1.0>  1.0 }
   plane { < 0.0  1.0  0.0>  1.0 }
   plane { < 0.0 -1.0  0.0>  1.0 }
}
