#pragma once
using namespace System;
using namespace System::Text;
using namespace Net;
using namespace Sockets;
using namespace Threading;
using namespace Collections::Generic;

delegate void LoginHandler(bool data);
delegate void MessageHandler(String^ author, String^ msg);

ref class Client
{
	String^ serverHost;
	int port = 5703;
	Socket^ socket;//�����������
	bool active = false;
	String^ name = "Anonymous";
	void SendData(String^ data);//��������� ������ � ����� � ����������
	String^ ReceiveData();//�������� �� ������ ������
	void ParseCommand(String^ req);//��������� ������
	void Communicate();//�������
public:
	event MessageHandler^ OnMessageReceived;//������� ��������� ���������
	event LoginHandler^ OnLoginResult;//�������
	Client(String^ serverHost);//�����������
	void Login(String^ name);//���������� �� ������ ����� � ��� ��������
	void SendMessage(String^ message);
	void Stop();
	virtual ~Client();
};