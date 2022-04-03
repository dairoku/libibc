// =============================================================================
//  qdoublespinbox_subcontract.h
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
#include "ibc/qt/property/qspinbox_subcontract.h"

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
      ibc::property::NodeBase *properties;
      properties = getPropertiesRoot(index);
      if (properties != NULL)
      {
        QSpinBoxSubcontract::getQAbstractSpinBoxParameters(editor, properties);
        //
        int value_int;
        double value_double;
        QString value_qstring;
        QAbstractSpinBox::StepType  value_steptype;
        //
        // TODO : add parameters in QAbstractSpinBox
        if (getChildValue<int>(properties, "decimals", &value_int))
          editor->setDecimals(value_int);
        if (getChildValue<double>(properties, "maximum", &value_double))
          editor->setMaximum(value_double);
        if (getChildValue<double>(properties, "minimum", &value_double))
          editor->setMinimum(value_double);
        if (getChildValue<QString>(properties, "prefix", &value_qstring))
          editor->setPrefix(value_qstring);
        if (getChildValue<double>(properties, "singleStep", &value_double))
          editor->setSingleStep(value_double);
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
