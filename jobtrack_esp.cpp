//****************************************//
//    Jobtrac -to- ESP - JCL conversion   //
//****************************************//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
FILE *fp,*fp1;

// To get the individual characters from the file
int getline(char **lineptr,int *n, FILE *stream)
{
        char c;
        int s=0;
               
        while((c=getc(stream)) != EOF)
        {
           if (c == '\n') 
             break;
            (*lineptr)[s]=c;
           ++s;
        }
        //(*lineptr)[s]='\n';
        
                 
        if (c == EOF && s == 0) 
        return -1;
        
        (*lineptr)[s]='\0';
        return s;
}

//To check replace the pattern present in the JCL
char * replace( char const * const original, char const * const pattern, char const * const replacement) 
{
  int const replen = strlen(replacement);
  int const patlen = strlen(pattern);
  int const orilen = strlen(original);

  int patcnt = 0;
  const char * oriptr;
  const char * patloc;

  // find how many times the pattern occurs in the original string
  for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
  {
    patcnt++;
  }

   // allocate memory for the new string
    int const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

    if (returned != NULL)
    {
      // copy the original string, 
      // replacing all the instances of the pattern
      char * retptr = returned;
      for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
      {
        int const skplen = patloc - oriptr;
        // copy the section until the occurence of the pattern
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replacement 
        strncpy(retptr, replacement, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
    return returned;
}

void CheckADI_1(const char *line,const char *temp) {
 bool INSERT=0;
 char c=37; 
 bool flag=0;
 int i;
 
 // To set INSERT to '1' if 'I' is found on the 73rd character
 if(line[72]== 'I')
   INSERT=1;
 
 if(INSERT) {
    printf("%INCLUDE IF(TODAY('");
    fputs ("%INCLUDE IF(TODAY('",fp1);
 }
 else {
    printf("%c",c);
    fputs ("%",fp1);
    printf("EXCLUDE IF(TODAY('");
    fputs ("EXCLUDE IF(TODAY('",fp1);
 }
   
 for(i=60;i<67;i++) {
   if(i==60 && line[i]!='.') {
      printf("MON"); 
      fputs ("MON",fp1);
      flag=1;  
   }
   else if(i==61 && line[i]!='.') {
      if(flag==1) {
         printf(","); flag=0; 
         fputs (",",fp1);
      } 
      printf("TUE");
      fputs ("TUE",fp1);
      flag=1;
   }
   else if(i==62 && line[i]!='.') {
       if(flag==1) {
          printf(","); flag=0; 
          fputs (",",fp1);
       } 
      printf("WED");
      fputs ("WED",fp1);
      flag=1;
   }
   else if(i==63 && line[i]!='.') {
       if(flag==1) {
          printf(","); flag=0; 
          fputs (",",fp1);
       } 
      printf("THU");
      fputs ("THU",fp1);
      flag=1; 
   }
   else if(i==64 && line[i]!='.') {
       if(flag==1) {
          printf(","); flag=0; 
          fputs (",",fp1);
       } 
      printf("FRI");
      fputs ("FRI",fp1);
      flag=1;
   }
   else if(i==65 && line[i]!='.') {
       if(flag==1) {
          printf(","); flag=0;
          fputs (",",fp1); 
       } 
      printf("SAT");
      fputs ("SAT",fp1);
      flag=1;
   }
   else if(i==66 && line[i]!='.') {
       if(flag==1) {
          printf(","); flag=0; 
          fputs (",",fp1);
       } 
      printf("SUN");
      fputs ("SUN",fp1);
      flag=1;
   }
 }
     printf("'))\n");
     fputs ("'))\n",fp1);
     
     // Once the constants are written the line is written to the file     
     for(i=0; i<59;i++) {
     fputc (temp[i],fp1);
     printf("%c",temp[i]);
     }
     fputs ("\n",fp1);
     printf("\n");
     
     // Since %E is not supported, the ASCII value of '%' which is '37' is used.
     if(INSERT) {
       printf("%c",c);
       printf("ENDINCL\n");
       fputs ("%ENDINCL\n",fp1);
     }
     else {
       printf("%c",c);
       printf("ENDEXCL\n");
       fputs ("%ENDEXCL\n",fp1);
     }    
 
} 

void CheckADI_2(const char *line,const char *temp) {
   char c=37;  
   char a;
   bool day=true,month=true,year=true;
   int Year,Day,Month;
   int Year1,Year2,Month2,Month1,Day1,Day2;
   int i;
   bool INSERT;
   char *var;
   char var1=37;
   
   // To check for INSERT or DELETE in the Jobtrac
   if(line[72]=='I' || line[72]=='A')  {
     INSERT=1;
     var="INCLUDE";
   }
   else if(line[72]=='D' || line[72]=='R ' ) {
     INSERT=0;
     var="EXCLUDE";
   }
      
   a=line[79]; Year2=atoi(&a);   
   a=line[78]; Year1=atoi(&a);   
   a=line[75]; Month2=atoi(&a);  
   a=line[74]; Month1=atoi(&a);  
   a=line[77]; Day2=atoi(&a);    
   a=line[76]; Day1=atoi(&a);
   
   if(line[76]=='*' && line[77]=='*' )
   Day=0;
   else 
   Day=(Day1*10)+Day2;
   
   if(line[74]=='*' && line[75]=='*' )
   Month=0;
   else
   Month=(Month1*10)+Month2;
   
   if(line[78]=='*' && line[79]=='*' )
   Year=0;
   else
   Year=Year1*10+Year2;
   
   // To set the flags
   if(Day==0)
   day=false;
   if(Month==0)
   month=false;
   if(Year==0)
   year=false;
   
   //To check for various combinations 
   // 1. I=111507
   if(day && month && year){
       printf("%c%s IF(ESPAYY EQ '%d' AND ESPAMM EQ '%d' AND ESPADD EQ '%d')\n",var1,var,Year,Month,Day);
       //fputc(var1,fp1);
       fprintf(fp1,"%c%s IF(ESPAYY EQ '%d' AND ESPAMM EQ '%d' AND ESPADD EQ '%d')\n",var1,var,Year,Month,Day);
   }
   // 2. I=1115**
   else if(day && month && !year){
     printf("%c%s IF(ESPAMM EQ '%d' AND ESPADD EQ '%d')\n",var1,var,Month,Day); 
     fprintf(fp1,"%c%s IF(ESPAMM EQ '%d' AND ESPADD EQ '%d')\n",var1,var,Month,Day);
   }
   // 3. I=11**07
   else if(day && !month && year) {
     printf("%c%s IF(ESPAYY EQ '%d' AND ESPADD EQ '%d')\n",var1,var,Year,Day); 
     fprintf(fp1,"%c%s IF(ESPAYY EQ '%d' AND ESPADD EQ '%d')\n",var1,var,Year,Day);
   }
   // 4. I=**1507
   else if(!day && month && year) {
     printf("%c%s IF(ESPAYY EQ '%d' AND ESPAMM EQ '%d')\n",var1,var,Year,Month);
     fprintf(fp1,"%c%s IF(ESPAYY EQ '%d' AND ESPAMM EQ '%d')\n",var1,var,Year,Month); 
   }
   //5. I=11****
   else if(day && !month && !year){
    printf("%c%s IF(ESPADD EQ '%d')\n",var1,var,Day); 
    fprintf(fp1,"%c%s IF(ESPADD EQ '%d')\n",var1,var,Day);
   }
   //6. I=**15**
   else if(!day && month && !year){
    printf("%c%s IF(ESPAMM EQ '%d')\n",var1,var,Month);
    fprintf(fp1,"%c%s IF(ESPAMM EQ '%d')\n",var1,var,Month); 
   }
   //7. I=****07
   else if(!day && !month && year){
    printf("%c%s IF(ESPAYY EQ '%d')\n",var1,var,Year); 
    fprintf(fp1,"%c%s IF(ESPAYY EQ '%d')\n",var1,var,Year); 
   }
   //8. Invalid Case I=******
   else if(!day && !month && !year){
    perror("Error in ADI Variable!! Day,Year and Month Not present\n");
    getch();
    exit(-1); 
   }
   
   for(i=0; i<59;i++) {
     fputc (temp[i],fp1);
     printf("%c",temp[i]);
   }
   printf("\n");
   fputs("\n",fp1);
     
   // To print %INCLUDE or %EXCLUDE
   printf("%c%s\n",var1,var);
   fprintf(fp1,"%c%s\n",var1,var);

}

void CheckADI_3(const char *line,const char *temp, char *pattern) {
 
 char *var,*var1;
 char c=37;
 
 if(line[72]=='I' )  {
     var="INCLUDE";
     var1="ENDINCL";
   }
   else if(line[72]=='D') {
     var="EXCLUDE";
     var1="ENDEXCL";
   }
 
 printf("%c%s IF(ESPADAY EQ '%s')\n",c,var,pattern);
 fprintf(fp1,"%c%s IF(ESPADAY EQ '%s')\n",c,var,pattern);
 //printf("%s\n",line);
 //fprintf(fp1,"%s\n",line);
 for(int i=0; i<59;i++) {
     fputc (temp[i],fp1);
     printf("%c",temp[i]);
   }
 fputs ("\n",fp1);
 printf("\n");
 
 printf("%c%s\n",c,var1);
 fprintf(fp1,"%c%s\n",c,var1);
}
// To handle MON#01, TUE#05,MON#LAST
void CheckADI_4(const char *line,const char *temp, char *pattern, int num) {
 char *var,*var1,*suffix;
 char c=37;
 
 if(line[72]=='I' )  {
     var="INCLUDE";
     var1="ENDINCL";
   }
   else if(line[72]=='D') {
     var="EXCLUDE";
     var1="ENDEXCL";
   }
   
   if(pattern=="MON")
   pattern="MONDAY";
 else if(pattern=="TUE")
   pattern="TUESDAY";
 else if(pattern=="WED")
   pattern="WEDNESDAY";
 else if(pattern=="THU")
   pattern="THURSDAY";
 else if(pattern=="FRI")
   pattern="FRIDAY";
 else if(pattern=="SAT")
   pattern="SATURDAY";
 else if(pattern=="SUN")
   pattern="SUNDAY";
   
 if(num==1|| num==21 )
 suffix ="ST";
 if(num==2 || num==22)
 suffix="ND";
 if(num==2 || num==23)
 suffix="RD";
 else
 suffix="TH";
   
 printf("%c%s IF(TODAY('%d%s %s OF MONTH'))\n",c,var,num,suffix,pattern);
 //printf("%c%s IF(ESPADAY EQ '%s')\n",c,var,pattern);
 fprintf(fp1,"%c%s IF(TODAY('%d%s %s OF MONTH'))\n",c,var,num,suffix,pattern);
 //printf("%s\n",line);
 //fprintf(fp1,"%s\n",line);
 for(int i=0; i<59;i++) {
     fputc (temp[i],fp1);
     printf("%c",temp[i]);
   }
 fputs ("\n",fp1);
 printf("\n");
 
 printf("%c%s\n",c,var1);
 fprintf(fp1,"%c%s\n",c,var1);
  
}
// Main Function
int main()
{
      int len=80;
      char *line;
      
      fp=fopen("ip_file.txt", "r");
      fp1=fopen("op_file.txt", "w");
      char temp[80],*pch;
      char *temp_ADI;
      bool SKIP_line;   // if temp[0]='/' && temp[1]='/' && temp[2]='*' then It is a comment. so skip any processing.
      bool SKIP_print;  // to skip the writing of the line to the file
      int choice=0;  // for CASE3 AJO - To set the switch case
      int i;
      
      char *weekDay;  // For weekday calculation in Case3
      int WeekDay=0;
      char *pattern;
      //case 4
      char *period,date[2],*Date; 
      int num2;
      
      if (fp==NULL)
      {
         perror("Error while opening the file.\n");
         getch();
         exit(-1);
      }
      
      line = (char*) malloc(len * sizeof(char));
       
      while (( getline(&line, &len, fp)) != -1)
      //while(fgets(line, 80, fp) != NULL)
      {
         // store the value in a temp
        //temp=line;
         strcpy(temp,line);
         printf("%s\n",temp);
         // parse through the array to check for the occurance of the pattern
                 for(i=0;temp[i]!='\0';i++)
         {  
            // CASE1: FOR JMF- To check for '&:' and replace with '%'               
            if(temp[i]=='&')
            {
               if(temp[i+1]==':')
               {
                  char * const newstr = replace(temp,"&:","%");
                  // The replaced pattern is replaced with Temp and the search is continued
                  //temp=newstr;
                  strcpy(temp, newstr);
               }
               //If a '%%' is encountered—these should be flagged with an error; 
               //this would include the following examples: %%BOB %%%BOB %%%%BOB %%%%%%%%%%%BOB
               else
               {
                  perror("Error in JMF Variable!!\n");
                  getch();
                  exit(-1); 
               }
            }//End of CASE1
            
            // CASE2: FOR ADI- To convert //:(XXXXXXX) to //%XXXXXXX
            //if(temp[i]==47 && temp[i+1]==47 && temp[i+2]==58)
            if(temp[i]=='/' && temp[i+1]=='/' && temp[i+2]==':')
            { 
               temp[i+2]='%';
               int k = i+3;
               int flag = 0;
                     
               for(k;temp[k]!='\0';k++)
               {
                 if(temp[k]=='(')
                 {
                   flag=1;
                   while(temp[k]!='\0') 
                   {
                     temp[k]=temp[k+1];
                     k++;
                   }
                 }
               }
               // Any use of OSD date(e.g.//:(MEMB1),\=_OSD+4_) should be flagged !!
               if(flag==1)
               {
                  for(int OSD = 0; OSD < (strlen(line)-2); OSD ++)
                  if(strncmp(line+OSD, "OSD",3) == 0 )
                  {
                    printf("//*!!OSD date in JMF Variable!!\n");
                    fputs ("//*!!OSD date in JMF Variable!!\n",fp1);
                  }
               }
               
               for(k = i+3;temp[k]!='\0';k++)
               {
                 if(temp[k]==')')
                 {
                   while(temp[k]!='\0') 
                   {
                     temp[k]=temp[k+1];
                     k++;
                   }
                 }
               }

            } //End of CASE2
            
         }//for
         // CASE3: For ADI
         
         //choice 1: For cases where '//STEP1A EXEC PGM=IEFBR14      =MT.TFSS I=SCHD' is encountered
         if(line[59]== '=' && ( line[72]== 'I' || line[72]== 'D') && line[73]== '=')
            choice=1;
            
         //choice 2: For cases where the combination of '//STEP2A EXEC PRODPRC1         I=****06'   
         //else if (line[59]!= '=' && ( line[72]== 'I' || line[72]== 'D') && line[73]== '=')
         else if (line[59]==' ' && ( line[72]== 'I'|| line[72]=='D'|| line[72]== 'A'|| line[72]=='R') && line[73]== '=' ) {
           char aa = line[74]; 
           num2 = atoi( &aa );
           //printf("%d",num2);
           bool flag1=num2;
           if(flag1==true || line[74]== '*' )
             choice=2;
         }
         // choice 3: For Monday ,Tuesday to occur in the Variable
         else if (( line[72]== 'I'|| line[72]=='D') && line[73]== '=') {
           
           int J, windowsize, count=1;
            bool flag=0;
                // choice 4: For MON#01, TUE#02
           if (line[62]=='#' && (line[63]=='0' || line[63]=='L'))
             J=3;
             // choice 5: For XMON#01, XTUE#02
           else if(line[63]=='#' || line[63]!=' ')
             J=4;
           
           for(J;J<9;J++) {
             if(flag==1)
                break;
           
             windowsize = J;
             for( count=1;count<=7;count++)  {
               
               if (J == 3) {
                  if(count==1)
                     pattern = "MON";
                  else if(count==2)
                     pattern = "TUE";
                  else if(count==3)
                     pattern = "WED";  
                  else if(count==4)
                     pattern = "THU";
                  else if(count==5)
                     pattern = "FRI";
                  else if(count==6)
                     pattern = "SAT";
                  else if(count==7)
                     pattern = "SUN";
               }   
               if (J == 4) {
                  if(count==1)
                     pattern = "XMON";
                  else if(count==2)
                     pattern = "XTUE";
                  else if(count==3)
                     pattern = "XWED";  
                  else if(count==4)
                     pattern = "XTHU";
                  else if(count==5)
                     pattern = "XFRI";
                  else if(count==6)
                     pattern = "XSAT";
                  else if(count==7)
                     pattern = "XSUN";
               }   
                  
               if (J == 6) {
                 if(count==1)
                   pattern = "MONDAY";
                 else if(count==2)
                   pattern = "FRIDAY";
                 else if(count==3)
                   pattern = "SUNDAY";  
               }
          
               else if( J==7)
                 pattern="TUESDAY";
             
               else if (J==8) {
                 if(count==1)
                   pattern ="THURSDAY"; 
                 else if(count==2)
                   pattern ="SATURDAY";
               } 
          
               else if (J == 9)
                 pattern = "WEDNESDAY";
          
              flag=0;
              
              for (int i=59;i<(strlen(line)-windowsize); i++) {
                if(strncmp(line+i, pattern ,strlen(pattern)) == 0 ) {
                  printf("%s in JMF Variable!!\n",pattern);
                  if(J==3) {
                    choice=4;flag=1;
                  }
                  else if(J==4) {
                    choice=5;flag=1;
                  }
                  else {
                    choice=3; flag=1;
                  }
                  break;
               }
             }
            
        
              if(flag==1) {
               if(choice==4) {           
                 if(line[63]=='L') 
                   Date="0";
                 else if (line[63]=='0') {
                   for(int k =63, i=0;k<65 ;k++,i++) {
                      date[i]=line[k];
                   }
                   Date=date;
                   //strcpy (Date,date);
                   num2 = atoi( Date );
                   printf("%d\n",num2);
                 }         
                 break;
              }//if
              }
              else if(choice==5) {
              }
            }// if flag
              
          }
         }
         //choice 4: 
      //   else if (( line[72]== 'I'|| line[72]=='D') && line[73]== '=') {
     //   }
            
             switch( choice ) {
             case 1:
             CheckADI_1(line , temp);
             SKIP_print=1;
             choice=0;
             break;
             
             case 2:
             CheckADI_2(line , temp);
             SKIP_print=1;
             choice=0;
             break; 
             
             case 3:
             CheckADI_3(line , temp, pattern);
             SKIP_print=1;
             choice=0;
             break;    
             
             case 4:
             CheckADI_4(line, temp, pattern, num2);
             SKIP_print=1;
             choice=0;
             break;   
                  
             default :
             // this means there is no AJO and we need to write the temp to the file
             SKIP_print=0;
             break;
             }   
            
         if(!SKIP_print) {
          printf("%s\n",temp);
          fputs (temp,fp1);
          fputs ("\n",fp1);
        }
        
        //free(line); 

      }//while
      
      if(line)
        free(line);
      fclose(fp);
      fclose(fp1);
      
      getch(); 
      return 0;
}

