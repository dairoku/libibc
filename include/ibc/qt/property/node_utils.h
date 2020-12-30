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

// Namespace -------------------------------------------------------------------
namespace ibc::qt::property
{
  // ---------------------------------------------------------------------------
  // SubcontractInterface class
  // ---------------------------------------------------------------------------
  class NodeUtils
  {
  public:
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
