#ifndef __CAMPAIGN_INFO_H__
#define __CAMPAIGN_INFO_H__

#include <vector> 
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include "AdInfo.h"

class CampaignInfo {
public:
	int id;
	std::vector<AdInfo *> adInfos;
};

class WeightedCampaign : public CampaignInfo {
public:
	int weight;
	int totalWeight;
	double dWeight;
};

class PrioritizedCampaign : public CampaignInfo {
public:
	int pri;
};

typedef boost::shared_ptr<WeightedCampaign> ExclusiveCampaignPtr;
typedef boost::shared_ptr<WeightedCampaign> RemnantCampaignPtr;
typedef boost::shared_ptr<PrioritizedCampaign> ContractCampaignPtr;
typedef std::map<int , ExclusiveCampaignPtr> ExclusiveCampaigns;
typedef boost::array<ContractCampaignPtr, 10> ContractCampaigns;
typedef std::map<int , RemnantCampaignPtr> RemnantCampaigns;


#endif