#include <iostream>
#include <ctype.h>
#include <string>
#include <fstream>
#include <unordered_map>

#include "Common.h"
#include "Scanner.h"

using namespace Clap;

int main(int argc, char **argv)
{
	// if(argc == 1)
	// {
	// 	ClapLang::RunCommand();
	// }
	// else
	// {
	// 	std::cout<<argv[1]<<std::endl;
	// 	ClapLang::RunFile(argv[1]);
	// }
	// char c;
	// std::cin>>c;
	ClapLang::RunFile("amogus.clap");
	return 0;
}
