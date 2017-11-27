#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define total_st 12
void showID_history(char *USER_ID);
void saveID_history(char *sUSER_ID, char *s_phone,char *s_name ,int is_station, int ie_station, int money);
void USER_INFO(char *NAME ,char *PHONE , char *ID);
void Show_item(void);
void Show_price_list(void);
void Show_this_buy(char *thisname ,char *thisphone,char *thisID,int this_is,int this_ie, int thispay);
int Ctotal_pay(int npay,int rpay,int is,int ie);
void choice_station(int *ci_s,int *ci_e);
void Chioce_State(int *Cstate);


struct User_Info
{            
        char user_name[20];
        char user_phone[20];
        char user_Fid[20];
};

struct T_Station
{
        int st_name;
        int st_fee;
        struct T_Station *next;
        struct T_Station *prev;
};

char train_station[12][10] = {"Taipei","Taoyuan","Hsinchu",
        "Taichung","Changhua","Nantou",
        "Tainan","Kaohsiung","Pingtung",
        "Taitung","Hualien","Yilan"};
int train_pay[12] = {10,15,20,
        18,22,25,
        10,13,24,
        22,17,16};

int main()
{

        //============================
        struct User_Info Buy1;
        struct T_Station tw_st[total_st];
        struct T_Station current , rcurrent;
        int i=0;
        int total_pay=0,n_pay=0,r_pay=0;
        int i_s,i_e,state_view;
        //============================
        //User's infomation 
        USER_INFO(&Buy1.user_name[0] ,&Buy1.user_phone[0] , &Buy1.user_Fid[0]);
        //=============================
        Chioce_State(&state_view);
        //=============================
        //.....
        switch(state_view)
        {
                case 1:
                        Show_price_list();
                        Show_item();
                        choice_station(&i_s,&i_e);
                        //....creat node
                        for(i=0;i<total_st;i++)
                        {
                                tw_st[i].st_name = i;
                                tw_st[i].st_fee = train_pay[i];  
                                if(i==total_st-1)
                                {
                                        tw_st[i].next = &tw_st[0];
                                }
                                else
                                {
                                        tw_st[i].next = &tw_st[i+1];
                                }
                                //
                                if(i==0)
                                {
                                        tw_st[i].prev = &tw_st[total_st-1];
                                }
                                else
                                {
                                        tw_st[i].prev = &tw_st[i-1];
                                }   
                        }
                        //search start train station 
                        for(i=0;i<12;i++)
                        {
                                if(tw_st[i].st_name == i_s)
                                {
                                        current = tw_st[i];
                                        rcurrent = tw_st[i];
                                }
                        }
                        //Calculate Train ticket
                        while(current.st_name != i_e)
                        {
                                n_pay = n_pay + current.st_fee;
                                current = *current.next;
                        }
                        while(rcurrent.st_name != i_e)
                        {   
                                rcurrent = *rcurrent.prev;
                                r_pay = r_pay + rcurrent.st_fee;
                        } 
                        //===========================================
                        total_pay = Ctotal_pay(n_pay,r_pay,i_s,i_e);
                        Show_this_buy(&Buy1.user_name[0],&Buy1.user_phone[0],&Buy1.user_Fid[0],i_s,i_e,total_pay);
                        //save file.....
                        saveID_history( &Buy1.user_Fid[0],&Buy1.user_phone[0],&Buy1.user_name[0] , i_s, i_e, total_pay);

                        break;
                case 2:
                        //=============================
                        //record
                        showID_history(&Buy1.user_Fid[0]);
                        break;
                default:
                        break;
        }

}


void showID_history(char *USER_ID)
{
        char check_i[4]="";
        char check_name[20]="";
        char check_my[20]="";
        char lineshow[100];
        int j,x;
        printf("=========================================\n");
        printf("user's record \n");
        FILE *fp_rd;
        char var;
        fp_rd = fopen("out.txt","r");
        //...
        while(!feof(fp_rd))
        {
                fgets(check_i,6,fp_rd);
                if( fgets (lineshow, 100, fp_rd)!=NULL)
                {
                        for (j=0;j<20;j++)
                        {
                                check_name[j]=lineshow[j];
                        }
                        /* writing content to stdout */
                        for(j=0;check_name[j]!='\t';j++)
                        {
                                if(check_name[j]!='\t')
                                {
                                        check_my[j]=check_name[j];
                                }
                        }
                        check_my[j-1] = '\n';
                        x = j-1;
                        j = strncmp(USER_ID,check_my,x);
                        if(j==0)
                        {
                                puts(lineshow);printf("\n");
                        }
                        //for(j=0;j<=x;j++)
                        //{
                        //    printf("%c : %c \n",USER_ID[j],check_my[j]);
                        //}


                }
        }

        //printf("%s",lineshow);

        //...   
        fclose(fp_rd);
        printf("\n=========================================\n");
}


void saveID_history(char *sUSER_ID, char *s_phone,char *s_name ,int is_station, int ie_station, int money)
{
        FILE *pfile;
        pfile = fopen("out.txt","a");
        fprintf(pfile,"#ID: %s \t",sUSER_ID);
        fprintf(pfile,"#name: %s \t",s_name); 
        fprintf(pfile,"#phone: %s \t",s_phone);
        fprintf(pfile,"#start station : %s \t",train_station[is_station]);
        fprintf(pfile,"#end station : %s \t",train_station[ie_station]);
        fprintf(pfile,"#pay : %d \n",money);
        fclose(pfile);
}

void USER_INFO(char *NAME ,char *PHONE , char *ID)
{
        printf("\n=========================================\n");
        printf("Please insert your name\n");
        scanf("%s" ,NAME);

        printf("Please insert your phone\n");
        scanf("%s" ,PHONE);

        printf("Please insert your ID\n");
        scanf("%s" ,ID);

        printf("\nname: %s \t",NAME);
        printf("phone: %s \t",PHONE);
        printf("ID: %s \n",ID);
}

void Show_item(void)
{
        int i;
        printf("\n=========================================\n");
        for(i=0;i<total_st;i++)
        {
                printf("%d %s \t",i+1,train_station[i]);
                if(i%4==(total_st/3-1))
                {
                        printf("\n");
                }
        }
        printf("\n=========================================\n");
}

void Show_price_list(void)
{
        int i;
        //show every city's train stations 
        printf("\n=========================================\n");
        //Price list
        printf("\tPrice List\n");
        for(i=0;i<total_st-1;i++)
        {
                printf("%s to %s is :%d NT. \n",train_station[i],train_station[i+1],train_pay[i]);
        }
        printf("%s to %s is :%d NT. \n",train_station[total_st-1],train_station[0],train_pay[total_st-1]);
        printf("\n=========================================\n");
        //============================================
}

void Show_this_buy(char *thisname ,char *thisphone,char *thisID,int this_is,int this_ie, int thispay)
{
        printf("\n\tYour infonation \nname: %s \t",thisname);
        printf("phone: %s \t",thisphone);
        printf("ID: %s \n",thisID);
        printf("Your start station is %s \n",train_station[this_is]);
        printf("Your end station is %s \n",train_station[this_ie]);
        printf("pay : %d \n",thispay);

}

int Ctotal_pay(int npay,int rpay,int is,int ie)
{
        int totalpay = 0;
        if(is == ie)
        {
                totalpay = 0;
        }
        else
        {
                if(npay<rpay)
                { 
                        totalpay = npay;
                }
                else
                {
                        totalpay = rpay;
                }   
        }
        return totalpay;
}

void choice_station(int *ci_s,int *ci_e)
{
        int i1;
        printf("\n");
        //start station
        printf("please choic start station : ");
        scanf("%d",&i1);
        *ci_s = (i1-1)%13;
        //end station
        printf("please choic end station : ");
        scanf("%d",&i1);
        *ci_e = (i1-1)%13;
}

void Chioce_State(int *Cstate)
{
        int ci;
        printf("\tChioce item\n");
        printf("1:Buy Train Ticket\t2:Watch your record\n");
        scanf("%d",&ci);
        *Cstate = ci;
}
