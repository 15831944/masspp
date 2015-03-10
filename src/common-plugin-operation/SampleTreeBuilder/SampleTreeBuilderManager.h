/**
 * @file SampleTreeBuilderManager.h
 * @brief interfaces of sample tree builder manager functions
 *
 * @author okada_h
 * @date 2012.09.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_SAMPLE_TREE_BUILDER_MANAGER_H__
#define __KOME_SAMPLE_TREE_BUILDER_MANAGER_H__


namespace kome{
	namespace SampleTreeBuilder{
		
		class SampleTreeBuilderManager{
		protected:
			/**
			 * @fn SampleTreeBuilderManager()
			 * @brief constructor
			 */
			SampleTreeBuilderManager();

			/**
			 * @fn virtual ~SampleTreeBuilderManager()
			 * @brief destructor
			 */
			virtual ~SampleTreeBuilderManager();

		private:
			// 現在システム内にある SampleTreeBuilderSampleSet* の一覧。各々コンストラクタでthisポインタを登録、destructorで登録消去
			std::set< kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* > m_setSsPtrs;

		public:
			/**
			 * @fn void registPtr( kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr )
			 * @brief SampleTreeBuilderSampleSet* をnewする際、この処理を呼び出し、オブジェクトのポインタを登録する
			 * @param ptr
			 */
			void registPtr( kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr ){
				if( m_setSsPtrs.find( ptr ) == m_setSsPtrs.end() ){
					// 登録されていない
					m_setSsPtrs.insert( ptr );	// 登録
				}
			}

			/**
			 * @fn void unregistPtr( kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr )
			 * @brief SampleTreeBuilderSampleSet* が破棄される際、この処理を呼び出し、オブジェクトのポインタの登録を消去する
			 * @param ptr
			 */
			void unregistPtr( kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr ){
				if( m_setSsPtrs.find( ptr ) == m_setSsPtrs.end() ){
					// 登録されていない
				}else{
					m_setSsPtrs.erase( ptr );	// 登録を消去
				}
			}

		public:

			/**
			 * @fn static SampleTreeBuilderManager& getInstance()
			 * @brief gets SampleTreeBuilderManager object
			 * @return SampleTreeBuilderManager object. (This is the only object.)
			 */
			static SampleTreeBuilderManager& getInstance();
		};
	}
}

using namespace kome::SampleTreeBuilder;


#endif
