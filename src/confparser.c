#include <stdio.h>
#include <stdlib.h>	
#include <string.h> 

#include "gpiocontrol.h"
#include "confparser.h"

#include "common.h"
//#define error_print(...) fprintf(stderr, __VA_ARGS__);

#define LOCALCONF "gpiomap.conf"
#define ETCCONF "/etc/gpiomap.conf"


void print_pin_selector(selector *sel, int pos)
{
	int i;
	
	for(i=0;i<pos;i++)
	{
		error_print("Pos: %d\t gpion: %d\t gpiodirection %d\n", i, sel->pin_n[i].gpion,  sel->pin_n[i].direction);
	}
	
}

int add_pin(selector *sel, pin p, int pos)
{
	sel->pin_n[pos].gpion=p.gpion;
	sel->pin_n[pos].direction=p.direction;
	return 0;
}

int parse_line(char *line, pin *newpin)
{
	char direction[10];
	int num, gpion;


	sscanf(line,"%s %d %d",direction,&num, &gpion);
	debug_print("\n Found %s %d %d", direction, num, gpion);
	if( line[0] != '#' && line[0] != ' ')
	{
	/*	if(strncmp(direction, "OUT",3) || strncmp(direction, "IN",2))
		{
			return 0;
		}*/	
		if(!strncmp(direction, "OUT",3))
		{
			newpin->direction=OUT;
			newpin->gpion=gpion;
			return 1;
		}
		if(!strncmp(direction, "IN",2))
		{
			newpin->direction=IN;
			newpin->gpion=gpion;
			return 1;
		}
		
	}
	return 0;
	
	
}



int read_single_line(FILE *fp1, char **line)
{
	char c,d, *templine;
	int count=0,i=0;
	

	
	debug_print("\n");
	/*if(feof(fp1))
	{
		return 1000;
	}*/
	while(c != '\n')
	{
		fscanf(fp1,"%c", &c);
		if(c == 0)
		{
			return 1000;
		}
		debug_print("%d ",c);
		count++;
	}
	fseek(fp1, -(count), SEEK_CUR);	
	*line=malloc((count+1)*sizeof(char*));
	templine=*line;

	debug_print("\n");

	//fgets(*line, count, fp1);
	while(d != '\n')
	{
		fscanf(fp1,"%c",&d);
		debug_print("%d ",d);
		if( d == '\n')
		{
			templine[i]='\0';
			i++;
		}
		else
		{
			templine[i]=d;
			i++;
		}
	} 

/*	for(i=0;i<=count;i++)	
	{
		fscanf(fp1,"%c",&c);
		debug_print("%d ",c);
		if( c == '\n')
		{
			templine[i]='\0';
		}
		else
		{
			templine[i]=c;
		}
	}*/
	//fseek(fp1, -1, SEEK_CUR);
	debug_print("\n Line readed by read_single_line(): %s", templine);

	return count;
}




int parseconf(char *conffile, selector *confpin)
{
	FILE *p1;
	char **line;
	pin newpin;
	int res, res2, pos=0;


	p1=fopen(conffile,"r");
	line=malloc(sizeof(char**));
	*line=NULL;


	while(!feof(p1))
	{
		if(*line != NULL)
		{
			free(*line);
			*line=NULL;
		}
		res2=read_single_line(p1, line);
		if(res2==1000)
		{
			debug_print("\n EOF reached, break\n");
			break;
		}
		debug_print("\nLinea letta: %s", *line);
		res=parse_line(*line, &newpin);
		debug_print("\n %d ", res);
		if(res)
		{
			debug_print("\n New pin detected: Direction %d, Gpio number %d \n", newpin.direction, newpin.gpion);
			add_pin(confpin, newpin, pos);
			pos++;
		}
	}

	fclose(p1);
	free(line);

	return pos;

}



int check_conf_file(char *filename)
{
	FILE *p;

	p=fopen(filename, "r");
	if(p!=NULL)
	{
		debug_print("Filename: %s\n", filename);
		fclose(p);		
		return 0;
	}
	
	return 1;
}


int select_conf_file(char *conffile)
{
	int result;


	//Trying local before
	result=check_conf_file(LOCALCONF);
	if(result == 0)
	{
		strcpy(conffile, LOCALCONF);
		return 0;	
	}
	//Then etc
	else
	{
		result=check_conf_file(ETCCONF);	
		if( result == 0 )
		{
			strcpy(conffile, ETCCONF);
			return 0;	
		}
	}	
	//Finally fail
	return 1;

}

int running_conf(selector *confpin)
{
	char conffile[64];
	int result=224;
	int pin_detect=0;

	result=select_conf_file(conffile);
	
	if(result == 0)
	{
		printf("\n Using the %s \n", conffile);
		pin_detect=parseconf(conffile, confpin);
		print_pin_selector(confpin, pin_detect);
		
	}
	else
	{
		printf("\n Fail to found a know conffile\n");
	}

	return pin_detect;
	


}
