#include "textbox.hpp"

// Constructor and destructor

Textbox::Textbox(float x, float y, float width, float height, sf::Font* font, std::string text,
	 sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor, int text_size, bool box, sf::Event *Event)
{
	Event_ = Event;
	text_size_ = text_size;
	boxBoolean_ = box;
	text_ = text;
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
		this->shape_.getPosition().x + 10,
		this->shape_.getPosition().y + (this->shape_.getGlobalBounds().height / 2.f) - this->buttonText_.getGlobalBounds().height / 2.f
		);

	this->idleColor_ = idleColor;
	this->hoverColor_ = hoverColor;
	this->activeColor_ = activeColor;

	this->shape_.setFillColor(this->idleColor_);
}

Textbox::~Textbox()
{

}

// Accessors

const bool Textbox::isPressed() const
{
	if (this->buttonState_ == BUTTON_ACTIVE)
	{
		return true;
	}

	return false;
}

// Functions

void Textbox::update(const sf::Vector2f mousePos)
{
	// Update the booleans for hover and pressed


	// Hover
	if (this->shape_.getGlobalBounds().contains(mousePos))
	{

		// Button pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Time elapsed = clock_.getElapsedTime();
			if(0.5 < elapsed.asSeconds()) {
				clock_.restart();
				if(this->buttonState_ == BUTTON_ACTIVE) {
					this->buttonState_ = BUTTON_IDLE;
				}
				else if(this->buttonState_ == BUTTON_IDLE) {
					this->buttonState_ = BUTTON_ACTIVE;
				}
			}
		}
	}

	// Change button color according to the button state
	switch (this->buttonState_)
	{
	case BUTTON_IDLE:
		this->shape_.setFillColor(this->idleColor_);
		this->buttonText_.setCharacterSize(text_size_);
		this->buttonText_.setFillColor(sf::Color::Red);
		this->shape_.setOutlineThickness(0);
		break;
	case BUTTON_ACTIVE:
		this->shape_.setFillColor(this->idleColor_);
		this->shape_.setOutlineColor(sf::Color::Blue);
		this->shape_.setOutlineThickness(1);
		this->buttonText_.setCharacterSize(text_size_);
		this->buttonText_.setFillColor(sf::Color::Red);
		break;
	default:
		this->shape_.setFillColor(sf::Color::Red); // If failure happens
		break;
	}

	if(this->Event_->type == sf::Event::TextEntered) {
		if(this->buttonState_ == 2) {
			if(this->Event_->text.unicode < 128) {
				sf::Time elapsed = clock_.getElapsedTime();
				if(0.15 < elapsed.asSeconds()) {
					clock_.restart();
					if (Event_->text.unicode == 8) {
    					if (!text_.empty()) {
       						text_.pop_back();
						}
					}
					else {
						if(text_.length() < 15) {
							this->text_ += (char)this->Event_->text.unicode;
						}

					}
					this->buttonText_.setString(this->text_);
		}
	}
	}
	}
}

void Textbox::render(sf::RenderTarget* target)
{
	// Draw shape and text on the window
	if (boxBoolean_)
		target->draw(this->shape_);
	target->draw(this->buttonText_);
}

std::string Textbox::GetText() {
	return this->text_;
}
