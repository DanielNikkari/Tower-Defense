// Tower Defense Group 3 main.cpp

/*#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>*/
#include <iostream>
#include <string>
#include <filesystem>

#include "game.hpp"
#include "game.cpp" // Needed for Mac compiler to link

// Window dimentions
//int WINDOW_WIDTH = 1920;
//int WINDOW_HEIGHT = 1080;

int main()
{
	std::cout << "Starting the program..." << std::endl;
	// File path if file is not in the same location as the code (Change to your own path!)
	std::string filepath = "/Users/danielnikkari/Documents/Textures";
	// File path if file is in the same location as the code
	std::string root_filepath = std::filesystem::current_path(); // Remember to be on /tower-defense-3 not /src to filepath to work
	std::cout << root_filepath << std::endl;


	// Create game
	Game game(root_filepath);
	game.run();

	return 0;
}
