#include "Select.h"

#include <utility/Double.h>
#include <sys/time.h>
#include <boost/foreach.hpp>
#include "../Models/CampaignInfo.h"

NS_USING_DRAGON

		static AdInfo *selectByWeight(ExclusiveCampaigns &exAds)
		{
			/*
						计算单个campaign占所有campaign总和的比重
			*/
			ExclusiveCampaigns::iterator exCampIterEnd = exAds.end();
			for (ExclusiveCampaigns::iterator iter=exAds.begin(); iter != exCampIterEnd; ++iter)
			{
				ExclusiveCampaignPtr camPtr =iter->second;
				camPtr->dWeight = (double)camPtr->weight / (double)camPtr->totalWeight;
				printf("dWeidht:%f, %d %d", camPtr->dWeight, camPtr->weight, camPtr->totalWeight);
			}


			/*
					根据广告所属campaign的比重重新计算广告的比重
			*/
			double total = 0.0;
			for (ExclusiveCampaigns::iterator iter=exAds.begin(); iter != exCampIterEnd; ++iter)
			{
					std::vector<AdInfo *> adInfos = iter->second->adInfos;
					double dWeight = iter->second->dWeight;

					std::vector<AdInfo *>::iterator adListIterEnd = adInfos.end();
					for (std::vector<AdInfo *>::iterator jter = adInfos.begin(); jter != adListIterEnd; ++jter)
					{
						AdInfo* adInfo = *jter;
						adInfo->priority = dWeight * adInfo->weight;
						total += adInfo->priority;
					}
			}
				
			if (Double::Greater(total, 0))
			{
				timeval tv;
				gettimeofday(&tv, NULL);	
				std::srand(tv.tv_usec);
			    double rnd = (double)std::rand() / RAND_MAX;

				double low = 0;
				double high = 0;
				for (ExclusiveCampaigns::iterator iter=exAds.begin(); 
					   iter != exCampIterEnd; ++iter)
				{
					std::vector<AdInfo *> adInfos = iter->second->adInfos;

					std::vector<AdInfo *>::iterator adListIterEnd = adInfos.end();
					for (std::vector<AdInfo *>::iterator jter = adInfos.begin(); jter != adListIterEnd; ++jter)
					{
						AdInfo* adInfo = *jter;

						// 归一化
						adInfo->priority /= total;
						low  = high;
						high +=  adInfo->priority;
						if (Double::Range(rnd, low, high))
							return adInfo;
					}
				}
			}

			return NULL;
		}

	static AdInfo *selectByPri(ContractCampaigns &contractCampaigns)
		{
			double used_pri = 0;
			for (int i=9; i>=0; --i)
			{
				if (contractCampaigns[i] != NULL)
				{
					std::vector<AdInfo *> &adList = contractCampaigns[i]->adInfos;

					double total_priority_orig  = 0;
					double remaining_priority = 0;
					double scaling_factor        = 0;

					std::vector<AdInfo *>::iterator end = adList.end();
					for (std::vector<AdInfo *>::iterator iter = adList.begin(); iter != end; ++iter)
					{
						AdInfo* adInfo = (*iter);
						total_priority_orig += adInfo->priority * adInfo->priority_factor;		
					}

					remaining_priority = 1 - used_pri;
			
					used_pri += total_priority_orig;
			
					if (total_priority_orig > remaining_priority)
						scaling_factor = 1 / total_priority_orig;
					else
						scaling_factor = 1 / remaining_priority; 
	
					timeval tv;
					gettimeofday(&tv, NULL);	
					std::srand(tv.tv_usec);
					double rnd = (double)std::rand() / RAND_MAX;

			
					if (Double::Greater(rnd, total_priority_orig))
					{
						continue;
					}
 
					double low  = 0;
					double high = 0;
					for (std::vector<AdInfo *>::iterator iter = adList.begin(); iter != end; ++iter)
					{
						AdInfo* adInfo = (*iter);
						double pri = adInfo->priority * adInfo->priority_factor * scaling_factor;
						low  = high;
						high += pri;

						if (Double::Range(rnd, low, high))
						{
							return adInfo;
						}
					}	
				}
			}

			return NULL;
		}

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

	printf("ex ad:%d\n", exclusiveCampaigns.size());
	BOOST_FOREACH(ExclusiveCampaignsPair p, exclusiveCampaigns)
	{
		ExclusiveCampaignPtr ptr = p.second;
		printf("id:%d, Weight:%d, \n", ptr->id, ptr->weight);
	}
	if (exclusiveCampaigns.size() > 0) 
	{	
		return selectByWeight(exclusiveCampaigns);
	} else if(contractCampaigns.size() > 0){
		return selectByPri(contractCampaigns); 
	} else {
		return selectByWeight(remnantCampaigns);
	}
	
}
