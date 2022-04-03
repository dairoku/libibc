// =============================================================================
//  qcombobox_subcontract.h
//
//  Written in 2021 by Dairoku Sekiguchi (sekiguchi at acm dot org)
//
//  To the extent possible under law, the author(s) have dedicated all copyright
//  and related and neighboring rights to this software to the public domain worldwide.
//  This software is distributed without any warranty.
//
//  You should have received a copy of the CC0 Public Domain Dedication along with
//  this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
// =============================================================================
/*!
  \file     ibc/qt/property/qcombobox_subcontract.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2021/01/01
*/

#ifndef IBC_QT_PROPERTY_QCOMBOBOX_SUBCONTRACT_H_
#define IBC_QT_PROPERTY_QCOMBOBOX_SUBCONTRACT_H_

// Includes --------------------------------------------------------------------
#include <QStyledItemDelegate>
#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QComboBox>
#include <QStringList>
#include "ibc/property/node.h"
#include "ibc/qt/property/subcontract_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // QComboBoxSubcontract class
  // ---------------------------------------------------------------------------
  class QComboBoxSubcontract : public virtual SubcontractInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // QComboBoxSubcontract
    // -------------------------------------------------------------------------
    QComboBoxSubcontract()
    {
    }
    // -------------------------------------------------------------------------
    // ~QComboBoxSubcontract
    // -------------------------------------------------------------------------
    virtual ~QComboBoxSubcontract()
    {
    }

    // Member functions (overrides) --------------------------------------------
    // -------------------------------------------------------------------------
    // createEditor
    // -------------------------------------------------------------------------
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index)
    {
      UNUSED(option);
      //
      QComboBox *editor = new QComboBox(parent);
      if (editor == NULL)
        return NULL;
      editor->setFrame(false);
      //
      ibc::property::NodeBase *properties;
      properties = getPropertiesRoot(index);
      if (properties != NULL)
      {
        bool  value_bool;
        int value_int;
        QSize value_qsize;
        QString value_qstring;
        QComboBox::InsertPolicy  value_insertpolicy;
        QComboBox::SizeAdjustPolicy  value_sizepolicy;
        QStringList  items;
        QList<int>  itemValues;
        //
        if (getChildValue<QStringList>(properties, "items", &items))
        {
          for (int i = 0; i < items.size(); i++)
            editor->addItem(items[i]);
          if (getChildValue<QList<int>>(properties, "itemValues", &itemValues))
          {
            for (int i = 0; i < itemValues.size(); i++)
              editor->setItemData(i, QVariant(itemValues[i]));
          }
        }
        if (getChildValue<bool>(properties, "duplicatesEnabled", &value_bool))
          editor->setDuplicatesEnabled(value_bool);
        if (getChildValue<bool>(properties, "editable", &value_bool))
          editor->setEditable(value_bool);
        if (getChildValue<QSize>(properties, "iconSize", &value_qsize))
          editor->setIconSize(value_qsize);
        if (getChildValue<QComboBox::InsertPolicy>(properties, "insertPolicy", &value_insertpolicy))
          editor->setInsertPolicy(value_insertpolicy);
        if (getChildValue<int>(properties, "maxCount", &value_int))
          editor->setMaxCount(value_int);
        if (getChildValue<int>(properties, "maxVisibleItems", &value_int))
          editor->setMaxVisibleItems(value_int);
        if (getChildValue<int>(properties, "minimumContentsLength", &value_int))
          editor->setMinimumContentsLength(value_int);
        if (getChildValue<int>(properties, "modelColumn", &value_int))
          editor->setModelColumn(value_int);
// only if QT > 5.15
//        if (getChildValue<QString>(properties, "placeholderText", &value_qstring))
//          editor->setPlaceholderText(value_qstring);
        if (getChildValue<QComboBox::SizeAdjustPolicy>(properties, "sizeAdjustPolicy", &value_sizepolicy))
          editor->setSizeAdjustPolicy(value_sizepolicy);
      }
      return editor;
    }
    // -------------------------------------------------------------------------
    // setEditorData
    // -------------------------------------------------------------------------
    void setEditorData(QWidget *editor, const QModelIndex &index)
    {
      QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
      if (comboBox == NULL)
        return;
      //
      DataType  dataType = getNodeDataType(index);
      if (dataType == DATA_TYPE_int)
      {
        ibc::property::Node<int>  *node = getNode<int>(index);
        if (node == NULL)
          return;
        comboBox->setCurrentIndex(node->getValue());
        return;
      }
      if (dataType == DATA_TYPE_QString)
      {
        ibc::property::Node<QString>  *node = getNode<QString>(index);
        if (node == NULL)
          return;
        int index = comboBox->findText(node->getValue()); // TODO : add flags?
        if (index < 0)
          return;
        comboBox->setCurrentIndex(index);
        return;
      }
    }
    // -------------------------------------------------------------------------
    // setModelData
    // -------------------------------------------------------------------------
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index)
    {
      UNUSED(model);
      //
      QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
      if (comboBox == NULL)
        return;
      DataType  dataType = getNodeDataType(index);
      if (dataType == DATA_TYPE_int)
      {
        ibc::property::Node<int>  *node = getNode<int>(index);
        if (node == NULL)
          return;
        node->setValue(comboBox->currentIndex());
      }
      if (dataType == DATA_TYPE_QString)
      {
        ibc::property::Node<QString>  *node = getNode<QString>(index);
        if (node == NULL)
          return;
        node->setValue(comboBox->currentText());
      }
    }
    // -------------------------------------------------------------------------
    // updateEditorGeometry
    // -------------------------------------------------------------------------
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index)
    {
      UNUSED(index);
      //
      editor->setGeometry(option.rect);
      //editor->setGeometry(option.rect.adjusted(0, 0, 0, -1));
    }
    // for Model
    // -------------------------------------------------------------------------
    // getModelData
    // -------------------------------------------------------------------------
    virtual QVariant getModelData(const QModelIndex &index)
    {
      ibc::property::NodeBase *nodeBase = getNodeBase(index);
      if (index.column() == 0) // Column is 0
        return QVariant(nodeBase->getName().c_str());
      //
      if (nodeBase->hasValue() == false)  // Has no value
        return QVariant();
      //
      DataType  dataType = getNodeDataType(index);
      if (dataType == DATA_TYPE_int)
      {
        ibc::property::Node<int>  *node = getNode<int>(index);
        if (node == NULL)
          return QVariant();
        ibc::property::NodeBase *properties = getPropertiesRoot(node);
        if (properties == NULL)
          return QVariant();
        int index = node->getValue();
        QStringList  items;
        if (getChildValue<QStringList>(properties, "items", &items) == false)
          return QVariant();
        if (index >= items.size() || index < 0)
          return QVariant();
        //
        return QVariant(items[index]);
      }
      if (dataType == DATA_TYPE_QString)
      {
        ibc::property::Node<QString>  *node = getNode<QString>(index);
        if (node == NULL)
          return QVariant();
        return QVariant(node->getValue());
      }
      return QVariant();
    }
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_QCOMBOBOX_SUBCONTRACT_H_
