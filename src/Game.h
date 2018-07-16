#pragma once
#include "Player.h"

class Game
{
public:
  static void SetLocal(const std::string&, Vector position, graphics::Color color);
  static void AddPlayer(const std::string& clientid, Vector position, graphics::Color color);
  static Player& GetPlayer(const std::string& clientid);
  static Player& GetLocal();
  static const std::vector<Player>& Players();
private:
  static Player local_;
  static std::vector<Player> players_;
};