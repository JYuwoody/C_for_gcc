#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
void main()
{
	FILE *fp_rd;
	int i , j ;
        int item1,item2,item3,item4,item5,item6;
	char id[20],name[20],phone[20],s_st[20],e_st[20],money[20],lineshow[200];
	fp_rd = fopen("out.txt","r");
        if(!fp_rd)
        {
                printf("open file error ... \n");
                exit(1);
        }
	while(!feof(fp_rd))
	{
                fscanf(fp_rd,"%d%s%d%s%d%s%d%s%d%s%d%s",&item1,id,&item2,name,&item3,phone,&item4,s_st,&item5,e_st,&item6,money);
		fgets (lineshow, 200, fp_rd)!=NULL;//change line
                printf("%s\t %s\t %s\t %s\t %s\t %s\n",id,name,phone,s_st,e_st,money);

	}

        fclose(fp_rd);
}
