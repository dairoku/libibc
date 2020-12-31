// =============================================================================
//  qlineedit_subcontract.h
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
  \file     ibc/qt/property/qlineedit_subcontract.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/12/27
*/

#ifndef IBC_QT_PROPERTY_QLINEEDIT_SUBCONTRACT_H_
#define IBC_QT_PROPERTY_QLINEEDIT_SUBCONTRACT_H_

// Includes --------------------------------------------------------------------
#include <QStyledItemDelegate>
#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QLineEdit>
#include "ibc/property/node.h"
#include "ibc/qt/property/subcontract_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // QLineEditSubcontract class
  // ---------------------------------------------------------------------------
  class QLineEditSubcontract : public virtual SubcontractInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // QLineEditSubcontract
    // -------------------------------------------------------------------------
    QLineEditSubcontract()
    {
    }
    // -------------------------------------------------------------------------
    // ~QLineEditSubcontract
    // -------------------------------------------------------------------------
    virtual ~QLineEditSubcontract()
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
      QLineEdit *editor = new QLineEdit(parent);
      if (editor == NULL)
        return NULL;
      editor->setFrame(false);
      //
      ibc::property::NodeBase *property;
      property = getPropertiesRoot(index);
      if (property != NULL)
      {
        bool  value_bool;
        int value_int;
        QString value_qstring;
        Qt::Alignment  value_alignment;
        Qt::CursorMoveStyle value_cusormovestyle;
        QLineEdit::EchoMode value_echomode;
        //
        if (getChildValue<Qt::Alignment>(property, "alignment", &value_alignment))
          editor->setAlignment(value_alignment);
        if (getChildValue<bool>(property, "clearButtonEnabled", &value_bool))
          editor->setClearButtonEnabled(value_bool);
        if (getChildValue<t::CursorMoveStyle>(property, "cursorMoveStyle", &value_cusormovestyle))
          editor->setMinimum(value_cusormovestyle);
        if (getChildValue<QString>(property, "dragEnabled", &value_bool))
          editor->setDragEnabled(value_bool);
        if (getChildValue<int>(property, "echoMode", &value_echomode))
          editor->setEchoMode(value_echomode);
        if (getChildValue<QString>(property, "inputMask", &value_qstring))
          editor->setInputMask(value_qstring);
        if (getChildValue<int>(property, "maxLength", &value_int))
          editor->setMaxLength(value_int);
        if (getChildValue<QString>(property, "placeholderText", &value_qstring))
          editor->setPlaceholderText(value_qstring);
      }
      return editor;
    }
    // -------------------------------------------------------------------------
    // setEditorData
    // -------------------------------------------------------------------------
    void setEditorData(QWidget *editor, const QModelIndex &index)
    {
      ibc::property::Node<QString>  *node = getNode<QString>(index);
      if (node == NULL)
        return;
      QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
      if (lineEdit == NULL)
        return;
      lineEdit->setText(node->getValue());
    }
    // -------------------------------------------------------------------------
    // setModelData
    // -------------------------------------------------------------------------
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index)
    {
      UNUSED(model);
      //
      ibc::property::Node<QString>  *node = getNode<QString>(index);
      if (node == NULL)
        return;
      QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
      if (lineEdit == NULL)
        return;
      node->setValue(lineEdit->text());
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
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_QLINEEDIT_SUBCONTRACT_H_
