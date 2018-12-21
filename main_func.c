#include<stdio.h>
#include<string.h>
#include "svllblrpt.h"

extern int gen_svllblrpt(char *user_param_str, char *report_file);

int main()
{
  char user_param_str[500] = "";
  char rpt_file[] = "/home/odev/label_integration/svl_lbl_SATO_ProC.rpt";
  short return_status = 0;

  sprintf( user_param_str, "\"\" \"%.*s\" \"%.*s\" \"%.*s\" \"%.*s\" \"%s\" \"%s\" \"%.*s\" \"%.*s\"",
                  (int)strlen("SATO"),"SATO",
                  (int)strlen("svlstd"),"svlstd",
                  (int)strlen("442132759790"),"442132759790",
                  (int)strlen("Y"),"Y",
                       "abcdef",
                       "Y",
                  (int)strlen(""),"",
                  (int)strlen(""),"" );
                  
   printf("Going to create file: parameters -%s\n",user_param_str);
   return_status = gen_svllblrpt( user_param_str, rpt_file);
   printf("Report generation done with status -%d.\n",return_status);
   
   sprintf( user_param_str, "\"\" \"%.*s\" \"%.*s\" \"%.*s\" \"%.*s\" \"%s\" \"%s\" \"%.*s\" \"%.*s\"",
                  (int)strlen("SATO"),"SATO",
                  (int)strlen("svlstd"),"svlstd",
                  (int)strlen("442132759790"),"442132759790",
                  (int)strlen("Y"),"Y",
                       "abcdef",
                       "Y",
                  (int)strlen(""),"",
                  (int)strlen("ERROR! BACHHAO"),"ERROR! BACHHAO" );
    printf("Going to create file: parameters -%s\n",user_param_str);
    return_status = gen_svllblrpt( user_param_str, "/home/odev/label_integration/svl_lbl_SATO_ProC_error.rpt");
    printf("Report generation done with status -%d.\n",return_status);
   
   return(return_status);
}
