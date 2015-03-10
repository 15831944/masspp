/**
 * @file PlotCanvas.h
 * @brief interfaces of PlotCanvas class
 *
 * @author S.Tanaka
 * @date 2009.07.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MATRIX_PLOT_CANVAS_H__
#define __KOME_MATRIX_PLOT_CANVAS_H__


#include "PlotPanel.h"


namespace kome {
	namespace matrix {
		namespace groupplot {

			/**
			* @class PlotCanvas
			* @brief matrix data profile canvas
			*/
			class PlotCanvas : public kome::window::ProfileCanvas {
			public:
				/**
				 * @fn PlotCanvas(
							wxWindow* parent,
							const int width,
							const int height,
							const int peakId,
							bool group_plot,
							bool box_plot
						)
				 * @brief constructor
				 * @param parent parent window
				 * @param width window width
				 * @param height window height
				 * @param peakId peak ID
				 * @param group_plot group state
				 * @param box_plot box state
				 */
				PlotCanvas(
					wxWindow* parent,
					const int width,
					const int height,
					const int peakId,
					bool group_plot,
					bool box_plot
				);

				/**
				 * @fn virtual ~PlotCanvas()
				 * @brief destructor
				 */
				virtual ~PlotCanvas();

			protected:
				/** peak ID */
				int m_peakId;

				/** min value */
				double m_minVal;

				/** max value */
				double m_maxVal;

				/** initialized flag */
				bool m_initialized;

				/** group plot */
				bool m_groupPlot;

				/** box plot */
				bool m_boxPlot;

			protected:
				
				/**
				 * @struct PosInfo
				 * @brief position information
				 */
				struct PosInfo {
					double x;
					double y;
					COLOR color;
				};
				
				/**
				 * @struct GroupInfo
				 * @brief group information
				 */
				struct GroupInfo {
					int groupId;
					std::string name;
					double avg;
					double width;
					double pos;
					std::vector< PosInfo > positions;
					double med;					
				};
				
				/** groups */
				std::vector< GroupInfo > m_groups;

				/** 
				 * @struct BoxInfo
				 * @brief box information
				 */
				struct BoxInfo {
					double quartile1;
					double median;
					double quartile3;
					std::vector< PosInfo > positions;
				};

			public:
				/**
				 * @fn void setPeakId( const int peakId )
				 * @brief sets the peak ID
				 * @param[in] peakId peak ID
				 */
				void setPeakId( const int peakId );

			public:
				/**
				 * @fn void addGroup( const int groupId )
				 * @brief adds group
				 * @param groupId group ID
				 */
				void addGroup( const int groupId );

			public:
				/**
				 * @fn bool getGroupPlot()
				 * @brief get the state of the Group Plot button
				 * @return state
				 */
				bool getGroupPlot(){ return m_groupPlot; }

				/**
				 * @fn void setGroupPlot( bool groupPlot )
				 * @brief set the state of the Group Plot button
				 * @param groupPlot
				 */
				void setGroupPlot( bool groupPlot ){ m_groupPlot = groupPlot; }

				/**
				 * @fn bool getBoxPlot()
				 * @brief get the state of the Box Plot button
				 * @return state
				 */
				bool getBoxPlot(){ return m_boxPlot; }

				/**
				 * @fn void setBoxPlot( bool boxPlot )
				 * @brief set hte state of the Box Plot button
				 * @param boxPlot
				 */
				void setBoxPlot( bool boxPlot ){ m_boxPlot = boxPlot; }

			protected:
				/**
				 * @fn void createArray()
				 * @brief creates array
				 */
				void createArray();

				/**
				 * @fn void setGroup( GroupInfo* group )
				 * @brief sets group information
				 * @param group group information structure
				 */
				void setGroup( GroupInfo* group );

				/**
				 * @fn void drawGroupPlot( kome::window::DCGraphics& g )
				 * @brief draw Group Plot
				 * @param g graphics object to draw
				 */
				void drawGroupPlot( kome::window::DCGraphics& g );

				/**
				 * @fn void drawGroupMedian( kome::window::DCGraphics& g )
				 * @brief draw group median
				 * @param g graphics object to draw
				 */
				void drawGroupMedian( kome::window::DCGraphics& g );

				/**
				 * @fn void drawBoxPlot( kome::window::DCGraphics& g )
				 * @brief draw Box Plot
				 * @param g graphics object to draw
				 */
				void drawBoxPlot( kome::window::DCGraphics& g );
				
				/**
				 * @fn void drawBoxMedian( kome::window::DCGraphics& g )
				 * @brief draw box median
				 * @param g graphics object to draw
				 */
				void drawBoxMedian( kome::window::DCGraphics& g );

				/**
				 * @fn void drawBox( 
						kome::window::DCGraphics& g, 
						double pos,
						double width,
						double quartile1,
						double quartile3
					)
				 * @brief draw box
				 * @param g graphics object to draw
				 * @param pos
				 * @param width
				 * @param quartile1
				 * @param quartile3
				 */
				void drawBox( 
					kome::window::DCGraphics& g, 
					double pos,
					double width,
					double quartile1,
					double quartile3
				);

				/**
				 * @fn void drawLine( 
						kome::window::DCGraphics& g, 
						bool bmin,
						BoxInfo info, 
						double pos,
						double width
					)
				 * @brief draw line
				 * @param g graphics object to draw
				 * @param bmin 下側かどうかのフラグ
				 * @param info
				 * @param pos
				 * @param width
				 */
				void drawLine( 
					kome::window::DCGraphics& g, 
					bool bmin,
					BoxInfo info, 
					double pos,
					double width
				);

				/**
				 * @fn void drawVerticalLine(
						kome::window::DCGraphics& g,
						const bool bmin,
						BoxInfo info, 
						double pos,
						double width,
						double xPos ,
						bool btmp,
						double yPos_up,
						double yPos_dwn
					)
				 * @brief draw vertical line
				 * @param g graphics object to draw
				 * @param bmin 下側かどうかのフラグ
				 * @param info
				 * @param pos
				 * @param width
				 * @param xPos
				 * @param yPos_up
				 * @param yPos_dwn
				 */
				void drawVerticalLine(
					kome::window::DCGraphics& g,
					const bool bmin,
					BoxInfo info, 
					double pos,
					double width,
					double xPos ,
					double yPos_up,
					double yPos_dwn
				);

				/**
				 * @fn void drawOutlier(
						kome::window::DCGraphics& g, 
						const bool bmin,
						BoxInfo info, 
						double pos,
						double width,
						double yPos_up,
						double yPos_dwn
					)
				 * @brief draw Outilier
				 * @param g graphics object to draw
				 * @param bmin 下側かどうかのフラグ
				 * @param info
				 * @param pos
				 * @param width
				 * @param yPos_up
				 * @param yPos_dwn
				 */
				void drawOutlier(
					kome::window::DCGraphics& g, 
					const bool bmin,
					BoxInfo info, 
					double pos,
					double width,
					double yPos_up,
					double yPos_dwn
				);

				/**
				 * @fn void calcQuartile( double val, bool bDivisible, std::vector< PosInfo > boxPos, bool bFirstQuartile, double& quartile1 )
				 * @brief 四分位 算出
				 * @param val
				 * @param bDivisible
				 * @param boxPos
				 * @param bFirstQuartile 
				 * @param[out] quartile  第1四分位を返す
				 */
				void calcQuartile( double val, bool bDivisible, std::vector< PosInfo > boxPos, bool bFirstQuartile, double& quartile1 );
							
				
			protected:
				/**
				 * @fn virtual void onPrepareDraw( kome::window::DCGraphics& g )
				 * @brief this method is called when prepareing (override method)
				 * @param g graphics object to draw
				 */
				virtual void onPrepareDraw( kome::window::DCGraphics& g );

				/**
				 * @fn virtual void onDrawProfile( kome::window::DCGraphics& g )
				 * @brief This method is called when drawing profile
				 * @param g graphics
				 */
				virtual void onDrawProfile( kome::window::DCGraphics& g );

				/**
				 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
				 * @brief This method is called when drawing graph foregorund
				 * @param g graphics
				 */
				virtual void onDrawGraphFg( kome::window::DCGraphics& g );

				/**
				 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
				 * @brief This method is called when drawing graph foreground
				 * @param g graphics
				 */
				virtual void onDrawWindowFg( kome::window::DCGraphics& g );

			protected:
				/**
				 * @fn static bool lessVal( std::pair< double, COLOR >& v0, std::pair< double, COLOR >& v1 )
				 * @brief compare to sort values
				 * @param[in] v0 value to be compared
				 * @param[in] v1 value to compare
				 * @return If true, v0 sorts before v1
				 */
				static bool lessVal( std::pair< double, COLOR >& v0, std::pair< double, COLOR >& v1 );

				/**
				 * @fn static bool lessPos( PosInfo& v0, PosInfo& v1 )
				 * @brief compare to sort values
				 * @param[in] v0 value to be compared
				 * @param[in] v1 value to compare
				 * @return If true, v0 sorts before v1
				 */
				static bool lessPos( PosInfo& v0, PosInfo& v1 );
			private:
				DECLARE_EVENT_TABLE()
			};
		}
	}
}


#endif		// __KOME_MATRIX_PLOT_CANVAS_H__
