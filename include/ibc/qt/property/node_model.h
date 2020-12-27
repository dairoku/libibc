// =============================================================================
//  node_delegate.h
//
//  MIT License
//
//  Copyright (c) 2020 Dairoku Sekiguchi
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
// =============================================================================
/*!
  \file     ibc/qt/property/node_model.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/12/27
*/

#ifndef IBC_QT_PROPERTY_NODE_MODEL_H_
#define IBC_QT_PROPERTY_NODE_MODEL_H_

// Includes --------------------------------------------------------------------
#include <QAbstractItemModel>
#include "ibc/property/node.h"

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // NodeDelegate class
  // ---------------------------------------------------------------------------
  class NodeModel : public QAbstractItemModel
  {
    Q_OBJECT

  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // NodeModel
    // -------------------------------------------------------------------------
    NodeModel(std::shared_ptr<ibc::property::NodeBase> inRootNode,
              QObject *parent = Q_NULLPTR)
    : QAbstractItemModel(parent)
    {
      mRootNode = inRootNode;
    }
    // -------------------------------------------------------------------------
    // ~NodeDelegate
    // -------------------------------------------------------------------------
    virtual ~NodeModel()
    {
    }

    // Member functions (overrides) --------------------------------------------
    // -------------------------------------------------------------------------
    // rowCount
    // -------------------------------------------------------------------------
    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
      return 1;
    }
    // -------------------------------------------------------------------------
    // columnCount
    // -------------------------------------------------------------------------
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
      return 2;
    }
    // -------------------------------------------------------------------------
    // index
    // -------------------------------------------------------------------------
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const
    {
      // Sanity check
      if (mRootNode == NULL)
        return QModelIndex(); // something wrong

      // Whether this is a root node or not
      if (parent.isValid() == false)
      {
        // Sanity check for the root node
        if (row != 0 || column != 0)
          return QModelIndex(); // something wrong
        // returns the root node index
        return createIndex(0, 0, mRootNode.get());
      }

      if (column > 1)
        return QModelIndex(); // Out of index

      ibc::property::NodeBase *parentNode = getNode(parent);
      if (row >= parentNode->getChildrenNum())
        return QModelIndex(); // Out of index

      return createIndex(row, column, parentNode->getChild(row).get());
    }
    // -------------------------------------------------------------------------
    // parent
    // -------------------------------------------------------------------------
    virtual QModelIndex parent(const QModelIndex &index) const
    {
      // Sanity check
      if (index.isValid() == false)
        return QModelIndex();

      ibc::property::NodeBase *node = getNode(index);
      std::shared_ptr<ibc::property::NodeBase> parent = node->getParent();
      if (parent == NULL)
        return QModelIndex();

      return createIndex(0, 0, parent.get());
    }
    // -------------------------------------------------------------------------
    // flags
    // -------------------------------------------------------------------------
    virtual Qt::ItemFlags flags(const QModelIndex &index) const
    {
      if (index.column() == 0)
        return QAbstractItemModel::flags(index);
      // Returns with editable flag, when column != 0
      return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
    // -------------------------------------------------------------------------
    // headerData
    // -------------------------------------------------------------------------
    QVariant headerData(int section, Qt::Orientation orientation,
                          int role = Qt::DisplayRole) const
    {
      if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
      //
      if (section == 0)
        return QVariant("Name");
      return QVariant("Data");
    }
    // -------------------------------------------------------------------------
    // data
    // -------------------------------------------------------------------------
    virtual QVariant data(const QModelIndex &index, int role) const
    {
      if (role != Qt::DisplayRole || index.isValid() == false)
        return QVariant();
      //
      ibc::property::NodeBase *nodeBase = getNode(index);
     if (index.column() == 0) // Column is 0
        return QVariant(nodeBase->getName().c_str());
      //
      if (nodeBase->hasValue() == false)  // Has no value
        return QVariant();
      //
      if (nodeBase->checkType<ibc::property::Node<int>>() == false)
        return QVariant(0);
      ibc::property::Node<int> *node = (ibc::property::Node<int> *)nodeBase;  // Up cast
      return QVariant(node->getValue());
    }

  protected:
    // Member variables --------------------------------------------------------
    std::shared_ptr<ibc::property::NodeBase>  mRootNode;

    // -------------------------------------------------------------------------
    // getNode
    // -------------------------------------------------------------------------
    ibc::property::NodeBase *getNode(const QModelIndex &inIndex) const
    {
      return (ibc::property::NodeBase *)inIndex.internalPointer();
    }
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_NODE_MODEL_H_
