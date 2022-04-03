// =============================================================================
//  qlineedit_subcontract.h
//
//  Written in 2020 by Dairoku Sekiguchi (sekiguchi at acm dot org)
//
//  To the extent possible under law, the author(s) have dedicated all copyright
//  and related and neighboring rights to this software to the public domain worldwide.
//  This software is distributed without any warranty.
//
//  You should have received a copy of the CC0 Public Domain Dedication along with
//  this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
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
        if (getChildValue<Qt::CursorMoveStyle>(property, "cursorMoveStyle", &value_cusormovestyle))
          editor->setCursorMoveStyle(value_cusormovestyle);
        if (getChildValue<bool>(property, "dragEnabled", &value_bool))
          editor->setDragEnabled(value_bool);
        if (getChildValue<QLineEdit::EchoMode>(property, "echoMode", &value_echomode))
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
