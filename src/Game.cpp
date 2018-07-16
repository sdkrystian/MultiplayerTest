#include "Game.h"
#include <algorithm>

Player Game::local_;
std::vector<Player> Game::players_ = std::vector<Player>();

void Game::SetLocal(const std::string& clientid, Vector position, graphics::Color color)
{
  local_ = Player(clientid, position.x, position.y, color);
}

void Game::AddPlayer(const std::string& clientid, Vector position, graphics::Color color)
{
  players_.emplace_back(clientid, position.x, position.y, color);
}

Player& Game::GetPlayer(const std::string& clientid)
{
  return *std::find_if(players_.begin(), players_.end(), [&clientid](Player& p){ return p.GetClientId() == clientid; });
}

Player& Game::GetLocal()
{
  return local_;
}

const std::vector<Player>& Game::Players()
{
  return players_;
}
