// Game class 

#include "game.hpp"

// Window size
float WINDOW_WIDTH = 1920;
float WINDOW_HEIGHT = 1080;


// Static functions

// Initializer functions

void Game::initVariables()
{
	// Initialize some variables
	this->window = NULL;
	this->fullscreen_ = false;
	this->dt = 0.f;
}

void Game::initWindow()
{
	// Create a window with WINDOW_WIDTH AND WINDOW_HEIGHT and apply window options
	//
	// Open window config file

	std::ifstream ifs(root_filepath_ + "/src/Config/window.ini");
	this->videoModes_ = sf::VideoMode::getFullscreenModes();
	std::string skip_str;
	std::string title = "None";
	sf::VideoMode window_bounds = sf::VideoMode::getDesktopMode();
	bool fullscreen = false;
	unsigned framerate_limit = 120;
	bool vertical_sync = false;
	unsigned antialiasing_level = 0;

	// Import window setting from a file
	if (ifs.is_open())
	{
		std::getline(ifs, title);
		ifs >> skip_str >> window_bounds.width;
		//std::cout << skip_str << " and " << window_bounds.width << std::endl;
		ifs >> skip_str >> window_bounds.height;
		//std::cout << skip_str << " and " << window_bounds.height << std::endl;
		ifs >> skip_str >> fullscreen;
		ifs >> skip_str >> framerate_limit;
		//std::cout << skip_str << " and " << framerate_limit << std::endl;
		ifs >> skip_str >> vertical_sync;
		//std::cout << skip_str << " and " << vertical_sync << std::endl;
		ifs >> skip_str >> antialiasing_level;
 
	}

	// Close window configure file
	ifs.close();

	this->fullscreen_ = fullscreen;
	this->windowSettings_.antialiasingLevel = antialiasing_level;

	// Check if fullscreen mode is turned on in the window configure file
	if (this->fullscreen_)
		this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Fullscreen, this->windowSettings_);
	else
		// sf::Style::Close ja sf::Style::Titlebar estää ikkunan koon muuttamisen
		this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Titlebar | sf::Style::Default, this->windowSettings_);

	this->window->setFramerateLimit(framerate_limit);
	this->window->setVerticalSyncEnabled(vertical_sync);

	if (!icon_.loadFromFile( root_filepath_ + "/src/Textures/Icon/towerDefense_Retina_Tower_Icon.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_ICON";
    }
    this->window->setIcon(icon_.getSize().x, icon_.getSize().y, icon_.getPixelsPtr());
}

void Game::initKeys()
{
	// Open keys cpnfigure file
	std::ifstream ifs(root_filepath_ + "/src/Config/keys.ini");
	if (ifs.is_open())
	{
		std::string key = "";
		int key_value = 0;
		while (ifs >> key >> key_value)
		{
			// Add supported keys to the supportedKeys map
			this->supportedKeys[key] = key_value;
		}
	}
	// Close keys configure file
	ifs.close();

	// !! TÄHÄN VOISI IMPLEMENTOIDA TESTIN!! 

	// Debug: Print all the keys
	for (auto i : this->supportedKeys)
	{
		std::cout << i.first << " " << i.second << "\n";
	}
}

void Game::initStates()
{
	// Push a new state into states stack
	this->states.push(new MainMenuState(this->window, &this->supportedKeys, this->root_filepath_, &this->states, &this->sfEvent));
}




// Constructor/Destructor functions

Game::Game(std::string& root_filepath)
{
	root_filepath_ = root_filepath;
	// Initialize the window
	this->initWindow();
	// Initialize keys
	this->initKeys();
	// Initialize the states
	this->initStates();
}

Game::~Game()
{
	// Delete the window pointer
	delete this->window;

	// Delete states and pop out of the stack
	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}
}

// Functions

void Game::endApplication()
{
	std::cout << "Ending application." << std::endl;
}

void Game::updateDt()
{
	// Measure the length of time for the frame to go through
	//
	// Updates the dt variable with the time it takes to update and render one frame
	this->dt = this->dtClock.restart().asSeconds();

	// Print refresh rate
	// std::cout << this->dt << "\n"; Uncomment to print the dt
}
	

void Game::updateSFMLEvents()
{
	// window event poll
	while (this->window->pollEvent(this->sfEvent))
	{
		// Close window event
		if(this->sfEvent.type == sf::Event::Closed)
			this->window->close();
	}
}

void Game::update()
{
	// Update UpdateSFMLEvents function
	this->updateSFMLEvents();
	sf::Time elapsed = quitclock_.getElapsedTime();
	

	if (!this->states.empty())
	{
		// Update states if it's not empty
		this->states.top()->update(this->dt);
		if (this->states.top()->getQuit()) {
			this->states.top()->quit_ = false;
			if(1 < elapsed.asSeconds()) {
				// Quit the game
				this->states.top()->endState();
				delete this->states.top();
				this->states.pop();
				quitclock_.restart();
			}
		}
	}

	// Application end
	else
	{
		// Call if the stack is empty
		this->endApplication();
		this->window->close();
	}
}

void Game::render()
{
	// Clear screen
	this->window->clear();

	// Render the state at the top of the stack
	if (!this->states.empty())
		this->states.top()->render();

	// Update the window
	this->window->display();
}

void Game::run()
{
	// Creating a game while loop
	while (this->window->isOpen())
	{
		// Call for updates and render
		this->updateDt();
		this->update();
		this->render();
		
	}
}
