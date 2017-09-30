#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED
#include <string>

typedef std::string ItemType;

class Sequence
{
public:
	Sequence();
	Sequence::~Sequence();
	Sequence& Sequence::operator= (const Sequence& value);
	Sequence(const Sequence& other);
	bool empty() const;
	int size() const;
	bool insert(int pos, const ItemType& value);
	int insert(const ItemType& value);
	bool erase(int pos);
	int remove(const ItemType& value);
	bool get(int pos, ItemType& value) const;
	bool set(int pos, const ItemType& value);
	int find(const ItemType& value) const;
	void swap(Sequence& other);
	void dump() const;
private:
	struct Node
	{
		Node(ItemType data);
		ItemType m_data;
		Node* m_next;
		Node* m_previous;
	};
	Node* m_head;
	int m_size;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);
#endif //SEQUENCE_INCLUDED