
#include "mapselector_state.hpp"

// Constructor/Destructor functions

Mapselector::Mapselector(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Font *font, sf::Event* Event, int volumeLevel, int soundEffectVolumeLevel) : State(window, supportedKeys, root_filepath, states)
{
	this->root_filepath_ = root_filepath;
	this->Event_ = Event;
	this->font_ = font;
	this->volumeLevel_ = volumeLevel;
	this->soundEffectVolumeLevel_ = soundEffectVolumeLevel;
	this->scrollUpperBoundary_ = 540; // window height / 2 + 10
	this->scrollLowerBoundary_ = 2030; // window height + 1500 - window height / 2
	this->initKeyBinds();
	this->openFolder();
	this->initVariables();
	this->initSoundEffect();

	// Set background to white
	this->backgroundColor_.setSize(sf::Vector2f(window_->getSize().x, window_->getSize().y + 1500));
	this->backgroundColor_.setFillColor(sf::Color::White);
}

Mapselector::~Mapselector()
{
	for(auto i : buttons_) {
		delete i.second;
	}

}

// Initializers

void Mapselector::initKeyBinds()
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
			std::cout << key_name << "\n";
		}
	}
	// Close keys configure file
	ifs.close();

	// !! TÄHÄN VOISI IMPLEMENTOIDA TESTIN!! 

}

void Mapselector::initSoundEffect()
{
    if (!this->buffer.loadFromFile(root_filepath_ + "/src/Audio/SoundEffects/Click.wav"))
    {
        throw "ERROR::TOWER::UNABLE_TO_LOAD_SOUND_EFFECT_FILE";
    }

    this->clickSound_.setBuffer(this->buffer);
}

void Mapselector::openFolder() {
	int x = 750;
	int y = 200;
	for(const auto & file : std::filesystem::directory_iterator(root_filepath_ + "/src/Maps/")) {
		this->buttons_[(std::string)file.path().filename()] = new Button(x, y, 400, 150, this->font_, (std::string)file.path().filename(), sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 60, false);
		y += 200;
	}
}

void Mapselector::initVariables()
{
	// Get window size
	windowX_ = static_cast<float>(this->window_->getSize().x);
	windowY_ = static_cast<float>(this->window_->getSize().y);
	std::cout << windowX_ << " and " << windowY_ << "\n";
	// Initialize timer and the max value for the timer
	this->timer_ .restart();
	this->timerMax_ = 200;

	// Initialize timer for pressing and the max value for the press timer
	this->pressTimer_.restart();
	pressTimerMax_ = 100;

	// Initialize volume
}

const bool Mapselector::getPressTimer()
{
	// Check for press timer
	if (this->pressTimer_.getElapsedTime().asMilliseconds() >= this->pressTimerMax_)
	{
		//std::cout << "PRESSED" << "\n";
		this->pressTimer_.restart();
		return true;
	}
	return false;
}

// Functions

void Mapselector::update(const float& dt)
{
	// Update called functions

	this->updateMousePosition();

	this->updateInput(dt);

	for (auto &it : this->buttons_)
	{
		it.second->update(this->mousePosView);        
	}

	sf::View view = window_->getView();
	// std::cout << view.getCenter().x << " and " << view.getCenter().y << "\n";
	/*if(Event_->type == sf::Event::MouseWheelScrolled) {
		int x = Event_->mouseWheelScroll.delta;
		view.move(0, -x);
		this->window_->setView(view);
		
	}*/

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

	for(auto i : buttons_) {
		if (this->buttons_[i.first]->isPressed()) {
			if (getPressTimer())
			{
				this->clickSound_.play();
				view = this->window_->getDefaultView();
				this->window_->setView(view);
				this->states->push(new GameState(this->window_, this->supportedKeys_, this->root_filepath_, this->states, this->Event_, i.first, font_, this->volumeLevel_, this->soundEffectVolumeLevel_));
			}	
		}
	}
}

void Mapselector::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window_;
	}

	target->draw(this->backgroundColor_);

	for (auto &it : this->buttons_)
	{
		it.second->render(target);
	}
}

void Mapselector::updateInput(const float& dt)
{
	this->checkForQuit();
	// Update keys pressed and move the entity accordingly
	
}

void Mapselector::endState()
{
	sf::View view = this->window_->getDefaultView();
	this->window_->setView(view);
	std::cout << "Ending game state!" << "\n";
}
