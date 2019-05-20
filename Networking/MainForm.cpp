#include "MainForm.h"

using namespace WindowsForms;

[STAThreadAttribute]
void main() 
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MainForm());
}