#include "gridpointdata.h"

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
        void serialize(Archive& ar, datamodel::DIYRGB &si, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(si.r);
            ar & BOOST_SERIALIZATION_NVP(si.g);
            ar & BOOST_SERIALIZATION_NVP(si.b);
            ar & BOOST_SERIALIZATION_NVP(si.sColor);
            ar & BOOST_SERIALIZATION_NVP(si.value);
        }

        template<class Archive>
        void serialize(Archive& ar, datamodel::DIYRGBArray &si, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(si.dRgbs);
        }

        template<class Archive>
        void serialize(Archive& ar, datamodel::GridArray &si, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(si.height);
            ar & BOOST_SERIALIZATION_NVP(si.xList);
            ar & BOOST_SERIALIZATION_NVP(si.yList);
            ar & BOOST_SERIALIZATION_NVP(si.dataList);
        }

        template<class Archive>
        void serialize(Archive& ar, datamodel::GridPointData &si, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(si.businessId);
            ar & BOOST_SERIALIZATION_NVP(si.tm);
            ar & BOOST_SERIALIZATION_NVP(si.forcastTime);
            ar & BOOST_SERIALIZATION_NVP(si.productName);
            ar & BOOST_SERIALIZATION_NVP(si.displayStyle);
            ar & BOOST_SERIALIZATION_NVP(si.displayStyles);
            ar & BOOST_SERIALIZATION_NVP(si.colors);
            ar & BOOST_SERIALIZATION_NVP(si.unit);
            ar & BOOST_SERIALIZATION_NVP(si.gridArray);
        }

        template<class Archive>
        void serialize(Archive& ar, datamodel::GridPointData_Time &si, const unsigned int version)
        {
            ar & BOOST_SERIALIZATION_NVP(si.businessId);
            ar & BOOST_SERIALIZATION_NVP(si.tm);
            ar & BOOST_SERIALIZATION_NVP(si.timeNums);
            ar & BOOST_SERIALIZATION_NVP(si.timeArray);
        }
    }
}

bool datamodel::GridArray::seralize(std::string& s, const GridArray& gA)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::binary_oarchive oa(os);
        oa << gA;
        s = os.str();
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }

    return bRet;
}

bool datamodel::GridArray::deseralize(const std::string& s, GridArray& gA)
{
    bool bRet = true;

    try
    {
        std::istringstream is(s);
        boost::archive::binary_iarchive ia(is);
        ia >> gA;
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool datamodel::GridArray::seralize_xml(std::string& s, const GridArray& gA)
{
    bool bRet = true;

    try
    {
        std::ofstream ofs(s.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(gA);
        ofs.flush();
        ofs.close();
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool datamodel::GridArray::deseralize_xml(const std::string& s, GridArray& gA)
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
        xia >> BOOST_SERIALIZATION_NVP(gA);
        ifs.close();
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool datamodel::GridPointData::seralize(std::string& s, const GridPointData& gPD)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::binary_oarchive oa(os);
        oa << gPD;
        s = os.str();
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }

    return bRet;
}

bool datamodel::GridPointData::deseralize(const std::string& s, GridPointData& gPD)
{
    bool bRet = true;

    try
    {
        std::istringstream is(s);
        boost::archive::binary_iarchive ia(is);
        ia >> gPD;
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool datamodel::GridPointData::seralize_xml(std::string& s, const GridPointData& gPD)
{
    bool bRet = true;

    try
    {
        std::ofstream ofs(s.c_str());
        boost::archive::xml_oarchive xoa(ofs);
        xoa << BOOST_SERIALIZATION_NVP(gPD);
        ofs.flush();
        ofs.close();
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool datamodel::GridPointData::deseralize_xml(const std::string& s, GridPointData& gPD)
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
        xia >> BOOST_SERIALIZATION_NVP(gPD);
        ifs.close();
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}

bool datamodel::GridPointData_Time::seralize(std::string& s, const datamodel::GridPointData_Time& gPDTm)
{
    bool bRet = true;

    try
    {
        std::ostringstream os;
        boost::archive::binary_oarchive oa(os);
        oa << gPDTm;
        s = os.str();
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));

        bRet = false;
    }

    return bRet;
}

bool datamodel::GridPointData_Time::deseralize(const std::string& s, datamodel::GridPointData_Time& gPDTm)
{
    bool bRet = true;

    try
    {
        std::istringstream is(s);
        boost::archive::binary_iarchive ia(is);
        ia >> gPDTm;
    }
    catch(std::exception& e)
    {
        std::string s(e.what(), strlen(e.what()));
        bRet = false;
    }

    return bRet;
}