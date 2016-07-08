#include "pbsApplication.h"

#include "config.h"
#include "Controllers/deliveryController.h"

#include <iostream>

NS_USING_DRAGON

void pbsApplication::Start()
{
	Application::Start();
	std::cout << "DE Start at path:" << appPath_ << std::endl;

	ADD_CONTROLLER(deliveryController);
}

void pbsApplication::End()
{
	std::cout << "DE End" << std::endl;
	Application::End();
}

void pbsApplication::SetAppPath()
{
	appPath_ = DE_APP_PATH;
}
