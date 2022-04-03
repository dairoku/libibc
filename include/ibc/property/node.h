// =============================================================================
//  node.h
//
//  Written in 2019 by Dairoku Sekiguchi (sekiguchi at acm dot org)
//
//  To the extent possible under law, the author(s) have dedicated all copyright
//  and related and neighboring rights to this software to the public domain worldwide.
//  This software is distributed without any warranty.
//
//  You should have received a copy of the CC0 Public Domain Dedication along with
//  this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
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
namespace ibc::property
{
  class NodeBase;
  // ---------------------------------------------------------------------------
  // UpdateEventInterface class
  // ---------------------------------------------------------------------------
  class  UpdateEventInterface
  {
  public:
    virtual void updateEventListener(
                      std::shared_ptr<NodeBase> inNodeBase) = 0;
  };

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
    // getChildrenNum (without hidden nodes)
    // -------------------------------------------------------------------------
    size_t  getChildrenNum()
    {
      size_t  num = 0;
      for (auto it = mChildren.begin(); it != mChildren.end(); it++)
        if ((*it)->isHiddenNode() == false)
          num++;
      return num;
    }
    // -------------------------------------------------------------------------
    // getChildrenNum_ALL (include hidden)
    // -------------------------------------------------------------------------
    size_t  getChildrenNum_ALL()
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
    // getParent
    // -------------------------------------------------------------------------
    std::shared_ptr<NodeBase>  getParent()
    {
      return mParent;
    }
    // -------------------------------------------------------------------------
    // getChild (without hidden nodes)
    // -------------------------------------------------------------------------
    std::shared_ptr<NodeBase>  getChild(size_t inIndex)
    {
      size_t  num = 0;
      for (auto it = mChildren.begin(); it != mChildren.end(); it++)
      {
        if ((*it)->isHiddenNode() == false)
        {
          if (num == inIndex)
            return *it;
          num++;
        }
      }
      return NULL;
    }
    // -------------------------------------------------------------------------
    // getChild_ALL (include hidden)
    // -------------------------------------------------------------------------
    std::shared_ptr<NodeBase>  getChild_ALL(size_t inIndex)
    {
      if (inIndex >= mChildren.size())
        return NULL;
      return mChildren[inIndex];
    }
    // -------------------------------------------------------------------------
    // getChildPointer
    // -------------------------------------------------------------------------
    NodeBase  *getChildPointer(size_t inIndex)
    {
      std::shared_ptr<NodeBase> nodeBase;
      nodeBase = getChild(inIndex);
      if (nodeBase == NULL)
        return NULL;
      return nodeBase.get();
    }
    // -------------------------------------------------------------------------
    // getChildPointer_ALL
    // -------------------------------------------------------------------------
    NodeBase  *getChildPointer_ALL(size_t inIndex)
    {
      std::shared_ptr<NodeBase> nodeBase;
      nodeBase = getChild_ALL(inIndex);
      if (nodeBase == NULL)
        return NULL;
      return nodeBase.get();
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
    // getChildPointer
    // -------------------------------------------------------------------------
    NodeBase  *getChildPointerByName(const char *inName)
    {
      std::shared_ptr<NodeBase> nodeBase;
      nodeBase = getChildByName(inName);
      if (nodeBase == NULL)
        return NULL;
      return nodeBase.get();
    }
    // -------------------------------------------------------------------------
    // getChildByPath
    // -------------------------------------------------------------------------
    std::shared_ptr<NodeBase>  getChildByPath(const char *inPath)
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
    // getChildPointerByPath
    // -------------------------------------------------------------------------
    NodeBase  *getChildPointerByPath(const char *inPath)
    {
      std::shared_ptr<NodeBase> nodeBase;
      nodeBase = getChildByPath(inPath);
      if (nodeBase == NULL)
        return NULL;
      return nodeBase.get();
    }
    // -------------------------------------------------------------------------
    // getChildAs
    // -------------------------------------------------------------------------
    template <class T> std::shared_ptr<T>  getChildAs(size_t inIndex)
    {
      return castChildTo<T>(getChild(inIndex));
    }
    // -------------------------------------------------------------------------
    // getChildAs_ALL
    // -------------------------------------------------------------------------
    template <class T> std::shared_ptr<T>  getChildAs_ALL(size_t inIndex)
    {
      return castChildTo<T>(getChild_ALL(inIndex));
    }
    // -------------------------------------------------------------------------
    // getChildPointerAs
    // -------------------------------------------------------------------------
    template <class T>T  *getChildPointerAs(size_t inIndex)
    {
      std::shared_ptr<T> nodeBase;
      nodeBase = getChildAs<T>(inIndex);
      if (nodeBase == NULL)
        return NULL;
      return nodeBase.get();
    }
    // -------------------------------------------------------------------------
    // getChildPointerAs_ALL
    // -------------------------------------------------------------------------
    template <class T>T  *getChildPointerAs_ALL(size_t inIndex)
    {
      std::shared_ptr<T> nodeBase;
      nodeBase = getChildAs_ALL<T>(inIndex);
      if (nodeBase == NULL)
        return NULL;
      return nodeBase.get();
    }
    // -------------------------------------------------------------------------
    // getChildAsByName
    // -------------------------------------------------------------------------
    template <class T> std::shared_ptr<T>  getChildAsByName(const char *inName)
    {
      return castChildTo<T>(getChildByName(inName));
    }
    // -------------------------------------------------------------------------
    // getChildPointerAsByName
    // -------------------------------------------------------------------------
    template <class T>T  *getChildPointerAsByName(const char *inName)
    {
      std::shared_ptr<T> nodeBase;
      nodeBase = getChildAsByName<T>(inName);
      if (nodeBase == NULL)
        return NULL;
      return nodeBase.get();
    }
    // -------------------------------------------------------------------------
    // getChildAsByPath
    // -------------------------------------------------------------------------
    template <class T> std::shared_ptr<T>  getChildAsByPath(const char *inPath)
    {
      return castChildTo<T>(getChildByPath(inPath));
    }
    // -------------------------------------------------------------------------
    // getChildPointerAsByPath
    // -------------------------------------------------------------------------
    template <class T>T  *getChildPointerAsByPath(const char *inPath)
    {
      std::shared_ptr<T> nodeBase;
      nodeBase = getChildAsByPath<T>(inPath);
      if (nodeBase == NULL)
        return NULL;
      return nodeBase.get();
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
    // isChild
    // -------------------------------------------------------------------------
    bool  isChild(const char *inName)
    {
      for (auto it = mChildren.begin(); it != mChildren.end(); it++)
        if ((*it)->mName.compare(inName) == 0)
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
    // removeChild_ALL
    // -------------------------------------------------------------------------
    bool  removeChild_ALL(size_t inIndex)
    {
      auto child = getChild_ALL(inIndex);
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
      if (inName == NULL)
        return;
      mName = inName;
    }
    // -------------------------------------------------------------------------
    // hasValue
    // -------------------------------------------------------------------------
    bool  hasValue()
    {
      return mHasValue;
    }
    // -------------------------------------------------------------------------
    // setAuxiliaryDataPointer
    // -------------------------------------------------------------------------
    void  setAuxiliaryDataPointer(void *inAuxiliaryDataPointer)
    {
      mAuxiliaryDataPointer = inAuxiliaryDataPointer;
    }
    // -------------------------------------------------------------------------
    // getAuxiliaryDataPointer
    // -------------------------------------------------------------------------
    void  *getAuxiliaryDataPointer()
    {
      return mAuxiliaryDataPointer;
    }
    // -------------------------------------------------------------------------
    // get_shared_ptr
    // -------------------------------------------------------------------------
    std::shared_ptr<NodeBase> get_shared_ptr()
    {
      if (mParent == NULL)
        return NULL;
      //
      for (size_t i = 0; i < mParent->mChildren.size(); i++)
      {
        if (mParent->mChildren[i].get() == this)
          return mParent->mChildren[i];
      }
      return NULL;
    }
    // -------------------------------------------------------------------------
    // addUpdateEventListener
    // -------------------------------------------------------------------------
    void  addUpdateEventListener(UpdateEventInterface *inInterface)
    {
      mUpdateEventListeners.push_back(inInterface);
    }
    // -------------------------------------------------------------------------
    // removeUpdateEventListener
    // -------------------------------------------------------------------------
    void  removeUpdateEventListener(UpdateEventInterface *inInterface)
    {
      decltype(mUpdateEventListeners)::iterator it;
      it = std::find(mUpdateEventListeners.begin(),
                     mUpdateEventListeners.end(),
                     inInterface);
      if (it == mUpdateEventListeners.end())
        return;
      mUpdateEventListeners.erase(it);
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
    // Member variables (protected) -----------------------------------------------
    bool  mHasValue;
    std::string mName;
    void  *mAuxiliaryDataPointer;
    std::vector<std::shared_ptr<NodeBase>>  mChildren;
    std::vector<UpdateEventInterface *>     mUpdateEventListeners;
    std::shared_ptr<NodeBase>  mParent;
    //
    char  mPathSeparator;
    char  mHiddenNodeChar;

    // Constructors ------------------------------------------------------------
    // -------------------------------------------------------------------------
    // NodeBasae
    // -------------------------------------------------------------------------
    NodeBase(const char *inName)
    {
      mHasValue             = false;
      mName                 = inName;
      mAuxiliaryDataPointer = NULL;
      mParent               = NULL;
      //
      mPathSeparator        = '/';
      mHiddenNodeChar       = '.';
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
      while (*inPath == mPathSeparator)
        inPath++;
      const char  *startPtr = inPath;
      size_t  len = 0;
      while (*inPath != 0)
      {
        if (*inPath == mPathSeparator && len != 0)
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
    // -------------------------------------------------------------------------
    // isHiddenNode
    // -------------------------------------------------------------------------
    bool  isHiddenNode()
    {
      if (mName.length() == 0)
        return true;
      if (mName[0] == mHiddenNodeChar)
        return true;
      return false;
    }
    // -------------------------------------------------------------------------
    // invokeUpdateEvent
    // -------------------------------------------------------------------------
    void  invokeUpdateEvent()
    {
      std::shared_ptr<NodeBase> nodeBase = get_shared_ptr();
      if (nodeBase == NULL)
        return;
      for (auto it = mUpdateEventListeners.begin(); it != mUpdateEventListeners.end(); it++)
        (*it)->updateEventListener(nodeBase);
    }
  };

  // ---------------------------------------------------------------------------
  // NoValueNode class
  // ---------------------------------------------------------------------------
  class  NoValueNode : public NodeBase
  {
  public:
    // Destructor --------------------------------------------------------------
    // -------------------------------------------------------------------------
    // ~NoValueNode
    // -------------------------------------------------------------------------
    virtual ~NoValueNode()
    {
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // createNode
    // -------------------------------------------------------------------------
    static std::shared_ptr<NoValueNode> createNode(const char *inName)
    {
      return std::shared_ptr<NoValueNode>(new NoValueNode(inName));
    }
    // -------------------------------------------------------------------------
    // createAsChildOf
    // -------------------------------------------------------------------------
    static std::shared_ptr<NoValueNode> createAsChildOf(
                                            std::shared_ptr<NodeBase> inParent,
                                            const char *inName)
    {
      std::shared_ptr<NoValueNode>  node = createNode(inName);
      NodeBase::addChild(inParent, node);
      return node;
    }
    // -------------------------------------------------------------------------
    // prepareAsChildOf (get or create a node, if not exist)
    // -------------------------------------------------------------------------
    static std::shared_ptr<NoValueNode> prepareAsChildOf(
                                            std::shared_ptr<NodeBase> inParent,
                                            const char *inName)
    {
      if (inParent->isChild(inName))
      {
        if (inParent->getChildByName(inName)->checkType<NoValueNode>() == false)
          return NULL;
        std::shared_ptr<NoValueNode> node;
        return inParent->getChildAsByName<NoValueNode>(inName);
      }
      return createAsChildOf(inParent, inName);
    }

  protected:
    // Constructors ------------------------------------------------------------
    // -------------------------------------------------------------------------
    // NoValueNode
    // -------------------------------------------------------------------------
    NoValueNode(const char *inName)
      : NodeBase(inName)
    {
      mHasValue = false;
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
    void  setValue(const ValueType &inValue, bool inInvokeUpdateEvent = true)
    {
      mValue = inValue;
      if (inInvokeUpdateEvent)
        invokeUpdateEvent();
    }
    // -------------------------------------------------------------------------
    // getValue
    // -------------------------------------------------------------------------
    ValueType getValue()
    {
      return mValue;
    }

    // Static Functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // createNode
    // -------------------------------------------------------------------------
    static std::shared_ptr<Node<ValueType>> createNode(const char *inName,
                                                       ValueType inValue)
    {
      return std::shared_ptr<Node<ValueType>>(new Node<ValueType>(inName, inValue));
    }
    // -------------------------------------------------------------------------
    // createAsChildOf
    // -------------------------------------------------------------------------
    static std::shared_ptr<Node<ValueType>> createAsChildOf(
                                            std::shared_ptr<NodeBase> inParent,
                                            const char *inName,
                                            ValueType inValue)
    {
      std::shared_ptr<Node<ValueType>>  node = createNode(inName, inValue);
      NodeBase::addChild(inParent, node);
      return node;
    }
    // -------------------------------------------------------------------------
    // prepareAsChildOf (get+set or create a node, if not exist)
    // -------------------------------------------------------------------------
    static std::shared_ptr<Node<ValueType>> prepareAsChildOf(
                                            std::shared_ptr<NodeBase> inParent,
                                            const char *inName,
                                            ValueType inValue)
    {
      if (inParent->isChild(inName))
      {
        if (inParent->getChildByName(inName)->checkType<Node<ValueType>>() == false)
          return NULL;
        std::shared_ptr<Node<ValueType>> node;
        node = inParent->getChildAsByName<Node<ValueType>>(inName);
        node->setValue(inValue);
        return node;
      }
      return createAsChildOf(inParent, inName, inValue);
    }

  protected:
    // Member variables (protected) --------------------------------------------
    ValueType mValue;

    // Constructors ------------------------------------------------------------
    // -------------------------------------------------------------------------
    // Node
    // -------------------------------------------------------------------------
    Node(const char *inName, ValueType inValue)
      : NodeBase(inName)
    {
      mHasValue = true;
      mValue = inValue;
    }
  };
};

#endif  // #ifdef IBC_PROPERTY_NODE_H_
