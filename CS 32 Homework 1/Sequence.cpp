#include "Sequence.h"
#include <iostream>

using namespace std;

Sequence::Sequence()	// Create an empty sequence (i.e., one whose size() is 0).
	: m_size(0)
{}

bool Sequence::empty() const  // Return true if the sequence is empty, otherwise false.
{
	if (m_size == 0)
		return true;
	else
		return false;
}

int Sequence::size() const    // Return the number of items in the sequence.
{
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	if (pos < 0 || pos > size() || size() == DEFAULT_MAX_ITEMS)
		return false;
	else
	{
		m_size++;
		for (int k = size() - 1; k > pos; k--)
		{
			m_array[k] = m_array[k - 1];
		}
		m_array[pos] = value;
		return true;
	}
}
	// Insert value into the sequence so that it becomes the item at position pos.  The original item at position pos and those that
	// follow it end up at positions one higher than they were at before.

int Sequence::insert(const ItemType& value)
{
	if (size() == DEFAULT_MAX_ITEMS)
		return -1;
	else
	{
		for (int k = 0; k < size(); k++)
		{
			if (value <= m_array[k])
			{
				m_size++;
				for (int j = size() - 1; j > k; j--)
				{
					m_array[j] = m_array[j - 1];
				}
				m_array[k] = value;
				return k;
			}
		}
		m_size++;
		m_array[size() - 1] = value;
		return size() - 1;
	}
}
// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item at position p. 

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= size())
		return false;
	else
	{
		for (int k = pos; k < size() - 1; k++)
		{
			m_array[k] = m_array[k + 1];
		}
		m_size--;
		return true;
	}
}
// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed this item end up at
// positions one lower than they were at before), and return true.

int Sequence::remove(const ItemType& value)
{
	int itemsremoved = 0;
	for (int k = 0; k < size(); k++)
	{
		if (m_array[k] == value)
		{
			for (int j = k; j < size() - 1; j++)
			{
				m_array[j] = m_array[j + 1];
			}
			m_size--;
			itemsremoved++;
			k--;
		}
	}
	return itemsremoved;
}
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= size())
		return false;
	else
	{
		value = m_array[pos];
		return true;
	}
}
// If 0 <= pos < size(), copy into value the item at position pos
// in the sequence and return true.  Otherwise, leave value unchanged
// and return false.

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= size())
		return false;
	else
	{
		m_array[pos] = value;
		return true;
	}
}
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.

int Sequence::find(const ItemType& value) const
{
	for (int k = 0; k < size(); k++)
	{
		if (m_array[k] == value)
			return k;
	}
	return -1;
}
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.

void Sequence::swap(Sequence& other)
{
	ItemType temparray[DEFAULT_MAX_ITEMS];

	int tempsize = size(); //Copy this Sequence to the temp storage
	for (int k = 0; k < size(); k++)
	{
		get(k,temparray[k]);
	}
	m_size = other.size(); //Copy other to this Sequence
	for (int k = 0; k < size(); k++)
	{
		set(k, other.m_array[k]);
	}
	other.m_size = tempsize; //Copy temp to other Sequence
	for (int k = 0; k < tempsize; k++)
	{
		other.m_array[k] = temparray[k];
	}
}
// Exchange the contents of this sequence with the other one.

void Sequence::dump() const //Print out the Sequence
{
	for (int k = 0; k < size(); k++)
	{
		cerr << m_array[k] << ", ";
	}
}