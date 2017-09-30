#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	AttractionMapper attMap;
	SegmentMapper segMap;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	MapLoader m;
	bool k = m.load(mapFile);
	if (!k)
		return false;
	else
	{
		attMap.init(m);
		segMap.init(m);
		return true;
	}
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord startgc, endgc;
	if (attMap.getGeoCoord(start, startgc) == false)
		return NAV_BAD_SOURCE;
	if (attMap.getGeoCoord(end, endgc) == false)
		return NAV_BAD_DESTINATION;
	MyMap<GeoCoord*, GeoCoord> locationOfPreviousWayPoint;
	queue<GeoCoord> visitedCoords;
	vector<double> distfromend;
	GeoCoord gofromhere = startgc;
	vector<StreetSegment> possible = segMap.getSegments(gofromhere);
	for (int m = 0; m < possible.size(); m++)
	{
		if (distanceEarthKM(possible[m].segment.start, endgc) < distanceEarthKM(possible[m].segment.end, endgc))
		{
			visitedCoords.push(possible[m].segment.start);
			visitedCoords.push(possible[m].segment.end);
		}
		else 
		{
			visitedCoords.push(possible[m].segment.end);
			visitedCoords.push(possible[m].segment.start);
		}
		locationOfPreviousWayPoint.associate(&possible[m].segment.start, gofromhere);
		locationOfPreviousWayPoint.associate(&possible[m].segment.end, gofromhere);
	}
	bool found = false;
	while (visitedCoords.size() != 0 || found == true)
	{
		gofromhere = visitedCoords.front();
		possible = segMap.getSegments(gofromhere);
		visitedCoords.pop();
		vector<GeoCoord> temppos;
		for (int m = 0; m < possible.size(); m++)
		{
			if (possible[m].segment.start.latitude == gofromhere.latitude && possible[m].segment.start.longitude == gofromhere.longitude)
				temppos.push_back(possible[m].segment.end);
			else if (possible[m].segment.end.latitude == gofromhere.latitude && possible[m].segment.end.longitude == gofromhere.longitude)
				temppos.push_back(possible[m].segment.start);
		}
		//vector<int> intrank;
		for (int m = 0; m < temppos.size(); m++)
		{
			if (locationOfPreviousWayPoint.find(&temppos[m]) == nullptr)
			{
				//intrank.push_back(distanceEarthKM(temppos[m], endgc));
				visitedCoords.push(temppos[m]);
				locationOfPreviousWayPoint.associate(&temppos[m], gofromhere);
				if (gofromhere.latitude == endgc.latitude && gofromhere.longitude == endgc.longitude)
				{
					found = true;
					break;
				}
			}
		}				
	}
	if (found == false)
		return NAV_NO_ROUTE;
	GeoCoord firstCoord = *locationOfPreviousWayPoint.find(&endgc), secondCoord = endgc;
	string prevStName = "";
	int numNavSegs = 0;
	while (firstCoord.latitude != startgc.latitude || firstCoord.longitude != startgc.longitude)
	{
		StreetSegment inputSeg;
		vector<StreetSegment> whatSeg = segMap.getSegments(secondCoord);
		for (int m = 0; m < whatSeg.size(); m++)
		{
			if ((whatSeg[m].segment.end.latitude == firstCoord.latitude && whatSeg[m].segment.end.latitude == firstCoord.latitude &&
				whatSeg[m].segment.start.longitude == secondCoord.longitude && whatSeg[m].segment.start.longitude == secondCoord.longitude) ||
				(whatSeg[m].segment.start.latitude == firstCoord.latitude && whatSeg[m].segment.start.latitude == firstCoord.latitude &&
					whatSeg[m].segment.end.longitude == secondCoord.longitude && whatSeg[m].segment.end.longitude == secondCoord.longitude))
			{
				inputSeg = whatSeg[m];
				break;
			}
		}
		double x, y;
		x = secondCoord.latitude - firstCoord.latitude;
		y = secondCoord.longitude - firstCoord.longitude;
		double angle = rad2deg(atan2(y, x));
		string nsewdir;
		if (angle >= 0 && angle <= 22.5)
			nsewdir = "east";
		else if (angle > 22.5 && angle <= 67.5)
			nsewdir = "northeast";
		else if (angle > 67.5 && angle <= 112.5)
			nsewdir = "north";
		else if (angle > 112.5 && angle <= 157.5)
			nsewdir = "northwest";
		else if (angle > 157.5 && angle <= 202.5)
			nsewdir = "west";
		else if (angle > 202.5 && angle <= 247.5)
			nsewdir = "southwest";
		else if (angle > 247.5 && angle <= 292.5)
			nsewdir = "south";
		else if (angle > 292.5 && angle <= 337.5)
			nsewdir = "southeast";
		else if (angle > 337.5 && angle <= 360)
			nsewdir = "east";
		if (inputSeg.streetName != directions[numNavSegs].m_streetName)
		{
			GeoSegment endSeg = directions[numNavSegs].m_geoSegment;
			GeoSegment startSeg = inputSeg.segment;
			string turn;
			if (angleBetween2Lines(startSeg, endSeg) > 180)
				turn = "left";
			else
				turn = "right";
			directions.push_back(NavSegment(turn, directions[numNavSegs].m_streetName));
		}
		directions.push_back(NavSegment(nsewdir, inputSeg.streetName, distanceEarthKM(firstCoord, secondCoord), inputSeg.segment));
		numNavSegs++;
		return NAV_SUCCESS;
	}
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
