#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> m_streets;
	int m_numSegs = 0;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
	for (int del = m_numSegs - 1; del >= 0; del--)
	{
	}
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);    // infile is a name of our choosing
	if ( ! infile )		        // Did opening the file fail?
	{
		return false; 
	}
	std::string s;  // getline returns infile; the while tests its success/failure state
	while (getline(infile, s))
	{
		StreetSegment v;
		m_streets.push_back(v);
	    m_streets[m_numSegs].streetName = s;
	    getline(infile, s);
	    int segCoords = 0;
	    string segmentCoords[4];
	    for (int k = 0; k < s.size(); k++)
	    {
			if (s[k] == ' ' || s[k] == ',');
			else if (k != 0 && (s[k - 1] == ' ' || s[k - 1] == ',') && (isdigit(s[k]) || s[k] == '-' || s[k] == '.'))
			{
				segCoords++;
				segmentCoords[segCoords] += s[k];
			}
			else
	    		segmentCoords[segCoords] += s[k];
	    }
		m_streets[m_numSegs].segment.start.latitudeText = segmentCoords[0];
		m_streets[m_numSegs].segment.start.latitude = stod(segmentCoords[0]);
		m_streets[m_numSegs].segment.start.longitudeText = segmentCoords[1];
		m_streets[m_numSegs].segment.start.longitude = stod(segmentCoords[1]);
		m_streets[m_numSegs].segment.end.latitudeText = segmentCoords[2];
		m_streets[m_numSegs].segment.end.latitude = stod(segmentCoords[2]);
		m_streets[m_numSegs].segment.end.longitudeText = segmentCoords[3];
		m_streets[m_numSegs].segment.end.longitude = stod(segmentCoords[3]);
	    getline(infile, s);
	    for (int att = 0; att < stod(s); att++)
	    {
			string f;
	    	getline(infile, f);
			Attraction k;
			m_streets[m_numSegs].attractions.push_back(k);
	    	int c = 0;
			while (f[c] != '|')
			{
				m_streets[m_numSegs].attractions[att].name += f[c];
				c++;
			}
			c++;
			string attCoor1, attCoor2;
			while (isdigit(f[c]) || f[c] == '-' || f[c] == '.')
			{
				attCoor1 += f[c];
				c++;
			}
			while (f[c] == ' ' || f[c] == ',')
				c++;
			while (c < f.size())
			{
				attCoor2 += f[c];
				c++;
			}
			m_streets[m_numSegs].attractions[att].geocoordinates.latitudeText = attCoor1;
			m_streets[m_numSegs].attractions[att].geocoordinates.latitude = stod(attCoor1);
			m_streets[m_numSegs].attractions[att].geocoordinates.longitudeText = attCoor2;
			m_streets[m_numSegs].attractions[att].geocoordinates.longitude = stod(attCoor2);
	    }	    
	    m_numSegs++;
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_numSegs;
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < 0 || segNum >= m_numSegs)
		return false;
	else
	{
		seg = m_streets[segNum];
		return true;
	}
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
