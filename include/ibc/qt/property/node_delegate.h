// =============================================================================
//  node_delegate.h
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
      SubcontractInterface  *subcontract;
      subcontract = NodeUtils::prepareSubcontract(index);
      if (subcontract == NULL)
        return NULL;
      //
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
      SubcontractInterface *subcontract;
      subcontract = SubcontractInterface::getSubcontract(index);
      bool  skipOriginalPaint = false;
      if (subcontract != NULL)
        skipOriginalPaint = subcontract->paint(painter, option, index);
      if (skipOriginalPaint == false)
        QStyledItemDelegate::paint(painter, option, index);
      //
      QStyleOptionViewItem opt = option;
      opt.palette.setCurrentColorGroup(QPalette::Active);
      QColor color = static_cast<QRgb>(QApplication::style()->styleHint(
                                        QStyle::SH_Table_GridLineColor, &opt));
      painter->save();
      painter->setPen(QPen(color));
      if (index.column() == 1)
      {
        // This is to draw a vertical line between columns
        int left = (opt.direction == Qt::LeftToRight) ? opt.rect.left() : opt.rect.right();
        painter->drawLine(left, opt.rect.y(), left, opt.rect.bottom());
      }
      painter->restore();
    }
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_NODE_DELEGATE_H_
