/*****************************************************************

   Program Package - XXX
   Program         - applib[Label Printing]
   Module          - label_lib.h
   Creation Date   - 01-Oct-2018
   Version         - V1.00
   Authors         - Ojal Dev

*****************************************************************/

/* Macro Definations */
#define AP_CHECK_DIGIT_FEDEX    'F'
#define AP_CHECK_DIGIT_UPS      'U'
#define AP_CHECK_DIGIT_XXX      'J'
#define AP_CHECK_DIGIT_EAN_UCC  'E'
#define AP_CHECK_DIGIT_UPCA     'A'
#define AP_JUST_LEFT             1
#define AP_JUST_RIGHT            2
#define AP_JUST_CENTER           4 
#define MAX_TRACK                50
#define MODE_SET                 1
#define MODE_ADD                 2
#define MODE_VAL                 3
#define MAX_STR                  400
#define NO_FMT_FILE              5
#define EXIT_NORMAL              0
#define EMPTY_STRING             "\0"

/* Functions Declaration */
int   cp_write_label ( char r_lbl_type[],char r_printer_type[] );
int   cp_process_label( char r_fname[], char p_type[] );
void  ap_add_chkdigit( char *track_num, char chk_digit_cd );
void  fedex_mod10 ( char *track_num );
void  ups_mod10_cd ( char *track_num );
void  ap_chkdigit( char *str, int len, char chk_digit_cd, int mode, int *val );
void  ap_just(char str[],int len, int just, char pad, char *out_str);
void  ap_code_to_str( char *dst_str, char *str, char code, char delim );
char* strntoupper( char *str );
char* strntolower( char *str );
char* str_split(int pos, char *str, char delim );
char* rpad(char *str, int len, char pad_char);
char* lpad(char *str, int len, char pad_char);
char* substr(char str[], int position, char length);
char* edit_str( char str[], char fmt[] );
char* rtrim(char* string, char junk);
char* ltrim(char *string, char junk);
extern void process_format_line( char format_line[] );
extern void debug( char msg[] );
char* currency_converter( double amount);


