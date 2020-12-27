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
#include <QSpinBox>

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
      QSpinBox *editor = new QSpinBox(parent);
      editor->setFrame(false);
      editor->setMinimum(0);
      editor->setMaximum(100);
      return editor;
    }
    // -------------------------------------------------------------------------
    // setEditorData
    // -------------------------------------------------------------------------
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
      int value = index.model()->data(index, Qt::EditRole).toInt();
      QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
      spinBox->setValue(value);
    }
    // -------------------------------------------------------------------------
    // setModelData
    // -------------------------------------------------------------------------
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
      QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
      spinBox->interpretText();
      int value = spinBox->value();
      model->setData(index, value, Qt::EditRole);
    }
    // -------------------------------------------------------------------------
    // updateEditorGeometry
    // -------------------------------------------------------------------------
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
    {
      editor->setGeometry(option.rect);
    }

  protected:
    // Member variables --------------------------------------------------------
    //const GLubyte *mRendererStr;
  };
};

#endif  // #ifdef IBC_QT_PROPERTY_NODE_DELEGATE_H_
