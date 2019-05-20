#pragma once

using namespace System;

namespace CommonNetworking {
	/*public ref class NetworkingConnector
	{
		void Client::SendData(String^ data)
		{
			if (!socket) return;
			try {
				if (!data || data->Trim()->Equals("")) data = "\0";
				array<Byte>^ b = Encoding::UTF8->GetBytes(data);
				Console::WriteLine("Sending: {0}", data);
				socket->Send(b);
				Console::WriteLine("Sent");
			}
			catch (...)
			{
				active = false;
			}
		}

		String^ Client::ReceiveData()
		{
			if (!socket) return "";
			String^ res = "";
			try {
				auto b = gcnew array<Byte>(65536);
				int bc = socket->Receive(b, 0, 65535, SocketFlags::None);
				res = Encoding::UTF8->GetString(b, 0, bc);
				Console::WriteLine("Response: {0}", res);
				return res;
			}
			catch (...) {
				active = false;
				return nullptr;
			}
		}*/




		// TODO: Добавьте сюда свои методы для этого класса.
	};

