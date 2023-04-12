// Persistence Of Vision raytracer version 1.0 sample file.
// The TEXTUREn.POV files demonstrate all textures in TEXTURES.INC

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"

camera 
 {location <0 5 -30>
  direction <0 0  1.35>
  up  <0 1 0>
  right <1.33333 0 0>
  look_at <0 5 0>
 }

object {light_source {<0 0 -1000> color LightGray}}

object {light_source {<150 50 -200> color LightGray}}

#declare Stack =
 union
  {box{UnitBox}
   intersection{Disk_Y translate <0 2 0>}
   sphere{<0 4 0> 1}
  }

#declare Dist = 0

#declare BD0  = -1.1
#declare BD1  =  1.1


#declare BR1t  = 15.6
#declare BR1b  =  9.4
#declare BR2t  =  8.6
#declare BR2b  =  2.4
#declare BR3t  =  1.6
#declare BR3b  = -4.6

#declare Row1 = 10.5
#declare Row2 = 3.5
#declare Row3 = -3.5


#declare BC1   = -13.2
#declare BC3   = -4.5
#declare BC6   =  4.5
#declare BC9   =  13.2

#declare Col1  = -12
#declare Col2  = -9 
#declare Col3  = -6 
#declare Col4  = -3 
#declare Col5  = 0  
#declare Col6  = 3  
#declare Col7  = 6  
#declare Col8  = 9  
#declare Col9  = 12 

composite
 {
  composite
   {
    object
     {union{Stack}
      texture {DMFLightOak}
      translate <Col1 Row1 Dist>
     }
    
    object
     {union{Stack}
      texture {DMFDarkOak}
      translate <Col2 Row1 Dist>
     }
    
    object
     {union{Stack}
      texture {Yellow_Pine}
      translate <Col3 Row1 Dist>
     }
    bounded_by{box{<BC1 BR1b BD0> <BC3 BR1t BD1>}}
   }
    
  composite
   {
    object
     {union{Stack}
      texture {Rosewood}
      translate <Col4 Row1 Dist>
     }
    
    object
     {union{Stack}
      texture {Sandalwood}
      translate <Col5 Row1 Dist>
     }
    
    object
     {union{Stack}
      texture {Dull color Blue}
      translate <Col6 Row1 Dist>
     }
    bounded_by{box{<BC3 BR1b BD0> <BC6 BR1t BD1>}}
   }
    

  composite
   {
    object
     {union{Stack}
      texture {Shiny color Blue}
      translate <Col7 Row1 Dist>
     }
    
    object
     {union{Stack}
      texture {Phong_Dull color Blue}
      translate <Col8 Row1 Dist>
     }
    
    object
     {union{Stack}
      texture {Phong_Shiny color Blue}
      translate <Col9 Row1 Dist>
     }
    bounded_by{box{<BC6 BR1b BD0> <BC9 BR1t BD1>}}
   }
  bounded_by{box{<BC1 BR1b BD0> <BC9 BR1t BD1>}}
 }
    
composite
 {
  composite
   {
    object
     {union{Stack}
      texture {Glossy color Blue}
      translate <Col1 Row2 Dist>
     }
    
    object
     {union{Stack}
      texture {Phong_Glossy color Blue}
      translate <Col2 Row2 Dist>
     }
    
    object
     {union{Stack}
      texture {Luminous color Blue}
      translate <Col3 Row2 Dist>
     }
    bounded_by{box{<BC1 BR2b BD0> <BC3 BR2t BD1>}}
   }
    
  composite
   {
    object
     {union{Stack}
      texture {Mirror}
      translate <Col4 Row2 Dist>
     }
    
    object
     {union{Stack}
      texture {Glass}
      translate <Col5 Row2 Dist>
     }
    
    object
     {union{Stack}
      texture {Glass2}
      translate <Col6 Row2 Dist>
     }
    bounded_by{box{<BC3 BR2b BD0> <BC6 BR2t BD1>}}
   }
    
  composite
   {
    object
     {union{Stack}
      texture {Glass3}
      translate <Col7 Row2 Dist>
     }
    
    object
     {union{Stack}
      texture {Green_Glass}
      translate <Col8 Row2 Dist>
     }
    
    object
     {union{Stack}
      texture {Chrome_Texture}
      translate <Col9 Row2 Dist>
     }
    bounded_by{box{<BC6 BR2b BD0> <BC9 BR2t BD1>}}
   }
  bounded_by{box{<BC1 BR2b BD0> <BC9 BR2t BD1>}}
 }
    
composite
 {
  composite
   {
    object
     {union{Stack}
      texture {Metal}
      translate <Col1 Row3 Dist>
     }
    
    object
     {union{Stack}
      texture {Brass_Texture  }
      translate <Col2 Row3 Dist>
     }
    
    object
     {union{Stack}
      texture {Gold_Texture   }
      translate <Col3 Row3 Dist>
     }
    bounded_by{box{<BC1 BR3b BD0> <BC3 BR3t BD1>}}
   }
    
  composite
   {
    object
     {union{Stack}
      texture {Bronze_Texture }
      translate <Col4 Row3 Dist>
     }
    
    object
     {union{Stack}
      texture {Copper_Texture }
      translate <Col5 Row3 Dist>
     }
    
    object
     {union{Stack}
      texture {Silver_Texture }
      translate <Col6 Row3 Dist>
     }
    bounded_by{box{<BC3 BR3b BD0> <BC6 BR3t BD1>}}
   }
    
  composite
   {
    object
     {union{Stack}
      texture {Brass_Valley}
      translate <Col7 Row3 Dist>
     }
    
    object
     {union{Stack}
      texture {Rusty_Iron}
      translate <Col8 Row3 Dist>
     }
    
    object
     {union{Stack}
      texture {Rust }
      translate <Col9 Row3 Dist>
     }

    bounded_by{box{<BC6 BR3b BD0> <BC9 BR3t BD1>}}
   }
  bounded_by{box{<BC1 BR3b BD0> <BC9 BR3t BD1>}}
 }
    
object{plane{<0 0 1> 1.1}texture{color White}}
