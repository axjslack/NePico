/*


int select_conf_file(char *conffile)
{
	FILE *p1,*p2;

	p1=fopen("gpiomap.conf", "r");
	if(p1!=NULL)
	{
		error_print("\n Using local conf file \n");
		conffile=malloc(sizeof(strlen("gpiomap.conf")));
		strcpy(conffile, "gpiomap.conf");
		error_print("Filename: %s\n", conffile);
		fclose(p1);
		return 0;
	}
	else
	{
		fclose(p1);
		p2=fopen("/etc/gpiomap.conf", "r");
		if(p2!=NULL)
		{
			error_print("\n Using etc conf file \n");
			conffile=malloc(sizeof(strlen("/etc/gpiomap.conf")));
			strcpy(conffile, "/etc/gpiomap.conf");
			error_print("Filename: %s\n", conffile);
			fclose(p2);
			return 0;
		}
		else
		{
			error_print("\n Unable to find valid conf file \n");
			fclose(p2);
			return 1;
		}
	}

}


*/
