$ define sys sys$include:
$ cc/nolist BEZIER         
$ cc/nolist BLOB           
$ cc/nolist BOXES          
$ cc/nolist COLOUR         
$ cc/nolist CSG            
$ cc/nolist DUMP           
$ cc/nolist GIF            
$ cc/nolist GIFDECOD       
$ cc/nolist HFIELD         
$ cc/nolist IFF            
$ cc/nolist LIGHTING       
$ cc/nolist MATRICES       
$ cc/nolist OBJECTS        
$ cc/nolist PARSE          
$ cc/nolist PLANES         
$ cc/nolist POINT          
$ cc/nolist POLY           
$ cc/nolist POVRAY         
$ cc/nolist PRIOQ          
$ cc/nolist QUADRICS       
$ cc/nolist RAW            
$ cc/nolist RAY            
$ cc/nolist RENDER         
$ cc/nolist SPHERES        
$ cc/nolist TARGA          
$ cc/nolist TEXTURE        
$ cc/nolist TOKENIZE       
$ cc/nolist TRIANGLE       
$ cc/nolist TXTBUMP        
$ cc/nolist TXTCOLOR       
$ cc/nolist TXTMAP         
$ cc/nolist TXTTEST        
$ cc/nolist VAX            
$ cc/nolist VECT           
$ cc/nolist VIEWPNT        
$ link/nomap/exe=povray -
POVRAY         , -
BEZIER         , -
BLOB           , -
BOXES          , -
COLOUR         , -
CSG            , -
DUMP           , -
GIF            , -
GIFDECOD       , -
HFIELD         , -
IFF            , -
LIGHTING       , -
MATRICES       , -
OBJECTS        , -
PARSE          , -
PLANES         , -
POINT          , -
POLY           , -
PRIOQ          , -
QUADRICS       , -
RAW            , -
RAY            , -
RENDER         , -
SPHERES        , -
TARGA          , -
TEXTURE        , -
TOKENIZE       , -
TRIANGLE       , -
TXTBUMP        , -
TXTCOLOR       , -
TXTMAP         , -
TXTTEST        , -
VAX            , -
VECT           , -
VIEWPNT        , -
sys$input:/option
sys$share:vaxcrtl/share
$ exit
