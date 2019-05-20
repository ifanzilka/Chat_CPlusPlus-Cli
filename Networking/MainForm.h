#pragma once
#include "Client.h"

namespace WindowsForms {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		void Connect()//пытаемся поделючится
		{
			try {
				c = gcnew Client(tbAddress->Text);//берем адрес сервера из формы
				c->OnLoginResult += gcnew LoginHandler(this, &MainForm::LoginResponse);//событие
				c->OnMessageReceived += gcnew MessageHandler(this, &MainForm::MessageReceived);//событие
				connected = true;
				btnSend->Text = "Войти";//если вошли меняем надпись на конпке
			}
			catch (Exception^ ex)
			{
				tbChat->Text += ex->Message + "\r\n";//если не вошли то в чат выводим ощибку
				c = nullptr;
			}
		}
		MainForm(void)
		{
			InitializeComponent();
			//c = gcnew Client();
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  msg;
	protected:

	protected:
	private: System::Windows::Forms::Button^  btnSend;
	private: System::Windows::Forms::TextBox^  tbChat;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  tbAddress;


	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->msg = (gcnew System::Windows::Forms::TextBox());
			this->btnSend = (gcnew System::Windows::Forms::Button());
			this->tbChat = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tbAddress = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// msg
			// 
			this->msg->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->msg->Location = System::Drawing::Point(16, 519);
			this->msg->Margin = System::Windows::Forms::Padding(4);
			this->msg->Name = L"msg";
			this->msg->Size = System::Drawing::Size(475, 22);
			this->msg->TabIndex = 0;
			// 
			// btnSend
			// 
			this->btnSend->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btnSend->Location = System::Drawing::Point(500, 517);
			this->btnSend->Margin = System::Windows::Forms::Padding(4);
			this->btnSend->Name = L"btnSend";
			this->btnSend->Size = System::Drawing::Size(144, 28);
			this->btnSend->TabIndex = 1;
			this->btnSend->Text = L"Подключится";
			this->btnSend->UseVisualStyleBackColor = true;
			this->btnSend->Click += gcnew System::EventHandler(this, &MainForm::btnSend_Click);
			// 
			// tbChat
			// 
			this->tbChat->Location = System::Drawing::Point(16, 16);
			this->tbChat->Margin = System::Windows::Forms::Padding(4);
			this->tbChat->MaxLength = 32767000;
			this->tbChat->Multiline = true;
			this->tbChat->Name = L"tbChat";
			this->tbChat->Size = System::Drawing::Size(627, 466);
			this->tbChat->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(16, 495);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(110, 17);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Адрес сервера:";
			// 
			// tbAddress
			// 
			this->tbAddress->Location = System::Drawing::Point(133, 495);
			this->tbAddress->Name = L"tbAddress";
			this->tbAddress->Size = System::Drawing::Size(358, 22);
			this->tbAddress->TabIndex = 4;
			this->tbAddress->Text = L"127.0.0.1";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(661, 559);
			this->Controls->Add(this->tbAddress);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->tbChat);
			this->Controls->Add(this->btnSend);
			this->Controls->Add(this->msg);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"MainForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		Client^ c;
		bool logged = false;
		bool connected = false;
	private:
		System::Void btnSend_Click(System::Object^  sender, System::EventArgs^  e) {
			if (!connected) Connect();
			else if (!logged) c->Login(msg->Text);
			else c->SendMessage(msg->Text);
		}

		void LoginResponse(bool result)
		{
			if (InvokeRequired)
			{
				Invoke(
					gcnew LoginHandler(this, &MainForm::LoginResponse),
					result
				);
			}
			else {
				if (result)
				{
					btnSend->Text = "Отправить";
					logged = true;
				}
				else
				{
					tbChat->Text += "Не удалось войти с указанным ником." + "\r\n";
				}
			}
		}

		void MessageReceived(String^ author, String^ data)
		{
			if (InvokeRequired)
			{
				Invoke(
					gcnew MessageHandler(this, &MainForm::MessageReceived),
					author,
					data
				);
			}
			else {
				tbChat->Text += author + ":\t" + data + "\r\n";
			}
		}

	private: System::Void MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
		c->Stop();//если закрыли обрываем связ
	}
};
}
