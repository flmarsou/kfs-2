#include "tty/draw/Button.hpp"
#include "drivers/vga/vga.hpp"

// ========================================================================== //
//    Constructors & Destructor                                               //
// ========================================================================== //

Button::Button(TTY &tty, u8 x, u8 y, const i8 *label,
				vga::Color selectedText, vga::Color selectedBack, vga::Color unselectedText, vga::Color unselectedBack)
	: tty_(tty)
	, x_(x)
	, y_(y)
	, label_(label)
	, selected_(false)
	, selectedText_(selectedText)
	, selectedBack_(selectedBack)
	, unselectedText_(unselectedText)
	, unselectedBack_(unselectedBack)
{

}

Button::~Button()
{

}

// ========================================================================== //
//    Getters                                                                 //
// ========================================================================== //

const i8	*Button::GetLabel() const
{
	return (label_);
}

// ========================================================================== //
//    Setters                                                                 //
// ========================================================================== //

void	Button::SetSelected(bool selected)
{
	selected_ = selected;
	Draw();
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	Button::Draw() const
{
	if (selected_)
	{
		tty_.SetBackgroundColor(selectedBack_);
		tty_.SetForegroundColor(selectedText_);
	}
	else
	{
		tty_.SetBackgroundColor(unselectedBack_);
		tty_.SetForegroundColor(unselectedText_);
	}
	tty_.PutStrAt(x_, y_, label_);
	tty_.SetDefaultColor();
}
