#pragma once

#include "HD_Math.h"
#include "HD_Utilities.h"

#include <cassert>

// AVL Tree
template<typename K, typename V>
class HD_MapNode
{
public:
	typedef K KeyType;
	typedef V ValueType;

	HD_MapNode();

	int GetBalanceFactor() const;
	int UpdateHeight();

	void SwapContents(HD_MapNode& aNodeToSwap);

	K myKey;
	V myValue;

	HD_MapNode* myLeft;
	HD_MapNode* myRight;
	HD_MapNode* myParent;
	int myHeight;
};

template<typename Node>
class HD_MapIterator
{
public:
	HD_MapIterator();
	HD_MapIterator(Node* aNode);
	HD_MapIterator(const HD_MapIterator& aIterator);

	HD_MapIterator& operator++();
	HD_MapIterator& operator--();
	HD_MapIterator operator++(int);
	HD_MapIterator operator--(int);
	HD_MapIterator& operator+=(int aIncrement);
	HD_MapIterator& operator-=(int aDecrement);
	bool operator==(const HD_MapIterator& aIterator) const;
	bool operator!=(const HD_MapIterator& aIterator) const;
	Node& operator*() const;
	Node* operator->() const;

private:
	Node* myNode;
};

template<typename K, typename V>
class HD_Map
{
public:
	typedef HD_MapNode<K, V> Node;
	typedef HD_MapIterator<Node> Iterator;
	typedef HD_MapIterator<const Node> ConstIterator;
	friend class Iterator;
	friend class ConstIterator;

	HD_Map();
	HD_Map(const HD_Map& aMap);
	HD_Map(HD_Map&& aMap);
	~HD_Map();

	const V* GetIfExists(const K& aKey) const;

	V& operator[](const K& aKey);

	void Remove(const K& aKey);

	HD_Map& operator=(const HD_Map& aMap);
	HD_Map& operator=(HD_Map&& aMap);

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

private:
	bool GetIfExistsAtRoot(Node*& outLastNodeVisited, const K& aKey, Node* aRoot) const;

	void RemoveAtRoot(const K& aKey, Node* aRoot);

	void InsertRetrace(Node* aRetraceBeginNode);
	void RemoveRetrace(Node* aRetraceBeginNode, Node* aRetraceEndNode);

	Node* Balance(Node* aNodeToBalance);
	Node* RotateLeft(Node* aNodeToRotate);
	Node* RotateRight(Node* aNodeToRotate);

	void Cleanup(Node*& aNode);

	static Node* GetNodeWithSmallestKeyAtRoot(Node* aRoot);
	static Node* GetNodeWithGreatestKeyAtRoot(Node* aRoot);

	Node* myRoot;
};

template<typename K, typename V>
HD_Map<K, V>::HD_Map()
	: myRoot(nullptr)
{
}

template<typename K, typename V>
HD_Map<K, V>::HD_Map(const HD_Map& aMap)
	: myRoot(nullptr)
{
	*this = aMap;
}

template<typename K, typename V>
HD_Map<K, V>::HD_Map(HD_Map&& aMap)
{
	myRoot = aMap.myRoot;
	aMap.myRoot = nullptr;
}

template<typename K, typename V>
HD_Map<K, V>::~HD_Map()
{
	Cleanup(myRoot);
}

template<typename K, typename V>
const V* HD_Map<K, V>::GetIfExists(const K& aKey) const
{
	Node* lastNodeVisited = nullptr;
	if (GetIfExistsAtRoot(lastNodeVisited, aKey, myRoot))
		return &lastNodeVisited->myValue;

	return nullptr;
}

template<typename K, typename V>
bool HD_Map<K, V>::GetIfExistsAtRoot(Node*& outLastNodeVisited, const K& aKey, Node* aRoot) const
{
	Node* current = aRoot;
	while (current != nullptr)
	{
		outLastNodeVisited = current;

		if (aKey == current->myKey)
			return true;

		if (aKey < current->myKey)
			current = current->myLeft;
		else
			current = current->myRight;
	}

	return false;
}

template<typename K, typename V>
V& HD_Map<K, V>::operator[](const K& aKey)
{
	Node* lastNodeVisited = nullptr;
	if (GetIfExistsAtRoot(lastNodeVisited, aKey, myRoot))
		return lastNodeVisited->myValue;

	Node* newNode = new Node();
	newNode->myKey = aKey;
	
	if (!lastNodeVisited)
	{
		myRoot = newNode;
	}
	else
	{
		if (aKey < lastNodeVisited->myKey)
			lastNodeVisited->myLeft = newNode;
		else
			lastNodeVisited->myRight = newNode;

		newNode->myParent = lastNodeVisited;
	}

	InsertRetrace(lastNodeVisited);

	return newNode->myValue;
}

template<typename K, typename V>
void HD_Map<K, V>::Remove(const K& aKey)
{
	RemoveAtRoot(aKey, myRoot);
}

template<typename K, typename V>
void HD_Map<K, V>::RemoveAtRoot(const K& aKey, Node* aRoot)
{
	Node* lastNodeVisited = nullptr;
	if (!GetIfExistsAtRoot(lastNodeVisited, aKey, aRoot))
		return;

	Node* nodeToRemove = lastNodeVisited;
	Node* retraceBeginNode = nullptr;
	Node* retraceEndNode = aRoot->myParent;

	if (!nodeToRemove->myLeft && !nodeToRemove->myRight)
	{
		if (nodeToRemove == aRoot && !aRoot->myParent)
		{
			HD_SafeDelete(myRoot);
		}
		else
		{
			Node* parentOfNodeToRemove = nodeToRemove->myParent;
			if (nodeToRemove == parentOfNodeToRemove->myLeft)
			{
				HD_SafeDelete(parentOfNodeToRemove->myLeft);
			}
			else
			{
				HD_SafeDelete(parentOfNodeToRemove->myRight);
			}

			retraceBeginNode = parentOfNodeToRemove;
		}
	}
	else if (nodeToRemove->myLeft && !nodeToRemove->myRight)
	{
		nodeToRemove->SwapContents(*nodeToRemove->myLeft);
		HD_SafeDelete(nodeToRemove->myLeft);
		retraceBeginNode = nodeToRemove;
	}
	else if (!nodeToRemove->myLeft && nodeToRemove->myRight)
	{
		nodeToRemove->SwapContents(*nodeToRemove->myRight);
		HD_SafeDelete(nodeToRemove->myRight);
		retraceBeginNode = nodeToRemove;
	}
	else
	{
		if (nodeToRemove->myLeft->myHeight > nodeToRemove->myRight->myHeight)
		{
			Node* leftNodeWithGreatestKey = GetNodeWithGreatestKeyAtRoot(nodeToRemove->myLeft);
			nodeToRemove->SwapContents(*leftNodeWithGreatestKey);
			RemoveAtRoot(aKey, nodeToRemove->myLeft);
		}
		else
		{
			Node* rightNodeWithSmallestKey = GetNodeWithSmallestKeyAtRoot(nodeToRemove->myRight);
			nodeToRemove->SwapContents(*rightNodeWithSmallestKey);
			RemoveAtRoot(aKey, nodeToRemove->myRight);
		}

		retraceBeginNode = nodeToRemove;
	}

	RemoveRetrace(retraceBeginNode, retraceEndNode);
}

template<typename K, typename V>
void HD_Map<K, V>::InsertRetrace(Node* aRetraceBeginNode)
{
	Node* currentNode = aRetraceBeginNode;
	while (currentNode)
	{
		currentNode->UpdateHeight();
		int absBalanceFactorAfterInsert = HD_Abs(currentNode->GetBalanceFactor());

		if (absBalanceFactorAfterInsert == 0)
		{
			break;
		}
		else if (absBalanceFactorAfterInsert == 1)
		{
			currentNode = currentNode->myParent;
		}
		else if (absBalanceFactorAfterInsert == 2)
		{
			Balance(currentNode);
			break;
		}
		else
		{
			assert(true && "Node balance factor is less than -2 or greater than +2 during insert retrace.");
		}
	}
}

template<typename K, typename V>
void HD_Map<K, V>::RemoveRetrace(Node* aRetraceBeginNode, Node* aRetraceEndNode)
{
	Node* currentNode = aRetraceBeginNode;
	while (currentNode != aRetraceEndNode)
	{
		currentNode->UpdateHeight();
		int absBalanceFactorAfterRemove = HD_Abs(currentNode->GetBalanceFactor());

		if (absBalanceFactorAfterRemove == 0)
		{
			currentNode = currentNode->myParent;
		}
		else if (absBalanceFactorAfterRemove == 1)
		{
			break;
		}
		else if (absBalanceFactorAfterRemove == 2)
		{
			Node* newRoot = Balance(currentNode);

			int absBalanceFactorAfterBalance = HD_Abs(newRoot->GetBalanceFactor());
			if (absBalanceFactorAfterBalance == 1)
				break;

			currentNode = newRoot->myParent;
		}
		else
		{
			assert(true && "Node balance factor is less than -2 or greater than +2 during remove retrace.");
		}
	}
}

template<typename K, typename V>
typename HD_Map<K, V>::Node* HD_Map<K, V>::Balance(Node* aNodeToBalance)
{
	int balanceFactor = aNodeToBalance->GetBalanceFactor();
	assert(HD_Abs(balanceFactor) < 3 && "Node balance factor is less than -2 or greater than +2.");
	assert(HD_Abs(balanceFactor) == 2 && "Unnecessary call to Balance function. Node is already balanced.");

	Node* newRoot = aNodeToBalance;
	if (balanceFactor == -2)
	{
		int leftChildBalanceFactor = aNodeToBalance->myLeft->GetBalanceFactor();
		if (leftChildBalanceFactor <= 0)
		{
			newRoot = RotateRight(aNodeToBalance);
		}
		else
		{
			RotateLeft(aNodeToBalance->myLeft);
			newRoot = RotateRight(aNodeToBalance);
		}
	}
	else if (balanceFactor == 2)
	{
		int rightChildBalanceFactor = aNodeToBalance->myRight->GetBalanceFactor();
		if (rightChildBalanceFactor >= 0)
		{
			newRoot = RotateLeft(aNodeToBalance);
		}
		else
		{
			RotateRight(aNodeToBalance->myRight);
			newRoot = RotateLeft(aNodeToBalance);
		}
	}

	return newRoot;
}

template<typename K, typename V>
typename HD_Map<K, V>::Node* HD_Map<K, V>::RotateLeft(Node* aNodeToRotate)
{
	Node* parent = aNodeToRotate->myParent;
	Node* rightChild = aNodeToRotate->myRight;

	aNodeToRotate->myRight = rightChild->myLeft;
	rightChild->myLeft = aNodeToRotate;

	aNodeToRotate->myParent = rightChild;

	if (aNodeToRotate->myRight)
		aNodeToRotate->myRight->myParent = aNodeToRotate;

	rightChild->myParent = parent;
	if (parent)
	{
		if (parent->myLeft == aNodeToRotate)
			parent->myLeft = rightChild;
		else
			parent->myRight = rightChild;
	}
	else
	{
		myRoot = rightChild;
	}

	rightChild->UpdateHeight();

	return rightChild;
}

template<typename K, typename V>
typename HD_Map<K, V>::Node* HD_Map<K, V>::RotateRight(Node* aNodeToRotate)
{
	Node* parent = aNodeToRotate->myParent;
	Node* leftChild = aNodeToRotate->myLeft;

	aNodeToRotate->myLeft = leftChild->myRight;
	leftChild->myRight = aNodeToRotate;
	
	aNodeToRotate->myParent = leftChild;

	if (aNodeToRotate->myLeft)
		aNodeToRotate->myLeft->myParent = aNodeToRotate;

	leftChild->myParent = parent;
	if (parent)
	{
		if (parent->myLeft == aNodeToRotate)
			parent->myLeft = leftChild;
		else
			parent->myRight = leftChild;
	}
	else
	{
		myRoot = leftChild;
	}

	leftChild->UpdateHeight();

	return leftChild;
}

template<typename K, typename V>
HD_Map<K, V>& HD_Map<K, V>::operator=(const HD_Map& aMap)
{
	Cleanup(myRoot);

	for (const auto& entry : aMap)
		(*this)[entry.myKey] = entry.myValue;

	return *this;
}

template<typename K, typename V>
HD_Map<K, V>& HD_Map<K, V>::operator=(HD_Map&& aMap)
{
	Cleanup(myRoot);

	myRoot = aMap.myRoot;
	aMap.myRoot = nullptr;

	return *this;
}

template<typename K, typename V>
typename HD_Map<K, V>::Iterator HD_Map<K, V>::begin()
{
	return Iterator(GetNodeWithSmallestKeyAtRoot(myRoot));
}

template<typename K, typename V>
typename HD_Map<K, V>::Iterator HD_Map<K, V>::end()
{
	return Iterator(nullptr);
}

template<typename K, typename V>
typename HD_Map<K, V>::ConstIterator HD_Map<K, V>::begin() const
{
	return ConstIterator(GetNodeWithSmallestKeyAtRoot(myRoot));
}

template<typename K, typename V>
typename HD_Map<K, V>::ConstIterator HD_Map<K, V>::end() const
{
	return ConstIterator(nullptr);
}

template<typename K, typename V>
void HD_Map<K, V>::Cleanup(Node*& aNode)
{
	if (!aNode)
		return;

	Cleanup(aNode->myLeft);
	Cleanup(aNode->myRight);
	HD_SafeDelete(aNode);
}

template<typename K, typename V>
typename HD_Map<K, V>::Node* HD_Map<K, V>::GetNodeWithSmallestKeyAtRoot(Node* aRoot)
{
	Node* nodeWithSmallestKey = aRoot;
	Node* leftChild = aRoot->myLeft;

	while (leftChild)
	{
		nodeWithSmallestKey = leftChild;
		leftChild = leftChild->myLeft;
	}

	return nodeWithSmallestKey;
}

template<typename K, typename V>
typename HD_Map<K, V>::Node* HD_Map<K, V>::GetNodeWithGreatestKeyAtRoot(Node* aRoot)
{
	Node* nodeWithGreatestKey = aRoot;
	Node* rightChild = aRoot->myRight;

	while (rightChild)
	{
		nodeWithGreatestKey = rightChild;
		rightChild = rightChild->myRight;
	}

	return nodeWithGreatestKey;
}

template<typename K, typename V>
HD_MapNode<K, V>::HD_MapNode()
	: myLeft(nullptr)
	, myRight(nullptr)
	, myParent(nullptr)
	, myHeight(1)
{
}

template<typename K, typename V>
int HD_MapNode<K, V>::GetBalanceFactor() const
{
	int rightHeight = myRight ? myRight->myHeight : 0;
	int leftHeight = myLeft ? myLeft->myHeight : 0;
	return rightHeight - leftHeight;
}

template<typename K, typename V>
int HD_MapNode<K, V>::UpdateHeight()
{
	int leftHeight = myLeft ? myLeft->UpdateHeight() : 0;
	int rightHeight = myRight ? myRight->UpdateHeight() : 0;
	myHeight = HD_Max(leftHeight, rightHeight) + 1;
	return myHeight;
}

template<typename K, typename V>
void HD_MapNode<K, V>::SwapContents(HD_MapNode& aNodeToSwap)
{
	K tempKey = myKey;
	V tempValue = myValue;

	myKey = aNodeToSwap.myKey;
	myValue = aNodeToSwap.myValue;

	aNodeToSwap.myKey = tempKey;
	aNodeToSwap.myValue = tempValue;
}

template<typename Node>
HD_MapIterator<Node>::HD_MapIterator()
	: myNode(nullptr)
{
}

template<typename Node>
HD_MapIterator<Node>::HD_MapIterator(Node* aNode)
	: myNode(aNode)
{
}

template<typename Node>
HD_MapIterator<Node>::HD_MapIterator(const HD_MapIterator& aIterator)
	: myNode(aIterator.myNode)
{
}

template<typename Node>
HD_MapIterator<Node>& HD_MapIterator<Node>::operator++()
{
	if (!myNode)
		return *this;

	if (myNode->myRight)
	{
		myNode = HD_Map<Node::KeyType, Node::ValueType>::GetNodeWithSmallestKeyAtRoot(myNode->myRight);
	}
	else if (myNode->myParent)
	{
		Node* parent = myNode->myParent;
		if (myNode == parent->myLeft)
		{
			myNode = parent;
		}
		else
		{
			while (parent)
			{
				if (myNode == parent->myLeft)
					break;

				myNode = parent;
				parent = parent->myParent;
			}

			myNode = parent;
		}
	}
	else
	{
		myNode = nullptr;
	}

	return *this;
}

template<typename Node>
HD_MapIterator<Node>& HD_MapIterator<Node>::operator--()
{
	if (!myNode)
		return *this;

	if (myNode->myLeft)
	{
		myNode = HD_Map<Node::KeyType, Node::ValueType>::GetNodeWithGreatestKeyAtRoot(myNode->myLeft);
	}
	else if (myNode->myParent)
	{
		Node* parent = myNode->myParent;
		if (myNode == parent->myRight)
		{
			myNode = parent;
		}
		else
		{
			while (parent)
			{
				if (myNode == parent->myRight)
					break;

				myNode = parent;
				parent = parent->myParent;
			}

			myNode = parent;
		}
	}

	return *this;
}

template<typename Node>
HD_MapIterator<Node> HD_MapIterator<Node>::operator++(int)
{
	HD_MapIterator iterator = *this;
	++(*this);
	return iterator;
}

template<typename Node>
HD_MapIterator<Node> HD_MapIterator<Node>::operator--(int)
{
	HD_MapIterator iterator = *this;
	--(*this);
	return iterator;
}

template<typename Node>
HD_MapIterator<Node>& HD_MapIterator<Node>::operator+=(int aIncrement)
{
	for (int i = 0; i < aIncrement; ++i)
		++(*this);

	return *this;
}

template<typename Node>
HD_MapIterator<Node>& HD_MapIterator<Node>::operator-=(int aDecrement)
{
	for (int i = 0; i < aDecrement; ++i)
		--(*this);

	return *this;
}

template<typename Node>
bool HD_MapIterator<Node>::operator==(const HD_MapIterator& aIterator) const
{
	return myNode == aIterator.myNode;
}

template<typename Node>
bool HD_MapIterator<Node>::operator!=(const HD_MapIterator& aIterator) const
{
	return !(*this == aIterator);
}

template<typename Node>
Node& HD_MapIterator<Node>::operator*() const
{
	return *myNode;
}

template<typename Node>
Node* HD_MapIterator<Node>::operator->() const
{
	return myNode;
}
