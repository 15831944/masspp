/**
 * @file TreeNode.h
 * @brief definition of Tree Node class
 *
 * @author S.Tanaka
 * @date 2006.08.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_TREE_NODE_H__
#define __KOME_CORE_TREE_NODE_H__


#include <vector>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <algorithm>


namespace kome {
	namespace core {

		/**
		 * @class TreeNode
		 * @brief tree structure data management class
		 */
		template < typename T > class TreeNode {		
		public:
			/**
			 * @fn TreeNode()
			 * @brief constructor
			 */
			TreeNode() : m_element() {
				m_parent = NULL;
			}

			/** 
			 * @fn TreeNode( T& elm )
			 * @brief constructor
			 */
			TreeNode( T& elm ) : m_element( elm ){
				m_parent = NULL;
			}

			/**
			 * @fn virtual ~TreeNode()
			 * @brief destructor
			 */
			virtual ~TreeNode(){
				clearChildren();
			}

			/**
			 * @fn T& getElement()
			 * @brief gets the element that the node has
			 */
			T& getElement(){
				return m_element;
			}

			/**
			 * @fn void setElement( T& elm )
			 * @brief sets the element that the node has
			 */
			void setElement( T& elm ){
				m_element = elm;
			}

			/**
			 * @fn TreeNode* getParentNode();
			 * @brief gets parent node
			 * @return parent node. ( If this node has no parent, this method returns NULL )
			 */
			TreeNode* getParentNode(){
				return m_parent;
			}

			/** 
			 * @fn unsigned int getNumberOfChildren()
			 * @brief gets the number of child nodes
			 * @return the number of child nodes
			 */
			unsigned int getNumberOfChildren(){
				return m_children.size();
			}

			/**
			 * @fn TreeNode* getChild( int index )
			 * @brief get child node
			 * @param[in] index child index
			 * @return child node ( If there is no child node that has index specified, this method returns NULL )
			 */
			TreeNode* getChild( int index ) {
				// check parameter
				if( index < 0 || index > m_children.size() ) {
					return NULL;
				}

				return m_children[index];
			}

			/**
			 * @fn void clearChildren()
			 * @brief remove all child nodes
			 */
			void clearChildren() {
				for( unsigned int i = 0; i < m_children.size(); i++ ){
					delete m_children[index];
				}
				m_children.clear();
			}

			/**
			 * @fn TreeNode* addChild()
			 * @brief create child node
			 * @return the child node created
			 */
			TreeNode* addChild(){
				TreeNode<T>* child = new TreeNode<T>();
				m_children.push_back( child );
				return child;
			}

			/**
			 * @fn TreeNode* addChild( T& elm )
			 * @brief create child node
			 * @param[in] elm the element that child node has
			 * @return the child node created
			 */
			TreeNode* addChild( T& elm ){
				TreeNode<T>* child = new TreeNode<T>( elm );
				m_children.push_back( child );
				return child;
			}

			/**
			 * @fn void sortChildren( boost::function< bool ( TreeNode<T>&, TreeNode<T>& ) > lessFun )
			 * @brief sort child nodes
			 * @param[in] lessFun the function to compare. This function has two Properties.
				  The first parameter is the node to compare.
				  The second parameter is the node to be compared.
				  If the node specified by the first parameter sorts before the node specified by second parameter,
				  this function returns true
			 */				 
			void sortChildren( boost::function< bool ( TreeNode<T>&, TreeNode<T>& ) > lessFun ) {
				// check member
				if( m_children.size() == 0 ) {
					return;
				}

				// sort ( This method is called recursively. )
				for( unsigned int i = 0; i < m_children.size(); i++ ) {
					m_children[i]->sortChildre( lessFun );
				}

				std::sort( m_children.begin(), m_children.end(), lessFun );
			}

		protected:
			/** the element that the node has */
			T m_element;

			/** the array of child nodes */
			std::vector< TreeNode<T>* > m_children;

			/** the parent node */
			TreeNode<T>* m_parent;

			/**
			 * @fn TreeNode( TreeNode<T>* parent )
			 * @brief constructor
			 * @param[in] parent parent node
			 */
			TreeNode( TreeNode<T>* parent ) : m_element() {
				m_parent = parent;
			}

			/** 
			 * @fn TreeNode( TreeNode<T>* parent, T& elm )
			 * @brief constructor
			 * @param[in] parent parent node
			 * @param[in] elm the element that the node has
			 */
			TreeNode( TreeNode<T>* parent, T& elm ) : m_element( elm ) {
				m_parent = parent;
			}
		};
	}
}

#endif	// __KOME_CORE_TREE_NODE_H__
