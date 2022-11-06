#pragma once

/**
 * @file src/core/shell.hpp
 * @brief Header of the main part.
 * @author LAPCoder
 * @version 0.1.0
 * 
 * MIT License
 */

// Class with the shell.

#ifndef SNABBGET_CORE

	#include <iostream>
	#include <string>
	#include <chrono>
	#include <ctime>
	#include <fstream>
	#include <cstdlib>
	#include <climits>
	#include <unistd.h>
	#ifdef __linux__
		#include <termios.h>
	#endif
	#include <functional>
	#include <vector>
	#include <iterator>
	#include <sstream>

	#include "utils.hpp"

	#define VERSION "0.1.0"
	#ifndef MAX_INPUT
		#define MAX_INPUT 255
	#endif

	#define ZERO_ANY(T, a, n) do{\
		T *a_ = (a);\
		size_t n_ = (n);\
		for (; n_ > 0; --n_, ++a_)\
			*a_ = (T) { 0 };\
	} while (0)

	#define SNABBGET_CORE

	/**
	 * @brief The SnabbGET Namespace
	 * @version 0.1.0
	 */
	namespace SnabbGET
	{
	
		/**
		 * @brief Initialise SnabbGET
		 * 
		 * @return Nothing
		 */
		void SnabbGET();
		/**
		 * @brief Initialise SnabbGET
		 * 
		 * @param cmd_line Is a command-line? ($ sget say hello)
		 * @return Nothing
		 */
		void SnabbGET(bool cmd_line);

		/**
		 * @brief Initialize the shell
		 * 
		 * @return [std::string] Welcome message
		 */
		std::string init();

		/**
		 * @brief Read the input from the user
		 * 
		 * @param input_user_t A string contain the user's command
		 * @return [std::string] Command result
		 */
		std::string read_input(std::string input_user_t);

		/**
		 * @brief Use user' settings for the prompt in file settings/prompt.sget.txt
		 *
		 * @return [std::string] The prompt text
		 */
		std::string promptSettings();

		/**
		 * @brief Print a new line
		 *
		 * @return [std::string] The new line text
		 */
		std::string new_line();

		/**
		 * @brief Active the raw mode to get a best terminal experience in Linux
		 * 
		 * @param echo [OPTIONAL] Set to 0 or see in code
		 */
		namespace Raw_mode
		{
			/**
			 * @brief Construct a new Raw_mode object
			 * 
			 * @return Nothing
			 */
			void Raw_mode();
			
			/**
			 * @brief Construct a new Raw_mode object
			 * 
			 * @param echo [OPTIONAL] Set to 0 or see in code
			 * @return Nothing
			 */
			void Raw_mode(int echo);

			/**
			 * @brief Stop the raw mode
			 * 
			 * @return Nothing
			 */
			void pause();

			/**
			 * @brief Restart the raw mode
			 * 
			 * @return Nothing
			 */
			void resume();

			/**
			 * @brief Define the key actions
			 */
			enum KEY_ACTIO
			{
				KEY_NULL = 0,		/* NULL */
				CTRL_C = 3,			/* Ctrl-c */
				CTRL_D = 4,			/* Ctrl-d */
				CTRL_F = 6,			/* Ctrl-f */
				CTRL_H = 8,			/* Ctrl-h */
				TAB = 9,			/* Tab */
				CTRL_L = 12,		/* Ctrl+l */
				ENTER = 13,			/* Enter */
				CTRL_Q = 17,		/* Ctrl-q */
				CTRL_S = 19,		/* Ctrl-s */
				CTRL_U = 21,		/* Ctrl-u */
				ESC = 27,			/* Escape */
				BACKSPACE =  127,	/* Backspace */
				ARROW_LEFT = 1000,
				ARROW_RIGHT,
				ARROW_UP,
				ARROW_DOWN,
				DEL_KEY,
				HOME_KEY,
				END_KEY,
				PAGE_UP,
				PAGE_DOWN
			};
			
			#ifdef __linux__
				static struct termios old, new1;
			#endif
		}

		/*void RWpause();
		void RWresume();*/

		//Raw_mode __rawmode;

		#ifdef __EMSCRIPTEN__
			bool one_line = true;
		#else
			bool one_line = false;
		#endif

		// Last user input.
		std::string input_user;
		// Time when the shell was started.
		std::time_t dateOpen;
		// File with the history.
		std::fstream historyFile;

		// User name and computer name.
		std::string userName = "User";
		std::string computerName = "Computer";
		std::string currentDir = "/";

		bool CMD_LINE = false;

		std::vector<std::string> SCREEN;

		/**
		 * @brief Set the user name object in the var
		 * 
		 * @return Nothing
		 */
		void set_user_name();
		
		/**
		 * @brief Set the machine name object in the var
		 * 
		 * @return Nothing
		 */
		void set_machine_name();
		
		/**
		 * @brief Set the current dir object  in the var
		 * 
		 * @return Nothing
		 */
		void set_current_dir();

		/**
		 * @brief Get the command and put user's command in array
		 * 
		 * @param input_user_t
		 * @return Nothing
		 */
		void get_command(std::string input_user_t);

		std::string cmd[MAX_INPUT];
		unsigned int cmdLen = 0;

		/**
		 * @brief All commands (aka. say, cd, help...) are here
		 */
		namespace CMDS
		{
			/**
			 * @brief Construct a new CMDS object
			 * 
			 * @return Nothing
			 */
			void CMDS();

			#ifndef CMDS_DEF
				#define CMDS_DEF
				
				#define EXIT 0
				#define HELP 1
				#define CLS  2
				#define SAY  3
				#define CD   4
				#define EXE  5
				#define LS   6
				#define CP   7
				#define MV   8
				#define MK   9
				#define RM   10
			#endif

			std::string _exit_(std::string[], int, std::string);
			std::string _help_(std::string[], int, std::string);
			std::string _cls_ (std::string[], int, std::string);
			std::string _say_ (std::string[], int, std::string);
			std::string _cd_  (std::string[], int, std::string);
			std::string _exe_ (std::string[], int, std::string);
			std::string _ls_  (std::string[], int, std::string);
			std::string _cp_  (std::string[], int, std::string);
			std::string _mv_  (std::string[], int, std::string);
			std::string _mk_  (std::string[], int, std::string);
			std::string _rm_  (std::string[], int, std::string);
			
			std::vector<std::function<std::string(std::string[], int, std::string)>> cmdLst;
			std::vector<const char*> allCmd;
		}

		std::string addToSCREEN(std::string txt)
			{SCREEN.emplace_back(txt); return txt;}

		std::string addToSCREEN(std::string txt, std::string txt2)
			{SCREEN.emplace_back(txt + txt2); return txt2;}

		/**
		 * @brief Command runner
		 * 
		 * @param id Id of the command
		 * @return [std::string] Command result
		 */
		std::string runCmd(int id, std::string[], int, std::string);


		/**
		 * @brief The syntax analyser
		 */
		class SYNTAX
		{

		};
	}

	// To replace 'SnabbGET::'
	namespace sget
	{
		using namespace SnabbGET;
		// To replace 'SnabbGET::Raw_mode::'
		namespace rw
		{
			using namespace Raw_mode;
		}
	}


	// WARNING!!!!! THOSE LINES MUST STAY LIKE THAT!!!
	//                          ====
	#include "shell.cpp"
	#include "./includesAll.hpp"
	#include "./settings/reader.cpp"

#endif // SNABBGET_CORE

