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
	Socket^ socket;//подключение
	bool active = false;
	String^ name = "Anonymous";
	void SendData(String^ data);//переводим строку в байты и отправляем
	String^ ReceiveData();//получаем из байтов строку
	void ParseCommand(String^ req);//разютраем строку
	void Communicate();//обзение
public:
	event MessageHandler^ OnMessageReceived;//союытие получения сообщения
	event LoginHandler^ OnLoginResult;//событие
	Client(String^ serverHost);//конструктор
	void Login(String^ name);//отправляем на сервер логин и там проверям
	void SendMessage(String^ message);
	void Stop();
	virtual ~Client();
};