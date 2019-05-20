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
	Socket^ socket;//���� �����������
	String^ name;
	List<ClientModel^>^ clients;//c����� ��������
	bool active = false;
public:
	ClientModel(Socket^ s, List<ClientModel^>^ clients);//����������� ������� ��������
private:
	void ParseData(String^ req);//��������� ������
	void Communicate();
	void SendData(String^ data);//���������� ������
	String^ ReceiveData();//�������� ������ �� ���������� ������
	void Login(String^ data);
	void SendMessage(String^ msg);
	void Stop();
	void ShutDownClient(String^ data)//�������� ����������
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
	List<ClientModel^>^ clients = gcnew List<ClientModel^>();//������ ��������
public:
	Server();
	virtual ~Server();
};