// Persistence Of Vision raytracer version 1.0 sample file.

// Include file for steiner.pov, grafbic.pov
// by Alexander Enzmann

// Declare the sizes of cylinders that represent coordinate axes 
#declare Axes_X_Radii = <1.0  0.01 0.01>
#declare Axes_Y_Radii = <0.01 1.0  0.01>
#declare Axes_Z_Radii = <0.01 0.01 1.0>

// Define the colors of the coordinate axes 
#declare X_Axis_Color = color green 1 blue 1
#declare Y_Axis_Color = color red 1 green 1
#declare Z_Axis_Color = color red 1 blue 1

// Define the ways that the coordinate axes can be translated 
#declare Translate_X_Axis_Up       = < 0.0  1.0  0.0>
#declare Translate_X_Axis_Down     = < 0.0 -1.0  0.0>
#declare Translate_X_Axis_Forward  = < 0.0  0.0  1.0>
#declare Translate_X_Axis_Back     = < 0.0  0.0 -1.0>
#declare Translate_Y_Axis_Left     = < 1.0  0.0  0.0>
#declare Translate_Y_Axis_Right    = <-1.0  0.0  0.0>
#declare Translate_Y_Axis_Forward  = < 0.0  0.0  1.0>
#declare Translate_Y_Axis_Back     = < 0.0  0.0 -1.0>
#declare Translate_Z_Axis_Up       = < 0.0  1.0  0.0>
#declare Translate_Z_Axis_Down     = < 0.0 -1.0  0.0>
#declare Translate_Z_Axis_Left     = < 1.0  0.0  0.0>
#declare Translate_Z_Axis_Right    = <-1.0  0.0  0.0>

// Define coordinate axes 
#declare Axes =
composite {
   object {
      quadric { Cylinder_X
         scale Axes_X_Radii
         texture { color X_Axis_Color }
      }
   }
   object {
      quadric { Cylinder_Y
         scale Axes_Y_Radii
         texture { color Y_Axis_Color }
      }
   }
   object {
      quadric { Cylinder_Z
         scale Axes_Z_Radii
         texture { color Z_Axis_Color }
      }
   }
}

// Translate the axis up, down, right, ...,  to form the three
// planes that divide the standard octants. 
#declare Axis_Box =
composite {
   object {
      quadric { Cylinder_X
         scale Axes_X_Radii
         translate Translate_X_Axis_Up
         texture { color X_Axis_Color }
      }
   }

   object {
      quadric { Cylinder_X
         scale Axes_X_Radii
         translate Translate_X_Axis_Down
         texture { color X_Axis_Color }
      }
   }

   object {
      quadric { Cylinder_Y
         scale Axes_Y_Radii
         translate Translate_Y_Axis_Forward
         texture { color Y_Axis_Color }
      }
   }

   object {
      quadric { Cylinder_Y
         scale Axes_Y_Radii
         translate Translate_Y_Axis_Back
         texture { color Y_Axis_Color }
      }
   }

   object {
      quadric { Cylinder_Z
         scale Axes_Z_Radii
         translate Translate_Z_Axis_Left
         texture { color Z_Axis_Color }
      }
   }

   object {
      quadric { Cylinder_Z
         scale Axes_Z_Radii
         translate Translate_Z_Axis_Right
         texture { color Z_Axis_Color }
      }
   }
}

