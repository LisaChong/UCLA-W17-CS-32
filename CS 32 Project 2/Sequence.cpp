#include "Sequence.h"
#include <iostream>

using namespace std;

Sequence::Node::Node(ItemType data)
	: m_data(data)
{
	m_next = nullptr;
	m_previous = nullptr;
}

Sequence::~Sequence()
{
	if (m_head == nullptr);
	else if (m_head->m_next == nullptr)
		delete m_head;
	else
	{
		Node* temp = m_head->m_next;
		Node* removeThis = m_head;
		while (temp != nullptr)
		{			
			delete removeThis;
			removeThis = temp;
			temp = temp->m_next;
		}
		delete removeThis;
	}
}

Sequence& Sequence::operator= (const Sequence& value) //Assignment Constructor
{
	if (this == &value)
		return *this;
	int size = value.size();
	Sequence temp;
	ItemType testValue;
	for (int k = 0; k < size; k++)
	{
		value.get(k, testValue);
		temp.insert(k, testValue);
	}
	swap(temp);
	this->m_size = size;
	return *this;
}

Sequence::Sequence(const Sequence& other) //Copy Constructor
{
	int size = other.size();
	for (int k = 0; k < size; k++)
	{
		ItemType testValue;
		other.get(k, testValue);
		this->insert(k, testValue);
	}
	this->m_size = size;
}

Sequence::Sequence()
	: m_size(0)
{
	m_head = nullptr;
}

bool Sequence::empty() const
{
	if (m_head == nullptr && m_size == 0)
		return true;
	else
		return false;
}

int Sequence::size() const
{
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	if (pos < 0 || pos > m_size)
		return false;
	else if (pos == 0 && m_head != nullptr)
	{
		Node* tempNew = new Node(value);
		tempNew->m_next = m_head;
		m_head->m_previous = tempNew;
		m_head = tempNew;
		m_size++;
	}
	else if (pos == 0 && m_head == nullptr)
	{
		Node* tempNew = new Node(value);
		m_head = tempNew;
		m_size++;
	}
	else if (pos == m_size)
	{
		Node* temp = m_head;
		while (temp->m_next != nullptr)
		{
			temp = temp->m_next;
		}
		Node* tempNew = new Node(value);
		tempNew->m_previous = temp;
		temp->m_next = tempNew;
		m_size++;
	}
	else
	{
		int counter = 0;
		Node* temp = m_head;
		while (counter != pos)
		{
			temp = temp->m_next;
			counter++;
		}
		Node* tempNew = new Node(value);
		tempNew->m_next = temp;
		tempNew->m_previous = temp->m_previous;
		temp->m_previous = tempNew;
		m_size++;
	}
	return true;
}

int Sequence::insert(const ItemType& value)
{
	int counter = 0;
	Node* temp = m_head;
	if (m_head == nullptr && m_size == 0)
	{
		Node* tempNew = new Node(value);
		m_head = tempNew;
		m_size++;
	}
	else
	{
		if (temp->m_data >= value)
		{
			Node* tempNew = new Node(value);
			tempNew->m_next = temp;
			temp->m_previous = tempNew;
			m_head = tempNew;
			m_size++;
		}
		else
		{
			while (temp->m_data < value)
			{
				if (temp->m_next != nullptr)
				{
					temp = temp->m_next;
					counter++;
				}
				if (temp->m_next == nullptr)
					break;
			}
			if (temp->m_next == nullptr)
			{
				Node* tempNew = new Node(value);
				tempNew->m_previous = temp;
				temp->m_next = tempNew;
				m_size++;
			}
			else
			{
				Node* tempNew = new Node(value);
				tempNew->m_next = temp;
				tempNew->m_previous = temp->m_previous;
				temp->m_previous = tempNew;
				m_size++;
			}
		}
	}
	return counter;
}

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= m_size || m_size == 0)
		return false;
	if (pos == 0 && m_size != 0)
	{
		if (m_size == 1)
		{
			delete m_head;
			m_head = nullptr;
			m_size--;
			return true;
		}
		else
		{
			Node* temp = m_head->m_next;
			temp->m_previous = nullptr;
			delete m_head;
			m_head = temp;
			m_size--;
			return true;
		}
	}
	int counter = 0;
	Node* temp = m_head;
	while (counter != pos)
	{
		temp = temp->m_next;
		counter++;
	}
	if (pos == m_size - 1)
	{
		Node* tempNew = temp->m_previous;
		tempNew->m_next = nullptr;
		delete temp;
		m_size--;
	}
	else
	{
		Node* tempNew = temp->m_previous;
		tempNew->m_next = temp->m_next;
		temp->m_next->m_previous = tempNew;
		delete temp;
		m_size--;
	}
	return true;
}

int Sequence::remove(const ItemType& value)
{
	int counter = 0;
	while (find(value) != -1)
	{
		erase(find(value));
		counter++;
	}
	return counter;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= m_size)
		return false;
	int counter = 0;
	Node* temp = m_head;
	while (counter != pos)
	{
		temp = temp->m_next;
		counter++;
	}
	value = temp->m_data;
	return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= m_size)
		return false;
	int counter = 0;
	Node* temp = m_head;
	while (counter != pos)
	{
		temp = temp->m_next;
		counter++;
	}
	temp->m_data = value;
	return true;
}

int Sequence::find(const ItemType& value) const
{
	if (m_size == 0 && m_head == nullptr)
		return -1;
	if (m_head->m_data == value)
		return 0;
	int counter = 0;
	Node* temp = m_head;
	while (temp->m_data != value)
	{
		if (temp->m_next == nullptr)
			return -1;
		temp = temp->m_next;
		counter++;
	}
	return counter++;
}

void Sequence::swap(Sequence& other)
{
	Node* temp = this->m_head;
	int tempSize = this->m_size;
	this->m_head = other.m_head;
	this->m_size = other.m_size;
	other.m_head = temp;
	other.m_size = tempSize;
}

void Sequence::dump() const
{
	Node* temp = m_head;
	while (temp != nullptr)
	{
		cerr << temp->m_data << ", ";
		temp = temp->m_next;
	}
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	if (seq2.size() > seq1.size())
		return -1;
	if (seq2.empty() == true)
		return 0;
	ItemType seq1value;
	ItemType seq2value;
	for (int k = 0; k < seq1.size(); k++)
	{
		if (seq1.size() - k - seq2.size() < 0)
			break;
		seq1.get(k, seq1value);
		seq2.get(0, seq2value);
		if (seq1value == seq2value)
		{
			for (int j = 1; j < seq2.size(); j++)
			{
				seq1.get(k+j, seq1value);
				seq2.get(j, seq2value);
				if (seq1value != seq2value)
					break;
				if (j = seq2.size() - 1)
					return k;
			}
		}
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	Sequence tempresult;
	if (seq2.empty() == true)
		result = seq1;
	if (seq1.empty() == true)
		result = seq2;
	int maxSize = 0;
	if (seq1.size() > seq2.size())
		maxSize = seq1.size();
	else
		maxSize = seq2.size();
	ItemType seq1value;
	ItemType seq2value;
	int counter = 0;
	for (int k = 0; k < maxSize; k++)
	{
		if (seq1.get(k, seq1value) == true)
		{
			tempresult.insert(counter, seq1value);
			counter++;
		}
		if (seq2.get(k, seq2value) == true)
		{
			tempresult.insert(counter, seq2value);
			counter++;
		}
	}
	result = tempresult;
}