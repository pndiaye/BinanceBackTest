#include <ctype.h>
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <ctime>
#include <json/json.h>

#define ORDER_FEE 0.001

int
main(int argc, char **argv)
{
	
    int c, realisation,skips=0;
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
			target = std::stod(optarg);
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
	std::string minutes_file = file+"_minute.json";
	
	
	

	Json::Value root;
	std::ifstream hourlyStream(hourly_file, std::ifstream::binary);
	hourlyStream >> root;
	//std::cout<<root["Data"]<<std::endl;
	int dataSize = root["Data"].size();
	double targetSellValue, targetBuyValue, high,low, prevHigh, prevLow, prevOpen,prevClose;
	bool lock = false;

	for  ( int i = 1 ; i < dataSize  ; i++ ) {
		
		//estimate target buying and selling values
		if(!lock)
		{
			
			prevHigh = std::stod(root["Data"][i-1]["high"].asString());
			prevLow = std::stod(root["Data"][i-1]["low"].asString());
			prevOpen = std::stod(root["Data"][i-1]["open"].asString());
			prevClose = std::stod(root["Data"][i-1]["close"].asString());
			
			if((prevHigh - prevLow)/prevOpen > 0.10)
			{
				std::time_t secsSinceEpoch =  std::stol(root["Data"][i]["time"].asString());
				std::cout<<"Skipped bar at the time: "<< std::asctime(std::localtime(&secsSinceEpoch))<<" Target: "<<targetSellValue<<std::endl;
				skips++;
				continue;
			}
			targetBuyValue = prevClose;
			
			
			
			targetSellValue = ((target+1.001)/0.999) * targetBuyValue;
		}
		
		//check if the buy and sell would have been realised in this hour
		high = std::stod(root["Data"][i]["high"].asString());
		low = std::stod(root["Data"][i]["low"].asString());
		if(targetSellValue < high && targetBuyValue >= low)//&& targetBuyValue >= low
		{
			realisation++;
			lock = false;
			std::time_t secsSinceEpoch =  std::stol(root["Data"][i]["time"].asString());
			std::cout<<"At the time: "<< std::asctime(std::localtime(&secsSinceEpoch))<<" Target: "<<targetSellValue<<std::endl;
		}
		else
		{	
			std::time_t secsSinceEpoch =  std::stol(root["Data"][i]["time"].asString());
			std::cout<<"At the time: "<< std::asctime(std::localtime(&secsSinceEpoch));
			std::cout<<"target buy: "<<targetBuyValue<<" and target sell: "<<targetSellValue<<" not met"<<std::endl;
			std::cout<<root["Data"][i]<<std::endl;
			lock = !lock;
		}
		
		
	}
	
	std::cout<<"realised: "<<realisation<<"/"<< dataSize-1<<" skipped:"<<skips<<std::endl;

   exit(EXIT_SUCCESS);
}