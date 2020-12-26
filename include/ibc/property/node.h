// =============================================================================
//  node.h
//
//  MIT License
//
//  Copyright (c) 2019 Dairoku Sekiguchi
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
  \file     ibc/property/node.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/09/16
  \brief    Header file for the Node class
*/
#ifndef IBC_PROPERTY_NODE_H_
#define IBC_PROPERTY_NODE_H_

// Includes --------------------------------------------------------------------
#include <iostream>
#include <memory>
#include <algorithm>
#include <variant>
#include <vector>
#include <string>
#include <string_view>
#include <typeinfo>
#include "ibc/base/types.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace property
 {
  // ---------------------------------------------------------------------------
  // NodeBase class
  // ---------------------------------------------------------------------------
  class  NodeBase
  {
  public:
    // Destructor --------------------------------------------------------------
    // -------------------------------------------------------------------------
    // ~NodeBasae
    // -------------------------------------------------------------------------
    virtual ~NodeBase()
    {
      printf("~NodeBase : %s\n", mName.c_str());
      removeAllChildren();
      mParent = NULL;
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // hasChild
    // -------------------------------------------------------------------------
    bool  hasChild()
    {
      if (getChildrenNum() == 0)
        return false;
      return true;
    }
    // -------------------------------------------------------------------------
    // checkType
    // -------------------------------------------------------------------------
    template <class T> bool checkType()
    {
      const std::type_info &info = typeid(*this);
      if (info == typeid(T))
        return true;
      return false;
    }
    // -------------------------------------------------------------------------
    // getChildrenNum
    // -------------------------------------------------------------------------
    size_t  getChildrenNum()
    {
      return mChildren.size();
    }
    // -------------------------------------------------------------------------
    // getChildren
    // -------------------------------------------------------------------------
    std::vector<std::shared_ptr<NodeBase>>  getChildren()
    {
      return mChildren;
    }
    // -------------------------------------------------------------------------
    // getChildrenByNane
    // -------------------------------------------------------------------------
    std::vector<std::shared_ptr<NodeBase>>  getChildrenByName(const char *inName)
    {
      std::vector<std::shared_ptr<NodeBase>>  list;
      for (auto it = mChildren.begin(); it != mChildren.end(); it++)
        if ((*it)->mName.compare(inName) == 0)
          list.push_back(*it);
      return list;
    }
    // -------------------------------------------------------------------------
    // getChild
    // -------------------------------------------------------------------------
    std::shared_ptr<NodeBase>  getChild(size_t inIndex)
    {
      if (inIndex >= mChildren.size())
        return NULL;
      return mChildren[inIndex];
    }
    // -------------------------------------------------------------------------
    // getChildByName
    // -------------------------------------------------------------------------
    std::shared_ptr<NodeBase>  getChildByName(const char *inName)
    {
      for (auto it = mChildren.begin(); it != mChildren.end(); it++)
        if ((*it)->mName.compare(inName) == 0)
          return *it;
      return NULL;
    }
    // -------------------------------------------------------------------------
    // getChildrenByPath
    // -------------------------------------------------------------------------
    std::shared_ptr<NodeBase>  getChildrenByPath(const char *inPath)
    {
      std::shared_ptr<NodeBase> node = NULL;
      std::vector<std::string>  list;
      parseNodePath(inPath, list);
      if (list.empty())
        return NULL;
      for (auto it = list.cbegin(); it != list.cend(); it++)
      {
        if (node == NULL)
          node = getChildByName(it->c_str());
        else
          node = node->getChildByName(it->c_str());
        if (node == NULL)
          return NULL;
      }
      return node;
    }
    // -------------------------------------------------------------------------
    // getChildAs
    // -------------------------------------------------------------------------
    template <class T> std::shared_ptr<T>  getChildAs(size_t inIndex)
    {
      return castChildTo<T>(getChild(inIndex));
    }
    // -------------------------------------------------------------------------
    // getChildAsByNane
    // -------------------------------------------------------------------------
    template <class T> std::shared_ptr<T>  getChildAsByName(const char *inName)
    {
      return castChildTo<T>(getChildByName(inName));
    }
    // -------------------------------------------------------------------------
    // getChildAsByPath
    // -------------------------------------------------------------------------
    template <class T> std::shared_ptr<T>  getChildAsByPath(const char *inPath)
    {
      return castChildTo<T>(getChildrenByPath(inPath));
    }
    // -------------------------------------------------------------------------
    // isChild
    // -------------------------------------------------------------------------
    bool  isChild(std::shared_ptr<NodeBase> inChild)
    {
      auto it = std::find(mChildren.cbegin(), mChildren.cend(), inChild);
      if (it != mChildren.cend())
        return true;
      return false;
    }
    // -------------------------------------------------------------------------
    // removeChild
    // -------------------------------------------------------------------------
    bool  removeChild(std::shared_ptr<NodeBase> inChild)
    {
      decltype(mChildren)::iterator it;
      if (getChildIterator(inChild, &it) == false)
        return false;
      (*it)->mParent = NULL;
      mChildren.erase(it);
      return true;
    }
    // -------------------------------------------------------------------------
    // removeChild
    // -------------------------------------------------------------------------
    bool  removeChild(size_t inIndex)
    {
      auto child = getChild(inIndex);
      if (child == NULL)
        return false;
      return removeChild(child);
    }
    // -------------------------------------------------------------------------
    // removeChildByName
    // -------------------------------------------------------------------------
    bool  removeChildByName(const char *inName)
    {
      auto child = getChildByName(inName);
      if (child == NULL)
        return false;
      return removeChild(child);
    }
    // -------------------------------------------------------------------------
    // removeAllChildren
    // -------------------------------------------------------------------------
    void  removeAllChildren()
    {
      for (auto it = mChildren.begin(); it != mChildren.end(); it++)
        (*it)->mParent = NULL;
      mChildren.clear();
    }
    // -------------------------------------------------------------------------
    // getName
    // -------------------------------------------------------------------------
    std::string getName()
    {
      return mName;
    }
    // -------------------------------------------------------------------------
    // setName
    // -------------------------------------------------------------------------
    void  setName(const char *inName)
    {
      mName = inName;
    }
    // -------------------------------------------------------------------------
    // hasValue
    // -------------------------------------------------------------------------
    bool  hasValue()
    {
      return mHasValue;
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // createEmptyNode
    // -------------------------------------------------------------------------
    static std::shared_ptr<NodeBase> createEmptyNode(const char *inName)
    {
      return std::shared_ptr<NodeBase>(new NodeBase(inName));
    }
    // -------------------------------------------------------------------------
    // addChild
    // -------------------------------------------------------------------------
    static void  addChild(std::shared_ptr<NodeBase> inParent, std::shared_ptr<NodeBase> inChild)
    {
      inParent->mChildren.push_back(inChild);
      inChild->mParent = inParent;
    }

  protected:
    // Member variables (public) -----------------------------------------------
    bool  mHasValue;
    std::string mName;
    std::vector<std::shared_ptr<NodeBase>>  mChildren;
    std::shared_ptr<NodeBase>  mParent;

    // Constructors ------------------------------------------------------------
    // -------------------------------------------------------------------------
    // NodeBasae
    // -------------------------------------------------------------------------
    NodeBase(const char *inName)
    {
      mHasValue   = false;
      mName       = inName;
      mParent     = NULL;
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // getChildConstIterator
    // -------------------------------------------------------------------------
    bool  getChildConstIterator(std::shared_ptr<NodeBase> inChild,
                          decltype(mChildren)::const_iterator *outIT)
    {
      decltype(mChildren)::iterator it;
      bool  result;
      result = getChildIterator(inChild, &it);
      *outIT = it;
      return result;
    }
    // -------------------------------------------------------------------------
    // getChildIterator
    // -------------------------------------------------------------------------
    bool  getChildIterator(std::shared_ptr<NodeBase> inChild,
                          decltype(mChildren)::iterator *outIT)
    {
      *outIT = std::find(mChildren.begin(), mChildren.end(), inChild);
      if (*outIT != mChildren.end())
        return true;
      return false;
    }
    // -------------------------------------------------------------------------
    // castChildTo
    // -------------------------------------------------------------------------
    template <class T> std::shared_ptr<T>  castChildTo(std::shared_ptr<NodeBase> inChild)
    {
      if (inChild == NULL)
        return NULL;
      if (inChild->checkType<T>() == false)
        return NULL;
      return std::static_pointer_cast<T>(inChild);
    }
    // -------------------------------------------------------------------------
    // parseNodePath
    // -------------------------------------------------------------------------
    void  parseNodePath(const char *inPath, std::vector<std::string> &ioList)
    {
      while (*inPath == '/')
        inPath++;
      const char  *startPtr = inPath;
      size_t  len = 0;
      while (*inPath != 0)
      {
        if (*inPath == '/' && len != 0)
        {
          ioList.push_back(std::string(startPtr, len));
          len = 0;
          startPtr = inPath;
          startPtr++;
        }
        else
        {
          len++;
        }
        inPath++;
      }
      if (len != 0)
        ioList.push_back(std::string(startPtr, len));
    }
  };

  // ---------------------------------------------------------------------------
  // Node class
  // ---------------------------------------------------------------------------
  template <typename ValueType> class  Node : public NodeBase
  {
  public:
    // Destructor --------------------------------------------------------------
    // -------------------------------------------------------------------------
    // ~Node
    // -------------------------------------------------------------------------
    virtual ~Node()
    {
    }

    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setValue
    // -------------------------------------------------------------------------
    void  setValue(const ValueType &inValue)
    {
      mValue = inValue;
      // ToDo Event Notifier
    }
    // -------------------------------------------------------------------------
    // getgetValue
    // -------------------------------------------------------------------------
    ValueType getValue()
    {
      return mValue;
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // createNode
    // -------------------------------------------------------------------------
    static std::shared_ptr<Node<ValueType>> createNode(ValueType inValue,
                                                        const char *inName)
    {
      return std::shared_ptr<Node<ValueType>>(new Node<ValueType>(inValue, inName));
    }
    // -------------------------------------------------------------------------
    // createAsChild
    // -------------------------------------------------------------------------
    static std::shared_ptr<Node<ValueType>> createAsChildOf(std::shared_ptr<NodeBase> inParent,
                                                        ValueType inValue,
                                                        const char *inName)
    {
      std::shared_ptr<Node<ValueType>>  node = createNode(inValue, inName);
      NodeBase::addChild(inParent, node);
      return node;
    }

  protected:
    // Member variables (protected) --------------------------------------------
    ValueType mValue;

    // Constructors ------------------------------------------------------------
    // -------------------------------------------------------------------------
    // Node
    // -------------------------------------------------------------------------
    Node(ValueType inValue, const char *inName)
      : NodeBase(inName)
    {
      mHasValue = true;
      mValue = inValue;
    }
  };
 };
};

#endif  // #ifdef IBC_PROPERTY_NODE_H_
