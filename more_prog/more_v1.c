/*more_v1.c --version 1 of more function
read and print 24 lines then pause for a new speial commands
*/
#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512
void do_more(FILE *);
int see_more();
int main(int argc, char *argv[])
{
    FILE *fp;
    if(argc == 1)
        do_more(stdin);
    else
    {
        while(-- argc)
            if((fp = fopen(*++argv,"r")) != NULL)
                {
                    do_more(fp);
                    fclose(fp);
                }
            else
                exit(1);
            
    }
    return 0;
}
void do_more(FILE *fp){
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(),reply;
    while(fgets(line,LINELEN,fp)){
        if(num_of_lines == PAGELEN){
            reply = see_more();
            if (reply == 0)
                break;
            num_of_lines-=reply;
        }
        if(fputs(line,stdout) == EOF)
            exit(1);
    num_of_lines++;
    }

}
int see_more()
{
    int c;
    printf("\033[7m more? \033[m");
    while((c=getchar()) != EOF){
        if(c == 'q')
            return 0;
        if(c == ' ')
            return PAGELEN;
        if(c == '\n')
            return 1;
    }
    return 0;
}