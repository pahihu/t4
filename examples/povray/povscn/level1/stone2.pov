// Persistence Of Vision raytracer version 1.0 sample file.
// The STONEn.POV files demonstrate all textures in STONES.INC

#include "colors.inc"
#include "shapes.inc"
#include "stones.inc"

camera{
   location <0 5 -30>
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
         texture{Grnt27}
         translate <Col1 Row1 Dist>
      }

      object
      {union{Stack}
         texture{Grnt28}
         translate <Col2 Row1 Dist>
      }

      object
      {union{Stack}
         texture{Grnt29}
         translate <Col3 Row1 Dist>
      }
      bounded_by{box{<BC1 BR1b BD0> <BC3 BR1t BD1>}}
   }

   composite
   {
      object
      {union{Stack}
         texture{Grnt0a}
         translate <Col4 Row1 Dist>
      }

      object
      {union{Stack}
         texture{Grnt1a}
         translate <Col5 Row1 Dist>
      }

      object
      {union{Stack}
         texture{Grnt2a}
         translate <Col6 Row1 Dist>
      }
      bounded_by{box{<BC3 BR1b BD0> <BC6 BR1t BD1>}}
   }

   composite
   {
      object
      {union{Stack}
         texture{Grnt3a}
         translate <Col7 Row1 Dist>
      }

      object
      {union{Stack}
         texture{Grnt4a}
         translate <Col8 Row1 Dist>
      }

      object
      {union{Stack}
         texture{Grnt5a}
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
         texture{Grnt6a}
         translate <Col1 Row2 Dist>
      }

      object
      {union{Stack}
         texture{Grnt7a}
         translate <Col2 Row2 Dist>
      }

      object
      {union{Stack}
         texture{Grnt8a}
         translate <Col3 Row2 Dist>
      }
      bounded_by{box{<BC1 BR2b BD0> <BC3 BR2t BD1>}}
   }

   composite
   {
      object
      {union{Stack}
         texture{Grnt9a}
         translate <Col4 Row2 Dist>
      }

      object
      {union{Stack}
         texture{Grnt10a}
         translate <Col5 Row2 Dist>
      }

      object
      {union{Stack}
         texture{Grnt11a}
         translate <Col6 Row2 Dist>
      }
      bounded_by{box{<BC3 BR2b BD0> <BC6 BR2t BD1>}}
   }

   composite
   {
      object
      {union{Stack}
         texture{Grnt12a}
         translate <Col7 Row2 Dist>
      }

      object
      {union{Stack}
         texture{Grnt13a}
         translate <Col8 Row2 Dist>
      }

      object
      {union{Stack}
         texture{Grnt14a}
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
         texture{Grnt15a}
         translate <Col1 Row3 Dist>
      }

      object
      {union{Stack}
         texture{Grnt16a}
         translate <Col2 Row3 Dist>
      }

      object
      {union{Stack}
         texture{Grnt17a}
         translate <Col3 Row3 Dist>
      }
      bounded_by{box{<BC1 BR3b BD0> <BC3 BR3t BD1>}}
   }

   composite
   {
      object
      {union{Stack}
         texture{Grnt18a}
         translate <Col4 Row3 Dist>
      }

      object
      {union{Stack}
         texture{Grnt19a}
         translate <Col5 Row3 Dist>
      }

      object
      {union{Stack}
         texture{Grnt20a}
         translate <Col6 Row3 Dist>
      }
      bounded_by{box{<BC3 BR3b BD0> <BC6 BR3t BD1>}}
   }

   composite
   {
      object
      {union{Stack}
         texture{Grnt21a}
         translate <Col7 Row3 Dist>
      }

      object
      {union{Stack}
         texture{Grnt22a}
         translate <Col8 Row3 Dist>
      }

      object
      {union{Stack}
         texture{Grnt23a}
         translate <Col9 Row3 Dist>
      }
      bounded_by{box{<BC6 BR3b BD0> <BC9 BR3t BD1>}}
   }
   bounded_by{box{<BC1 BR3b BD0> <BC9 BR3t BD1>}}
}

//object{plane{<0 0 1> 1.1}texture{color White}}
