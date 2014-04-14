#include "MotuFilter.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>

#include"../rapidjson/document.h"
#include <utility/StringAlogrithms.h>

using namespace dragon;
using boost::asio::ip::tcp;

bool MAX_checkMotuFilter_Interface_map1(MotuInfo *_motu_Info);
bool MAX_checkMotuFilter_Interface_map2(MotuInfo *_motu_Info);
void MAX_checkMotuFilter_GetCarInfo(MotuInfo *_motu_Info, boost::unordered_map<std::string, std::string> &carInfo);
bool MAX_limitationsMatchMotuFilterPrice(float price, std::vector<std::string> &prices);
bool MAX_limitationsMatchMotuFilterType(std::string &type, std::string &limitTypeString);
float prasePrice(std::string &price);
float prasePrice(std::string &max_price, std::string &min_price);
void MAX_checkMotuFilter_Interface(MotuInfo *_motu_Info, std::string &car_Info);

typedef boost::function<int (std::string &, std::vector<std::string> &, MotuInfo *)> motu_check_func_t;
typedef boost::unordered_map<std::string, motu_check_func_t> motu_check_func_map_t;
motu_check_func_map_t motu_check_func_map;
													
 typedef struct {
		std::string func_name;
		bool (*func)(std::string &, std::vector<std::string> &, MotuInfo *);
 } motu_func_map;

 bool IfengCarInfoInterface(std::string &car_id, std::vector<std::string> &limitions,  MotuInfo *_motu_Info);
 bool XcarInterface(std::string &car_id, std::vector<std::string> &limitions,  MotuInfo *_motu_Info);
 bool BitAutoInterface(std::string &car_id, std::vector<std::string> &limitions,  MotuInfo *_motu_Info);
 bool QqBookInterface(std::string &car_id, std::vector<std::string> &limitions,  MotuInfo *_motu_Info);

 static motu_func_map motu_map[] = {
						{"IfengCarInfoInterface", IfengCarInfoInterface},
						{"XcarInterface", XcarInterface},
						{"BitAutoInterface", BitAutoInterface},
						{"QqBookInterface", QqBookInterface}
};

bool MotuFilter::motu_filter(	std::string &limition, std::string &op, std::string &car_id)
{
	std::vector<std::string> limitions;
	limitions = StringAlogrithms::explode("|",limition);
	
	motu_check_func_t func = motu_check_func_map[limitions[0]];
	if (func == NULL) {
			return true;
	}
	return func(car_id, limitions, &_motuInfo);
}

void MotuFilter::init()
{
	int len = sizeof(motu_map) /  sizeof(motu_map[0]);
	for (int i=0; i<len; ++i)
	{
		std::string key = motu_map[i].func_name;
		motu_check_func_t func = motu_map[i].func;
		motu_check_func_map[key] = func;
	}
}

void MotuFilter::dispose()
{

}

bool MAX_checkMotuFilter_Interface_map1(MotuInfo *_motu_Info)
{
	bool match = false;
	std::string limitPriceString = _motu_Info->price;
	std::string limitTypeString = _motu_Info->type;
    std::vector<std::string> limitPrice;
	bool hasPriceAll = false;
	bool hasTypeAll = false;
	boost::unordered_map<std::string, std::string> carInfo;
	float price;
	std::string type;	
	limitPrice = StringAlogrithms::explode(",",limitPriceString);	
	if (limitPriceString.find("all",0) != std::string::npos)
		hasPriceAll = true;
	if (limitTypeString.find("all",0) != std::string::npos)	
		hasTypeAll = true;
	
	if(_motu_Info->car_id.empty())
	{
		if (((limitPriceString.empty()) && (limitTypeString.empty()))
                || (hasPriceAll && hasTypeAll))
        {
            return true;
        } else {
            return false;
        }	
	}else
	{
		MAX_checkMotuFilter_GetCarInfo(_motu_Info, carInfo);
		if(carInfo.empty())
			return false;
		if(_motu_Info->name != "xcar")
			price = prasePrice(carInfo["price"]);
		else
			price = prasePrice(carInfo["max_price"],carInfo["min_price"]);
		type = carInfo["type"];
		if (!limitPriceString.empty() && !limitTypeString.empty())
        {
            if ((hasPriceAll || MAX_limitationsMatchMotuFilterPrice(price, limitPrice))
                    && (hasTypeAll || MAX_limitationsMatchMotuFilterType(type, limitTypeString)))
            {
                match = true;
            }
        }else
		{
			            // 单独匹配价格
            if (!limitPriceString.empty())
            {
                if (hasPriceAll || MAX_limitationsMatchMotuFilterPrice(price, limitPrice))
                {
                    match = true;
                }
            }

			            // 单独匹配车型
            if (!limitTypeString.empty())
            {
                if (hasTypeAll || MAX_limitationsMatchMotuFilterType(type, limitTypeString))
                {
                    match = true;
                }
            }		
		}
	}
	return match;
}


bool MAX_checkMotuFilter_Interface_map2(MotuInfo *_motu_Info)
{
	std::string limitPriceString = _motu_Info->price;
	std::string limitTypeString = _motu_Info->type;
    std::vector<std::string> limitPrice;
	bool hasPriceAll = false;
	bool hasTypeOther = false;
	bool hasTypeAll = false;
	boost::unordered_map<std::string, std::string> carInfo;
	float price;
	std::string type;
	limitPrice = StringAlogrithms::explode(",",limitPriceString);
	if (limitPriceString.find("all",0) != std::string::npos)
		hasPriceAll = true;
	if (limitTypeString.find("全部车型",0) != std::string::npos)
		hasTypeAll = true;
	if(limitTypeString.find("其他",0) != std::string::npos)
		hasTypeOther = true;

    if (!limitPriceString.empty())
    {
        // 无车型ID
        if (_motu_Info->car_id.empty())
        {
            // 价格限制包好全部
            if (hasPriceAll)
            {
                return true;
            }
        }
    }

    // type match
    if (!limitTypeString.empty())
    {
        // 无车型ID
        if (_motu_Info->car_id.empty())
        {
            // 车型级别限制包含 其他、全部
            if (hasTypeOther || hasTypeAll)
            {
                return true;
            }
        }
    }
	
	MAX_checkMotuFilter_GetCarInfo(_motu_Info, carInfo);
	if(carInfo.empty())
		return false;
	price = prasePrice(carInfo["price"]);
	type = carInfo["type"];

	    if (!limitPriceString.empty())
    {
        // 返回车型信息中，价格区间为空
        if (carInfo["price"].empty())
        {
            if (hasPriceAll)
            {
                return true;
            }
        } else {
            if (hasPriceAll || MAX_limitationsMatchMotuFilterPrice(price, limitPrice))
            {
                return true;
            }
        }
    }

    // type match
    if (!limitTypeString.empty())
    {
        // 返回车型信息中，车型为空
        if (carInfo["type"].empty())
        {
            if(hasTypeOther)
            {
                return true;
            }
        } else {
            // 车型级别限制包含 全部
            if (hasTypeAll || MAX_limitationsMatchMotuFilterType(type, limitTypeString))
            {
                return true;
            }
        }
    }
	return false;
}

void MAX_checkMotuFilter_GetCarInfo(MotuInfo *_motu_Info, boost::unordered_map<std::string, std::string> &carInfo)
{
	std::string cacheFileName = _motu_Info->name + "_" + _motu_Info->car_id;
	
	if(! _motu_Info->car_id.empty())
	{
	std::string car_Info;
	MAX_checkMotuFilter_Interface(_motu_Info, car_Info);
	if(!car_Info.empty())
	{
			const char *json = car_Info.c_str();
			rapidjson::Document d;
			d.Parse<0>(json);
			if(_motu_Info->name != "xcar")
			{
				if( !d["type"].IsNull()) {
					carInfo["type"] = d["type"].GetString();
				}
				else
				{
					carInfo["type"] = "null";
				}
				if( !d["price"].IsNull()){
				 carInfo["price"] = d["price"].GetString();
				}
				else
				{
					carInfo["price"] = "0";
				}
			 }
			 else
			 {
				 if(!d["level_name"].IsNull()){
				 carInfo["type"] = d["level_name"].GetString();
				 }else
				 {
					carInfo["type"] = "null";
				 }

				 if(!d["min_price"].IsNull()){
				 carInfo["min_price"] = d["min_price"].GetString();
				 }else
				 {
					carInfo["min_price"] = "0";
				 }

				 if(!d["max_price"].IsNull()){
				 carInfo["max_price"] = d["max_price"].GetString();
				 }else
				 {
					carInfo["max_price"] = "0";
				 }				 
			 }
		}
	}
}

bool MAX_limitationsMatchMotuFilterPrice(float price,std::vector<std::string> &prices)
{
	int i = 0,num = prices.size();
	bool match = false;
	while(num--)
	{
		std::string priceStr = prices[i++];
		size_t index = 0;
		index = priceStr.find("&&",0);
		if(index != std::string::npos)
		{
			std::vector<std::string> value;
			value = StringAlogrithms::explode("&&",priceStr);	
			value[1] =  value[1].substr(4,value[1].length());
			if(atof(value[0].c_str())<=price && price < atof(value[1].c_str()))
				{match = true;break;}		
		}else
		{
			int num = atof(priceStr.c_str());
			if (num == 0)
            {
				index = priceStr.find("<",0);
				if(index != std::string::npos)
				{
                    priceStr = priceStr.substr(index+1,priceStr.length());
                    if(price < atof(priceStr.c_str()))
                            {match = true;break;}
				}
			}
			else
			{				
				if(price >= atof(priceStr.c_str()))
					{match = true;break;}		
			}		
		}	
	}
	return match;
}

bool MAX_limitationsMatchMotuFilterType(std::string &type, std::string &limitTypeString)
{
	if(limitTypeString.find(type,0) != std::string::npos)
		return true;
	else
		return false;
}

float prasePrice(std::string &price)
{
	std::vector<std::string> prices;	
	prices = StringAlogrithms::explode("-",price);	
	float parsePrice;
	if(prices.size() == 2)
		parsePrice = (atof(prices[0].c_str()) + atof(prices[1].c_str()))/2;
	else
		parsePrice = atof(prices[0].c_str());
	return parsePrice;
}

float prasePrice(std::string &max_price, std::string &min_price)
{
	float parsePrice = (atof(max_price.c_str()) + atof(min_price.c_str()))/2;
	return parsePrice;
}

void MAX_checkMotuFilter_Interface(MotuInfo *_motu_Info, std::string &car_Info)
{
	std::string temp;

	try {
			boost::asio::io_service io_service;
			boost::asio::ip::tcp::socket socket(io_service);
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(_motu_Info->api_host, "http");
			boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
 
			boost::asio::connect(socket, iter);
			boost::system::error_code err;

			size_t len = socket.write_some(boost::asio::buffer(_motu_Info->httpRequestStr), err);

			for (;;)
			{
				boost::array<char, 128> buf;
				boost::system::error_code error;
 
				len = socket.read_some(boost::asio::buffer(buf), error);
				if (error == boost::asio::error::eof)
				break; // Connection closed clealy by peer.
				else if (error)
				throw boost::system::system_error(error); // Some other error
 
				//std::cout.write(buf.data(), len);
				temp=buf.data();
				car_Info += temp.substr(0,len);
			}
			socket.close();
			} catch (std::exception & e) {
				std::cerr << e.what() << std::endl;
		}
	//std::cout << car_Info <<std::endl;
	size_t index1 = car_Info.find_last_of("{");
	if(index1 == std::string::npos)
		car_Info =  "";
	else
	{	
		size_t index2 = car_Info.find_last_of("}");
		if(index2 ==  std::string::npos)
			car_Info =  "";
		else
			car_Info =car_Info.substr(index1,index2-index1+1);
	}
}

bool IfengCarInfoInterface(std::string &car_id, std::vector<std::string> &limitions,  MotuInfo *_motu_Info)
{
	_motu_Info->name = boost::trim_copy(limitions[1]);
	_motu_Info->price = boost::trim_copy(limitions[2]);
	_motu_Info->type = boost::trim_copy(limitions[3]);
	_motu_Info->car_id = car_id;
	_motu_Info->api_host = "car.auto.ifeng.com";	
	_motu_Info->httpRequestStr = "GET /api/ad_car_info.php?car_id=" + car_id + "&jsoncallback=paize_carinfo HTTP/1.1\r\n";
	_motu_Info->httpRequestStr += "Host: " + _motu_Info->api_host + "\r\nReferer: /dap/www/delivery/mmt.php\r\nConnection: Close\r\n\r\n";
	return MAX_checkMotuFilter_Interface_map1(_motu_Info);
}

 bool XcarInterface(std::string &car_id, std::vector<std::string> &limitions,  MotuInfo *_motu_Info)
 {
 	_motu_Info->name = boost::trim_copy(limitions[1]);
	_motu_Info->price = boost::trim_copy(limitions[2]);
	_motu_Info->type = boost::trim_copy(limitions[3]);
	_motu_Info->car_id = car_id;
	_motu_Info->api_host = "newcar.xcar.com.cn";	
	_motu_Info->httpRequestStr ="GET /auto/index.php?r=service/SeriseParentApi/getInfo/&pserid=" + car_id + " HTTP/1.1\r\n";
	_motu_Info->httpRequestStr += "Host: " + _motu_Info->api_host + "\r\nReferer: /dap/www/delivery/mmt.php\r\nConnection: Close\r\n\r\n"; 
	return MAX_checkMotuFilter_Interface_map1(_motu_Info);
 }

 bool BitAutoInterface(std::string &car_id, std::vector<std::string> &limitions,  MotuInfo *_motu_Info)
 {
  	_motu_Info->name = boost::trim_copy(limitions[1]);
	_motu_Info->price = boost::trim_copy(limitions[2]);
	_motu_Info->type = boost::trim_copy(limitions[3]);
	_motu_Info->car_id = car_id;
	_motu_Info->api_host = "api.car.bitauto.com";	
	_motu_Info->httpRequestStr ="GET /Cooperation/GetCarDataJson.ashx?dept=pagechoice&jsoncallback=bitAutoCallback&car_id=" + car_id + " HTTP/1.1\r\n";
	_motu_Info->httpRequestStr += "Host: " + _motu_Info->api_host + "\r\nReferer: /dap/www/delivery/mmt.php\r\nConnection: Close\r\n\r\n";
	return MAX_checkMotuFilter_Interface_map2(_motu_Info);
 }

 bool QqBookInterface(std::string &car_id, std::vector<std::string> &limitions,  MotuInfo *_motu_Info)
 {
   	_motu_Info->name = boost::trim_copy(limitions[1]);
	_motu_Info->price = boost::trim_copy(limitions[2]);
	_motu_Info->type = boost::trim_copy(limitions[3]);
	_motu_Info->car_id = car_id;
	_motu_Info->api_host = "cgi.data.auto.qq.com";
	_motu_Info->httpRequestStr ="GET /php/getmemval.php?type=serial&id=" + car_id + "&jsoncallback=qqbook HTTP/1.1\r\n";
	_motu_Info->httpRequestStr += "Host: " + _motu_Info->api_host + "\r\nReferer: /dap/www/delivery/mmt.php\r\nConnection: Close\r\n\r\n";
	return MAX_checkMotuFilter_Interface_map2(_motu_Info);
 }