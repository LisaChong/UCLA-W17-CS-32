// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

struct coords;

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap()
		: m_root(nullptr) {};
	~MyMap() { clear(); };
	void clear() { clearTree(m_root); };
	int size() const { return m_size; };
	void associate(const KeyType& key, const ValueType& value)
	{
		addNode(key, value, m_root);
	};

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const
	{
		return findTree(key, m_root);
	};

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	class Node
	{
	public:
		Node(KeyType key, ValueType value)
			: m_left(nullptr), m_right(nullptr), m_key(key), m_value(value) {};
		Node* m_left;
		Node* m_right;
		KeyType m_key;
		ValueType m_value;
	};
	void clearTree(Node* current)
	{
		if (nullptr == current)
			return;
		else
		{
			clearTree(current->m_left);
			clearTree(current->m_right);
			delete current;
		}
	};
	ValueType* findTree(const KeyType& key, Node* current) const
	{
		if (current == nullptr)
			return nullptr;
		else if (key == current->m_key)
		{
			ValueType valueReturn = current->m_value;
			return &valueReturn;
		}
		else if (key > current->m_key)
			return findTree(key, current->m_right);
		else
			return findTree(key, current->m_left);
	};
	void addNode(const KeyType& key, const ValueType& value, Node* cur)
	{
		if (m_root == nullptr)
		{
			m_root = new Node(key, value);
			m_size++;
			return;
		}
		if (key == cur->m_key)
		{
			cur->m_value = value;
			return;
		}
		else if (key > cur->m_key)
		{
			if (cur->m_right != nullptr)
				addNode(key, value, cur->m_right);
			else
			{
				cur->m_right = new Node(key, value);
				m_size++;
			}
		}
		else
		{
			if (cur->m_left != nullptr)
				addNode(key, value, cur->m_left);
			else
			{
				cur->m_left = new Node(key, value);
				m_size++;
			}
		}
	};
	Node* m_root;
	int m_size = 0;
};
