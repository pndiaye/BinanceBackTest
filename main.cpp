#include <ctype.h>
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <iostream>
#include <fstream>
#include <getopt.h>

#define ORDER_FEE 0.001

int
main(int argc, char **argv)
{
	
    int c;
	double target;
	std::string file;

	
   while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"file",    required_argument, 0,  0 },
			{"target",    required_argument, 0,  0 },
            {0,         0,                 0,  0 }
        };

       c = getopt_long(argc, argv, "f:t:",
                 long_options, &option_index);
        if (c == -1)
            break;

       switch (c) {
        case 0:
            std::cout<<"option "<<long_options[option_index].name;
            if (optarg)
			{
				std::cout<<" with arg "<<optarg;
				file = optarg;
			}
            std::cout<<std::endl;
            break;
		case 'f':
			std::cout<<"option f with value '"<<optarg<<"' \n";
			file = optarg;
            break;
		case 't':
            std::cout<<"option t with value '"<<optarg<<"' \n";
			target = atof(optarg);
            break;
/*
       case 'o':
            printf("option open\n");
            break;

       case 'd':
            printf("option d with value '%s'\n", optarg);
            break;

       case '?':
            break;
*/
       default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

   if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
	
	std::string hourly_file = file+"_hourly.json";
	std::string minutes_file = file+"_minutes.json";
	
	
	
	if(false)
	{
		std::ifstream config_doc("config_doc.json", std::ifstream::binary);
		//config_doc >> root;
	}
	
	

   exit(EXIT_SUCCESS);
}