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
#include "ibc/property/node.h"
#include "ibc/qt/property/subcontract_interface.h"
#include "ibc/qt/property/qspinbox_subcontract.h"
#include "ibc/qt/property/qdoublespinbox_subcontract.h"

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
      //
      WIDGET_TYPE_ANY           = 0xFFFF
    };

    // Static Functions --------------------------------------------------------
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
                const char *inPrefix = NULL, const char *inSuffix = NULL)
    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = addQSpinBoxNode(inParent, inName, inValue, inMin, inMax);
      if (propertiesRoot == NULL)
        return false;
      return prepareQSpinBoxExtraParams(propertiesRoot,
                    inSingleStep, inStepType,
                    inDisplayIntegerBase,
                    inPrefix, inSuffix);
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
                const char *inPrefix = NULL, const char *inSuffix = NULL)
    {
      std::shared_ptr<ibc::property::NoValueNode> propertiesRoot;
      propertiesRoot = addQDoubleSpinBoxNode(inParent, inName, inValue, inMin, inMax);
      if (propertiesRoot == NULL)
        return false;
      return prepareQDoubleSpinBoxExtraParams(propertiesRoot,
                    inSingleStep, inStepType,
                    inDecimals,
                    inPrefix, inSuffix);
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
    //
    // -------------------------------------------------------------------------
    // -------------------------------------------------------------------------
    // getSubcontract
    // -------------------------------------------------------------------------
    static SubcontractInterface *getSubcontract(ibc::property::NodeBase *inNode)
    {
      return getSubcontract(getWidgetTypeFromNode(inNode));
    }
    // -------------------------------------------------------------------------
    // getSubcontract
    // -------------------------------------------------------------------------
    static SubcontractInterface *getSubcontract(WidgetType inType)
    {
      static QSpinBoxSubcontract       sQSpinBoxSubcontract;
      static QDoubleSpinBoxSubcontract sQDobuleSpinBoxSubcontract;
      //
      switch(inType)
      {
        case WIDGET_TYPE_QSpinBox:
          return &sQSpinBoxSubcontract;
        case WIDGET_TYPE_QDoubleSpinBox:
          return &sQDobuleSpinBoxSubcontract;
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
      ibc::property::NodeBase *property;
      property = SubcontractInterface::getPropertiesRoot(inNode);
      if (property == NULL)
        return getWidgetTypeFromDataType(dataType);
      QString name;
      if (SubcontractInterface::getChildValue<QString>(property, "widget", &name))
        return getWidgetTypeFromName(name);
      return getWidgetTypeFromDataType(dataType);
    }
    // -------------------------------------------------------------------------
    // getWidgetTypeFromDataType
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
      }
      return type;
    }
    // -------------------------------------------------------------------------
    // getWidgetTypeFromName
    // -------------------------------------------------------------------------
    static WidgetType getWidgetTypeFromName(QString &inName)
    {
      if (inName == "QSpinBox")
        return WIDGET_TYPE_QSpinBox;
      if (inName == "QDoubleSpinBox")
        return WIDGET_TYPE_QDoubleSpinBox;
      //
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
