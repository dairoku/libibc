// =============================================================================
//  qdoublespinbox_subcontract.h
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
  \file     ibc/qt/property/qdoublespinbox_subcontract.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/12/27
*/

#ifndef IBC_QT_PROPERTY_QDOUBLESPINBOX_SUBCONTRACT_H_
#define IBC_QT_PROPERTY_QDOUBLESPINBOX_SUBCONTRACT_H_

// Includes --------------------------------------------------------------------
#include <QStyledItemDelegate>
#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QDoubleSpinBox>
#include "ibc/property/node.h"
#include "ibc/qt/property/subcontract_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // QDoubleSpinBoxSubcontract class
  // ---------------------------------------------------------------------------
  class QDoubleSpinBoxSubcontract : public virtual SubcontractInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // QDoubleSpinBoxSubcontract
    // -------------------------------------------------------------------------
    QDoubleSpinBoxSubcontract()
    {
    }
    // -------------------------------------------------------------------------
    // ~QDoubleSpinBoxSubcontract
    // -------------------------------------------------------------------------
    virtual ~QDoubleSpinBoxSubcontract()
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
      QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
      if (editor == NULL)
        return NULL;
      editor->setFrame(false);
      //
      ibc::property::NodeBase *property;
      property = getPropertiesRoot(index);
      if (property != NULL)
      {
        int value_int;
        double value_double;
        QString value_qstring;
        QAbstractSpinBox::StepType  value_steptype;
        //
        // TODO : add parameters in QAbstractSpinBox
        if (getChildValue<int>(property, "decimals", &value_int))
          editor->setDecimals(value_int);
        if (getChildValue<double>(property, "maximum", &value_double))
          editor->setMaximum(value_double);
        if (getChildValue<double>(property, "minimum", &value_double))
          editor->setMinimum(value_double);
        if (getChildValue<QString>(property, "prefix", &value_qstring))
          editor->setPrefix(value_qstring);
        if (getChildValue<double>(property, "singleStep", &value_double))
          editor->setSingleStep(value_double);
        if (getChildValue<QAbstractSpinBox::StepType>(property, "stepType", &value_steptype))
          editor->setStepType(value_steptype);
        if (getChildValue<QString>(property, "suffix", &value_qstring))
          editor->setSuffix(value_qstring);
      }
      return editor;
    }
    // -------------------------------------------------------------------------
    // setEditorData
    // -------------------------------------------------------------------------
    void setEditorData(QWidget *editor, const QModelIndex &index)
    {
      ibc::property::Node<double>  *node = getNode<double>(index);
      if (node == NULL)
        return;
      QDoubleSpinBox *spinBox = dynamic_cast<QDoubleSpinBox*>(editor);
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
      ibc::property::Node<double>  *node = getNode<double>(index);
      if (node == NULL)
        return;
      QDoubleSpinBox *spinBox = dynamic_cast<QDoubleSpinBox*>(editor);
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
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_QDOUBLESPINBOX_SUBCONTRACT_H_
