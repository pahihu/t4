#define SP_MIN_PACKET_DATA_SIZE 6
#define SP_MAX_PACKET_DATA_SIZE 510
#define SP_READ_BLOCK_SIZE (SP_MAX_PACKET_DATA_SIZE-3)

#define SPT_BINARY          1
#define SPT_TEXT            2

#define SPM_INPUT           1
#define SPM_OUTPUT          2
#define SPM_APPEND          3
#define SPM_EXISTING_UPDATE 4
#define SPM_NEW_UPDATE      5
#define SPM_APPEND_UPDATE   6

#define SPO_START           1
#define SPO_CURRENT         2
#define SPO_END             3


                                     /*  primary server operation tags  */

#define SP_ZERO        0

#define SP_OPEN       10             /*  filec.c  */
#define SP_CLOSE      11
#define SP_READ       12
#define SP_WRITE      13
#define SP_GETS       14
#define SP_PUTS       15
#define SP_FLUSH      16
#define SP_SEEK       17
#define SP_TELL       18
#define SP_EOF        19
#define SP_FERROR     20
#define SP_REMOVE     21
#define SP_RENAME     22
#define SP_GETBLOCK   23
#define SP_PUTBLOCK   24

#define SP_GETKEY     30             /*  hostc.c  */
#define SP_POLLKEY    31
#define SP_GETENV     32
#define SP_TIME       33
#define SP_SYSTEM     34
#define SP_EXIT       35

#define SP_COMMAND    40             /*  serverc.c  */
#define SP_CORE       41
#define SP_ID         42

#define SP_MSDOS      50             /*  msdos.c  */

/* NOT USED AT INMOS - MAY BE USED BY THIRD PARTY ???
#define SP_SUN        60
#define SP_MSC        61
#define SP_VMS        62
*/

#define SP_ALSYS      100  /* Not used by inmos iserver */
#define SP_KPAR       101  /* Not used by inmos iserver */

                                     /*  INMOS reserves all numbers up to 127  */


#define SP_SUCCESS 0                 /*  operation results  */
#define SP_UNIMPLEMENTED 1
#define SP_ERROR 129

#define ER_LINK_BAD     (-1)         /*  Failure codes for LinkIO functions */
#define ER_LINK_CANT    (-2)
#define ER_LINK_SOFT    (-3)
#define ER_LINK_NODATA  (-4)
#define ER_LINK_NOSYNC  (-5)
#define ER_LINK_BUSY    (-6)
#define ER_NO_LINK      (-7)
#define ER_LINK_SYNTAX  (-8)
#define SUCCEEDED       (1)

#define TERMINATE_OK_EXIT     (iserv_success)
#define TERMINATE_FAIL_EXIT   (iserv_fail)
#define USER_EXIT             (iserv_break)
#define ERROR_FLAG_EXIT       (iserv_errorflag)
#define MISC_EXIT             (iserv_misc)
#define TERMINATE_OTHER_STATUS   (iserv_other)

#define STANDARD_ERROR stdout
                                                   /*  some global inlines  */
#define DEBUG(x) { if (CocoPops)\
                   { fputs("<<", stdout);\
                     printf x; fputs(">>", stdout);\
                     fputc('\n', stdout);\
                     fflush(stdout); } }
