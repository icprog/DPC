#include "serializeObj.h"

#include "boost/serialization/serialization.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/list.hpp"
#include "boost/serialization/map.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/nvp.hpp"

#include <sstream>
#include <fstream>

namespace boost
{
namespace serialization
{
	template<class Archive>
	void serialize(Archive& ar, MysqlConnectionInfo &si, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(si.host);
		ar & BOOST_SERIALIZATION_NVP(si.user);
		ar & BOOST_SERIALIZATION_NVP(si.passwd);
		ar & BOOST_SERIALIZATION_NVP(si.dbName);
		ar & BOOST_SERIALIZATION_NVP(si.connectionCount);
	}


}
}

bool MysqlConnectionInfo::seralize_xml(std::string& s, const MysqlConnectionInfo& site)
{
	bool bRet = true;

	try
	{
		std::ofstream ofs(s.c_str());
		boost::archive::xml_oarchive xoa(ofs);
		xoa << BOOST_SERIALIZATION_NVP(site);
		ofs.flush();
	}
	catch(std::exception& e)
	{
		std::string s(e.what(), strlen(e.what()));
		bRet = false;
	}

	return bRet;
}
bool MysqlConnectionInfo::deseralize_xml(const std::string& s, MysqlConnectionInfo& site)
{
	bool bRet = true;

	try
	{
		std::ifstream ifs(s.c_str());
		if (!ifs.good())
		{
			return false;
		}
		boost::archive::xml_iarchive xia(ifs);
		xia >> BOOST_SERIALIZATION_NVP(site);
	}
	catch(std::exception& e)
	{
		std::string s(e.what(), strlen(e.what()));
		bRet = false;
	}

	return bRet;
}