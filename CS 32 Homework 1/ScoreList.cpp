#include "ScoreList.h"
#include <limits>

ScoreList::ScoreList()       // Create an empty score list.
{
}

bool ScoreList::add(unsigned long score)
{
	if (score < 0 || score > 100 || m_Sequence.size() == DEFAULT_MAX_ITEMS)
		return false;
	else
	{
		m_Sequence.insert(score);
		return true;
	}
}
	// If the score is valid (a value from 0 to 100) and the score list
	// has room for it, add it to the score list and return true.
	// Otherwise, leave the score list unchanged and return false.

bool ScoreList::remove(unsigned long score)
{
	return m_Sequence.erase(m_Sequence.find(score));
}
	// Remove one instance of the specified score from the score list.
	// Return true if a score was removed; otherwise false.

int ScoreList::size() const  // Return the number of scores in the list.
{
	return m_Sequence.size();
}

unsigned long ScoreList::minimum() const
{
	if (m_Sequence.empty())
		return NO_SCORE;
	else
	{
		ItemType lowestScore;
		m_Sequence.get(0, lowestScore);
		return lowestScore;
	}
}
	// Return the lowest score in the score list.  If the list is
	// empty, return NO_SCORE.

unsigned long ScoreList::maximum() const
{
	if (m_Sequence.empty())
		return NO_SCORE;
	else
	{
		ItemType highestScore;
		m_Sequence.get(m_Sequence.size() - 1, highestScore);
		return highestScore;
	}
}
	// Return the highest score in the score list.  If the list is
	// empty, return NO_SCORE.