#include "provided.h"
#include "MyMap.h"
#include <vector>
#include <iostream>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<string, vector<StreetSegment>> m_segMap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (size_t l = 0; l < ml.getNumSegments(); l++)
	{
		StreetSegment temp;
		ml.getSegment(l, temp);
		vector<StreetSegment> newSegVec, existSegVec;
		if (nullptr == m_segMap.find(temp.segment.start.latitudeText + temp.segment.start.longitudeText))
		{
			newSegVec.push_back(temp);
			m_segMap.associate(temp.segment.start.latitudeText + temp.segment.start.longitudeText, newSegVec);
		}
		else
		{
			existSegVec = *m_segMap.find(temp.segment.start.latitudeText + temp.segment.start.longitudeText);
			size_t m = 0;
			for (m; m < existSegVec.size(); m++)
				if (existSegVec[m].streetName == temp.streetName)
					break;
			if (m == existSegVec.size())
				existSegVec.push_back(temp);
		}
		if (nullptr == m_segMap.find(temp.segment.end.latitudeText + temp.segment.end.longitudeText))
		{
			newSegVec.push_back(temp);
			m_segMap.associate(temp.segment.end.latitudeText + temp.segment.end.longitudeText, newSegVec);
		}
		else
		{
			existSegVec = *m_segMap.find(temp.segment.end.latitudeText + temp.segment.end.longitudeText);
			size_t m = 0;
			for (m; m < existSegVec.size(); m++)
				if (existSegVec[m].streetName == temp.streetName)
					break;
			if (m == existSegVec.size())
				existSegVec.push_back(temp);
		}
		for (size_t a = 0; a < temp.attractions.size(); a++)
		{
			if (nullptr == m_segMap.find(temp.attractions[a].geocoordinates.latitudeText+ temp.attractions[a].geocoordinates.longitudeText))
			{
				m_segMap.associate(temp.attractions[a].geocoordinates.latitudeText + temp.attractions[a].geocoordinates.longitudeText, newSegVec);
				newSegVec.push_back(temp);
			}
			else
			{
				existSegVec = *m_segMap.find(temp.attractions[a].geocoordinates.latitudeText + temp.attractions[a].geocoordinates.longitudeText);
				size_t m = 0;
				for (m; m < existSegVec.size(); m++)
					if (existSegVec[m].streetName == temp.streetName)
						break;
				if (m == existSegVec.size())
					existSegVec.push_back(temp);
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments = *m_segMap.find(gc.latitudeText + gc.longitudeText);
	return segments;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}