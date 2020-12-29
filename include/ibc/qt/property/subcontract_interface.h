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
#include <QString>

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // SubcontractInterface class
  // ---------------------------------------------------------------------------
  class SubcontractInterface
  {
  public:
    // Enum --------------------------------------------------------------------
    enum  DataType
    {
      DATA_TYPE_NOT_SPECIFIED = 0,
      //
      DATA_TYPE_int           = 1,
      DATA_TYPE_uint,
      DATA_TYPE_double,
      DATA_TYPE_bool,
      //
      DATA_TYPE_QString,
      //DATA_TYPE_QColor,
      //DATA_TYPE_QRect,
      //DATA_TYPE_QTime,
      //
      DATA_TYPE_ANY           = 0xFFFF
    };

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
    // getPropertiesRoot
    // -------------------------------------------------------------------------
    static ibc::property::NodeBase *getPropertiesRoot(const QModelIndex &inIndex)
    {
      return getPropertiesRoot(getNodeBase(inIndex));
    }
    // -------------------------------------------------------------------------
    // getPropertiesRoot
    // -------------------------------------------------------------------------
    static ibc::property::NodeBase *getPropertiesRoot(ibc::property::NodeBase *inNode)
    {
      if (inNode == NULL)
        return NULL;
      ibc::property::NodeBase *propertiesRoot;
      propertiesRoot = inNode->getChildPointerByName(".properties");
      return propertiesRoot;
    }
    // -------------------------------------------------------------------------
    // getNode
    // -------------------------------------------------------------------------
    template <class T> static ibc::property::Node<T> *getNode(const QModelIndex &inIndex)
    {
      return (ibc::property::Node<T> *)inIndex.internalPointer();
    }
    // -------------------------------------------------------------------------
    // getChildValue
    // -------------------------------------------------------------------------
    template <class T> static bool getChildValue(
                ibc::property::NodeBase *inParent,
                const char *inName,
                T *outValue)
    {
      ibc::property::Node<T>  *node;
      node = inParent->getChildPointerAsByName<ibc::property::Node<T>>(inName);
      if (node == NULL)
        return false;
      *outValue = node->getValue();
      return true;
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
    // -------------------------------------------------------------------------
    // getNodeType
    // -------------------------------------------------------------------------
    static DataType getNodeType(ibc::property::NodeBase *inNode)
    {
      if (inNode == NULL)
        return DATA_TYPE_NOT_SPECIFIED;
      //
      ibc::property::NodeBase *child;
      child = inNode->getChildPointerByName(".node_type_qt");
      if (child != NULL)
        if (child->checkType<ibc::property::Node<DataType>>())
        {
          ibc::property::Node<DataType> *dataTypeNode;
          dataTypeNode = (ibc::property::Node<DataType> *)child;
          return dataTypeNode->getValue();
        }
      //
      DataType  dataType = DATA_TYPE_NOT_SPECIFIED;
      if (child->checkType<ibc::property::Node<int>>())
        dataType = DATA_TYPE_int;
      else if (child->checkType<ibc::property::Node<unsigned int>>())
        dataType = DATA_TYPE_uint;
      else if (child->checkType<ibc::property::Node<double>>())
        dataType = DATA_TYPE_double;
      else if (child->checkType<ibc::property::Node<bool>>())
        dataType = DATA_TYPE_bool;
      else if (child->checkType<ibc::property::Node<QString>>())
        dataType = DATA_TYPE_QString;
      //
      std::shared_ptr<ibc::property::NodeBase> node_shared_ptr = inNode->get_shared_ptr();
      if (node_shared_ptr != NULL)
        ibc::property::Node<DataType>::createAsChildOf(
                          node_shared_ptr, dataType, ".node_type_qt");
      return dataType;
    }
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_SUBCONTRACT_INTERFACE_H_