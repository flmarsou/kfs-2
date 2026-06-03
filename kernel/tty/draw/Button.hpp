#pragma once

# include "tty/TTY.hpp"
# include "drivers/vga/vga.hpp"

class	Button
{
	public:

		// ================================================================== //
		//    Constructors & Destructor                                       //
		// ================================================================== //

		Button(TTY &tty, u8 x, u8 y, const i8 *label,
				vga::Color selectedText, vga::Color selectedBack, vga::Color unselectedText, vga::Color unselectedBack);
		~Button();

		// ================================================================== //
		//    Getters                                                         //
		// ================================================================== //

		const i8	*GetLabel() const;

		// ================================================================== //
		//    Setters                                                         //
		// ================================================================== //

		void	SetSelected(bool selected);

		// ================================================================== //
		//    Methods                                                         //
		// ================================================================== //

		void	Draw() const;

	private:

		// ================================================================== //
		//    Data                                                            //
		// ================================================================== //

		TTY	&tty_;

		u8	x_;
		u8	y_;

		const i8	*label_;

		bool	selected_;

		vga::Color	selectedText_;
		vga::Color	selectedBack_;
		vga::Color	unselectedText_;
		vga::Color	unselectedBack_;
};
