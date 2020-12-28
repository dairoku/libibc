// =============================================================================
//  (delegate_)subcontract_interface.h
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
  \file     ibc/qt/property/subcontract_interface.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/12/27
*/

#ifndef IBC_QT_PROPERTY_SUBCONTRACT_INTERFACE_H_
#define IBC_QT_PROPERTY_SUBCONTRACT_INTERFACE_H_

// Includes --------------------------------------------------------------------
#include <QStyledItemDelegate>

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // SubcontractInterface class
  // ---------------------------------------------------------------------------
  class SubcontractInterface
  {
  public:
    // Member functions (overrides) --------------------------------------------
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) = 0;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) = 0;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) = 0;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) = 0;

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getNodeBase
    // -------------------------------------------------------------------------
    static ibc::property::NodeBase *getNodeBase(const QModelIndex &inIndex)
    {
      return (ibc::property::NodeBase *)inIndex.internalPointer();
    }
    // -------------------------------------------------------------------------
    // getNode
    // -------------------------------------------------------------------------
    template <class T> static ibc::property::Node<T> *getNode(const QModelIndex &inIndex)
    {
      return (ibc::property::Node<T> *)inIndex.internalPointer();
    }
    // -------------------------------------------------------------------------
    // getSubcontract
    // -------------------------------------------------------------------------
    static SubcontractInterface *getSubcontract(const QModelIndex &inIndex)
    {
      ibc::property::NodeBase  *nodeBase;
      nodeBase = getNodeBase(inIndex);
      if (nodeBase == NULL)
        return NULL;

      return (SubcontractInterface  *)nodeBase->getAuxiliaryDataPointer();
    }
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_SUBCONTRACT_INTERFACE_H_
