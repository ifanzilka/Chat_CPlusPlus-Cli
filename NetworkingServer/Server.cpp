#include "Server.h"

Server::Server()//конструктор (когда запустили сервер)
{
	Socket^ sSocket = gcnew Socket(
		SocketType::Stream,
		ProtocolType::Tcp//протокол соединения
	);
	try
	{
		sSocket->Bind(
			gcnew IPEndPoint(
				Dns::GetHostEntry(host)->AddressList[0],//первый ip
				port
			)
		);
		sSocket->Listen(10);//
		while (true) {
			Socket^ cSocket = sSocket->Accept();//ждем подключения Acceept возвращает клиентский сокет
			gcnew ClientModel(cSocket, clients);//если кто то соединился создаем клиента
		}
	}
	catch (Exception^ ex)
	{
		Console::WriteLine("Что-то пошло не так... " + ex->Message);
		Console::ReadLine();
	}
}

Server::~Server()
{
}


ClientModel::ClientModel(Socket^ s, List<ClientModel^>^ clients)//конструктор клиента
{
	socket = s;
	name = "Anonymous";
	this->clients = clients;
	clients->Add(this);//добавляем в список клиентов
	ThreadStart^ ts = gcnew ThreadStart(this, &ClientModel::Communicate);//выделяем поток для общения
	Thread^ t = gcnew Thread(ts);
	active = true;
	t->Start();
}

void ClientModel::ParseData(String^ req)//разбираем строку
{
	if (!req || req->Trim()->Equals("")) return;
	array<String^>^ sep = { ":" };
	array<String^>^ parts = req->Split(sep, 2, StringSplitOptions::None);
	if (parts[0]->ToUpper()->Equals("LOGIN"))
	{
		Login(parts[1]);
	}
	else if (parts[0]->ToUpper()->Equals("MESSAGE"))
	{
		SendMessage(name + ": " + parts[1]);
	}
	else if (parts[0]->ToUpper()->Equals("SHUTDOWN"))
	{
		ShutDownClient("");
	}
}

void ClientModel::SendData(String^ data)
{
	try {
		if (!socket || !socket->Connected) throw gcnew Exception();//проверяем подключен ли клиент
		if (!data || data->Trim()->Equals("")) data = "\0";//проверям пустая строка или нет
		array<Byte>^ b = Encoding::UTF8->GetBytes(data);//строку в байты
		Console::WriteLine("Sending: {0}", data);
		socket->Send(b);//отпраыляем этому сокету
		Console::WriteLine("Sent");
	}
	catch (Exception^ ex)
	{
		active = false;
		clients->Remove(this);
	}
}

String^ ClientModel::ReceiveData()//если получили байты то преобразуем в строку
{
	String^ res = "";
	try {
		if (!socket || !socket->Connected) throw gcnew Exception();
		auto b = gcnew array<Byte>(65536);
		int bc = socket->Receive(b, 0, 65535, SocketFlags::None);
		res = Encoding::UTF8->GetString(b, 0, bc);//переводим строку в байты
		Console::WriteLine("Request: {0}", res);//пишем полученное сообщение в командной строке
		return res;
	}
	catch (Exception^ ex) {
		active = false;
		clients->Remove(this);
		return "";
	}
}

void ClientModel::Communicate()
{
	if (!socket) return;
	Console::WriteLine("Starting communication");//начали общение с клиентом
	while (active) {
		Console::WriteLine("Receiving request...");//ждем команды от клиента
		auto req = ReceiveData();
		ParseData(req);
	}
}

void ClientModel::Login(String^ data)//если пришла комнда с логином
{
	bool ok = true;
	for each(ClientModel^ c in clients)//проверям нет ли такого логина
	{
		if (c->Name->Equals(data))
		{
			ok = false;
			SendData("Login: Failed. Client already logged in.");//если есть то отправляем сообщение
			break;
		}
	}
	if (ok) {//если нету то 
		SendData("Login: OK");
		name = data;
		SendMessage(name + " вошёл в чат...");
	}
}

void ClientModel::SendMessage(String^ data)//отправка сообщения
{
	for each(ClientModel^ c in clients)//отправлям всем сокетам из списка
	{
		c->SendData("Message:" + data);
	}
}

void ClientModel::Stop()
{

}

int main()
{
	gcnew Server();
}
