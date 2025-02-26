#include <TCP/Server/TCPServer.h>
#include <asio.hpp>

using asio::ip::tcp;

TCPServer::TCPServer(asio::io_context& ioContext)
: ioContext(ioContext)
{

}

void TCPServer::StartAccept()
{
  std::shared_ptr<TCPConnection> newConnection = TCPConnection::Create(ioContext, rxData);
  connections.push_back(newConnection);

  acceptor->async_accept(newConnection->GetSocket(),
      std::bind(&TCPServer::HandleAccept, this, newConnection,
        asio::placeholders::error));
}

void TCPServer::HandleAccept(std::shared_ptr<TCPConnection> newConnection, const asio::error_code& error)
{
    if (!error)
    {
      newConnection->Start();
    }else
    {
      newConnection->Close();
    }

    StartAccept();
}

void TCPServer::Listen(uint16_t listeningPort)
{
    acceptor.reset(new asio::ip::tcp::acceptor(ioContext, tcp::endpoint(tcp::v4(), listeningPort)));
    std::shared_ptr<TCPConnection> newConnection = TCPConnection::Create(ioContext, rxData);
    connections.push_back(newConnection);
    
    acceptor->async_accept(newConnection->GetSocket(),
        std::bind(&TCPServer::HandleAccept, this, newConnection,
          asio::placeholders::error));
               
  isListening = true;
}

void TCPServer::Run()
{
  ioContext.run();
}

void TCPServer::StopListen()
{
  acceptor->close();
  isListening = false; 

  for(std::shared_ptr<TCPConnection> connection : connections)
  {
    connection->Close();
  }
  connections.clear();
}

bool TCPServer::IsListening()
{
  return isListening;
}

std::string TCPServer::ConsumeRXData()
{
  std::string output = rxData;
  rxData = "";
  return output;
}

void TCPServer::SendMessage(const std::string& message)
{
  for(auto& connection : connections)
  {
    connection->SendMessage(message);
  }
}