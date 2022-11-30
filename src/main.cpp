// Main file

/**
 * @file src/main.cpp
 * @brief Main file of the project.
 * @author LAPCoder
 * @version 0.1.1
 * 
 * MIT License
 */

/* DEFINES */
//See shell.hpp
//#define VERSION "0.1.1"

/* INCLUDES */

#include "core/shell.hpp"
//#include "interface/gui.cpp"

#ifndef std_def
	#include <iostream>
	#include <cstdio>
	#include <string>
	#include <chrono>
	#include <cstdio>
	#include <vector>
	#include <sstream>
	#define std_def
	#define print(x, ...) std::cout << x // For a friend :D
#endif

// WebAssembly
#ifdef __EMSCRIPTEN__ 
	#include <emscripten.h>
	bool one_line = true;
#else
	bool one_line = false;
#endif

#ifdef __cplusplus
	#define EXTERN extern "C"
#else
	#define EXTERN
#endif

#ifndef EMSCRIPTEN_KEEPALIVE
	#define EMSCRIPTEN_KEEPALIVE
#endif

#ifdef _FORJAVAGUI_

int main()
{	
	sget::SnabbGET();
	sget::rw::Raw_mode(0);
	sget::init();
	std::cout << sget::new_line();
	sget::read_input("exit");
	return EXIT_SUCCESS;
}

#else 

EXTERN EMSCRIPTEN_KEEPALIVE void RunSnabbGETCommand(
	/*int argc, char **argv*/)
{
	sget::SnabbGET();
	sget::rw::Raw_mode(0);
	sget::init();
	std::string cmd;
	std::cin >> cmd;
	std::cout << sget::read_input(cmd) << "\r\n";
	std::string tmp[0];
	sget::CMDS::_exit_(tmp, 0, "");
}

// End Wasm

/* Start */

//std::string input_user;
std::vector<char16_t> input;
long pos;

int main(int argc, char *argv[])
{
	sget::SnabbGET();
	sget::rw::Raw_mode(0);
	sget::io::io.outFunct = &std::printf;
	sget::io::io.inFunct = &std::scanf;
	
	system(""); // I don't kwon why I must put that, but if I don't add that,
				// escape codes don't work on Windows :(
	if (argc == 1 && !one_line)
	{
		//using namespace SnabbGET sget;
		//using SnabbGET::Raw_mode rw(0);
		sget::addToSCREEN(sget::init());
		// Printf is faster than std::cout and \n is faster than std::endl
		printf(sget::FRAME().c_str());
		while (true)
		{
			//getline(std::cin, input_user);
			// Raw mode: BIG SH*T

			int c = '\0', right_count = 0;
	// chars_count: Not really char count, but the number of chars left to read

			//input_user = "";
			input.clear();
			pos = 0;

			// Use the Raw mode to read the input from the user
			// and hide escape sequences (e.g. ^[[A).
			while (true)
			{
				printf(sget::FRAME().c_str());
				std::string in(input.begin(),input.end());
				// std::cout is faster than printf for concat (<< vs. "%", )
				std::cout << "\n\n\033[9999;0H\033[1A──────────────────────\r\n"
					<< sget::new_line().c_str() << "\0337" << in << "\0338"
					<< std::string("\033[C")*pos // TODO: accents & end of line
					#ifdef __linux__
					<< "\033[1A\n"
					#endif
					;
				read(0, &c, 1);
				if (c == '\n') break;
				if (((32  <= c) &&
					(126 >= c)) ||
					((161 <= c) &&
					(255 >= c)) ||
					c == sget::rw::TAB)
				{
					input.emplace(input.begin() + pos, (char)c);
					pos++;
				}
				if (c == '(')
				{
					input.emplace(input.begin() + pos, ')');
					right_count++;
				}
				if (c == '[')
				{
					input.emplace(input.begin() + pos, ']');
					right_count++;
				}
				if (c == '{')
				{
					input.emplace(input.begin() + pos, '}');
					right_count++;
				}
				if (c == '"' || c == '\'' || c == '`')
				{
					input.emplace(input.begin() + pos, (char)c);
					right_count++;
				}
				// TODO: Make auto-close.
				if (c == sget::rw::BACKSPACE)
				{
					if (pos > 0)
					{
						input.erase(input.end()-1);
						pos--;
					}
				}
				if ((int)c == sget::rw::ESC) // ESC like \[ \033 \x1b
				{
					read(0, &c, 1);
					if (c == '[')
					{
						read(0, &c, 1);
						//if (c == 'A') input_user += "UP";
						//if (c == 'B') input_user += "DOWN";
						if (c == 'C' && right_count > 0)
						{
							//printf("\033[C");
							right_count--;
							pos++;
						}
						if (c == 'D' && pos > 0)
						{
							//printf("\033[D");
							right_count++;
							pos--;
						}
					}
				}
				#ifdef __linux__
					//std::cout << input_user_tmp << "\033[1A\n";
				#endif
			}
			sget::addToSCREEN(sget::new_line());
			std::string in(input.begin(), input.end());
			sget::SCREEN.back() += in;
			sget::addToSCREEN(sget::read_input(in));
			printf(sget::FRAME().c_str());

			if (in == "exit") 
			{
				std::cout << "";
				return EXIT_SUCCESS;
				exit(EXIT_SUCCESS);
			}
			else
			{
				//printf(sget::FRAME().c_str());
				//std::cout << "";
			}
		}
		return 0;
	}
	else if (argc > 1 && !one_line)
	{
		sget::SnabbGET(true);
		//sget::rw::Raw_mode(0);
		sget::init();
		std::string arr[MAX_INPUT];
		for(int b = 0; b < argc - 1 ; b++)
		{
			arr[b] = argv[b + 1];
		}
		//std::cout << concatArr(arr, argc) << std::endl;
		std::cout << sget::read_input(concatArr(arr, argc)) << "\r\n";
		sget::read_input("exit");
		//sget::rw::pause();
		return EXIT_SUCCESS;
	}
	else // one_line
	{
		/*SnabbGET sget(true);
		SnabbGET::Raw_mode rw(0);
		sget.init();
		std::string cmd;
		std::cin >> cmd;
		std::cout << sget.read_input(cmd) << "\r\n";
		return EXIT_SUCCESS;*/
	}
}

#endif
