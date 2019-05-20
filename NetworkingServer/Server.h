#pragma once
using namespace System;
using namespace Net;
using namespace Sockets;
using namespace Text;
using namespace Threading;
using namespace Collections::Generic;

delegate void CommandHandler(List<String^>^ data);

ref class ClientModel
{
	Socket^ socket;//само подключение
	String^ name;
	List<ClientModel^>^ clients;//cписок клиентов
	bool active = false;
public:
	ClientModel(Socket^ s, List<ClientModel^>^ clients);//конструктор моделей коиентов
private:
	void ParseData(String^ req);//разбираем строку
	void Communicate();
	void SendData(String^ data);//отправляем строку
	String^ ReceiveData();//получаям строку из полученных байтов
	void Login(String^ data);
	void SendMessage(String^ msg);
	void Stop();
	void ShutDownClient(String^ data)//обрываем соединение
	{
		try
		{
			active = false;
			if (socket)
			{
				socket->Shutdown(SocketShutdown::Both);
				socket->Disconnect(false);
				socket->Close();
				clients->Remove(this);
			}
		}
		catch (...) {}
	}
public:
	property String^ Name
	{
		String^ get()
		{
			return name;
		}
	}
};

ref class Server
{
	int port = 5703;
	String^ host = "127.0.0.1";
	List<ClientModel^>^ clients = gcnew List<ClientModel^>();//список клинетов
public:
	Server();
	virtual ~Server();
};