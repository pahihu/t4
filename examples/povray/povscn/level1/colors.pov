// Persistence Of Vision raytracer version 1.0 sample file.

// By Chris Young
// Illustrates all colors in standard include file "colors.inc"

#include "colors.inc"

#declare Col0 = -36
#declare Col1 = -30
#declare Col2 = -24
#declare Col3 = -18
#declare Col4 = -12
#declare Col5 = -6
#declare Col6 = 0
#declare Col7 = 6
#declare Col8 = 12
#declare Col9 = 18
#declare Col10 = 24
#declare Col11 = 30
#declare Col12 = 36

#declare Row1 = 24
#declare Row2 = 18
#declare Row3 = 12
#declare Row4 = 6
#declare Row5 = 0
#declare Row6 = -6
#declare Row7 = -12
#declare Row8 = -18
#declare Row9 = -24

camera {
   location <Col6  Row5  -65.0>
   right <1.3333 0.0 0.0>
   look_at <Col6 Row5 0.0>
}

/* Light behind viewer postion (pseudo-ambient light) */
object {  
   light_source { <-50.0  50.0  -150.0>
      colour White
   }
}


default {
   texture {
      phong 1 phong_size 100
      ambient .15
      diffuse .7
   }
}

#declare Dist = -4
#declare Radius = 3

object {
   sphere {  <Col0 Row1 Dist> Radius }
   texture {
      color Clear 
   }
}

object {
   sphere {  <Col1 Row1 Dist> Radius }
   texture {
      color White 
   }
}

object {
   sphere {  <Col2 Row1 Dist> Radius }
   texture {
      color Red 
   }
}

object {
   sphere {  <Col3 Row1 Dist> Radius }
   texture {
      color Green 
   }
}

object {
   sphere {  <Col4 Row1 Dist> Radius }
   texture {
      color Blue 
   }
}

object {
   sphere {  <Col5 Row1 Dist> Radius }
   texture {
      color Yellow 
   }
}

object {
   sphere {  <Col6 Row1 Dist> Radius }
   texture {
      color Cyan 
   }
}

object {
   sphere {  <Col7 Row1 Dist> Radius }
   texture {
      color Magenta 
   }
}

object {
   sphere {  <Col8 Row1 Dist> Radius }
   texture {
      color Black 
   }
}

object {
   sphere {  <Col9 Row1 Dist> Radius }
   texture {
      color Aquamarine 
   }
}

object {
   sphere {  <Col10 Row1 Dist> Radius }
   texture {
      color BlueViolet 
   }
}

object {
   sphere {  <Col11 Row1 Dist> Radius }
   texture {
      color Brown 
   }
}

object {
   sphere {  <Col12 Row1 Dist> Radius }
   texture {
      color CadetBlue 
   }
}

object {
   sphere {  <Col0 Row2 Dist> Radius }
   texture {
      color Coral 
   }
}

object {
   sphere {  <Col1 Row2 Dist> Radius }
   texture {
      color CornflowerBlue 
   }
}

object {
   sphere {  <Col2 Row2 Dist> Radius }
   texture {
      color DarkGreen 
   }
}

object {
   sphere {  <Col3 Row2 Dist> Radius }
   texture {
      color DarkOliveGreen 
   }
}

object {
   sphere {  <Col4 Row2 Dist> Radius }
   texture {
      color DarkOrchid 
   }
}

object {
   sphere {  <Col5 Row2 Dist> Radius }
   texture {
      color DarkSlateBlue 
   }
}

object {
   sphere {  <Col6 Row2 Dist> Radius }
   texture {
      color DarkSlateGray 
   }
}

object {
   sphere {  <Col7 Row2 Dist> Radius }
   texture {
      color DarkSlateGrey 
   }
}

object {
   sphere {  <Col8 Row2 Dist> Radius }
   texture {
      color DarkTurquoise 
   }
}

object {
   sphere {  <Col9 Row2 Dist> Radius }
   texture {
      color DimGray 
   }
}

object {
   sphere {  <Col10 Row2 Dist> Radius }
   texture {
      color DimGrey 
   }
}

object {
   sphere {  <Col11 Row2 Dist> Radius }
   texture {
      color Firebrick 
   }
}

object {
   sphere {  <Col12 Row2 Dist> Radius }
   texture {
      color ForestGreen 
   }
}

object {
   sphere {  <Col0 Row3 Dist> Radius }
   texture {
      color Gold 
   }
}

object {
   sphere {  <Col1 Row3 Dist> Radius }
   texture {
      color Goldenrod 
   }
}

object {
   sphere {  <Col2 Row3 Dist> Radius }
   texture {
      color Gray 
   }
}

object {
   sphere {  <Col3 Row3 Dist> Radius }
   texture {
      color GreenYellow 
   }
}

object {
   sphere {  <Col4 Row3 Dist> Radius }
   texture {
      color Grey 
   }
}

object {
   sphere {  <Col5 Row3 Dist> Radius }
   texture {
      color IndianRed 
   }
}

object {
   sphere {  <Col6 Row3 Dist> Radius }
   texture {
      color Khaki 
   }
}

object {
   sphere {  <Col7 Row3 Dist> Radius }
   texture {
      color LightBlue 
   }
}

object {
   sphere {  <Col8 Row3 Dist> Radius }
   texture {
      color LightGray 
   }
}

object {
   sphere {  <Col9 Row3 Dist> Radius }
   texture {
      color LightGrey 
   }
}

object {
   sphere {  <Col10 Row3 Dist> Radius }
   texture {
      color LightSteelBlue 
   }
}

object {
   sphere {  <Col11 Row3 Dist> Radius }
   texture {
      color LimeGreen 
   }
}

object {
   sphere {  <Col12 Row3 Dist> Radius }
   texture {
      color Maroon 
   }
}

object {
   sphere {  <Col0 Row4 Dist> Radius }
   texture {
      color MediumAquamarine 
   }
}

object {
   sphere {  <Col1 Row4 Dist> Radius }
   texture {
      color MediumBlue 
   }
}

object {
   sphere {  <Col2 Row4 Dist> Radius }
   texture {
      color MediumForestGreen 
   }
}

object {
   sphere {  <Col3 Row4 Dist> Radius }
   texture {
      color MediumGoldenrod 
   }
}

object {
   sphere {  <Col4 Row4 Dist> Radius }
   texture {
      color MediumOrchid 
   }
}

object {
   sphere {  <Col5 Row4 Dist> Radius }
   texture {
      color MediumSeaGreen 
   }
}

object {
   sphere {  <Col6 Row4 Dist> Radius }
   texture {
      color MediumSlateBlue 
   }
}

object {
   sphere {  <Col7 Row4 Dist> Radius }
   texture {
      color MediumSpringGreen 
   }
}

object {
   sphere {  <Col8 Row4 Dist> Radius }
   texture {
      color MediumTurquoise 
   }
}

object {
   sphere {  <Col9 Row4 Dist> Radius }
   texture {
      color MediumVioletRed 
   }
}

object {
   sphere {  <Col10 Row4 Dist> Radius }
   texture {
      color MidnightBlue 
   }
}

object {
   sphere {  <Col11 Row4 Dist> Radius }
   texture {
      color Navy 
   }
}

object {
   sphere {  <Col12 Row4 Dist> Radius }
   texture {
      color NavyBlue 
   }
}

object {
   sphere {  <Col0 Row5 Dist> Radius }
   texture {
      color Orange 
   }
}

object {
   sphere {  <Col1 Row5 Dist> Radius }
   texture {
      color OrangeRed 
   }
}

object {
   sphere {  <Col2 Row5 Dist> Radius }
   texture {
      color Orchid 
   }
}

object {
   sphere {  <Col3 Row5 Dist> Radius }
   texture {
      color PaleGreen 
   }
}

object {
   sphere {  <Col4 Row5 Dist> Radius }
   texture {
      color Pink 
   }
}

object {
   sphere {  <Col5 Row5 Dist> Radius }
   texture {
      color Plum 
   }
}

object {
   sphere {  <Col6 Row5 Dist> Radius }
   texture {
      color Salmon 
   }
}

object {
   sphere {  <Col7 Row5 Dist> Radius }
   texture {
      color SeaGreen 
   }
}

object {
   sphere {  <Col8 Row5 Dist> Radius }
   texture {
      color Sienna 
   }
}

object {
   sphere {  <Col9 Row5 Dist> Radius }
   texture {
      color SkyBlue 
   }
}

object {
   sphere {  <Col10 Row5 Dist> Radius }
   texture {
      color SlateBlue 
   }
}

object {
   sphere {  <Col11 Row5 Dist> Radius }
   texture {
      color SpringGreen 
   }
}

object {
   sphere {  <Col12 Row5 Dist> Radius }
   texture {
      color SteelBlue 
   }
}

object {
   sphere {  <Col0 Row6 Dist> Radius }
   texture {
      color Tan 
   }
}

object {
   sphere {  <Col1 Row6 Dist> Radius }
   texture {
      color Thistle 
   }
}

object {
   sphere {  <Col2 Row6 Dist> Radius }
   texture {
      color Turquoise 
   }
}

object {
   sphere {  <Col3 Row6 Dist> Radius }
   texture {
      color Violet 
   }
}

object {
   sphere {  <Col4 Row6 Dist> Radius }
   texture {
      color VioletRed 
   }
}

object {
   sphere {  <Col5 Row6 Dist> Radius }
   texture {
      color Wheat 
   }
}

object {
   sphere {  <Col6 Row6 Dist> Radius }
   texture {
      color YellowGreen 
   }
}

object {
   sphere {  <Col7 Row6 Dist> Radius }
   texture {
      color SummerSky 
   }
}

object {
   sphere {  <Col8 Row6 Dist> Radius }
   texture {
      color RichBlue 
   }
}

object {
   sphere {  <Col9 Row6 Dist> Radius }
   texture {
      color Brass 
   }
}

object {
   sphere {  <Col10 Row6 Dist> Radius }
   texture {
      color Copper 
   }
}

object {
   sphere {  <Col11 Row6 Dist> Radius }
   texture {
      color Bronze 
   }
}

object {
   sphere {  <Col12 Row6 Dist> Radius }
   texture {
      color Bronze2 
   }
}

object {
   sphere {  <Col0 Row7 Dist> Radius }
   texture {
      color Silver 
   }
}

object {
   sphere {  <Col1 Row7 Dist> Radius }
   texture {
      color BrightGold 
   }
}

object {
   sphere {  <Col2 Row7 Dist> Radius }
   texture {
      color OldGold 
   }
}

object {
   sphere {  <Col3 Row7 Dist> Radius }
   texture {
      color Feldspar 
   }
}

object {
   sphere {  <Col4 Row7 Dist> Radius }
   texture {
      color Quartz 
   }
}

object {
   sphere {  <Col5 Row7 Dist> Radius }
   texture {
      color Mica 
   }
}

object {
   sphere {  <Col6 Row7 Dist> Radius }
   texture {
      color VLightGrey 
   }
}

object {
   sphere {  <Col7 Row7 Dist> Radius }
   texture {
      color NeonPink 
   }
}

object {
   sphere {  <Col8 Row7 Dist> Radius }
   texture {
      color DarkPurple 
   }
}

object {
   sphere {  <Col9 Row7 Dist> Radius }
   texture {
      color NeonBlue 
   }
}

object {
   sphere {  <Col10 Row7 Dist> Radius }
   texture {
      color CoolCopper 
   }
}

object {
   sphere {  <Col11 Row7 Dist> Radius }
   texture {
      color MandarinOrange 
   }
}

object {
   sphere {  <Col12 Row7 Dist> Radius }
   texture {
      color LightWood 
   }
}

object {
   sphere {  <Col0 Row8 Dist> Radius }
   texture {
      color MediumWood 
   }
}

object {
   sphere {  <Col1 Row8 Dist> Radius }
   texture {
      color DarkWood 
   }
}

object {
   sphere {  <Col2 Row8 Dist> Radius }
   texture {
      color SpicyPink 
   }
}

object {
   sphere {  <Col3 Row8 Dist> Radius }
   texture {
      color SemiSweetChoc 
   }
}

object {
   sphere {  <Col4 Row8 Dist> Radius }
   texture {
      color BakersChoc 
   }
}

object {
   sphere {  <Col5 Row8 Dist> Radius }
   texture {
      color Flesh 
   }
}

object {
   sphere {  <Col6 Row8 Dist> Radius }
   texture {
      color NewTan 
   }
}

object {
   sphere {  <Col7 Row8 Dist> Radius }
   texture {
      color NewMidnightBlue 
   }
}

object {
   sphere {  <Col8 Row8 Dist> Radius }
   texture {
      color VeryDarkBrown 
   }
}

object {
   sphere {  <Col9 Row8 Dist> Radius }
   texture {
      color DarkBrown 
   }
}

object {
   sphere {  <Col10 Row8 Dist> Radius }
   texture {
      color DarkTan 
   }
}

object {
   sphere {  <Col11 Row8 Dist> Radius }
   texture {
      color GreenCopper 
   }
}

object {
   sphere {  <Col12 Row8 Dist> Radius }
   texture {
      color DkGreenCopper 
   }
}

object {
   sphere {  <Col0 Row9 Dist> Radius }
   texture {
      color DustyRose 
   }
}

object {
   sphere {  <Col1 Row9 Dist> Radius }
   texture {
      color HuntersGreen 
   }
}

object {
   sphere {  <Col2 Row9 Dist> Radius }
   texture {
      color Scarlet 
   }
}
/*
object {
    sphere {  <Col3 Row9 Dist> Radius }
    texture {
color 
    }
}

object {
    sphere {  <Col4 Row9 Dist> Radius }
    texture {
color 
    }
}

object {
    sphere {  <Col5 Row9 Dist> Radius }
    texture {
color 
    }
}

object {
    sphere {  <Col6 Row9 Dist> Radius }
    texture {
color 
    }
}

object {
    sphere {  <Col7 Row9 Dist> Radius }
    texture {
color 
    }
}

object {
    sphere {  <Col8 Row9 Dist> Radius }
    texture {
color 
    }
}

object {
    sphere {  <Col9 Row9 Dist> Radius }
    texture {
color 
    }
}

object {
    sphere {  <Col10 Row9 Dist> Radius }
    texture {
color 
    }
}

object {
    sphere {  <Col11 Row9 Dist> Radius }
    texture {
color 
    }
}

object {
    sphere {  <Col12 Row9 Dist> Radius }
    texture {
color 
    }
}

*/
