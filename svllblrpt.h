/*****************************************************************************
*
*  Program Package - XXX
*  Program         - applib[Label Printing]
*  Module          - svllblrpt.h
*  Description     - Prints the SVL standard or combination label depending 
*                    on the form code
*
*  Version  Date           Author
*  V1.0     04-Oct-2018    Ojal Dev 
*
******************************************************************************/

/*   System Parameters   */
#define SYS_PARM_DEBUG_FLG      'SVLLBL_D'     // Debug switch
#define SYS_PARM_TOKDELIM       "TD"           // concat with printer type to get the system parameter name for token delimiter
/*   File number 1 is already used by getlblfmt.sqr    */
#define INSERT_FILE_NUM         98
#define DEBUG_FILE_NUM          99
#define DEBUG_FILE_NAME         "svllblrpt.log"    

#define TOKEN_DELIMITER         '^'            // default token delimiter (for Monarch)
#define LBL_TYPE_ERROR          "errorsvllbl"  // Error label type  (future: use sys_parameter)

#define EMSG1_LEN                38            // Length of text
#define MSG_LEN                  38            // Length of message text
#define QTY_LEN                  9
#define CITY_LEN                 18
#define TOADDR2_LEN              22
#define ROUTING_CD_LEN           6
#define TO_STATE_LEN             2

/*    Return status values    */

#define TRUE               1
#define FALSE              0
#define RPT_NORMAL         0
#define RPT_INPUT_ERROR    1
#define RPT_TO_STORE_ERROR 2
#define RPT_CASE_ERROR     3
#define RPT_ITEM_ERROR     4
#define RPT_PRINTER_ERROR  5
#define RPT_STORE_ERROR    6
#define DB_NORMAL          0
#define AP_ERROR           9


/*    Price inspection indicator     */

#define PRICE_IND_FAIL   "P"
#define PRICE_IND_PASS   ""

#define JUST_CENTER 'C'
#define JUST_LEFT   'L'
#define JUST_NONE   ' '
#define JUST_RIGHT  'R'

/*    Format File Tokens (keep in alphabetical order of token text (not macro name)   */

#define TOK_ARRIVE_DATE   "ARRIVE_DATE" /* Case arrival date (mm/dd/yy)  */
#define TOK_ARRIVE_MMDD   "ARRIVE_MMDD" /* Case arrival date (mm/dd)  */
#define TOK_BCASE         "BCASE"       /* Case Id/carton num for bar code (used with XXXCASE/XXXCTN)  */
#define TOK_BCASE20       "BCASE20"     /* 20 digit case for bar code */
#define TOK_BCASE12       "BCASE12"     /* 12 digit carton number for bar code */
#define TOK_BTYPE         "BTYPE"       /* barcode type text string */
#define TOK_CASE          "CASE"        /* Case Id for man readable */
#define TOK_CASE5DIG      "CASE5DIG"    /* Last 5 digits of case id */
#define TOK_CASESTR       "CASESTR"     /* Case formatted as (12) 3 4567890 123456789 0 */
#define TOK_CASESTRC      "CASESTRC"    /* Case formatted and center justified  */
#define TOK_CASESTRR      "CASESTRR"    /* Case formatted and right justified  */
#define TOK_CENTER        "CENTER"      /* Turns on center alignment - must be followed by integer indicating field width */
                                        /* Use TOK_UNJUST to turn off center justification after */
#define TOK_CFGFILE       "CFGFILE"     /* Printer configuration file name */
#define TOK_CRLF          "CRLF"        /* Carriage return line feed */
#define TOK_CTNSTR        "CTNSTR"      /* Carton number formattted as 12-3456-78901-2 */
#define TOK_DATE1         "DATE1"       /* Create date/time of case */
#define TOK_DEBUTIND      "DEBUTIND"    /* Debut indicator */
#define TOK_DEBUTSTR      "DEBUTSTR"    /* Debut string */
#define TOK_DIV           "DIV"         /* Item division */
#define TOK_DVADD         "DVADD"       /* Detail section manual increment */
#define TOK_DVFLD         "DVFLD"       /* Detail section vertical field number for (Monarch printers) */
#define TOK_DVINC         "DVINC"       /* Detail section vertical increment */
#define TOK_DVPOS         "DVPOS"       /* Detail section vertical position */
#define TOK_EMSG1         "EMSG1"       /* Error message text centered and padded to EMSG_LEN */
#define TOK_FLOWSTR       "FLOWSTR"     /* Flow Information VI string */
#define TOK_FSDATE        "FSDATE"      /* Floor set date */
#define TOK_HOT           "HOT"         /* 'HOT' text */
#define TOK_INBDI         "INBDI"       /* Minimum Inbound DI (PO number when order case is used) */
#define TOK_INBDIFLG      "INBDIFLG"    /* Inbound DI FLag (now used to indicate multiple POs in case) */
#define TOK_INFOVI        "INFOVI"      /* Information VI code */
#define TOK_ISDATE        "ISDATE"      /* In-store date (mm/dd/yy) */
#define TOK_XXXCASE       "XXXCASE"     /* Used for SSCC-18 case barcode and text */
#define TOK_XXXCTN        "XXXCTN"      /* Used XXX carton barcode and text */
#define TOK_LEFT          "LEFT"        /* Turns on left justify alignment */
#define TOK_MAXLEN        "MAXLEN"      /* Maximum length of form */
#define TOK_MSG           "MSG"         /* Text message (as is) */
#define TOK_NOINC         "NOINC"       /* No auto increment */
#define TOK_NOJUST        "NOJUST"      /* Turns off justification */
#define TOK_NOWIDTH       "NOWIDTH"     /* Turns off filed width limit */
#define TOK_PONUM         "PONUM"       /* Purchase order */
#define TOK_PRICEIND      "PRICEIND"    /* Pirce inspection inidicator for case or shipment/sdv ('P' indicates failure) */
#define TOK_PUT           "PUT"         /* Put token/value in token array */
#define TOK_RFID          "RFID"        /* RFID Tag indicator */
#define TOK_RI            "RI"          /* Routing Indicator */
#define TOK_RIGHT         "RIGHT"       /* Turns on right justify alignment */
#define TOK_ROUTECD       "ROUTECD"     /* Routing code from RF Inbound Scan */
#define TOK_ROUTECDC      "ROUTECDC"    /* Routing code from RF Inbound Scan (center alignment ) */
#define TOK_ROUTECDCE     "ROUTECDCE"   /* Routing code from RF Inbound Scan (center alignment for even length ) */
#define TOK_ROUTECDCO     "ROUTECDCO"   /* Routing code from RF Inbound Scan (center alignment for odd length ) */
#define TOK_ROUTECDR      "ROUTECDR"    /* Routing code from RF Inbound Scan (right alignment) */
#define TOK_SDV           "SDV"         /* item subdivision */
#define TOK_SKIP          "SKIP"        /* Skip line (dont include in print file) */
#define TOK_STORE         "STORE"       /* Ship to Store number formatted as 1234-5 */
#define TOK_TEXT1         "TEXT1"       /* whse_sdv.svl_text_1 or whse_sdv_lot.svl_text_1 */
#define TOK_TOADDR1       "TOADDR1"     /* Ship to store address 1 */
#define TOK_TOADDR2       "TOADDR2"     /* Ship to city, state (left alignment)   */
#define TOK_TOADDR2C      "TOADDR2C"    /* Ship to city, state (center alignment) */
#define TOK_TOADDR2R      "TOADDR2R"    /* Ship to city, state (right alignment) */
#define TOK_TOSTATE       "TOSTATE"     /* Ship to Store state code (2 bytes) */
#define TOK_TONUM         "TONUM"       /* Ship to Store number */
#define TOK_UPCA          "UPCA"        /* Used to invoke the UPC-A barcode (not used as of IR 5063) */
#define TOK_UNITS         "UNITS"       /* Case quantity (in units) */
#define TOK_USER          "USER"        /* User who created case */
#define TOK_WIDTH         "WIDTH"       /* Limit following field(s) to this number of characters */
#define AP_JUST_LEFT          1  
#define AP_JUST_RIGHT         2 
#define AP_JUST_CENTER        4 
#define AP_SYS_PARAM_FORM_SVLCMB         "FORMSVLC"
#define AP_FORM_CD_SVLCMB_LABEL          "SVLCMB"
#define ITEM_SDV_LEN                          3
#define INFO_VI_CD_LEN                        2
#define CARTON_NUM_LEN                        12
#define CASE_ID_LEN                           20 
#define AP_RFID_STRINGS           "NNone~FRFFA~IRFID~VRFVD"
#define AP_RFID_REQUIRED          'I'
#define AP_RFID_PASS              'V'
#define SQL_LEN                   16000
#define PRINTER_TYP_LEN           40
#define FORM_CD_LEN               40
#define CASE_LEN                  21
#define PRINTER_ID_LEN            10
#define LABEL_TYPE_LEN            40


#define SUBSTITUTE case_id,cases_flag,case_id,cases_flag,src_case_id,cases_flag,case_id,cases_flag,\
                   case_id,case_id,cases_flag,case_id,cases_flag,case_id,cases_flag,case_id,cases_flag,\
                   case_id,cases_flag
                   
/* #define SUBSTITUTE1 case_id,cases_flag,case_id,cases_flag,src_case_id,cases_flag
   #define SUBSTITUTE2 case_id,cases_flag,case_id,case_id,cases_flag,case_id,cases_flag,case_id,cases_flag,case_id,cases_flag,case_id,cases_flag  */
                   
/* Function prototype */

void add_msg(char message[]);
void debug( char msg[] );
void get_printer_info();
void get_token_delimiter();
void generate_error_label( char msg[] );
void generate_label ();
void get_case_info();
void put_token ( char name[], char value[] );
void query_gen( );
void get_syspar ( char name[], char *value );
void process_format_line( char format_line[] );
void get_token_str( char token[], char format_line[], char *token_str );
char* get_token( char name[]);
void insert_file( char file_name[] );
void write_maxlen( );
void insert(char data[], int n);
void write_label();
char* extract_arg( char str[], int pos );
