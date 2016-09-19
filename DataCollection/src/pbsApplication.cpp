#include "pbsApplication.h"

#include "config.h"
#include "Controllers/collectionController.h"

#include <iostream>

NS_USING_DRAGON

void pbsApplication::Start()
{
	Application::Start();
	std::cout << "DataCollection Start at path:" << appPath_ << std::endl;

	ADD_CONTROLLER(collectionController);
}

void pbsApplication::End()
{
	std::cout << "DataCollection End" << std::endl;
	Application::End();
}

void pbsApplication::SetAppPath()
{
	appPath_ = DE_APP_PATH;
}
