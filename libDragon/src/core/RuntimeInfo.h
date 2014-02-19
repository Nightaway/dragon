#ifndef __RUNTIME_INFO_H__
#define __RUNTIME_INFO_H__

#include "Macro.h"
#include "QueryString.h"

#include <string>
#include <map>

#define DECLARE_CONTROLLER(cls_name)                 \
protected:                                           \
	static dragon::ClassTable clsTable_;         \
	static dragon::ClassInfo  clsInfo_;          \
public:                                              \
	static dragon::Controller *Create_();        \
	static void InitClass_();                    \
	static dragon::ClassInfo  *GetClassInfo_();   \
	static dragon::ClassTable *GetClassTable_();

#define IMPLEMENT_CONTROLLER(parent, cls_name)  \
	dragon::ClassTable cls_name::clsTable_ = {&Controller::clsTable_, NULL, &parent::clsTable_, &clsInfo_}; \
	dragon::ClassInfo  cls_name::clsInfo_  = {#cls_name, cls_name::Create_}; \
	Controller *cls_name::Create_()	{		          \
		return new cls_name; }				  \
	void cls_name::InitClass_() {                             \
		Controller::lastClassTable_->pNext = &clsTable_;  \
		Controller::lastClassTable_        = &clsTable_;} \
	ClassInfo *cls_name::GetClassInfo_() {			  \
	        return &clsInfo_; }				  \
	ClassTable *cls_name::GetClassTable_() {	          \
		return &clsTable_;}

#define DECLARE_ACTIONT_MAP \
protected:		    \
	static dragon::ActionTable actionTable[]; \
public: \
	virtual dragon::ActionTable *GetActionTable() \
	{ return actionTable; }

#define BEGIN_ACTION_MAP(cls_name) \
	ActionTable cls_name::actionTable[] = {

#define ACTION(cls_name, act_name) \
	{#act_name, (Action)&cls_name::act_name, "&"},

#define ACTION_OP1(cls_name, act_name, seperator) \
	{#act_name, (Action)&cls_name::act_name, seperator},

#define END_ACTION_MAP \
	{NULL, NULL, NULL} };

#define CLASS_FOREACH(tbl_name)        			        \
	ClassTable *clsCtrlTbl = Controller::GetClassTable_();  \
	ClassTable *tbl_name = clsCtrlTbl->pHead;               \
	for(; tbl_name != NULL; tbl_name = tbl_name->pNext)

#define ACTION_FOREACH(ctrl, tbl_name)                   \
	ActionTable *tbl_name =  ctrl->GetActionTable(); \
	for (; tbl_name->action != NULL; tbl_name++)	

NS_DRAGON

class Controller;

typedef void (Controller::*Action)(QueryString &);

struct ClassInfo{
	const char *clsName_;
	Controller *(*createHandler_)();
};

struct ClassTable {
	struct ClassTable *pHead;
	struct ClassTable *pNext;
	struct ClassTable *pParent;
	
	ClassInfo *pClsInfo;
};

struct ActionTable {
	const char *actName;
	Action action;
	const char *separator;
};

class RuntimeInfo {
	
};

NS_END

#endif
