#pragma once
#include "SimpleNetworking/SimpleNetworking.h"
#include "Graphics.h"
#include "Vector.h"

class Player
{
public:
  Player(const std::string& clientid, int x, int y, graphics::Color color);
  Player();
  void MoveTo(int x, int y);
  const std::string& GetClientId();
  const Vector& GetPosition() const;
  const graphics::Color GetColor();
  simplenetworking::PacketData CreatePacket();
private:
  std::string clientid_;
  Vector position_;
  graphics::Color color_;
};