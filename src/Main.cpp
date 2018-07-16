#include "Game.h"
#include "Input.h"

simplenetworking::Server server;
simplenetworking::Client client;

void SendClientData(std::string cid, simplenetworking::PacketData packet)
{
  std::thread thread([&]()
  {
    Sleep(200);
    simplenetworking::ServerPacket outpacket("playerconnected");
    outpacket.data.AddValue("clientid", cid);
    outpacket.data.AddValue("x", packet.GetInt("x"));
    outpacket.data.AddValue("y", packet.GetInt("y"));
    outpacket.data.AddValue("color", packet.GetInt("color"));
    for (simplenetworking::ClientInfo& c : server.clients)
    {
      if (c.clientid != cid)
      {
        server.Send(c, simplenetworking::EConnectionType::TCP, outpacket);
        simplenetworking::ServerPacket playerpacket("playerconnected", Game::GetPlayer(c.clientid).CreatePacket());
        server.Send(server.GetClient(cid), simplenetworking::EConnectionType::TCP, playerpacket);
      }
    }
    server.Send(server.GetClient(cid), simplenetworking::EConnectionType::TCP, simplenetworking::ServerPacket("playerconnected", Game::GetLocal().CreatePacket()));
    Game::AddPlayer(cid, { packet.GetInt("x"), packet.GetInt("y") }, static_cast<graphics::Color>(packet.GetInt("color")));
  });
  thread.join();
}

void ClientConnect(std::string cid, simplenetworking::PacketData packet)
{
  SendClientData(cid, packet);
}

void ClientMove(std::string cid, simplenetworking::PacketData packet)
{
  simplenetworking::ServerPacket movepacket("move");
  movepacket.data.AddValue("clientid", cid);
  movepacket.data.AddValue("x", packet.GetInt("x"));
  movepacket.data.AddValue("y", packet.GetInt("y"));
  for (simplenetworking::ClientInfo& c : server.clients)
  {
    if (c.clientid != cid)
      server.Send(c, simplenetworking::EConnectionType::UDP, movepacket);
  }
  Game::GetPlayer(cid).MoveTo(packet.GetInt("x"), packet.GetInt("y"));
}

void ServerConnect(simplenetworking::PacketData packet)
{
  Game::SetLocal(client.clientid, { simplenetworking::util::RandomRange(0, 25), simplenetworking::util::RandomRange(0, 25) }, static_cast<graphics::Color>(simplenetworking::util::RandomRange(9, 15)));
  client.Send(simplenetworking::EConnectionType::TCP, simplenetworking::ClientPacket(client.clientid, "playerconnected", Game::GetLocal().CreatePacket()));
}

void ServerMove(simplenetworking::PacketData packet)
{
  Game::GetPlayer(packet.GetString("clientid")).MoveTo(packet.GetInt("x"), packet.GetInt("y"));
}

void ServerPlayerConnect(simplenetworking::PacketData packet)
{
  Game::AddPlayer(packet.GetString("clientid"), { packet.GetInt("x"), packet.GetInt("y") }, static_cast<graphics::Color>(packet.GetInt("color")));
}

void main()
{
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  GetConsoleCursorInfo(handle, &info);
  info.bVisible = false;
  SetConsoleCursorInfo(handle, &info);
  bool isserver = true;
  if (isserver)
  {
    server.Init(444, { simplenetworking::Request("playerconnected", ClientConnect), simplenetworking::Request("move", ClientMove)});
    server.Start();
    Game::SetLocal("server", { simplenetworking::util::RandomRange(0, 25), simplenetworking::util::RandomRange(0, 25) }, static_cast<graphics::Color>(simplenetworking::util::RandomRange(9, 15)));
    while (true)
    {
      if (Input::GetKeyDown(VK_UP))
      {
        simplenetworking::ServerPacket packet("move");
        packet.data.AddValue("clientid", "server");
        packet.data.AddValue("x", Game::GetLocal().GetPosition().x);
        packet.data.AddValue("y", Game::GetLocal().GetPosition().y - 1);
        server.SendAll(simplenetworking::EConnectionType::UDP, packet);
        Game::GetLocal().MoveTo(Game::GetLocal().GetPosition().x, Game::GetLocal().GetPosition().y - 1);
      }
      if (Input::GetKeyDown(VK_DOWN))
      {
        simplenetworking::ServerPacket packet("move");
        packet.data.AddValue("clientid", "server");
        packet.data.AddValue("x", Game::GetLocal().GetPosition().x);
        packet.data.AddValue("y", Game::GetLocal().GetPosition().y + 1);
        server.SendAll(simplenetworking::EConnectionType::UDP, packet);
        Game::GetLocal().MoveTo(Game::GetLocal().GetPosition().x, Game::GetLocal().GetPosition().y + 1);
      }
      if (Input::GetKeyDown(VK_LEFT))
      {
        simplenetworking::ServerPacket packet("move");
        packet.data.AddValue("clientid", "server");
        packet.data.AddValue("x", Game::GetLocal().GetPosition().x - 1);
        packet.data.AddValue("y", Game::GetLocal().GetPosition().y);
        server.SendAll(simplenetworking::EConnectionType::UDP, packet);
        Game::GetLocal().MoveTo(Game::GetLocal().GetPosition().x - 1, Game::GetLocal().GetPosition().y);
      }
      if (Input::GetKeyDown(VK_RIGHT))
      {
        simplenetworking::ServerPacket packet("move");
        packet.data.AddValue("clientid", "server");
        packet.data.AddValue("x", Game::GetLocal().GetPosition().x + 1);
        packet.data.AddValue("y", Game::GetLocal().GetPosition().y);
        server.SendAll(simplenetworking::EConnectionType::UDP, packet);
        Game::GetLocal().MoveTo(Game::GetLocal().GetPosition().x + 1, Game::GetLocal().GetPosition().y);
      }
    }
  }
  else
  {
    client.Init("127.0.0.1", 444, { simplenetworking::Command("playerconnected", ServerPlayerConnect), simplenetworking::Command("connected", ServerConnect), simplenetworking::Command("move", ServerMove) });
    client.Start();
    while (true)
    {
      if (Input::GetKeyDown('W'))
      {
        simplenetworking::ClientPacket packet(client.clientid, "move");
        packet.data.AddValue("clientid", client.clientid);
        packet.data.AddValue("x", Game::GetLocal().GetPosition().x);
        packet.data.AddValue("y", Game::GetLocal().GetPosition().y - 1);
        client.Send(simplenetworking::EConnectionType::UDP, packet);
        Game::GetLocal().MoveTo(Game::GetLocal().GetPosition().x, Game::GetLocal().GetPosition().y - 1);
      }
      if (Input::GetKeyDown('S'))
      {
        simplenetworking::ClientPacket packet(client.clientid, "move");
        packet.data.AddValue("clientid", client.clientid);
        packet.data.AddValue("x", Game::GetLocal().GetPosition().x);
        packet.data.AddValue("y", Game::GetLocal().GetPosition().y + 1);
        client.Send(simplenetworking::EConnectionType::UDP, packet);
        Game::GetLocal().MoveTo(Game::GetLocal().GetPosition().x, Game::GetLocal().GetPosition().y + 1);
      }
      if (Input::GetKeyDown('A'))
      {
        simplenetworking::ClientPacket packet(client.clientid, "move");
        packet.data.AddValue("clientid", client.clientid);
        packet.data.AddValue("x", Game::GetLocal().GetPosition().x - 1);
        packet.data.AddValue("y", Game::GetLocal().GetPosition().y);
        client.Send(simplenetworking::EConnectionType::UDP, packet);
        Game::GetLocal().MoveTo(Game::GetLocal().GetPosition().x - 1, Game::GetLocal().GetPosition().y);
      }
      if (Input::GetKeyDown('D'))
      {
        simplenetworking::ClientPacket packet(client.clientid, "move");
        packet.data.AddValue("clientid", client.clientid);
        packet.data.AddValue("x", Game::GetLocal().GetPosition().x + 1);
        packet.data.AddValue("y", Game::GetLocal().GetPosition().y);
        client.Send(simplenetworking::EConnectionType::UDP, packet);
        Game::GetLocal().MoveTo(Game::GetLocal().GetPosition().x + 1, Game::GetLocal().GetPosition().y);
      }
    }
  }
  std::cin.ignore();
}