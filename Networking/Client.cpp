#include "Client.h"

void Client::SendData(String^ data)//переводим строку в байты и отправл€ем
{
	if (!socket) return;//если есть подключение
	try {
		if (!data || data->Trim()->Equals("")) data = "\0";
		array<Byte>^ b = Encoding::UTF8->GetBytes(data);//строки переводим в байты
		Console::WriteLine("Sending: {0}", data);
		socket->Send(b);//отправл€ем байты
		Console::WriteLine("Sent");
	}
	catch (...)
	{
		active = false;
	}
}

String^ Client::ReceiveData()//получаем из байтов строку
{
	if (!socket) return "";
	String^ res = "";
	try {
		auto b = gcnew array<Byte>(65536);//автоматически определ€ем тип
		int bc = socket->Receive(b, 0, 65535, SocketFlags::None);
		res = Encoding::UTF8->GetString(b, 0, bc);
		Console::WriteLine("Response: {0}", res);
		return res;
	}
	catch (...) {
		active = false;
		return nullptr;
	}
}

void Client::ParseCommand(String^ req)//разбираем команду
{
	if (!req || req->Trim()->Equals("")) return;
	array<String^>^ sep = { ":" };
	array<String^>^ parts = req->Split(sep, 2, StringSplitOptions::None);//разбираем полученную строку на две строки и закидываем в массив (раздел€ем с ':')
	if (parts[0]->ToUpper()->Equals("LOGIN"))//если первый элемент логин 
	{
		OnLoginResult(parts[1]->Contains("OK"));
	}
	else if (parts[0]->ToUpper()->Equals("MESSAGE"))//если первый элемент сообщение
	{
		array<String^>^ am = parts[1]->Split(sep, 2, StringSplitOptions::None);
		if (am->Length > 1) {
			OnMessageReceived(am[0], am[1]);
		}
		else
		{
			OnMessageReceived("„атЅот", am[0]);
		}
	}
}

void Client::Communicate()
{
	if (!socket) return;
	Console::WriteLine("Starting communication");
	while (active) {
		Console::WriteLine("Receiving response...");//получение ответа
		auto req = ReceiveData();//получение сообщени€
		ParseCommand(req);
	}
}

Client::Client(String^ serverHost)//конструктор
{
	socket = gcnew Socket(
		SocketType::Stream,
		ProtocolType::Tcp
	);
	this->serverHost = serverHost;
	socket->Connect(this->serverHost, port);
	ThreadStart^ th = gcnew ThreadStart(this, &Client::Communicate);//выдел€ем поток
	Thread^ t = gcnew Thread(th);
	active = true;
	t->Start();//запускаем поток
	if (!socket->Connected)//если не соединились
	{
		socket = nullptr;
		throw gcnew Exception("ќшибка соединени€");
	}
}

void Client::Login(String^ name)//отправл€ем на серврел логин и там проверка
{
	if (name->Equals("Login")|| name->Equals(" ")) name = "Anonymous";//есои не указали им€ то будет аноним
	SendData("Login:" + name);
}

void Client::SendMessage(String^ message)
{
	SendData("Message:" + message);//отправл€ем строку
}

void Client::Stop()
{
	try
	{
		SendData("SHUTDOWN:1");
		active = false;
		if (socket)
		{
			socket->Shutdown(SocketShutdown::Both);//закрываем соединение
			socket->Disconnect(false);
			socket->Close();
		}
	}
	catch (...) {}
}

Client::~Client()
{
	Stop();
}
