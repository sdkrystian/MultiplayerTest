// Copyright 2018 Krystian Stasiowski

#include "Player.h"
#include "Game.h"

Player::Player(const std::string& clientid, int x, int y, graphics::Color color) : clientid_(clientid), position_(x, y), color_(color)
{
  graphics::DrawPixelAt(x, y, color);
}

Player::Player() : clientid_(""), position_(-1, -1), color_(graphics::Color::WHITE) { }

void Player::MoveTo(int x, int y)
{
  if ((x < 0 || y < 0) || std::any_of(Game::Players().begin(), Game::Players().end(), [&](const Player& p) { return p.GetPosition().x == x && p.GetPosition().y == y; }))
    return;
  graphics::DrawPixelAt(position_.x, position_.y, graphics::Color::BLACK);
  position_.x = x;
  position_.y = y;
  graphics::DrawPixelAt(position_.x, position_.y, color_);
}

const std::string& Player::GetClientId()
{
  return clientid_;
}

const Vector& Player::GetPosition() const
{
  return position_;
}

const graphics::Color Player::GetColor()
{
  return color_;
}

simplenetworking::PacketData Player::CreatePacket()
{
  simplenetworking::PacketData data;
  data.AddValue("clientid", clientid_);
  data.AddValue("x", position_.x);
  data.AddValue("y", position_.y);
  data.AddValue("color", static_cast<int>(color_));
  return data;
}
