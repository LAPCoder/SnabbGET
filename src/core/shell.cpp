// Core (main part)

/*-------------------------------------------------
 * ------------------************------------------
 * -----------------**************-----------------
 * ----------------****************----------------
 * ---------------********--********---------------
 * --------------********----********--------------
 * -------------********--##--********-------------
 * ------------********---##---********------------
 * -----------********----##----********-----------
 * ----------********-----##-----********----------
 * ---------********------##------********---------
 * --------********-------##-------********--------
 * -------********--------##--------********-------
 * ------********--------------------********------
 * -----********----------------------********-----
 * ----********-----------##-----------********----
 * ---********------------##------------********---
 * --********************************************--
 * -**********************************************-
 * ************************************************
 * -                                              -
 * -  USE ALWAYS CRLF (\r\n) TO ENTER A NEW LINE  -
 * -            Don't use "std::endl"             -
 * -  ==========================================  -
 * -                                              -
 *##  ## ###### ######   ###### ###### ##     ######
 *##  ## ##     ##       ##     ##  ## ##     ##    
 *##  ## ###### ######   ##     ###### ##     ######
 *##  ##     ## ##       ##     ## ##  ##     ##
 *###### ###### ######   ###### ##  ## ###### ##
 *       It can be read on a mini-map ;D
 */

/**
 * @file src/core/core.cpp
 * @brief Main part of the project.
 * @author LAPCoder
 * @version 0.0.1
 * 
 * MIT License
 */

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <unistd.h>
#include <algorithm>
#include <unistd.h>
#ifdef __linux__
	#include <termios.h>
#endif
#include <functional>

#ifdef _WIN32
	#include <windows.h>
	#include <Lmcons.h>
#endif

SnabbGET::SnabbGET()
{
	dateOpen = std::time(0);
	__snabbget.is____snabbget = true; 
}

SnabbGET::SnabbGET(bool cmd_line)
{
	dateOpen = std::time(0);
	CMD_LINE = cmd_line;
	__snabbget.CMD_LINE = CMD_LINE;
}

SnabbGET::~SnabbGET()
{
	if (! CMD_LINE && !this->is____snabbget)
		std::cout << "\r\nShell closed.\r\n";
}

std::string SnabbGET::init()
{
	set_current_dir();
	std::string msg = htmlToRgbEsc("#ff0000", true) + "\r\
       SnabbGET\r\n\033[0m\
 Welcome to SnabbGET!\r\n\
Version: ";
	msg += VERSION;
	msg += "\r\n\
Copyright (c) SnabbGET\r\n\
----------------------\r\n";
	msg += "\033[1A\r\n";
	msg += new_line();
	return msg;
}

void SnabbGET::get_command(std::string input_user_t)
{
	std::string tmp = "";
	size_t j = 0;
	ZERO_ANY(std::string, cmd, MAX_INPUT);

	for (int i = 0; i < (int)input_user_t.length(); i++)
		if (input_user_t[i] != ' ')
			tmp += input_user_t[i];
		else if (tmp != "")
		{
			cmd[j++] = tmp;
			tmp = "";
		}
	if (tmp != "") cmd[j++] = tmp;
	cmdLen = j;
}

std::string SnabbGET::runCmd(int id, std::string cmd[], int cmdLen, std::string input_user)
{
	auto& cmdTmp = run.cmdLst[id];
	return cmdTmp(cmd, cmdLen, input_user);
}

std::string SnabbGET::read_input(std::string input_user_t)
{
	input_user = input_user_t;

	get_command(input_user);

	#ifdef DEBUG
		/*std::cout << "\r\nCommand: [";
		for (unsigned int i = 0; i < cmdLen; i++)
			std::cout << cmd[i] << ", ";
		std::cout << "]" << "\r\n" << "Input: " << input_user
			<< "\r\n" << "CmdLen: " << cmdLen << "\r\n";*/
	#endif

	//std::cout << &cmd << std::endl;
	if (! one_line)
	{
		historyFile.open("dist/.history.txt", std::ios_base::app);

		// Check the file
		if (!historyFile.is_open())
		{
			#ifdef DEBUG
				std::cout << "Error opening history file!" << std::endl;
			#endif

			// Try to create the file
			historyFile.open("dist/.history.txt", std::ios_base::out);
			if (!historyFile.is_open())
			{
				#ifdef DEBUG
					std::cout << "Error creating history file!" << std::endl;
				#endif

				// Use system() to create the file
				system("mkdir dist");
				system("echo \"\" > dist/.history.txt");

				// Try to open the file again
				historyFile.open("dist/.history.txt", std::ios_base::app);
				if (!historyFile.is_open())
				{
					#ifdef DEBUG
						std::cout << "Error opening history file!" << std::endl;
					#endif
					
					exit(EXIT_FAILURE);
				}

			}
			else
			{
				historyFile.close();
				historyFile.open("dist/.history.txt", std::ios_base::app);
			}
		}
		historyFile << input_user;
		historyFile << "\r\n";
		historyFile.close();
	}

	if (cmdLen == 0) return "";

/* ####### ####### ##   ## ##   ##   ###   ##   ## #####    ######
 * ##      ##   ## ### ### ### ###  ## ##  ###  ## ##   ## ##
 * ##      ##   ## ####### ####### ##   ## #### ## ##   ##   ##
 * ##      ##   ## ## # ## ## # ## ####### ## #### ##   ##    ##
 * ##      ##   ## ##   ## ##   ## ##   ## ##  ### ##   ##      ##
 * ####### ####### ##   ## ##   ## ##   ## ##   ## #####   ###### 
 */

	//Check if 'cmd' contain '-?'
	else if (cmd[0] != "exe" && contain(cmd, cmdLen, "-?"))
		return help_params(cmd[0]);

	// Exit
	else if (cmd[0] == "exit")
		return runCmd(EXIT, cmd, cmdLen, input_user);

	// Help
	else if (cmd[0] == "help")
		return runCmd(HELP, cmd, cmdLen, input_user);

	// Clear
	else if (cmd[0] == "clear" || cmd[0] == "cls")
		return runCmd(CLS,  cmd, cmdLen, input_user);

	// Say (echo)
	else if (cmd[0] == "say" || cmd[0] == "echo")
		return runCmd(SAY,  cmd, cmdLen, input_user);

	// Cd
	else if (cmd[0] == "cd")
		return runCmd(CD,   cmd, cmdLen, input_user);

	// Execute
	else if (cmd[0] == "exe" && cmdLen > 1)
		return runCmd(EXE,  cmd, cmdLen, input_user);
	else if (cmd[0] == "exe" && cmdLen == 1)
		return "You must enter a command!\r\n";

	// Ls
	else if (cmd[0] == "ls")
		return runCmd(LS,  cmd, cmdLen, input_user);

	// Cp
	else if (cmd[0] == "cp")
		return runCmd(CP,  cmd, cmdLen, input_user);
	/*{
		Raw_mode::pause();
		std::string msg = "cd ";
		#ifdef _WIN32
			if (currentDir.substr(0, 1) == "~")
			{
				msg += "&& cd ";
				msg += currentDir.substr(2);
			}
		#else
			msg += currentDir;
		#endif
		msg += " && ";
		msg += input_user.substr(4);
		system(msg.c_str());
		Raw_mode::resume();
		// DEBUG: Success command execution message

		#ifdef DEBUG
			return "\r\n\033[92mCommand executed!\033[0m\r\n";
		#else
			if (cmd[1] == "cd")
				return "WARNING! You had enter a 'cd' command. THE DIRECTORY IS NOT SAVED! Use the SnabbGET command.\r\n";
			else
				return "";
		#endif
	}*/

	// Not found
	else return "Command not found!\r\n";
}

std::string SnabbGET::new_line()
{
	this->set_user_name();
	this->set_machine_name();
	this->set_current_dir();
	
	std::string msg = "\033[1m\033[92m";
	msg += this->userName;
	msg += "@";
	msg += this->computerName;
	msg += "\033[39m:\033[96m";
	msg += this->currentDir;
	#ifdef DEBUG
		msg += "\033[93m [DEBUG]";
	#endif
	msg += "\033[95m |>\033[0m ";

	#ifdef __linux__
		msg += "\033[1A\n";
	#endif
	return msg;
}

void SnabbGET::set_user_name()
{
	this->userName = "guest";
	#ifdef _WIN32
	
		TCHAR name [UNLEN + 1];
		DWORD size = UNLEN + 1;

		if (GetUserName((TCHAR*)name, &size ))
			this->userName = name;
		else
		{
			#ifdef DEBUG
				std::cout << "Error getting user name on Windows!" << std::endl;
			#endif
		}

	#elif __linux__
		this->userName = getenv("USER");
		if (this->userName == "")
		{
			#ifdef DEBUG
				std::cout << "Error getting user name on Linux!" << std::endl;
			#endif
		}

	#else 
		#ifdef DEBUG
			std::cout << "Get user name is not avilable for MacOS." << std::endl;
		#endif
		this->userName = "user";
	#endif
}

void SnabbGET::set_machine_name()
{
	this->computerName = "computer";
	#ifdef _WIN32
		//std::string computerName = getenv("COMPUTERNAME");
		this->computerName = getenv("COMPUTERNAME"); //this->computerName = computerName;
		if (this->computerName == "")
		{
			#ifdef DEBUG
				std::cout << "Error getting machine name on Windows!" << std::endl;
			#endif
		}
	#elif __linux__
		char hostname[HOST_NAME_MAX];
		if (gethostname(hostname, HOST_NAME_MAX) == 1)
		{
			#ifdef DEBUG
				std::cout << "Error getting machine name on Linux!" << std::endl;
			#endif
		}
		else this->computerName = hostname;
	#else
		#ifdef DEBUG
			std::cout << "Get machine name is not avilable for MacOS." << std::endl;
		#endif
	#endif
}

void SnabbGET::set_current_dir()
{ //USE PWD COMMAND
	this->currentDir = "/";
	#ifdef _WIN32
		this->currentDir = getcwd(NULL, 0);
		#ifdef DEBUG
			std::cout << "Current dir: " << this->currentDir << std::endl;
		#endif
	#elif __linux__
		this->currentDir = getenv("PWD");
		// Minimify the path (if start of path is user home, replace it with ~)
		if (this->currentDir.substr(0, ((std::string)getenv("HOME")).size()) == getenv("HOME"))
			this->currentDir = "~" + this->currentDir.substr(((std::string)getenv("HOME")).size());

		#ifdef DEBUG
			//std::cout << "Current dir: " << this->currentDir << std::endl;
		#endif
	#else
		#ifdef DEBUG
			std::cout << "Get current dir is not avilable for MacOS." << std::endl;
		#endif
	#endif
	__snabbget.currentDir = this->currentDir;
}

/* Initialize new terminal i/o settings */

#ifdef __linux__
	static struct termios old, new1;
#endif

SnabbGET::Raw_mode::Raw_mode(int echo)
{
	#ifdef __linux__
		tcgetattr(0, &old); // grab old terminal i/o settings 
		new1 = old; /* make new settings same as old settings */
		new1.c_lflag &= ~ICANON; /* disable buffered i/o */
		new1.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
		new1.c_oflag &= ~(OPOST); /* disable output processing */
		tcsetattr(0, TCSANOW, &new1); // use these new terminal i/o settings now
	#endif
	if (echo) {}
	// /o o\ ?
	// \---/  Hum...
}

SnabbGET::Raw_mode::Raw_mode()
{
	#ifdef __linux__
		tcgetattr(0, &old); // grab old terminal i/o settings 
		new1 = old; /* make new settings same as old settings */
		new1.c_lflag &= ~ICANON; /* disable buffered i/o */
		new1.c_lflag &= ~ECHO; /* set echo mode */
		new1.c_oflag &= ~(OPOST); /* disable output processing */
		tcsetattr(0, TCSANOW, &new1); // use these new terminal i/o settings now
	#endif
}

SnabbGET::Raw_mode::~Raw_mode()
{
	#ifdef __linux__
		tcsetattr(0, TCSANOW, &old);
	#endif
}

void SGET_RWpause()
{
	#ifdef __linux__
		tcsetattr(0, TCSANOW, &old);
	#endif
}

void SGET_RWresume()
{
	#ifdef __linux__
		tcsetattr(0, TCSANOW, &new1);
	#endif
}

SnabbGET::CMDS::CMDS()
{
	cmdLst.emplace_back(_exit_);
	cmdLst.emplace_back(_help_);
	cmdLst.emplace_back(_cls_ );
	cmdLst.emplace_back(_say_ );
	cmdLst.emplace_back( _cd_ );
	cmdLst.emplace_back(_exe_ );
	cmdLst.emplace_back( _ls_ );
	cmdLst.emplace_back( _cp_ );
}

SnabbGET::CMDS::~CMDS()
{}
