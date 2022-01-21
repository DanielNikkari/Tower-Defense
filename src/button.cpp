#include "button.hpp"

// Constructor and destructor

Button::Button(float x, float y, float width, float height, sf::Font* font, std::string text,
	 sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor, int text_size, bool box)
{
	text_size_ = text_size;
	boxBoolean_ = box;
	this->buttonState_ = BUTTON_IDLE; // BUTTON_IDLE = 0 (check button.hpp enum)
	// Shape options
	this->shape_.setPosition(sf::Vector2f(x, y));
	this->shape_.setSize(sf::Vector2f(width, height));

	// Text options
	this->font_ = font;
	this->buttonText_.setFont(*this->font_);
	this->buttonText_.setString(text);
	this->buttonText_.setFillColor(sf::Color::Red);
	this->buttonText_.setCharacterSize(text_size_);
	// Set button text position
	this->buttonText_.setPosition(
		this->shape_.getPosition().x + (this->shape_.getGlobalBounds().width / 2.f) - this->buttonText_.getGlobalBounds().width / 2.f,
		this->shape_.getPosition().y + (this->shape_.getGlobalBounds().height / 2.f) - this->buttonText_.getGlobalBounds().height / 2.f
		);

	this->idleColor_ = idleColor;
	this->hoverColor_ = hoverColor;
	this->activeColor_ = activeColor;

	this->shape_.setFillColor(this->idleColor_);
}

Button::~Button()
{

}

// Accessors

const bool Button::isPressed() const
{
	if (this->buttonState_ == BUTTON_ACTIVE)
	{
		return true;
	}

	return false;
}

void Button::changeText(std::string text) {
	this->buttonText_.setString(text);
	this->buttonText_.setPosition(
		this->shape_.getPosition().x + (this->shape_.getGlobalBounds().width / 2.f) - this->buttonText_.getGlobalBounds().width / 2.f,
		this->shape_.getPosition().y + (this->shape_.getGlobalBounds().height / 2.f) - this->buttonText_.getGlobalBounds().height / 2.f
		);
}

// Functions

void Button::update(const sf::Vector2f mousePos)
{
	// Update the booleans for hover and pressed

	// Idle
	this->buttonState_ = BUTTON_IDLE;

	// Hover
	if (this->shape_.getGlobalBounds().contains(mousePos))
	{
		this->buttonState_ = BUTTON_HOVER;

		// Button pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->buttonState_ = BUTTON_ACTIVE;
		}
	}

	// Change button color according to the button state
	switch (this->buttonState_)
	{
	case BUTTON_IDLE:
		this->shape_.setFillColor(this->idleColor_);
		this->buttonText_.setCharacterSize(text_size_);
		this->buttonText_.setFillColor(sf::Color::Red);
		break;
	case BUTTON_HOVER:
		this->shape_.setFillColor(this->hoverColor_);
		this->buttonText_.setCharacterSize(text_size_+5);
		this->buttonText_.setFillColor(sf::Color::Green);
		break;
	case BUTTON_ACTIVE:
		this->shape_.setFillColor(this->activeColor_);
		this->buttonText_.setCharacterSize(text_size_+3);
		this->buttonText_.setFillColor(sf::Color::Green);
		break;
	default:
		this->shape_.setFillColor(sf::Color::Red); // If failure happens
		break;
	}
}

void Button::render(sf::RenderTarget* target)
{
	// Draw shape and text on the window
	if (boxBoolean_)
		target->draw(this->shape_);
	target->draw(this->buttonText_);
}
