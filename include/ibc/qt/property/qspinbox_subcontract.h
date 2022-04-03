// =============================================================================
//  qspinbox_subcontract.h
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

#ifndef IBC_QT_PROPERTY_QSPINBOX_SUBCONTRACT_H_
#define IBC_QT_PROPERTY_QSPINBOX_SUBCONTRACT_H_

// Includes --------------------------------------------------------------------
#include <QStyledItemDelegate>
#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QSpinBox>
#include "ibc/property/node.h"
#include "ibc/qt/property/subcontract_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // QSpinBoxSubcontract class
  // ---------------------------------------------------------------------------
  class QSpinBoxSubcontract : public virtual SubcontractInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // QSpinBoxSubcontract
    // -------------------------------------------------------------------------
    QSpinBoxSubcontract()
    {
    }
    // -------------------------------------------------------------------------
    // ~QSpinBoxSubcontract
    // -------------------------------------------------------------------------
    virtual ~QSpinBoxSubcontract()
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
      QSpinBox *editor = new QSpinBox(parent);
      if (editor == NULL)
        return NULL;
      editor->setFrame(false);
      //
      ibc::property::NodeBase *properties;
      properties = getPropertiesRoot(index);
      if (properties != NULL)
      {
        getQAbstractSpinBoxParameters(editor, properties);
        //
        int value_int;
        QString value_qstring;
        QAbstractSpinBox::StepType  value_steptype;
        //
        if (getChildValue<int>(properties, "displayIntegerBase", &value_int))
          editor->setDisplayIntegerBase(value_int);
        if (getChildValue<int>(properties, "maximum", &value_int))
          editor->setMaximum(value_int);
        if (getChildValue<int>(properties, "minimum", &value_int))
          editor->setMinimum(value_int);
        if (getChildValue<QString>(properties, "prefix", &value_qstring))
          editor->setPrefix(value_qstring);
        if (getChildValue<int>(properties, "singleStep", &value_int))
          editor->setSingleStep(value_int);
        if (getChildValue<QAbstractSpinBox::StepType>(properties, "stepType", &value_steptype))
          editor->setStepType(value_steptype);
        if (getChildValue<QString>(properties, "suffix", &value_qstring))
          editor->setSuffix(value_qstring);
      }
      return editor;
    }
    // -------------------------------------------------------------------------
    // setEditorData
    // -------------------------------------------------------------------------
    void setEditorData(QWidget *editor, const QModelIndex &index)
    {
      ibc::property::Node<int>  *node = getNode<int>(index);
      if (node == NULL)
        return;
      QSpinBox *spinBox = dynamic_cast<QSpinBox*>(editor);
      if (spinBox == NULL)
        return;
      spinBox->setValue(node->getValue());
    }
    // -------------------------------------------------------------------------
    // setModelData
    // -------------------------------------------------------------------------
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index)
    {
      UNUSED(model);
      //
      ibc::property::Node<int>  *node = getNode<int>(index);
      if (node == NULL)
        return;
      QSpinBox *spinBox = dynamic_cast<QSpinBox*>(editor);
      if (spinBox == NULL)
        return;
      spinBox->interpretText();
      node->setValue(spinBox->value());
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
    // -------------------------------------------------------------------------
    // getQAbstractSpinBoxParameters
    // -------------------------------------------------------------------------
    static void getQAbstractSpinBoxParameters(QAbstractSpinBox *inEditor,
                              ibc::property::NodeBase *inProperties)
    {
      bool  value_bool;
      Qt::Alignment value_alignment;
      QAbstractSpinBox::ButtonSymbols value_symbols;
      QAbstractSpinBox::CorrectionMode value_mode;
      QString value_qstring;
      //
      if (getChildValue<bool>(inProperties, "accelerated", &value_bool))
        inEditor->setAccelerated(value_bool);
      if (getChildValue<Qt::Alignment>(inProperties, "alignment", &value_alignment))
        inEditor->setAlignment(value_alignment);
      if (getChildValue<QAbstractSpinBox::ButtonSymbols>(inProperties, "buttonSymbols", &value_symbols))
        inEditor->setButtonSymbols(value_symbols);
      if (getChildValue<QAbstractSpinBox::CorrectionMode>(inProperties, "correctionMode", &value_mode))
        inEditor->setCorrectionMode(value_mode);
      if (getChildValue<bool>(inProperties, "keyboardTracking", &value_bool))
        inEditor->setKeyboardTracking(value_bool);
      if (getChildValue<bool>(inProperties, "showGroupSeparator", &value_bool))
        inEditor->setGroupSeparatorShown(value_bool);
      if (getChildValue<QString>(inProperties, "specialValueText", &value_qstring))
        inEditor->setSpecialValueText(value_qstring);
      if (getChildValue<bool>(inProperties, "wrapping", &value_bool))
        inEditor->setWrapping(value_bool);
    }
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_QSPINBOX_SUBCONTRACT_H_
