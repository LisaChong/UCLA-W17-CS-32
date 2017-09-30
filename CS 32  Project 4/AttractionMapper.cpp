#include "provided.h"
#include "MyMap.h"
#include <string>
#include <iostream>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> attractions;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	StreetSegment temp;
	for (size_t l = 0; l < ml.getNumSegments(); l++)
	{
		ml.getSegment(l, temp);
		for (size_t a = 0; a < temp.attractions.size(); a++)
		{
			string nameTemp = temp.attractions[a].name;
			for (size_t n = 0; n < nameTemp.size(); n++)
				nameTemp[n] = tolower(nameTemp[n]);
			attractions.associate(nameTemp, temp.attractions[a].geocoordinates);
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	GeoCoord* temp = nullptr;
	for (size_t a = 0; a < attraction.size(); a++)
		attraction[a] = tolower(attraction[a]);
	if (nullptr != attractions.find(attraction))
	{
		gc = *attractions.find(attraction);
		return true;
	}
	else
		return false;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
