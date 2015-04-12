#ifndef _I_TREE_NODE_H
#define _I_TREE_NODE_H

#include <vector>

template <class T>
class ITreeNode
{
protected:
	T* m_Parent; // The parent of this tree node
	std::vector<T*> m_Children; // All of the children of this tree node

	bool childrenContains(T* child)
	{
		for (unsigned int i = 0; i < m_Children.size(); i++)
		{
			if (child == m_Children[i])
			{
				return true;
			}
		}

		return false;
	}

	void removeFromChildList(T* child)
	{
		for (unsigned int i = 0; i < m_Children.size(); i++)
		{
			if (child == m_Children[i])
			{
				m_Children.erase(m_Children.begin() + i);
				return;
			}
		}
	}

public:
	ITreeNode()
	{

	}

	bool isRoot() { return (m_Parent == NULL); }
	bool isAttached() { return !isRoot(); }

	// Getters
	T* getParent() { return m_Parent; }
	std::vector<T*> getChildren() { return m_Children; }

	// Setters
	void setParent(T* parent) { m_Parent = parent; }
	void setChildren(std::vector<T*> children) { m_Children = children; }

	void addChild(T* child)
	{
		child->setParent(this);
		m_Children.push_back(child);
	}

	void addChildren(std::vector<T*> children)
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			addChild(children[i]);
		}
	}

	void removeChild(T* child)
	{
		if (child->getParent() != this || !childrenContains(child))
			return;

		child->setParent(NULL);
		removeFromChildList(child);
	}
};

#endif