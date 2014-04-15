#include "Select.h"

#include <boost/foreach.hpp>
#include "../Models/CampaignInfo.h"

AdInfo *SelectOne(std::vector<AdInfo *> &adInfos)
{
	ExclusiveCampaigns exclusiveCampaigns;
	ContractCampaigns contractCampaigns;
	RemnantCampaigns remnantCampaigns;

	BOOST_FOREACH(AdInfo *adInfo, adInfos)
	{
		if (adInfo->campaign_priority == -1) {
			ExclusiveCampaignPtr &exCam = exclusiveCampaigns[adInfo->placement_id];
			if (exCam == NULL)
			{
				exCam = ExclusiveCampaignPtr(new WeightedCampaign());			
				exCam->id = adInfo->placement_id;
				exCam->weight = adInfo->campaign_weight;
				exCam->totalWeight = 0;
				exCam->dWeight = 0.0;
			}
			exCam->totalWeight += adInfo->weight;
			exCam->adInfos.push_back(adInfo);

		} else if ((adInfo->campaign_priority >= 1) && (adInfo->campaign_priority <= 10)) {
			int idx = adInfo->campaign_priority - 1;
			ContractCampaignPtr &ctCam = contractCampaigns[idx];
			if (ctCam == NULL)
			{
				ctCam = ContractCampaignPtr(new PrioritizedCampaign());
				ctCam->id = adInfo->banner_id;
				ctCam->pri = adInfo->campaign_priority;
			}
			ctCam->adInfos.push_back(adInfo);

		} else if (adInfo->campaign_priority == 0) {
			printf("remnant ad\n");
			RemnantCampaignPtr  &campaign = remnantCampaigns[adInfo->placement_id];
			if (campaign == NULL)
			{
				campaign = RemnantCampaignPtr(new WeightedCampaign());						
				campaign->id = adInfo->placement_id;
				campaign->weight = adInfo->campaign_weight;
				campaign->totalWeight = 0;
				campaign->dWeight = 0.0;
			}
			campaign->totalWeight += adInfo->weight;
			campaign->adInfos.push_back(adInfo);
		}
	}

	return NULL;
}