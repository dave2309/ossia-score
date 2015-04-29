#include "NodePath.hpp"

#include <QModelIndex>
#include "DeviceExplorer/Node/Node.hpp"

Path::Path()
{

}

Path::Path(QModelIndex index)
{
    QModelIndex iter = index;

    while(iter.isValid())
    {
        m_path.prepend(iter.row());
        iter = iter.parent();
    }
}

Path::Path(Node *node)
{
    Node* iter = node;

    while(! iter->isDevice())
    {
        m_path.prepend(iter->parent()->indexOfChild(iter));
        iter = iter->parent();
    }
    m_path.prepend(iter->parent()->indexOfChild(iter));
}

Node *Path::toNode(Node *iter)
{
    const int pathSize = m_path.size();

    for (int i = 0; i < pathSize; ++i)
    {
        if (m_path.at(i) < iter->childCount())
        {
            iter = iter->childAt(m_path.at(i));
        }
        else
        {
            return nullptr;
        }
    }

    return iter;
}

void Path::append(int i)
{
    m_path.append(i);
}

void Path::prepend(int i)
{
    m_path.prepend(i);
}

const int &Path::at(int i) const
{
    return m_path.at(i);
}

int &Path::back()
{
    return m_path.back();
}

int Path::size() const
{
    return m_path.size();
}

void Path::removeLast()
{
    m_path.removeLast();
}

void Path::clear()
{
    m_path.clear();
}

void Path::reserve(int size)
{
    m_path.reserve(size);
}

void Path::serializePath(QDataStream &d) const
{
    d << m_path;/*
    const int size = m_path.size();
    d << (qint32) size;

    for(int i = 0; i < size; ++i)
    {
        d << (qint32) m_path.at(i);
    }*/
}

void Path::deserializePath(QDataStream &d)
{
    d >> m_path;
    /*
    qint32 size;
    d >> size;
    m_path.reserve(size);

    for(int i = 0; i < size; ++i)
    {
        qint32 v;
        d >> v;
        m_path.append(v);
    }
    */
}

