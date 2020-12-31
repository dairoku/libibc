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
  \file     ibc/qt/property/node_delegate.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/12/27
*/

#ifndef IBC_QT_PROPERTY_NODE_DELEGATE_H_
#define IBC_QT_PROPERTY_NODE_DELEGATE_H_

// Includes --------------------------------------------------------------------
#include <QStyledItemDelegate>
#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QSpinBox>
#include "ibc/property/node.h"
#include "ibc/qt/property/node_utils.h"

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // NodeDelegate class
  // ---------------------------------------------------------------------------
  class NodeDelegate : public QStyledItemDelegate
  {
    Q_OBJECT

  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // NodeDelegate
    // -------------------------------------------------------------------------
    NodeDelegate(QObject *parent=NULL)
      : QStyledItemDelegate(parent)
    {
    }
    // -------------------------------------------------------------------------
    // ~NodeDelegate
    // -------------------------------------------------------------------------
    virtual ~NodeDelegate()
    {
    }

    // Member functions (overrides) --------------------------------------------
    // -------------------------------------------------------------------------
    // createEditor
    // -------------------------------------------------------------------------
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
      ibc::property::NodeBase  *nodeBase;
      nodeBase = SubcontractInterface::getNodeBase(index);
      if (nodeBase == NULL)
        return NULL;

      SubcontractInterface  *subcontract;
      subcontract = NodeUtils::getSubcontract(nodeBase);
      nodeBase->setAuxiliaryDataPointer(subcontract);

      return subcontract->createEditor(parent, option, index);
    }
    // -------------------------------------------------------------------------
    // setEditorData
    // -------------------------------------------------------------------------
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
      SubcontractInterface *subcontract;
      subcontract = SubcontractInterface::getSubcontract(index);
      if (subcontract == NULL)
        return;
      subcontract->setEditorData(editor, index);
    }
    // -------------------------------------------------------------------------
    // setModelData
    // -------------------------------------------------------------------------
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
      SubcontractInterface *subcontract;
      subcontract = SubcontractInterface::getSubcontract(index);
      if (subcontract == NULL)
        return;
      subcontract->setModelData(editor, model, index);
    }
    // -------------------------------------------------------------------------
    // updateEditorGeometry
    // -------------------------------------------------------------------------
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
    {
      SubcontractInterface *subcontract;
      subcontract = SubcontractInterface::getSubcontract(index);
      if (subcontract == NULL)
        return;
      subcontract->updateEditorGeometry(editor, option, index);
    }
    // -------------------------------------------------------------------------
    // -------------------------------------------------------------------------
    // paint
    // -------------------------------------------------------------------------
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
            const QModelIndex &index) const
    {
      QStyleOptionViewItemV3 opt = option;
      QStyledItemDelegate::paint(painter, opt, index);
      //
      opt.palette.setCurrentColorGroup(QPalette::Active);
      QColor color = static_cast<QRgb>(QApplication::style()->styleHint(
                                        QStyle::SH_Table_GridLineColor, &opt));
      painter->save();
      painter->setPen(QPen(color));
      if (index.column() == 1)
      {
        // This is to draw a vertical line between columns
        int left = (option.direction == Qt::LeftToRight) ? option.rect.left() : option.rect.right();
        painter->drawLine(left, option.rect.y(), left, option.rect.bottom());
      }
      painter->restore();
    }
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_NODE_DELEGATE_H_
