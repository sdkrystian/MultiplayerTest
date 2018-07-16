#include "Graphics.h"

void graphics::DrawPixelAt(short x, short y, Color color)
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(handle, { x, y });
  SetConsoleTextAttribute(handle, static_cast<WORD>(color) << 4);
  std::cout << " ";
  SetConsoleTextAttribute(handle, static_cast<WORD>(Color::WHITE));
}