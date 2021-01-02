// =============================================================================
//  node_utils.h
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
  \file     ibc/qt/property/node_utils.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2020/12/27
*/

#ifndef IBC_QT_PROPERTY_NODE_UTILS_H_
#define IBC_QT_PROPERTY_NODE_UTILS_H_

// Includes --------------------------------------------------------------------
#include <QStyledItemDelegate>
#include <QString>
#include <QStringList>
#include <climits>
#include "ibc/property/node.h"
#include "ibc/qt/property/subcontract_interface.h"
#include "ibc/qt/property/qspinbox_subcontract.h"
#include "ibc/qt/property/qdoublespinbox_subcontract.h"
#include "ibc/qt/property/qlineedit_subcontract.h"
#include "ibc/qt/property/qcombobox_subcontract.h"

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // SubcontractInterface class
  // ---------------------------------------------------------------------------
  class NodeUtils
  {
  public:
    // Enum --------------------------------------------------------------------
    enum  WidgetType
    {
      WIDGET_TYPE_NOT_SPECIFIED = 0,
      //
      WIDGET_TYPE_QSpinBox    = 1,
      WIDGET_TYPE_QDoubleSpinBox,
      WIDGET_TYPE_QLineEdit,
      WIDGET_TYPE_QComboBox,
      //
      WIDGET_TYPE_ANY           = 0xFFFF
    };

    // Static Functions --------------------------------------------------------
    // QAbstractSpinBox ----------------------------------------------------------------
    // -------------------------------------------------------------------------
    // prepareQAbstractSpinBoxExtraParams
    // -------------------------------------------------------------------------
    static bool prepareQAbstractSpinBoxExtraParams(
        std::shared_ptr<ibc::property::NoValueNode> inPropertiesRoot,
        QAbstractSpinBox::ButtonSymbols inSymbols = QAbstractSpinBox::UpDownArrows,
        bool inSeparator = false,
        bool inAccelerated = false,
        bool inWrapping = false,
        const char inValueText = NULL,
        QAbstractSpinBox::CorrectionMode inMode = QAbstractSpinBox::CorrectToPreviousValue,
        Qt::Alignment inAlignment = Qt::AlignLeft,
        bool inTracking = true)
    {
      if (inAccelerated != false)
        preparePropertyNode<bool>(inPropertiesRoot, "accelerated", inAccelerated);
      if (inAlignment != Qt::AlignLeft)
        preparePropertyNode<Qt::Alignment>(inPropertiesRoot, "alignment", inAlignment);
      if (inSymbols != QAbstractSpinBox::UpDownArrows)
        preparePropertyNode<QAbstractSpinBox::ButtonSymbols>(
            inPropertiesRoot, "buttonSymbols", inSymbols);
      if (inMode != QAbstractSpinBox::CorrectToPreviousValue)
        preparePropertyNode<QAbstractSpinBox::CorrectionMode>(
            inPropertiesRoot, "correctionMode", inMode);
      if (inTracking != true)
        preparePropertyNode<bool>(inPropertiesRoot, "keyboardTracking", inTracking);
      if (inSeparator != false)
        preparePropertyNode<bool>(inPropertiesRoot, "showGroupSeparator", inSeparator);
      if (inValueText != NULL)
        preparePropertyNode<QString>(
            inPropertiesRoot, "specialValueText", QString(inValueText));
      if (inWrapping != false)
        preparePropertyNode<bool>(inPropertiesRoot, "wrapping", inWrapping);
      return true;
    }

    // QSpinBox ----------------------------------------------------------------
    // -------------------------------------------------------------------------
    // addQSpinBoxNode
    // -------------------------------------------------------------------------
    static std::shared_ptr<ibc::property::NoValueNode> addQSpinBoxNode(
                        std::shared_ptr<ibc::property::NodeBase> inParent,
                        const char *inName,
                        int inValue,
                        int inMin, int inMax)
    {
      std::shared_ptr<ibc::property::Node<int>> node;
      node = ibc::property::Node<int>::createAsChildOf(
                              inParent, inName, inValue);
      if (node == NULL)
        return NULL;
      return prepareQSpinBoxParams(node, inMin, inMax);
    }
    // -------------------------------------------------------------------------
    // addQSpinBoxNodeWithAllParams
    // -------------------------------------------------------------------------
    static bool addQSpinBoxNodeWithAllParams(
        std::shared_ptr<ibc::property::NodeBase> inParent,
        const char *inName,
        int inValue,
        int inMin, int inMax,
        int inSingleStep, QAbstractSpinBox::StepType inStepType,
        int inDisplayIntegerBase = -1,
        const char *inPrefix = NULL, const char *inSuffix = NULL,
        QAbstractSpinBox::ButtonSymbols inSymbols = QAbstractSpinBox::UpDownArrows,
        bool inSeparator = false,
        bool inAccelerated = false,
        bool inWrapping = false,
        const char inValueText = NULL,
        QAbstractSpinBox::CorrectionMode inMode = QAbstractSpinBox::CorrectToPreviousValue,
        Qt::Alignment inAlignment = Qt::AlignLeft,
        bool inTracking = true)
    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = addQSpinBoxNode(inParent, inName, inValue, inMin, inMax);
      if (propertiesRoot == NULL)
        return false;
      if (prepareQSpinBoxExtraParams(propertiesRoot,
                    inSingleStep, inStepType,
                    inDisplayIntegerBase,
                    inPrefix, inSuffix) == false)
        return false;
      return prepareQAbstractSpinBoxExtraParams(propertiesRoot,
                    inSymbols,
                    inSeparator,
                    inAccelerated,
                    inWrapping,
                    inValueText,
                    inMode,
                    inAlignment,
                    inTracking);
    }
    // -------------------------------------------------------------------------
    // prepareQSpinBoxParams
    // -------------------------------------------------------------------------
    static std::shared_ptr<ibc::property::NoValueNode> prepareQSpinBoxParams(
                              std::shared_ptr<ibc::property::Node<int>> inNode,
                              int inMin, int inMax)
    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = preparePropertiesRoot(inNode);
      if (propertiesRoot == NULL)
        return NULL;
      //
      preparePropertyNode<QString>(propertiesRoot, "widget", QString("QSpinBox"));
      preparePropertyNode<int>(propertiesRoot, "minimum", inMin);
      preparePropertyNode<int>(propertiesRoot, "maximum", inMax);
      return propertiesRoot;
    }
    // -------------------------------------------------------------------------
    // prepareQSpinBoxExtraParams
    // -------------------------------------------------------------------------
    static bool prepareQSpinBoxExtraParams(
                std::shared_ptr<ibc::property::NoValueNode> inPropertiesRoot,
                int inSingleStep, QAbstractSpinBox::StepType inStepType,
                int inDisplayIntegerBase = -1,
                const char *inPrefix = NULL, const char *inSuffix = NULL)
    {
      preparePropertyNode<int>(inPropertiesRoot, "singleStep", inSingleStep);
      preparePropertyNode<QAbstractSpinBox::StepType>(
                                inPropertiesRoot, "stepType", inStepType);
      if (inDisplayIntegerBase > 0)
        preparePropertyNode<int>(
                    inPropertiesRoot, "displayIntegerBase", inDisplayIntegerBase);
      if (inPrefix != NULL)
        preparePropertyNode<QString>(
                    inPropertiesRoot, "prefix", QString(inPrefix));
      if (inSuffix != NULL)
        preparePropertyNode<QString>(
                    inPropertiesRoot, "suffix", QString(inSuffix));
      return true;
    }

    // QDoubleSpinBox ----------------------------------------------------------
    // -------------------------------------------------------------------------
    // addQDoubleSpinBoxNode
    // -------------------------------------------------------------------------
    static std::shared_ptr<ibc::property::NoValueNode> addQDoubleSpinBoxNode(
                        std::shared_ptr<ibc::property::NodeBase> inParent,
                        const char *inName,
                        double inValue,
                        double inMin, double inMax)
    {
      std::shared_ptr<ibc::property::Node<double>> node;
      node = ibc::property::Node<double>::createAsChildOf(
                              inParent, inName, inValue);
      if (node == NULL)
        return NULL;
      return prepareQDoubleSpinBoxParams(node, inMin, inMax);
    }
    // -------------------------------------------------------------------------
    // addQDoubleSpinBoxNodeWithAllParams
    // -------------------------------------------------------------------------
    static bool addQDoubleSpinBoxNodeWithAllParams(
        std::shared_ptr<ibc::property::NodeBase> inParent,
        const char *inName,
        double inValue,
        double inMin, double inMax,
        double inSingleStep, QAbstractSpinBox::StepType inStepType,
        int inDecimals = 2,
        const char *inPrefix = NULL, const char *inSuffix = NULL,
        QAbstractSpinBox::ButtonSymbols inSymbols = QAbstractSpinBox::UpDownArrows,
        bool inSeparator = false,
        bool inAccelerated = false,
        bool inWrapping = false,
        const char inValueText = NULL,
        QAbstractSpinBox::CorrectionMode inMode = QAbstractSpinBox::CorrectToPreviousValue,
        Qt::Alignment inAlignment = Qt::AlignLeft,
        bool inTracking = true)

    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = addQDoubleSpinBoxNode(inParent, inName, inValue, inMin, inMax);
      if (propertiesRoot == NULL)
        return false;
      if (prepareQDoubleSpinBoxExtraParams(propertiesRoot,
                    inSingleStep, inStepType,
                    inDecimals,
                    inPrefix, inSuffix) == false)
        return false;
      return prepareQAbstractSpinBoxExtraParams(propertiesRoot,
                    inSymbols,
                    inSeparator,
                    inAccelerated,
                    inWrapping,
                    inValueText,
                    inMode,
                    inAlignment,
                    inTracking);
    }
    // -------------------------------------------------------------------------
    // prepareQDoubleSpinBoxParams
    // -------------------------------------------------------------------------
    static std::shared_ptr<ibc::property::NoValueNode> prepareQDoubleSpinBoxParams(
                              std::shared_ptr<ibc::property::Node<double>> inNode,
                              double inMin, double inMax)
    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = preparePropertiesRoot(inNode);
      if (propertiesRoot == NULL)
        return NULL;
      //
      preparePropertyNode<QString>(propertiesRoot, "widget", QString("QDoubleSpinBox"));
      preparePropertyNode<double>(propertiesRoot, "minimum", inMin);
      preparePropertyNode<double>(propertiesRoot, "maximum", inMax);
      return propertiesRoot;
    }
    // -------------------------------------------------------------------------
    // prepareQDoubleSpinBoxExtraParams
    // -------------------------------------------------------------------------
    static bool prepareQDoubleSpinBoxExtraParams(
                std::shared_ptr<ibc::property::NoValueNode> inPropertiesRoot,
                double inSingleStep, QAbstractSpinBox::StepType inStepType,
                int inDecimals = 2,
                const char *inPrefix = NULL, const char *inSuffix = NULL)
    {
      preparePropertyNode<double>(inPropertiesRoot, "singleStep", inSingleStep);
      preparePropertyNode<QAbstractSpinBox::StepType>(
                                inPropertiesRoot, "stepType", inStepType);
      if (inDecimals != 2)
        preparePropertyNode<int>(
                    inPropertiesRoot, "decimals", inDecimals);
      if (inPrefix != NULL)
        preparePropertyNode<QString>(
                    inPropertiesRoot, "prefix", QString(inPrefix));
      if (inSuffix != NULL)
        preparePropertyNode<QString>(
                    inPropertiesRoot, "suffix", QString(inSuffix));
      return true;
    }

    // QLineEdit ----------------------------------------------------------------
    // -------------------------------------------------------------------------
    // addQLineEditNode
    // -------------------------------------------------------------------------
    static std::shared_ptr<ibc::property::NoValueNode> addQLineEditNode(
                        std::shared_ptr<ibc::property::NodeBase> inParent,
                        const char *inName,
                        QString inValue,
                        int inMaxLength = 32767)
    {
      std::shared_ptr<ibc::property::Node<QString>> node;
      node = ibc::property::Node<QString>::createAsChildOf(
                              inParent, inName, inValue);
      if (node == NULL)
        return NULL;
      return prepareQLineEditParams(node, inMaxLength);
    }
    // -------------------------------------------------------------------------
    // addQLineEditNodeWithAllParams
    // -------------------------------------------------------------------------
    static bool addQLineEditNodeWithAllParams(
        std::shared_ptr<ibc::property::NodeBase> inParent,
        const char *inName,
        QString &inValue,
        int inMaxLength = 32767,
        QLineEdit::EchoMode inEchoMode = QLineEdit::Normal,
        const char  inMaskStr = NULL,
        const char  inPlaceholderStr = NULL,
        bool  inClearButton = false,
        bool  inDragEnabled = false,
        Qt::Alignment inAlignment = Qt::AlignLeft | Qt::AlignVCenter,
        Qt::CursorMoveStyle inStyle = Qt::LogicalMoveStyle)
    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = addQLineEditNode(inParent, inName, inValue, inMaxLength);
      if (propertiesRoot == NULL)
        return false;
      return prepareQLineEditExtraParams(propertiesRoot,
                    inEchoMode,
                    inMaskStr, inPlaceholderStr,
                    inClearButton, inDragEnabled,
                    inAlignment, inStyle);
    }
    // -------------------------------------------------------------------------
    // prepareQLineEditParams
    // -------------------------------------------------------------------------
    static std::shared_ptr<ibc::property::NoValueNode> prepareQLineEditParams(
          std::shared_ptr<ibc::property::Node<QString>> inNode,
          int inMaxLength = 32767)
    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = preparePropertiesRoot(inNode);
      if (propertiesRoot == NULL)
        return NULL;
      //
      preparePropertyNode<QString>(propertiesRoot, "widget", QString("QLineEdit"));
      if (inMaxLength != 32767)
        preparePropertyNode<int>(propertiesRoot, "maxLength", inMaxLength);
      return propertiesRoot;
    }
    // -------------------------------------------------------------------------
    // prepareQLineEditExtraParams
    // -------------------------------------------------------------------------
    static bool prepareQLineEditExtraParams(
          std::shared_ptr<ibc::property::NoValueNode> inPropertiesRoot,
          QLineEdit::EchoMode inEchoMode = QLineEdit::Normal,
          const char  inMaskStr = NULL,
          const char  inPlaceholderStr = NULL,
          bool  inClearButton = false,
          bool  inDragEnabled = false,
          Qt::Alignment inAlignment = Qt::AlignLeft | Qt::AlignVCenter,
          Qt::CursorMoveStyle inStyle = Qt::LogicalMoveStyle)
    {
      if (inEchoMode != QLineEdit::Normal)
        preparePropertyNode<QLineEdit::EchoMode>(
                    inPropertiesRoot, "echoMode", inEchoMode);
      if (inMaskStr != NULL)
        preparePropertyNode<QString>(
                    inPropertiesRoot, "inputMask", QString(inMaskStr));
      if (inPlaceholderStr != NULL)
        preparePropertyNode<QString>(
                    inPropertiesRoot, "placeholderText", QString(inPlaceholderStr));
      if (inClearButton != false)
        preparePropertyNode<bool>(
                    inPropertiesRoot, "clearButtonEnabled", inClearButton);
      if (inDragEnabled != false)
        preparePropertyNode<bool>(
                    inPropertiesRoot, "dragEnabled", inDragEnabled);
      if (inAlignment != (Qt::AlignLeft | Qt::AlignVCenter))
        preparePropertyNode<int>(inPropertiesRoot, "alignment", inAlignment);
      if (inStyle != Qt::LogicalMoveStyle)
        preparePropertyNode<Qt::CursorMoveStyle>(
                      inPropertiesRoot, "echoMode", inStyle);
      return true;
    }

    // QComboBox ----------------------------------------------------------------
    // -------------------------------------------------------------------------
    // addQComboBoxNode
    // -------------------------------------------------------------------------
    static std::shared_ptr<ibc::property::NoValueNode> addQComboBoxNode(
        std::shared_ptr<ibc::property::NodeBase> inParent,
        const char *inName,
        int inValue,
        int  inItemNum,
        const char *inItems[],
        const int  inItemValues[] = NULL)
    {
      std::shared_ptr<ibc::property::Node<int>> node;
      node = ibc::property::Node<int>::createAsChildOf(
                              inParent, inName, inValue);
      if (node == NULL)
        return NULL;
      return prepareQComboBoxParams(node, inItemNum, inItems, inItemValues);
    }
    // -------------------------------------------------------------------------
    // addQComboBoxNodeWithAllParams
    // -------------------------------------------------------------------------
    static bool addQComboBoxNodeWithAllParams(
        std::shared_ptr<ibc::property::NodeBase> inParent,
        const char *inName,
        int inValue,
        int  inItemNum,
        const char *inItems[],
        const int  inItemValues[] = NULL,
        bool  inEditable = false,
        bool  inDuplicatesEnabled = false,
        QComboBox::InsertPolicy inIsertPolicy = QComboBox::InsertAtBottom,
        int inMaxVisibleItems = 10,
        int inMinimumContentsLength = 0,
        int inMaxCount = INT_MAX,
        int inModelColumn = 0,
        const char *inPlaceholderText = NULL,
        QComboBox::SizeAdjustPolicy inSizepolicy = QComboBox::AdjustToContentsOnFirstShow,
        QSize *inIconSize = NULL)
    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = addQComboBoxNode(
                    inParent, inName, inValue, inItemNum, inItems, inItemValues);
      if (propertiesRoot == NULL)
        return false;
      return prepareQComboBoxExtraParams(propertiesRoot,
                    inEditable,
                    inDuplicatesEnabled,
                    inIsertPolicy,
                    inMaxVisibleItems,
                    inMinimumContentsLength,
                    inMaxCount,
                    inModelColumn,
                    inPlaceholderText,
                    inSizepolicy,
                    inIconSize);
    }
    // -------------------------------------------------------------------------
    // prepareQComboBoxParams
    // -------------------------------------------------------------------------
    static std::shared_ptr<ibc::property::NoValueNode> prepareQComboBoxParams(
                        std::shared_ptr<ibc::property::Node<int>> inNode,
                        int  inItemNum,
                        const char *inItems[],
                        const int  inItemValues[] = NULL)
    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = preparePropertiesRoot(inNode);
      if (propertiesRoot == NULL)
        return NULL;
      //
      preparePropertyNode<QString>(propertiesRoot, "widget", QString("QComboBox"));
      //preparePropertyNode<int>(propertiesRoot, "itemNum", inItemNum);
      QStringList items;
      for (int i = 0; i < inItemNum; i++)
        items.append(inItems[i]);
      preparePropertyNode<QStringList>(propertiesRoot, "items", items);
      if (inItemValues == NULL)
        return propertiesRoot;
      //
      QList<int>  itemValues;
      for (int i = 0; i < inItemNum; i++)
        itemValues.append(inItemValues[i]);
      preparePropertyNode<QList<int>>(propertiesRoot, "itemValues", itemValues);
      return propertiesRoot;
    }
    // -------------------------------------------------------------------------
    // prepareQComboBoxExtraParams
    // -------------------------------------------------------------------------
    static bool prepareQComboBoxExtraParams(
        std::shared_ptr<ibc::property::NoValueNode> inPropertiesRoot,
        bool  inEditable = false,
        bool  inDuplicatesEnabled = false,
        QComboBox::InsertPolicy inIsertPolicy = QComboBox::InsertAtBottom,
        int inMaxVisibleItems = 10,
        int inMinimumContentsLength = 0,
        int inMaxCount = INT_MAX,
        int inModelColumn = 0,
        const char *inPlaceholderText = NULL,
        QComboBox::SizeAdjustPolicy inSizepolicy = QComboBox::AdjustToContentsOnFirstShow,
        QSize *inIconSize = NULL)
    {
      if (inEditable != false)
        preparePropertyNode<bool>(
                    inPropertiesRoot, "editable", inEditable);
      if (inEditable != false)
        preparePropertyNode<bool>(
                    inPropertiesRoot, "duplicatesEnabled", inDuplicatesEnabled);
      if (inIsertPolicy != QComboBox::InsertAtBottom)
        preparePropertyNode<QComboBox::InsertPolicy>(
                    inPropertiesRoot, "insertPolicy", inIsertPolicy);
      if (inMaxVisibleItems != 10)
        preparePropertyNode<int>(
                    inPropertiesRoot, "maxVisibleItems", inMaxVisibleItems);
      if (inMinimumContentsLength != 0)
        preparePropertyNode<int>(
                    inPropertiesRoot, "minimumContentsLength", inMinimumContentsLength);
      if (inMaxCount != INT_MAX)
        preparePropertyNode<int>(
                    inPropertiesRoot, "maxCount", inMaxCount);
      if (inModelColumn != 0)
        preparePropertyNode<int>(
                    inPropertiesRoot, "modelColumn", inModelColumn);
      if (inPlaceholderText != NULL)
        preparePropertyNode<QString>(
                    inPropertiesRoot, "placeholderText", QString(inPlaceholderText));
      if (inSizepolicy != QComboBox::AdjustToContentsOnFirstShow)
        preparePropertyNode<QComboBox::SizeAdjustPolicy>(
                    inPropertiesRoot, "sizeAdjustPolicy", inSizepolicy);
      if (inIconSize != NULL)
        preparePropertyNode<QSize>(
                    inPropertiesRoot, "iconSize", *inIconSize);
      return true;
    }

    //
    // -------------------------------------------------------------------------
    // -------------------------------------------------------------------------
    // prepareSubcontract
    // -------------------------------------------------------------------------
    static SubcontractInterface *prepareSubcontract(const QModelIndex &inIndex)
    {
      ibc::property::NodeBase  *nodeBase;
      nodeBase = SubcontractInterface::getNodeBase(inIndex);
      if (nodeBase == NULL)
        return NULL;

      SubcontractInterface  *subcontract;
      subcontract = (SubcontractInterface  *)nodeBase->getAuxiliaryDataPointer();
      if (subcontract != NULL)
        return subcontract;

      subcontract = selectSubcontract(nodeBase);
      if (subcontract == NULL)
        return NULL;
      nodeBase->setAuxiliaryDataPointer(subcontract);
      return subcontract;
    }
    // -------------------------------------------------------------------------
    // selectSubcontract
    // -------------------------------------------------------------------------
    static SubcontractInterface *selectSubcontract(ibc::property::NodeBase *inNode)
    {
      return selectSubcontract(getWidgetTypeFromNode(inNode));
    }
    // -------------------------------------------------------------------------
    // selectSubcontract (**)
    // -------------------------------------------------------------------------
    static SubcontractInterface *selectSubcontract(WidgetType inType)
    {
      static QSpinBoxSubcontract        sQSpinBoxSubcontract;
      static QDoubleSpinBoxSubcontract  sQDobuleSpinBoxSubcontract;
      static QLineEditSubcontract       sQLineEditSubcontract;
      static QComboBoxSubcontract       sQComboBoxSubcontract;
      //
      switch(inType)
      {
        case WIDGET_TYPE_QSpinBox:
          return &sQSpinBoxSubcontract;
        case WIDGET_TYPE_QDoubleSpinBox:
          return &sQDobuleSpinBoxSubcontract;
        case WIDGET_TYPE_QLineEdit:
          return &sQLineEditSubcontract;
        case WIDGET_TYPE_QComboBox:
          return &sQComboBoxSubcontract;
      }
      return NULL;
    }
    // -------------------------------------------------------------------------
    // getWidgetTypeFromNode
    // -------------------------------------------------------------------------
    static WidgetType getWidgetTypeFromNode(ibc::property::NodeBase *inNode)
    {
      SubcontractInterface::DataType dataType;
      dataType = SubcontractInterface::getNodeDataType(inNode);
      if (dataType == SubcontractInterface::DATA_TYPE_NOT_SPECIFIED)
        return WIDGET_TYPE_NOT_SPECIFIED;
      ibc::property::NodeBase *properties;
      properties = SubcontractInterface::getPropertiesRoot(inNode);
      if (properties == NULL)
        return getWidgetTypeFromDataType(dataType);
      QString name;
      if (SubcontractInterface::getChildValue<QString>(properties, "widget", &name))
        return getWidgetTypeFromName(name);
      return getWidgetTypeFromDataType(dataType);
    }
    // -------------------------------------------------------------------------
    // getWidgetTypeFromDataType  (*)
    // -------------------------------------------------------------------------
    static WidgetType getWidgetTypeFromDataType(SubcontractInterface::DataType inDataType)
    {
      WidgetType type = WIDGET_TYPE_NOT_SPECIFIED;

      switch(inDataType)
      {
        case SubcontractInterface::DATA_TYPE_int:
          type = WIDGET_TYPE_QSpinBox;
          break;
        case SubcontractInterface::DATA_TYPE_double:
          type = WIDGET_TYPE_QDoubleSpinBox;
          break;
        case SubcontractInterface::DATA_TYPE_QString:
          type = WIDGET_TYPE_QLineEdit;
          break;
      }
      return type;
    }
    // -------------------------------------------------------------------------
    // getWidgetTypeFromName  (*)
    // -------------------------------------------------------------------------
    static WidgetType getWidgetTypeFromName(QString &inName)
    {
      if (inName == "QSpinBox")
        return WIDGET_TYPE_QSpinBox;
      if (inName == "QDoubleSpinBox")
        return WIDGET_TYPE_QDoubleSpinBox;
      if (inName == "QDoubleSpinBox")
        return WIDGET_TYPE_QDoubleSpinBox;
      if (inName == "QLineEdit")
        return WIDGET_TYPE_QLineEdit;
      if (inName == "QComboBox")
        return WIDGET_TYPE_QComboBox;
      //
      printf("Internal Error\n");
      return WIDGET_TYPE_NOT_SPECIFIED;
    }

  protected:
    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // preparePropertiesRoot
    // -------------------------------------------------------------------------
    static std::shared_ptr<ibc::property::NoValueNode> preparePropertiesRoot(
                              std::shared_ptr<ibc::property::NodeBase> inNode)
    {
      return ibc::property::NoValueNode::prepareAsChildOf(
                        inNode,
                        SubcontractInterface::getPropertiesNodeNameStr());
    }
    // -------------------------------------------------------------------------
    // preparePropertyNode
    // -------------------------------------------------------------------------
    template <class T> static std::shared_ptr<ibc::property::Node<T>> preparePropertyNode(
                std::shared_ptr<ibc::property::NoValueNode> inPropertiesRoot,
                const char *inName,
                T inValue)
    {
      return ibc::property::Node<T>::prepareAsChildOf(
                        inPropertiesRoot,
                        inName, inValue);
    }

  };
};

#endif  // #ifdef IBC_QT_PROPERTY_NODE_UTILS_H_
