#pragma once

# include "tty/TTY.hpp"
# include "utils/stdint.hpp"
# include "tty/draw/Button.hpp"
# include "drivers/vga/vga.hpp"

class	Menu
{
	public:

		// ================================================================== //
		//    Constructors & Destructor                                       //
		// ================================================================== //

		Menu(TTY &tty);
		~Menu();

		// ================================================================== //
		//    Methods                                                         //
		// ================================================================== //

		void	Run();

	private:

		// ================================================================== //
		//    Functions                                                       //
		// ================================================================== //

		void	drawTitle() const;
		void	drawMenu() const;
		void	drawBanner() const;
		void	drawButtons() const;

	private:

		// ================================================================== //
		//    Data                                                            //
		// ================================================================== //

		TTY	&tty_;

		static inline constexpr i32	columnSize_[2] = {1, 1};
		static inline constexpr i32	columnAmount_ = 2;

		Button	appButtons_[columnSize_[0]] =
		{
			Button(tty_, 3, 7, "Terminal", vga::Color::White, vga::Color::LightBlue, vga::Color::Black, vga::Color::Gray),
		};

		Button	gameButtons_[columnSize_[1]] =
		{
			Button(tty_, 43, 7, "Blackjack", vga::Color::White, vga::Color::LightBlue, vga::Color::Black, vga::Color::Gray),
		};

		Button	*columns_[2] = {appButtons_, gameButtons_};

		i32	currentColumn_;
		i32	currentRow_[2];
};
