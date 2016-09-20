#include "Controller.h"

NS_USING_DRAGON

ClassTable *Controller::lastClassTable_ = &clsTable_;
ClassTable Controller::clsTable_ = {&clsTable_, NULL, &clsInfo_};
ClassInfo  Controller::clsInfo_  = {"Controller", Controller::Create_};
Controller *Controller::Create_()		          
{							  
	return new Controller;				  
}

void Controller::InitClass_()
{

}

ClassTable *Controller::GetClassTable_()
{	          
	return &clsTable_;
}

ActionTable *Controller::GetActionTable()
{
	return NULL;
}
