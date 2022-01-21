#include "main_menu_state.hpp"

// Constructor/Destructor functions

MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Event *Event) : State(window, supportedKeys, root_filepath, states)
{
	this->window_ = window;
	this->root_filepath_ = root_filepath;
	this->Event_ = Event;

	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeyBinds();
	this->initButtons();
	this->initMusic();

	// init background animation
	this->initBGAnimation();

	this->background_.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	this->background_.setFillColor(sf::Color::White );

	// Set background shader
	this->backgroundShader_.setPosition(sf::Vector2f(180, 0));
	this->backgroundShader_.setSize(sf::Vector2f(windowX_/3,windowY_));
	this->backgroundShader_.setFillColor(sf::Color(0, 0, 0, 130));

	// Volume text for music
	this->volumeLevelText_.setFont(font_);
	this->volumeLevelText_.setString(std::to_string(volumeLevel_) + " %");
	this->volumeLevelText_.setFillColor(sf::Color::Red);
	this->volumeLevelText_.setCharacterSize(70);
	this->volumeLevelText_.setPosition(windowX_-windowX_/4+240, windowY_-300+volumeLevelText_.getGlobalBounds().height/4); // 175 = button width + 25 pix

	// Volume text for sound effects
	this->soundEffectvolumeLevelText_.setFont(font_);
	this->soundEffectvolumeLevelText_.setString(std::to_string(soundEffectVolumeLevel_) + " %");
	this->soundEffectvolumeLevelText_.setFillColor(sf::Color::Red);
	this->soundEffectvolumeLevelText_.setCharacterSize(70);
	this->soundEffectvolumeLevelText_.setPosition(windowX_-windowX_/4+240, windowY_-200+volumeLevelText_.getGlobalBounds().height/4); // 175 = button width + 25 pix

	// Background text
	this->backgroundTextUsingFont_.setFont(font_);
	this->backgroundTextUsingFont_.setString("TOWER DEFENSE\nGroup 3");
	this->backgroundTextUsingFont_.setFillColor(sf::Color::Black);
	if (windowX_ > 1920)
		this->backgroundTextUsingFont_.setCharacterSize(120);
	else
		this->backgroundTextUsingFont_.setCharacterSize(90);
	this->backgroundTextUsingFont_.setPosition(windowX_-backgroundTextUsingFont_.getGlobalBounds().width-windowX_/20, windowY_/14);

}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons_.begin();
	for (it = this->buttons_.begin(); it != buttons_.end(); ++it)
	{
		delete it->second;
	}
}

// Initializers

void MainMenuState::initVariables()
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
	volumeLevel_ = 50;
	soundEffectVolumeLevel_ = 50;
}

void MainMenuState::initBackground()
{
	// Set background size to the window size
	this->backgroundText_.setSize(sf::Vector2f(static_cast<float>(this->window_->getSize().x), static_cast<float>(this->window_->getSize().y)));

	// Load background png file
	if(!this->backgroundTexture_.loadFromFile(root_filepath_ + "/src/Textures/Backgrounds/Menu_Text_Transparent.png"))
	{
		throw "ERROR::MAINMENUSTATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}

	// Set background texture
	this->backgroundText_.setTexture(&this->backgroundTexture_);
}

void MainMenuState::initKeyBinds()
{
	// Open keys cpnfigure file
	std::ifstream ifs(root_filepath_ + "/src/Config/mainmenustate_keybinds.ini");
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

	// !! TÄHÄN VOISI IMPLEMENTOIDA TESTIN!! 

}

void MainMenuState::initFonts()
{
	// Load a font
	std::cout << "LOADING FONT" << " " << root_filepath_ << "\n";
	if (!this->font_.loadFromFile(root_filepath_ + "/src/Fonts/Azonix.otf"))
	{
		// Throw a exception if the font doesn't load
		throw("ERROR::MAINMENUSTATE::UNABLE TO LOAD FONT");
	}

}

void MainMenuState::initButtons()
{
	// Get window size
	windowX_ = static_cast<float>(this->window_->getSize().x);
	windowY_ = static_cast<float>(this->window_->getSize().y);
	std::cout << windowX_ << " and " << windowY_ << "\n";

	// Initialize buttons
	this->buttons_["PLAY"] = new Button(windowX_/6, windowY_/2-275-150/2, 400, 150, &this->font_, "Play", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 60, false);
	this->buttons_["CREATEMAP"] = new Button(windowX_/6, windowY_/2-100-150/2, 400, 150, &this->font_, "Create map", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 60, false);
	this->buttons_["HIGHSCORES"] = new Button(windowX_/6, windowY_/2, 400, 150, &this->font_, "Highscores", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 60, false);
	this->buttons_["QUIT"] = new Button(windowX_/6, windowY_/2+100+150/2, 400, 150, &this->font_, "Quit", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 60, false);

	// Volume buttons
	this->buttons_["MORE"] = new Button(windowX_-windowX_/4, windowY_-300, 180, 100, &this->font_, "+ music", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 40, false);
	this->buttons_["LESS"] = new Button(windowX_-windowX_/4-250, windowY_-300, 180, 100, &this->font_, "- music", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 40, false);
	this->buttons_["MORE2"] = new Button(windowX_-windowX_/4, windowY_-200, 180, 100, &this->font_, "+ sfx", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 40, false);
	this->buttons_["LESS2"] = new Button(windowX_-windowX_/4-250, windowY_-200, 180, 100, &this->font_, "- sfx", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 40, false);
}

void MainMenuState::initMusic()
{
	// Initialize music
	if (!this->backgroundMusic_.openFromFile(root_filepath_ + "/src/Audio/BackgroundMusic/aura.wav"))
	{
		throw "ERROR::MAINMENUSTATE::UNABLE_TO_LOAD_BACKGROUND_MUSIC_FILE";
	}
	if (!this->buffer.loadFromFile(root_filepath_ + "/src/Audio/SoundEffects/Click.wav"))
    {
        throw "ERROR::TOWER::UNABLE_TO_LOAD_SOUND_EFFECT_FILE";
    }

    this->clickSound_.setBuffer(this->buffer);
	this->backgroundMusic_.play();
	this->backgroundMusic_.setLoop(true);
	this->backgroundMusic_.setVolume(volumeLevel_);
}

void MainMenuState::initBGAnimation() // For background animation
{
	rocketMoveSpeed_ = 550.f;
	// Load textures
	if(!this->rocketTexture_.loadFromFile(root_filepath_ + "/src/Textures/mainMenu/Menu_Rocket.png"))
	{
		throw "ERROR::MAINMENUSTATE::FAILED_TO_LOAD_ROCKET_TEXTURE";
	}
	if(!this->rocketExhaustTX_.loadFromFile(root_filepath_ + "/src/Textures/mainMenu/towerDefense_tile020.png"))
	{
		throw "ERROR::MAINMENUSTATE::FAILED_TO_LOAD_ROCKET_EXHAUST_TEXTURE";
	}
	// Set textures
	this->rocket_.setTexture(this->rocketTexture_);
	this->rocketExhaust_.setTexture(this->rocketExhaustTX_);

	// set initial position
	std::cout << windowX_+rocketTexture_.getSize().x << "\n";
	this->rocket_.setPosition(sf::Vector2f(windowX_, windowY_));
	this->rocket_.setRotation(315);
	this->rocketExhaust_.setPosition(sf::Vector2f(rocket_.getPosition().x+1500, rocket_.getPosition().y+1100));
	this->rocketExhaust_.setScale(sf::Vector2f(2, 2));


	// Set initial pass state
	passState_ = true;

}

// Functions

int MainMenuState::getSFXVolume()
{
	return soundEffectVolumeLevel_;
}

const bool MainMenuState::getTimer()
{
	// Check for timer
	if (this->timer_.getElapsedTime().asMilliseconds() >= this->timerMax_)
	{
		//std::cout << "PRESSED" << "\n";
		this->timer_.restart();
		return true;
	}
	return false;
}

const bool MainMenuState::getPressTimer()
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

void MainMenuState::update(const float& dt)
{
	//
	this->updateMousePosition();
	this->updateInput(dt);
	this->updateButtons();
	this->updateAnimation(dt);

}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window_;
	}

	target->draw(this->background_); 
	target->draw(this->rocket_);
	target->draw(this->rocketExhaust_);
	//target->draw(this->backgroundText_);
	target->draw(this->backgroundTextUsingFont_);
	target->draw(this->backgroundShader_);

	// Render buttons
	this->renderButtons(target);
	target->draw(volumeLevelText_);
	target->draw(soundEffectvolumeLevelText_);
}

void MainMenuState::updateInput(const float& dt)
{
	this->updateMousePosition();
	this->checkForQuit();

	//std::cout << this->mousePosView.x << " " << this->mousePosView.y << "\n"; //For debugging

}

void MainMenuState::updateButtons()
{
	// Updates all the buttons in the state and handels their functionality
	for (auto &it : this->buttons_)
	{
		it.second->update(this->mousePosView);
	}

		// New game
	if (this->buttons_["PLAY"]->isPressed())
	{
		if (getPressTimer())
		{
			this->clickSound_.play();
			backgroundMusic_.stop();
			//this->states->push(new GameState(this->window_, this->supportedKeys_, this->root_filepath_, this->states, "map.txt"));
			this->states->push(new Mapselector(this->window_, this->supportedKeys_, this->root_filepath_, this->states, &this->font_, this->Event_, this->volumeLevel_, this->soundEffectVolumeLevel_));
		}	
	}

	//Map creation state
	if (this->buttons_["CREATEMAP"]->isPressed())
	{
		if (getPressTimer())
		{
			this->clickSound_.play();
			this->states->push(new Createmap(this->window_, this->supportedKeys_, this->root_filepath_, this->states, &this->font_, this->Event_));
		}		
	}

	//Highscores
	if (this->buttons_["HIGHSCORES"]->isPressed())
	{
		if (getPressTimer())
		{
			this->clickSound_.play();
			this->states->push(new Highscorestate(this->window_, this->supportedKeys_, this->root_filepath_, this->states, &this->font_));
		}		
	}

	// Quit the game
	if (this->buttons_["QUIT"]->isPressed())
	{
		if (getPressTimer())
		{
			this->clickSound_.play();
			this->quit_ = true;
		}		
	}

	// Volume
	// + music
	
	if (this->buttons_["MORE"]->isPressed())
	{
		if (getPressTimer())
		{
			this->clickSound_.play();
			if (volumeLevel_ != 100)
			{
				// Add volume
				volumeLevel_ += 5;

			}
			// Update volume level
			this->backgroundMusic_.setVolume(volumeLevel_);
			this->volumeLevelText_.setString(std::to_string(volumeLevel_) + " %");
		}		
		
	}
	// - music
	if (this->buttons_["LESS"]->isPressed())
	{
		if (getPressTimer())
		{
			this->clickSound_.play();
			if (volumeLevel_ != 0)
			{
				// Lessen volume
				volumeLevel_ -= 5;
				
			}
			// Update volume level
			this->backgroundMusic_.setVolume(volumeLevel_);
			this->volumeLevelText_.setString(std::to_string(volumeLevel_) + " %");
		}	
		
	}
	// + sfx
	if (this->buttons_["MORE2"]->isPressed())
	{
		if (getPressTimer())
		{
			this->clickSound_.play();
			if (soundEffectVolumeLevel_ != 100)
			{
				// Add volume
				soundEffectVolumeLevel_ += 5;

			}
			// Update volume level
			this->soundEffectvolumeLevelText_.setString(std::to_string(soundEffectVolumeLevel_) + " %");
		}		
		
	}
	// - sfx
	if (this->buttons_["LESS2"]->isPressed())
	{
		if (getPressTimer())
		{
			this->clickSound_.play();
			if (soundEffectVolumeLevel_ != 0)
			{
				// Lessen volume
				soundEffectVolumeLevel_ -= 5;
				
			}
			// Update volume level
			this->soundEffectvolumeLevelText_.setString(std::to_string(soundEffectVolumeLevel_) + " %");
		}	
		
	}
}

void MainMenuState::updateAnimation(const float& dt)
{
	// Move sprites
	this->rocket_.move(-1 * rocketMoveSpeed_ * dt, -1 * rocketMoveSpeed_ * dt);
	this->rocketExhaust_.move(1 * rocketMoveSpeed_*3 * dt, 1 * rocketMoveSpeed_*3 * dt);

	//this->rocketExhaust_.setPosition(sf::Vector2f(rocket_.getPosition().x+1500, rocket_.getPosition().y+1100));

	//std::cout << this->rocket_.getPosition().x << " " << this->rocket_.getPosition().y << "\n";

	if (getTimer())
	{
		if (passState_)
			this->rocketExhaust_.setPosition(sf::Vector2f(rocket_.getPosition().x+1500, rocket_.getPosition().y+1100));
		else
			this->rocketExhaust_.setPosition(sf::Vector2f(rocket_.getPosition().x+750, rocket_.getPosition().y+550));
	}

	// Animate different positions
	if (this->rocket_.getPosition().y < -1450 && passState_)
	{
		this->rocket_.setScale(sf::Vector2f(0.5, 0.5));
		this->rocket_.setPosition(sf::Vector2f(windowX_+rocketTexture_.getSize().x, windowY_+rocketTexture_.getSize().y));
		this->rocketExhaust_.setScale(sf::Vector2f(1, 1));
		passState_ = false;
	}
	else if (this->rocket_.getPosition().y < -1450 && !passState_)
	{
		this->rocket_.setScale(sf::Vector2f(1, 1));
		this->rocket_.setPosition(sf::Vector2f(windowX_, windowY_));
		this->rocketExhaust_.setScale(sf::Vector2f(2, 2));
		passState_ = true;
	}
}

void MainMenuState::renderButtons(sf::RenderTarget* target)
{
	// Render buttons
	for (auto &it : this->buttons_)
	{
		it.second->render(target);
	}
}

void MainMenuState::endState()
{
	std::cout << "Ending main menu state!" << "\n";
}
