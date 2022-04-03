// =============================================================================
//  qcheckbox_subcontract.h
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
  \file     ibc/qt/property/qspinbox_subcontract.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/12/27
*/

#ifndef IBC_QT_PROPERTY_QCHECKBOX_SUBCONTRACT_H_
#define IBC_QT_PROPERTY_QCHECKBOX_SUBCONTRACT_H_

// Includes --------------------------------------------------------------------
#include <QStyledItemDelegate>
#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QCheckBox>
#include "ibc/property/node.h"
#include "ibc/qt/property/subcontract_interface.h"


// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // QCheckBoxSubcontract class
  // ---------------------------------------------------------------------------
  class QCheckBoxSubcontract : public virtual SubcontractInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // QCheckBoxSubcontract
    // -------------------------------------------------------------------------
    QCheckBoxSubcontract()
    {
      mIsEditing = false;
    }
    // -------------------------------------------------------------------------
    // ~QCheckBoxSubcontract
    // -------------------------------------------------------------------------
    virtual ~QCheckBoxSubcontract()
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
      QCheckBox *editor = new QCheckBox(parent);
      if (editor == NULL)
        return NULL;
      //
      mIsEditing = true;
      setProperties(editor, index);
      return editor;
    }
    // -------------------------------------------------------------------------
    // setEditorData
    // -------------------------------------------------------------------------
    void setEditorData(QWidget *editor, const QModelIndex &index)
    {
      ibc::property::Node<bool>  *node = getNode<bool>(index);
      if (node == NULL)
        return;
      QCheckBox *checkBox = dynamic_cast<QCheckBox*>(editor);
      if (checkBox == NULL)
        return;
      checkBox->setChecked(node->getValue());
    }
    // -------------------------------------------------------------------------
    // setModelData
    // -------------------------------------------------------------------------
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index)
    {
      UNUSED(model);
      //
      ibc::property::Node<bool>  *node = getNode<bool>(index);
      if (node == NULL)
        return;
      QCheckBox *checkBox = dynamic_cast<QCheckBox*>(editor);
      if (checkBox == NULL)
        return;
      mIsEditing = false;
      node->setValue(checkBox->isChecked());
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
    }
    // -------------------------------------------------------------------------
    // paint
    // -------------------------------------------------------------------------
    bool paint(QPainter *painter, const QStyleOptionViewItem &option,
            const QModelIndex &index)
    {
      if (mIsEditing)
        return false;
      //
      setProperties(&mCheckBox, index);
      setEditorData(&mCheckBox, index);
      //
      mCheckBox.setGeometry(option.rect);
      painter->save();
      {
        QPixmap map = mCheckBox.grab();
        painter->drawPixmap(option.rect.x()+1, option.rect.y(), map);
      }
      painter->restore();
      return true;
    }
    // -------------------------------------------------------------------------
    // getQAbstractButtonParameters
    // -------------------------------------------------------------------------
    static void getQAbstractButtonParameters(QAbstractButton *inEditor,
                              ibc::property::NodeBase *inProperties)
    {
      bool  value_bool;
      int  value_int;
      QIcon value_qicon;
      QSize value_qsize;
      QKeySequence value_qkeysequence;
      QString value_qstring;
      //
      if (getChildValue<bool>(inProperties, "autoExclusive", &value_bool))
        inEditor->setAutoExclusive(value_bool);
      if (getChildValue<bool>(inProperties, "autoRepeat", &value_bool))
        inEditor->setAutoRepeat(value_bool);
      if (getChildValue<int>(inProperties, "autoRepeatDelay", &value_int))
        inEditor->setAutoRepeatDelay(value_int);
      if (getChildValue<int>(inProperties, "autoRepeatInterval", &value_int))
        inEditor->setAutoRepeatInterval(value_int);
      if (getChildValue<bool>(inProperties, "checkable", &value_bool))
        inEditor->setCheckable(value_bool);
      if (getChildValue<QIcon>(inProperties, "icon", &value_qicon))
        inEditor->setIcon(value_qicon);
      if (getChildValue<QSize>(inProperties, "iconSize", &value_qsize))
        inEditor->setIconSize(value_qsize);
      if (getChildValue<QKeySequence>(inProperties, "shortcut", &value_qkeysequence))
        inEditor->setShortcut(value_qkeysequence);
      if (getChildValue<QString>(inProperties, "text", &value_qstring))
        inEditor->setText(value_qstring);
    }

  protected:
    // Member variables --------------------------------------------------------
    QCheckBox     mCheckBox;
    bool  mIsEditing;

    // Member functions (overrides) --------------------------------------------
    // -------------------------------------------------------------------------
    // setProperties
    // -------------------------------------------------------------------------
    void  setProperties(QCheckBox *inCheckBox, const QModelIndex &inIndex)
    {
      ibc::property::NodeBase *properties;
      properties = getPropertiesRoot(inIndex);
      if (properties != NULL)
      {
        getQAbstractButtonParameters(inCheckBox, properties);
        //
        bool  value_bool;
        if (getChildValue<bool>(properties, "tristate", &value_bool))
          inCheckBox->setTristate(value_bool);
      }
    }
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_QCHECKBOX_SUBCONTRACT_H_
