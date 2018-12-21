/*****************************************************************

   Program Package - XXX
   Program         - applib[Label Printing]
   Module          - label_lib.c
   Creation Date   - 01-Oct-2018
   Version         - V1.00
   Authors         - Ojal Dev

*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "label_lib.h"


void ap_add_chkdigit( char *track_num, char chk_digit_cd )
{
    
    /*              Local Variables		 */
    char temp_track_num[MAX_TRACK];   
    int  len = strlen( track_num ) + 1;
    int  val = 1;
    
    snprintf(temp_track_num, (MAX_TRACK * sizeof(char)) , "%s", track_num);
    
    switch (chk_digit_cd)
    {
        case AP_CHECK_DIGIT_FEDEX :
                    fedex_mod10( temp_track_num );
                    break;
        case AP_CHECK_DIGIT_UPS :
                    ups_mod10_cd( temp_track_num );
                    break;
        case AP_CHECK_DIGIT_XXX     :
        case AP_CHECK_DIGIT_EAN_UCC :
        case AP_CHECK_DIGIT_UPCA    :
                    ap_chkdigit( temp_track_num, len, chk_digit_cd, MODE_ADD, &val );
                    break;
        default:
                    break;
    }    
    
}


/**************************************************************************************************************
* The track number passed to this function is not the '96' bar code it is the shipper id + package number     *
* The track number passed to the function must not include a place holder value for the check digit.          *
*                                                                                                             *
* The following is an excerpt from the FedEx Barcode and Label Layout Specification                           *
* Digit positions are labeled from right to left                                                              *
* Digit 1 is the check character                                                                              *
* Digits 16 through 22 are not included (i.e. stop after position 15 is processed)                            *
*                                                                                                             *
* 1. Starting from position 2 add up the values of the even numbered positions                                *
* 2. Multiply the result of step 1 by three                                                                   *
* 3. Starting from position 3, add up the values of the odd numbered positions.                               *
* 4. Add the result of step Step 2. To the result of Step 3.                                                  *
* 5. Determine the smallest number which when added to the number from Step 4 results in a multiple of 10.    *
*    This is the digit.                                                                                       *
*                                                                                                             *
*    DATA        1 2 3 4 5 6 7 1 2 3 4 5 6 7 c <- check digit is not present in our string                    *
*    Odd/Even    O E O E O E O E O E O E O E O                                                                *
*    Position    5 4 3 2 1 0 9 8 7 6 5 4 3 2 1                                                                *
*                                                                                                             *
**************************************************************************************************************/

void fedex_mod10 ( char *track_num )
{
    
    /*              Local Variables		 */
    int x       = strlen(track_num) - 1;
    int odd     = 0;
    int temp    = 0;
    int total   = 0;
    int pos     = 2;
    int chk_dgt = 0;
    
    char temp_track_num[MAX_TRACK]; 
    char tmp_hold[2];
    
    snprintf(temp_track_num, (MAX_TRACK * sizeof(char)), "%s", track_num);
    
    /* We start at the right - digit number 2 */
    while ( pos <= 15 )
    {
        snprintf(tmp_hold, sizeof(tmp_hold), "%c", temp_track_num[x]);
        temp = atoi( tmp_hold );
        if ( odd == 1 )
            odd = 0;
        else
        {
            temp = temp * 3;
            odd  = 1;
        }
        
        pos++;
        x--;            
    }
    
    chk_dgt = total % 10;
    if ( chk_dgt > 0 )
        chk_dgt = 10 - chk_dgt;
    
    snprintf( temp_track_num, (MAX_TRACK * sizeof(char)), "%s%d", temp_track_num, chk_dgt);
    snprintf( track_num, (MAX_TRACK * sizeof(char)), "%s", temp_track_num);
}


/************************************************************************************************
*  From -- Guide to Bar Coding with UPS Online (For Customers Generating Bar                    *
*	   Code Labels) version 5:                                                                  *
*                                                                                               *
*  UPS Check Digit Calculation                                                                  *
*                                                                                               *
*  1. Ignore the leading '1Z'                                                                   *
*  2. Convert all alpha charactors to numeric equivalents (see cross-reference                  *
*     table below)                                                                              *
*  3. From left, add all odd positions (Note start with 1)                                      *
*  4. From left, add all even positions and multiply by 2                                       *
*  5. Add results of steps 3 and 4                                                              *
*  6. Subtract result from next highest multiple of 10                                          *
*  7. The remainder is your check digit                                                         *
*                                                                                               *
*  Alpha to Numeric Cross Reference                                                             *
*                                                                                               *
*	 A = 2	*E = 6	 I = 0	 M = 4	 Q = 8	 U = 2	 Y = 6                                      *
*	 B = 3	 F = 7	 J = 4	 N = 5	 R = 9	 V = 3	 Z = 7                                      *
*	 C = 4	 G = 8	 K = 4	 O = 6	 S = 0	*W = 4                                              *
*	 D = 5	 H = 9	 L = 4	 P = 7	 T = 1	*X = 5                                              *
*                                                                                               *
*   *Letter that could currently appear in UPS Shipper Number field                             *
*                                                                                               *
*  Example:                                                                                     *
*  Tracking Number without check digit:		1Z	913  E47  03  1463   850                        *
*  1. Ingore the leading '1Z'				913  E47  03  1463   850                            *
*  2. Convert all alpha charactors			913  647  03  1463   850                            *
*  3. Add all odd positions						      31                                        *
*  4. Add all even position and multiply by 2			     29 * 2 = 58                        *
*  5. Add result of steps 2 and 3				    31 + 58 = 89                                *
*  6. Subtract result from next multiple				 90 - 89                                *
*  7. Remainder								       1                                            *
*                                                                                               *
*  Note if the remainder is 10, the check digit is 0                                            *
*                                                                                               *
************************************************************************************************/

void ups_mod10_cd ( char *track_num )
{
    /*              Local Variables		 */
    int track_num_len = strlen( track_num );
    int x             = 2;
    int temp          = 0;
    int total         = 0;
    int remainder     = 0;
    int chk_dgt       =-1;
    int ascii_A       ='A';
    int ascii_Z       ='Z';
    int ascii_zero    ='0';
    int track_ascii   = 0;
    int num_type      = 0;
    
    char temp_track_num[MAX_TRACK]; 
    
    snprintf(temp_track_num, (MAX_TRACK * sizeof(char)), "%s", track_num);
    
    while ( x < track_num_len )
    {
        track_ascii = temp_track_num[x];
        if ( track_ascii >= ascii_A && track_ascii <= ascii_Z )
            temp = ( (track_ascii - ( ascii_A + 2 ) ) % 10 );
        else
            temp = track_ascii - ascii_zero;
        
        num_type = x % 2;
        
        if ( num_type == 0 )
            temp = temp * 2;
        
        total += temp;
        x++;
    }
    
    remainder = total % 10;
    
    if ( remainder == 0 )
        chk_dgt = remainder;
    else
        chk_dgt = 10 - remainder ;
    
    snprintf( temp_track_num, (MAX_TRACK * sizeof(char)), "%s%d", temp_track_num, chk_dgt);
    snprintf( track_num, (MAX_TRACK * sizeof(char)), "%s", temp_track_num);    
}



void ap_chkdigit( char *str, int len, char chk_digit_cd, int mode, int *val )
{
    
    /*              Local Variables		 */
    char chk_dgt;
    int error           = 0;     /* 0 stands for FALSE */
    int odd             = 1;     /* 1 stands for TRUE  */
    int temp            = 0;
    int total           = 0;
    int mult            = 0;
    int x               = len - 2; /* 1 is replaced by 2 as in C language index starts from 0*/
    
    char temp_str[MAX_TRACK];
    char tmp[MAX_TRACK];
    char tmp_hold[2];
    
    
    snprintf(temp_str, (MAX_TRACK * sizeof(char)), "%s", str);
    
    switch ( chk_digit_cd )
    {
        case AP_CHECK_DIGIT_XXX :
            /* From right to left, odds are multiplied by 2, but if > 4, need adjustment (-9) */
            while ( x >= 0 )
            {
                snprintf(tmp_hold, sizeof(tmp_hold), "%c", temp_str[x]);
                temp = atoi( tmp_hold );
                if ( odd == 1 )
                {
                    odd = 0;
                    total = total + ( 2 * temp );
                    if ( temp > 4 )
                        total -= 9;
                }
                else
                {
                    odd = 1;
                    total += temp;
                }
                x--;
            }
            break;
         
        case AP_CHECK_DIGIT_EAN_UCC :
        case AP_CHECK_DIGIT_UPCA    :
            /* From right to left, odds are multiplied by 3 */
            while ( x >= 0 )
            {
                snprintf(tmp_hold, sizeof(tmp_hold), "%c", temp_str[x]);
                temp = atoi( tmp_hold );
                if ( odd == 1 )
                {
                    odd = 0;
                    mult += temp;
                }
                else
                {
                    odd = 1;
                    total += temp;
                }
                x--;
            }
            total = total + ( 3 * mult );
            break;
            
        default:
            error = 1;
            break;
    }
    
    if ( len < 1 )
        error = 1;
    
    if ( error == 0 )
    {
        total = total % 10;
        if ( total >0 )
            total = 10 - total;
        
        
        chk_dgt = (48 + total);    /* 48 is the ascii value of 0 */
        
        switch ( mode )
        {
            case MODE_ADD :
                snprintf(str, (MAX_TRACK * sizeof(char)), "%s%c", str, chk_dgt);
                break;
                
            case MODE_SET :
                strncpy( tmp, str, (len - 1));
                snprintf(str, (MAX_TRACK * sizeof(char)), "%s%c", tmp, chk_dgt);
                break;
                
            case MODE_VAL :
                if ( temp_str[strlen(temp_str) - 1] != chk_dgt )
                    *val = 0;
                else
                    *val = 1;
                break;
                
            default :
                snprintf(str, (MAX_TRACK * sizeof(char)), "%s%c", str, ' ');
                break;                
        }        
    }
    
}


void ap_just(char str[],int len, int just, char pad, char *out_str)
{
    int end = 0, diff =0;
    
    memset(out_str, sizeof(out_str), '\0');
    
    if ( strcmp(str,"") != 0 )
    {
        end = strlen( str );
        diff = len - end;
        
        switch ( just )
        {
            case AP_JUST_RIGHT :
                //strcpy( out_str, lpad( str, len, pad ));
                sprintf( out_str, "%s", lpad( str, len, pad ));
                break;
                
            case AP_JUST_LEFT  :
                //strcpy( out_str, rpad( str, len, pad ));
                sprintf( out_str, "%s", rpad( str, len, pad ));
                break;
                
            case AP_JUST_CENTER :
                if ( diff > 0 )
                    sprintf( out_str, "%s", rpad( lpad( str, (end + diff/2), pad ), len, pad ));                
                else
                    strcpy( out_str, str);
                break;
                
            default :
                break;
        }
    }
}


/**********************************************************************************
 * Function: strntoupper                                                          *
 * ----------------------------                                                   *
 *   Change passed string to upper-case                                           *
 *                                                                                *
 *   Arguments:                                                                   *
 *   str: Character pointer                                                       *
 *                                                                                *
 *   returns: Convert the string pointed by str pointer to upper-case             *
 *********************************************************************************/

char* strntoupper(char *str)
{
   /*              Local Variables		 */
   static char     dst[MAX_STR + 1];
   int             i;
   
   /* Start */
   memset(dst,'\0',(MAX_STR + 1) * sizeof(char));
   for (i=0; i < strlen(str) ; dst[i]=toupper(str[i]), i++);   
   dst[i] = '\0';   
   return (dst);
}

/**********************************************************************************
 * Function: strntolower                                                          *
 * ----------------------------                                                   *
 *   Change passed string to lower-case                                           *
 *                                                                                *
 *   Arguments:                                                                   *
 *   str: Character pointer                                                       *
 *                                                                                *
 *   returns: Convert the string pointed by str pointer to lower-case             *
 *********************************************************************************/

char* strntolower(char *str)
{
   /*              Local Variables		 */
   static char     dst[MAX_STR + 1];
   int             i;
   
   /* Start */
   memset(dst,'\0',(MAX_STR + 1) * sizeof(char));
   for (i=0; i < strlen(str) ; dst[i]=tolower(str[i]), i++);   
   dst[i] = '\0';   
   return (dst);
}


/**********************************************************************************
 * Function: str_split                                                            *
 * ----------------------------                                                   *
 *   Extract the delimiter separated substring from the string                    *
 *                                                                                *
 *  Arguments:                                                                    *
 *   pos  : Position of the substring to extracted                                *
 *   str  : String                                                                *
 *   delim: Character delimiter                                                   *
 *                                                                                *
 *  returns: Extracted substring based on the given position in the passed string *
 *********************************************************************************/

char* str_split(int pos, char *str, char delim )
{
    /*              Local Variables		 */
    int lp_cnt = 0,sub_cnt = 0,str_pos = 0;
    static char sub_str[MAX_STR + 1];
    //char* sub_str = (char*)malloc((MAX_STR + 1) * sizeof(char));
    
    memset(sub_str,'\0',(MAX_STR + 1) * sizeof(char));
    
    for( ; (sub_cnt < pos || str[lp_cnt] != '\0') && lp_cnt <= strlen(str)  ; lp_cnt++, str_pos++ ) 
    {
        if( str[lp_cnt] != delim ||  str[lp_cnt] == '\0' )
        {
            sub_str[str_pos] = str[lp_cnt];
            if ( str[lp_cnt] == '\0' )
                sub_cnt++;            
        }
        else
        {
            sub_str[str_pos] = '\0';
            sub_cnt++;
        }
        
        if( sub_cnt == pos )
            return (sub_str);
        
        if ( (str[lp_cnt] == delim ||  str[lp_cnt] == '\0') && sub_cnt != pos )
        {
            memset(sub_str,'\0',(MAX_STR + 1) * sizeof(char));
            str_pos = -1;
        }        
    }
    return (EMPTY_STRING);
}

/**********************************************************************************
 * Function: cp_write_label                                                       *
 * ----------------------------                                                   *
 *   Wrapper to cp_process_label function call                                    *
 *                                                                                *
 *  Arguments:                                                                    *
 *   r_lbl_type      : Label Type                                                 *
 *   r_printer_type  : Printer Type                                               * 
 *                                                                                *
 *  returns: Status return from cp_process_label function call                    *
 *********************************************************************************/


int cp_write_label ( char r_lbl_type[],char r_printer_type[] )
{
    /*              Local Variables		 */
    char r_fname[200];
    char r_lbl_name[100];
    
    snprintf( r_lbl_name,sizeof(r_lbl_name),"%s%s.fmt",r_lbl_type,r_printer_type) ;
    snprintf( r_fname,sizeof(r_fname),"%s/%s",getenv("DM_DAT"),strntolower(r_lbl_name) );
    return(cp_process_label(r_fname,r_printer_type));    
}

/**********************************************************************************
 * Function: cp_process_label                                                     *
 * ----------------------------                                                   *
 *   Label Generation Routines.These routines are intended to be included in      *
 *   label generation reports to create the labels.                               *
 *                                                                                *
 *  Note : Function "process_format_line" must be defined by the calling report.  *
 *                                                                                *
 *  Arguments:                                                                    *
 *   r_fname      : FMT file name with absoulte path                              *
 *   p_type       : Printer Type                                                  * 
 *                                                                                *
 *  returns: Return true for success and false for failure                        *
 *********************************************************************************/


int cp_process_label( char r_fname[], char p_type[] )
{    
    /*              Local Variables		 */
    FILE *infile = NULL;    
    char in_line[MAX_STR];
    int return_status = 0;    
    
    infile = fopen(r_fname,"r");
    
    if ( infile == NULL )
    {
        debug( "File Open Failed" );              /* Comments to be removed */
        debug( r_fname );                         /* Comments to be removed */
        return_status = NO_FMT_FILE; 
        exit(return_status);      
    }
    else
    {
        while(fgets(in_line, sizeof(in_line), infile)!=NULL)
        {
            if ( in_line[0] != ' ' )
                process_format_line(in_line);  
        }
    }
    
    if ( infile != NULL )
    {
        fclose( infile );
    }
    
    return_status = EXIT_NORMAL;
    return(return_status);
}


void ap_code_to_str( char *dst_str, char *str, char code, char delim )
{
    /*              Local Variables		 */
    int str_pos = 0;
    char sub_str[MAX_STR + 1];
    char dst[MAX_STR + 1];
    
    memset(dst,'\0',(MAX_STR + 1) * sizeof(char));
    
    do
    {
        str_pos++;
        memset(sub_str,'\0',(MAX_STR + 1) * sizeof(char));
        strcpy(sub_str,str_split(str_pos,str,delim));
        if ( sub_str[0] == code )
        {
            snprintf(dst,((MAX_STR + 1) * sizeof(char)),"%s",sub_str+1);
            break;
        }        
        
    }while ( strcmp(sub_str,"") > 0 );
    
    if  ( !strcmp(sub_str,"") )
    {
        snprintf(dst,((MAX_STR + 1) * sizeof(char)),"%c",code);
    }
    
    strcpy(dst_str,dst);
}
    
char* rpad(char str[], int len, char pad_char)
{
    static char out_str[MAX_STR];
    //const char* str_ptr;
    int i = 0;
    if( len < strlen(str) )
        len = strlen(str);
    memset( out_str, pad_char, len);
    out_str[ len + 1 ] = '\0';
    
    for( ; str[i] != '\0'; i++)
    {
        out_str[i] = str[i];
    }
    
    //str_ptr = out_str;
    
    return (out_str);
}

char* lpad(char str[], int len, char pad_char)
{
    static char out_str[MAX_STR];
    //const char* str_ptr;
    int diff, i = 0;
    
    if( len < strlen(str) )
        len = strlen(str);
    
    diff = len - strlen(str);
    memset( out_str, pad_char, len);
    out_str[ len ] = '\0';
    
    for( ; str[i] != '\0'; i++)
    {
        out_str[ diff + i ] = str[i];
    }
    
    //str_ptr = out_str;
    
    return (out_str);
}

char* substr(char str[], int position, char length)
{
    static char out_str[MAX_STR];
    int c = 0;
    
    memset( out_str, '\0', sizeof(out_str) );
    
    while (c < length) 
    {
      out_str[c] = str[position + c - 1];
      c++;
    }
    out_str[c] = '\0';
    
    return (out_str);
}


char* edit_str( char str[], char fmt[] )
{
    static char out_str[MAX_STR] = "";
    int c = 0, i = 0;
    
    for ( c = 0; c < strlen(fmt); c++ )
    {
        if ( fmt[c] == 'X' || fmt[c] == 'x' )
        {
            if( i >= strlen(str) )
                out_str[c] = ' ';
            else
                out_str[c] = str[i];
            i++;            
        }
        else
            out_str[c] = fmt[c];
    }
    out_str[c] = '\0';    
    
    return (out_str);
}


char* rtrim(char* string, char junk)
{
    char* original = string + strlen(string);
    while(*--original == junk);
    *(original + 1) = '\0';
    return string;
}


char* ltrim(char *string, char junk)
{
    char* original = string;
    char *p = original;
    int trimmed = 0;
    do
    {
        if (*original != junk || trimmed)
        {
            trimmed = 1;
            *p++ = *original;
        }
    }
    while (*original++ != '\0');
    return string;
}


char* currency_converter( double amount)
{
    static char out_str[20] = "";
    char temp[20] = "";
    int int_part = 0;
    int out_str_len = 0;
    int in_str_len = 0;
    int pos_count  = 0;
    int comma_flag = 0;
    int place_pointer = 5;
    
    memset(out_str,'\0',sizeof(out_str));
    memset(temp,'\0',sizeof(temp));
    snprintf(temp,sizeof(temp),"%.2f",amount);
    in_str_len = strlen(temp);
    
    for ( ; temp[int_part] != '.'; int_part++);
    
    if ( (int_part%3) == 0)
        out_str_len = strlen(temp) + (int_part/3);
    else
        out_str_len = strlen(temp) + (int_part/3) + 1;
    
    for ( ; in_str_len > 0; )
    {
        if( pos_count == place_pointer)
        {
            place_pointer = 2;
            comma_flag = 1;
            out_str[--out_str_len] = temp[--in_str_len];
            pos_count++;
        }
        else
        {
            if ( comma_flag == 1)
            {
                out_str[--out_str_len] = ',';
                pos_count = 0;
                comma_flag = 0;
            }
            out_str[--out_str_len] = temp[--in_str_len];
            pos_count++;
        }
    }
    
    out_str[0] = '$';   
    return (out_str);
}

