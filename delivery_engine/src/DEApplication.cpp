#include "DEApplication.h"

#include "config.h"
#include "Controllers/deliveryController.h"
#include "Filters/Filter.h"

#include <iostream>

NS_USING_DRAGON

void DEApplication::Start()
{
	Application::Start();
	std::cout << "DE Start at path:" << appPath_ << std::endl;

	ADD_CONTROLLER(deliveryController);

	// Filter Init
	StandardFilterInit();
}

void DEApplication::End()
{
	std::cout << "DE End" << std::endl;
	Application::End();
}

void DEApplication::SetAppPath()
{
	appPath_ = DE_APP_PATH;
}
