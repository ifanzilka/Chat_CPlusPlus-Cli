#include "Server.h"

Server::Server()//����������� (����� ��������� ������)
{
	Socket^ sSocket = gcnew Socket(
		SocketType::Stream,
		ProtocolType::Tcp//�������� ����������
	);
	try
	{
		sSocket->Bind(
			gcnew IPEndPoint(
				Dns::GetHostEntry(host)->AddressList[0],//������ ip
				port
			)
		);
		sSocket->Listen(10);//
		while (true) {
			Socket^ cSocket = sSocket->Accept();//���� ����������� Acceept ���������� ���������� �����
			gcnew ClientModel(cSocket, clients);//���� ��� �� ���������� ������� �������
		}
	}
	catch (Exception^ ex)
	{
		Console::WriteLine("���-�� ����� �� ���... " + ex->Message);
		Console::ReadLine();
	}
}

Server::~Server()
{
}


ClientModel::ClientModel(Socket^ s, List<ClientModel^>^ clients)//����������� �������
{
	socket = s;
	name = "Anonymous";
	this->clients = clients;
	clients->Add(this);//��������� � ������ ��������
	ThreadStart^ ts = gcnew ThreadStart(this, &ClientModel::Communicate);//�������� ����� ��� �������
	Thread^ t = gcnew Thread(ts);
	active = true;
	t->Start();
}

void ClientModel::ParseData(String^ req)//��������� ������
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
		if (!socket || !socket->Connected) throw gcnew Exception();//��������� ��������� �� ������
		if (!data || data->Trim()->Equals("")) data = "\0";//�������� ������ ������ ��� ���
		array<Byte>^ b = Encoding::UTF8->GetBytes(data);//������ � �����
		Console::WriteLine("Sending: {0}", data);
		socket->Send(b);//���������� ����� ������
		Console::WriteLine("Sent");
	}
	catch (Exception^ ex)
	{
		active = false;
		clients->Remove(this);
	}
}

String^ ClientModel::ReceiveData()//���� �������� ����� �� ����������� � ������
{
	String^ res = "";
	try {
		if (!socket || !socket->Connected) throw gcnew Exception();
		auto b = gcnew array<Byte>(65536);
		int bc = socket->Receive(b, 0, 65535, SocketFlags::None);
		res = Encoding::UTF8->GetString(b, 0, bc);//��������� ������ � �����
		Console::WriteLine("Request: {0}", res);//����� ���������� ��������� � ��������� ������
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
	Console::WriteLine("Starting communication");//������ ������� � ��������
	while (active) {
		Console::WriteLine("Receiving request...");//���� ������� �� �������
		auto req = ReceiveData();
		ParseData(req);
	}
}

void ClientModel::Login(String^ data)//���� ������ ������ � �������
{
	bool ok = true;
	for each(ClientModel^ c in clients)//�������� ��� �� ������ ������
	{
		if (c->Name->Equals(data))
		{
			ok = false;
			SendData("Login: Failed. Client already logged in.");//���� ���� �� ���������� ���������
			break;
		}
	}
	if (ok) {//���� ���� �� 
		SendData("Login: OK");
		name = data;
		SendMessage(name + " ����� � ���...");
	}
}

void ClientModel::SendMessage(String^ data)//�������� ���������
{
	for each(ClientModel^ c in clients)//��������� ���� ������� �� ������
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
