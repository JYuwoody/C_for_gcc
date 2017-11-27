#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define INPUT_MAX_CHAR_SIZE 20
void ADD_NEW_NAME(void);
void SHOW_NAME_LIST(void);
void SHOW_SELECT(void);

int item_numer_counter = 0;
int total_size = 0;
char *name_list;
char **name_address = &name_list;



void main()
{
	int menu_state;
	char check_input_name[] = "y";
	char new_name[INPUT_MAX_CHAR_SIZE] = "";
	
	while(menu_state != 4)
	{
		printf("1:Add new name ;2:Show the order name ;3:Find number of name ;4:END \n");
		scanf("%d",&menu_state);
		switch(menu_state)
		{
			case 1:
					check_input_name[0] = 'y';
					while(check_input_name[0]=='y'||check_input_name[0]=='Y')
					{
						//...
						ADD_NEW_NAME();
						//...
						printf("DO you want key new name in the list(y or n)");
	                	//scanf("%d",&check_input_name);
	                	scanf("%s",check_input_name); 
	                	printf("%s\n",check_input_name);
					}
			break;
			case 2:
					SHOW_NAME_LIST();
			break;
			case 3:
					SHOW_SELECT();
			break;
			case 4:
					free(name_list);
					printf("END\n");
			break;
			default:
				printf("The select error \n");
			break;
		}

	}
	


	return;
}


void ADD_NEW_NAME(void)
{
	int item__add_name_size = 0;
	char new_name[20] = "";
	int i;
	int new_add_memory_start;
	new_add_memory_start = total_size;
	//...
	printf("Add new name : ");
	scanf("%s",new_name);
	printf("\n");
	item_numer_counter = item_numer_counter + 1;
	item__add_name_size = strlen(new_name);
	//printf("%d\n",item__add_name_size);
	printf("Your add name : %s\n",new_name);
	//add hesd mark
	total_size = total_size + 1;
	name_list = realloc(name_list,total_size*sizeof(char));
	strcat(name_list,"(");
	//add data
	total_size = total_size + item__add_name_size;
	name_list = realloc(name_list,total_size*sizeof(char));
	strcat(name_list,new_name);
	//add end mark
	total_size = total_size + 1;
	name_list = realloc(name_list,total_size*sizeof(char));
	strcat(name_list,")");


	//...
    for(i=0;i<=total_size;i++)
    {
        printf("%c",*(name_list+i));
    }
    printf("\n");
    
    for(i=new_add_memory_start;i<=total_size;i++)
    {
        printf("%p\n",name_list+i);
        *(name_address+i) = &name_list[i];
    }

}


void SHOW_NAME_LIST(void)
{
	//item_numer_counter
	int i,j,temp;
	int sub_name_memery_start,sub_name_memery_end;
	temp = 0;
	for(i=0;i<item_numer_counter;i++)
	{
		j = temp;
		printf("%d : ",i+1);
		for(;j<=total_size;j++)
		{
			if((**(name_address+j)==')'))
			{
				sub_name_memery_end = j-1;
				break;
			}
			else if(**(name_address+j)!='(')
			{
				printf("%c",**(name_address+j));
			}else if(**(name_address+j)=='(')
			{
				sub_name_memery_start = j+1;
			}
			
		}
		//printf("%p\n",*(name_address+i));printf("%c\n",**(name_address+i));
		printf("\tAddress Start: %p\tAddress End: %p\n",
			*(name_address+sub_name_memery_start),*(name_address+sub_name_memery_end));
		temp = j+1;
	}

}

void SHOW_SELECT(void)
{
	int select_number;
	int i,j,temp;
	int sub_name_memery_start,sub_name_memery_end;
	temp = 0;
	//...	item_numer_counter
	printf("Please enter select number \n");
	//printf("total : %d\n",item_numer_counter);
	scanf("%d",&select_number);
	if(select_number<1||select_number>item_numer_counter)
	{
		printf("Select number out of range \n");
	}
	else
	{
		for(i=0;i<item_numer_counter;i++)
		{
			//j = temp;
			//printf("%d\n",j );
			for(j = temp;j<=total_size;j++)
			{
				if((i+1)==select_number)
				{	
					if((**(name_address+j)==')'))
					{
						sub_name_memery_end = j-1;
						break;
					}
					else if(**(name_address+j)!='(')
					{
						printf("%c",**(name_address+j));
					}else if(**(name_address+j)=='(')
					{
						sub_name_memery_start = j+1;
					}
				}
				else
				{
					if((**(name_address+j)==')'))
					{
						break;
					}
				}	
			}
			temp = j+1;

		}
		printf("\tAddress Start: %p\tAddress End: %p\n",
				*(name_address+sub_name_memery_start),*(name_address+sub_name_memery_end));
	}
}