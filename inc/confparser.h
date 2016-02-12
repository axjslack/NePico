#ifndef _CONFPARSER_H_
#define _CONFPARSER_H
#include "gpiocontrol.h"


int check_conf_file(char *filename);
int add_pin(selector *sel, pin p, int pos);
int parse_line(char *line, pin *newpin);
int read_single_line(FILE *fp1, char **line);
int parseconf(char *conffile, selector *confpin);
int check_conf_file(char *filename);
int select_conf_file(char *conffile);
int running_conf(selector *confpin);
void print_pin_selector(selector *sel, int pos);

#endif //_CONFPARSER_H
