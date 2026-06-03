#include "apps/Menu.hpp"

# include "tty/input/Input.hpp"
# include "tty/draw/shapes.hpp"
# include "drivers/vga/vga.hpp"

// ========================================================================== //
//    Constructors & Destructor                                               //
// ========================================================================== //

Menu::Menu(TTY &tty)
	: tty_(tty)
	, currentColumn_(0)
	, currentRow_{0, 0}
{
	drawTitle();
	drawMenu();
	drawBanner();

	columns_[currentColumn_][currentRow_[currentColumn_]].SetSelected(true);
	drawButtons();
}

Menu::~Menu()
{

}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	Menu::Run()
{
	while (true)
	{
		while (Input::Poll())
		{
			const InputEvent	INPUT = Input::GetInput();

			switch (INPUT.type)
			{
				case (InputKey::ArrowUp):
				{
					columns_[currentColumn_][currentRow_[currentColumn_]].SetSelected(false);

					if (currentRow_[currentColumn_] > 0)
						--currentRow_[currentColumn_];
					else
						currentRow_[currentColumn_] = columnSize_[currentColumn_] - 1;

					columns_[currentColumn_][currentRow_[currentColumn_]].SetSelected(true);

					drawButtons();
					break ;
				}

				case (InputKey::ArrowDown):
				{
					columns_[currentColumn_][currentRow_[currentColumn_]].SetSelected(false);

					if (currentRow_[currentColumn_] < columnSize_[currentColumn_] - 1)
						++currentRow_[currentColumn_];
					else
						currentRow_[currentColumn_] = 0;

					columns_[currentColumn_][currentRow_[currentColumn_]].SetSelected(true);

					drawButtons();
					break ;
				}

				case (InputKey::ArrowLeft):
				{
					columns_[currentColumn_][currentRow_[currentColumn_]].SetSelected(false);

					if (currentColumn_ > 0)
						--currentColumn_;
					else
						currentColumn_ = columnAmount_ - 1;

					columns_[currentColumn_][currentRow_[currentColumn_]].SetSelected(true);

					drawButtons();
					break ;
				}

				case (InputKey::ArrowRight):
				{
					columns_[currentColumn_][currentRow_[currentColumn_]].SetSelected(false);

					if (currentColumn_ == columnAmount_ - 1)
						currentColumn_ = 0;
					else
						++currentColumn_;

					columns_[currentColumn_][currentRow_[currentColumn_]].SetSelected(true);

					drawButtons();
					break ;
				}

				default: break ;
			}

			asm volatile("hlt");
		}
	}
}

// ========================================================================== //
//    Functions                                                               //
// ========================================================================== //

void	Menu::drawTitle() const
{
	tty_.SetBackgroundColor(vga::Color::Blue);
	tty_.SetForegroundColor(vga::Color::White);
	tty_.Clear(' ');

	tty_.PutStrAt(8, 0, "                    ____  ____");
	tty_.PutStrAt(8, 1, " _    _____ _    __/ __ \\/ __/");
	tty_.PutStrAt(8, 2, "| |/|/ / _ \\ |/|/ / /_/ /\\ \\  ");
	tty_.PutStrAt(8, 3, "|__,__/\\___/__,__/\\____/___/  ");

	auto	drawBlock = [&](u8 x, vga::Color topColor, vga::Color bottomColor)
	{
		tty_.SetBackgroundColor(topColor);
		tty_.PutStrAt(x, 1, "   ");
		tty_.SetBackgroundColor(bottomColor);
		tty_.PutStrAt(x, 3, "   ");
	};

	u8	colorOffset = 48;

	drawBlock(colorOffset, vga::Color::Black, vga::Color::DarkGray);
	drawBlock(colorOffset += 3, vga::Color::Red, vga::Color::LightRed);
	drawBlock(colorOffset += 3, vga::Color::Green, vga::Color::LightGreen);
	drawBlock(colorOffset += 3, vga::Color::Brown, vga::Color::Yellow);
	drawBlock(colorOffset += 3, vga::Color::Blue, vga::Color::LightBlue);
	drawBlock(colorOffset += 3, vga::Color::Purple, vga::Color::LightPurple);
	drawBlock(colorOffset += 3, vga::Color::Cyan, vga::Color::LightCyan);
	drawBlock(colorOffset += 3, vga::Color::Gray, vga::Color::White);

	tty_.SetDefaultColor();
}

void	Menu::drawMenu() const
{
	tty_.SetBackgroundColor(vga::Color::Gray);
	shapes::quad(tty_, 1, 5, 38, 18, ' ');
	shapes::quad(tty_, 41, 5, 38, 18, ' ');

	tty_.SetBackgroundColor(vga::Color::DarkGray);
	tty_.PutStrAt(1, 5, " Applications                         ");
	tty_.PutStrAt(41, 5, " Games                                ");

	tty_.SetDefaultColor();
}

void	Menu::drawBanner() const
{
	tty_.SetBackgroundColor(vga::Color::LightBlue);
	tty_.PutStrAt(0, 24, "                                                                                ");

	tty_.SetForegroundColor(vga::Color::White);
	tty_.PutStrAt(2, 24, "Up/Down:");
	tty_.PutStrAt(28, 24, "Right/Left:");
	tty_.PutStrAt(60, 24, "Enter:");

	tty_.SetForegroundColor(vga::Color::Black);
	tty_.PutStrAt(11, 24, "Select Row");
	tty_.PutStrAt(40, 24, "Select Column");
	tty_.PutStrAt(67, 24, "Run Program");

	tty_.SetDefaultColor();
}

void	Menu::drawButtons() const
{
	for (i32 i = 0; i < columnSize_[0]; ++i)
		appButtons_[i].Draw();
	for (i32 i = 0; i < columnSize_[1]; ++i)
		gameButtons_[i].Draw();
}
