#include "highscore_state.hpp"


//Constructor of Highscorestate class
Highscorestate::Highscorestate(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Font* font) : State(window, supportedKeys, root_filepath, states)
{
	this->root_filepath_ = root_filepath;
	this->font_ = font;
	this->initVariables();
	this->initKeyBinds();
	this->initButtons();
	this->initHighscores();
	this->initSoundEffect();

	this->scrollUpperBoundary_ = 540; // window height / 2 + 10
	this->scrollLowerBoundary_ = 2030; // window height + 1500 - window height / 2
	
	// Set background to white
	this->background_.setSize(sf::Vector2f(window_->getSize().x, window_->getSize().y + 1500));
	this->background_.setFillColor(sf::Color::White);
}

//Destructor of Highscorestate class
Highscorestate::~Highscorestate()
{
	// Delete the buttons
	auto it = this->highscroeButtons_.begin();
	for (it = this->highscroeButtons_.begin(); it != highscroeButtons_.end(); ++it)
	{
		delete it->second;
	}
}

void Highscorestate::initSoundEffect()
{
    if (!this->buffer.loadFromFile(root_filepath_ + "/src/Audio/SoundEffects/Click.wav"))
    {
        throw "ERROR::TOWER::UNABLE_TO_LOAD_SOUND_EFFECT_FILE";
    }

    this->clickSound_.setBuffer(this->buffer);
}

//Reads the textfile and initializes every text object to be drawn starting from highest to lowest score
void Highscorestate::initHighscores() {
	int startingheight = 150;
	
	this->Readhighscores();
	
	sf::Text text;
	text.setFont(*this->font_);
	text.setCharacterSize(100);
	text.setFillColor(sf::Color::Red);
	text.setString("LEADERBOARD");
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(0.3*windowX_, 20));
	Texts_.push_back(text);
	text.setCharacterSize(50);
	
	for (auto map_high_score : highscores_) {

		// map name
		std::string map_name = map_high_score.first;
		text.setString(map_name);
		text.setPosition(sf::Vector2f(0.3*windowX_, startingheight));
		Texts_.push_back(text);
		startingheight += 50;

		int currenthighest = 1;

		for (auto i : map_high_score.second) { 

			// player scores
			text.setString(std::to_string(currenthighest) + ". " + i.second + " " + std::to_string(i.first));
			text.setPosition(sf::Vector2f(0.4*windowX_, startingheight));
			Texts_.push_back(text);
			startingheight += 50;
			currenthighest += 1;

		}

		startingheight += 50;
	}

}

//Initializes time and window variables
void Highscorestate::initVariables()
{
	// Initialize timer and the max value for the timer
	this->pressTimer_ .restart();
	this->pressTimerMax_ = 500;

	// Get window size
	windowX_ = static_cast<float>(this->window_->getSize().x);
	windowY_ = static_cast<float>(this->window_->getSize().y);
	std::cout << windowX_ << " and " << windowY_ << "\n";
	
}


//Initializes keybinds
void Highscorestate::initKeyBinds()
{
	// Open keys cpnfigure file
	std::ifstream ifs(root_filepath_ + "/src/Config/mapselector_state.ini");
	if (ifs.is_open())
	{
		std::string key = "";
		std::string key_name = "";
		while (ifs >> key >> key_name )
		{
			// Add supported keys to the supportedKeys map
			this->keybinds_[key] = this->supportedKeys_->at(key_name);
		}
	}
	// Close keys configure file
	ifs.close();

}

//Initializes Buttons
void Highscorestate::initButtons()
{
	std::cout << "X window " << windowX_ << "\n";
	// Add buttons
	this->highscroeButtons_["BACK"] = new Button(50, 50, 300, 150, this->font_, "Back", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 60, true);
	// std::cout << "X: "<< this->windowX_*0.4 << "Y: " << 3.75*this->windowY_ << "\n";
	// std::cout << this->view.getSize
}






// Functions

const bool Highscorestate::getPressTimer()
{
	// Check for timer
	if (this->pressTimer_.getElapsedTime().asMilliseconds() >= this->pressTimerMax_)
	{
		std::cout << "PRESSED" << "\n";
		this->pressTimer_.restart();
		return true;
	}
	return false;
}

//Adds new highscore to the textfile
void Highscorestate::Addhighscore(const std::string name, int highscore) {
	std::fstream File;
	File.open("highscores.txt", std::ios::app);
	if (File.is_open()) {
		File << "\n" << name << ";" << highscore;
		File.close();
	}
}

//Reads the textfile and puts every highscore to a list<pair> 
void Highscorestate::Readhighscores() {
	std::fstream File;
	File.open("highscores.txt", std::ios::in);
	if (File.is_open()) {
		std::string line;
		while (std::getline(File, line)) {

			// final line is empty
			if (line.length() < 2) {
				break;
			}

			std::string map_name;
			std::string name;
			std::string highscoretext;
			int highscore;

			std::stringstream ss(line);
			std::getline(ss, map_name, ';');
			std::getline(ss, name, ';');
			std::getline(ss, highscoretext, ';');
			highscore = std::stoi(highscoretext);

			std::pair<int, std::string> pari;
    		pari.second = name;
   			pari.first = highscore;
  		  	highscores_[map_name].push_back(pari);
		}
		File.close();
		
		for (auto map_high_score : highscores_) {
			//std::cout << map_high_score.second << "\n";
		map_high_score.second.sort(std::greater<>());
		}
	}
}


void Highscorestate::update(const float& dt)
{
	// Update called functions

	this->updateMousePosition();

	this->updateInput(dt);

	this->updateButtons();

}

void Highscorestate::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window_;
	}

	target->draw(this->background_);

	for(auto i : Texts_) {
		target->draw(i);
	}

	// Render buttons
	this->renderButtons(target);
	
}

void Highscorestate::updateInput(const float& dt)
{
	this->updateMousePosition();
	this->checkForQuit();
	
}

void Highscorestate::updateButtons()
{
	// Updates all the buttons in the state and handels their functionality
	for (auto &it : this->highscroeButtons_)
	{
		it.second->update(this->mousePosView);
	}

	sf::View view = window_->getView();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds_.at("SCROLL_UP"))) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds_.at("MOVE_UP")))) {
		// std::cout << "SCROLL_UP" << "\n";
		if (view.getCenter().y >= scrollUpperBoundary_)
		{
			view.move(0, -5);
			this->window_->setView(view);
		}
		// this->backgroundColor_.setPosition(view.getCenter().x - view.getCenter().x/2, view.getCenter().y - view.getCenter().y/2);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds_.at("SCROLL_DOWN"))) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds_.at("MOVE_DOWN")))) {
		// std::cout << "SCROLL_DOWN" << "\n";
		if (view.getCenter().y <= scrollLowerBoundary_)
		{
			view.move(0, 5);
			this->window_->setView(view);	
		}
		// this->backgroundColor_.setPosition(view.getCenter().x - view.getCenter().x/2, view.getCenter().y - view.getCenter().y/2);
	}

	// Quit the game
	if (this->highscroeButtons_["BACK"]->isPressed())
	{
		this->clickSound_.play();
		//if (getPressTimer())
		//{
			std::cout << "BACK PRESSED" << "\n";
			delete this->states->top();
			this->states->pop();
		//}	
	}
}

void Highscorestate::renderButtons(sf::RenderTarget* target)
{
	// Render buttons
	for (auto &it : this->highscroeButtons_)
	{
		it.second->render(target);
	}
}

void Highscorestate::endState()
{
	std::cout << "Ending game state!" << "\n";
}
