#ifndef LIB_MATH_HIMATH_H
#define LIB_MATH_HIMATH_H

//==============================================================================
///                                                                             
///@par Classification: unclassified, OPEN SOURCE                                                                                        
///                                                                             
//==============================================================================
/// \page himath    himath                                                      
/// \version 1.21                                                               
/// \section Introduction                                                       
///                                                                             
/// himath is a collection of functions that perform mathematical computations. 
/// As of version 1.10, the module contains three types of functions:  Double   
/// Value Generation, Linear Algebra Computations, and Coordinate System        
/// computations.                                                               
///                                                                             
/// As time has gone one, I have added other functions that are either quite    
/// generic (e.g. Abs) or quite esoteric (e.g. Reflect).                        
///                                                                             
/// \section DVG Double Value Generation                                        
/// These functions generate the following non-numeric values and return them   
/// as type double:  Quiet NaN (not-a-number), Signal NaN, Positive Infinity,   
/// and Negative Infinity.                                                      
///                                                                             
/// \section LAC Linear Algebra Computations                                    
/// These functions perform addition and multiplication of matrices.            
///                                                                             
/// \section CSC Coordinate System Computations                                 
/// These function rotate or transform points and vectors from one coordinate   
/// system to another                                                           
///                                                                             
/// <b>Definition</b>                                                           
///                                                                             
/// For the purposes of this collection of functions, matrices are stored in    
/// the standard template library's valarray container.  Elements of the matrix 
/// are written top-to-bottom and left-to-right.  That is, elements 0           
/// through n of the valarray represent the first column of the matrix,         
/// elements n+1 through 2n represent the second column, and so on.             
///                                                                             
/// <b>Bounds Checking</b>                                                      
///                                                                             
/// There is none!  These functions make the highly dangerous assumption that   
/// the calling routine was written by someone who knows what he's doing.  As   
/// a result, the functions perform no checks of the size of the input          
/// valarrays; they simply assume that they are sufficiently large to support   
/// the requested operation.                                                    
///                                                                             
/// If the valarrays are too small, the output valarray's values cannot be      
/// predicted.  However, no memory corruption will occur (the input valarrays   
/// are not modified by any of these functions).  If the valarrays have a size  
/// of zero, calls to the multiplication functions will cause a crash if the    
/// resulting exceptions are not caught.                                        
///                                                                             
/// <b>Matrix Multiplication Rules</b>                                          
///                                                                             
/// These functions allow for three types of matrix multiplication operations:  
/// multiplication of 2 nxn square matrices, multiplication of a 1xn row matrix 
/// with an nxn square matrix, and multiplication of an nxn square matrix by an 
/// nx1 column matrix.  In each case, the value of "n" must be the same for     
/// both matrices in the operation and will be defined by the "order" argument  
/// of each function.                                                           
///                                                                             
/// Multiplication of, for example, a 3x3 matrix by a 5x5 matrix is not         
/// defined--not by these functions and not by the discipline of Linear         
/// Algebra.                                                                    
///                                                                             
/// @version    2010-02-17  DHF     v1.22   Moved from tools to lib namespace.  
///                                         Made compatible with lib code style.
/// @version    2009-01-22  AGB     v1.21   Added the interpolate function.     
/// @version    2009-01-07  AGB     v1.20   Added the mean and stddev functions.
/// @version    2008-12-31  AGB     v1.19   Happy Birthday to the American      
///                                         Nightmare!  Added the CloseEnough   
///                                         function.  (Please use it in cases  
///                                         when Round is not up to the task.)  
///                                         Discovered that VStudio 2008 reacts 
///                                         very badly to attempts to manipulate
///                                         empty valarrays.   Modified all     
///                                         routines that operate on valarrays  
///                                         to throw an exception when          
///                                         encountering an empty one.          
/// @version    2008-04-02  AGB     v1.18   Overloaded the checksum function.   
/// @version    2008-02-06  AGB     v1.17   Added the checksum function.        
/// @version    2008-01-09  AGB     v1.16   Overloaded the perpCalendar function
/// @version    2007-11-22  AGB     v1.15   Added the perpCalendar function.    
/// @version    2007-11-14  AGB     v1.14   Added the round function.           
/// @version    2007-06-12  AGB     v1.13   Tear Down This Wall!                
///                                         Added the makeDouble function.      
/// @version    2007-05-25  AGB     v1.12   Added the abs and reflect functions.
///                                         Fixed bu in the isQNaN function.    
/// @version    2006-11-24  AGB     v1.11   Added the isNaN, isSNaN, and isQNaN 
/// @version    2006-09-06  AGB     v1.10   @li Added tools::math namespace     
///                                         @li Moved to tools_math_himath.cpp  
///                                         @li Added add2x2, multi2sqares,     
///                                             multRowBySquare, multSquareByCol
///                                             , convLLE2EFG, rotateEFG2NED,   
///                                             and RSS                         
///                                         @li added definitions of Pi, Earth's
///                                             equatoral radius in feet, and   
///                                             the eccentricity of earth (the  
///                                             latter taken from WGS-84).      
/// @version    2005-10-14  AGB     v1.00   Original release.                   
//============================================================================= 
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#include <valarray>
#include <math.h>
#include <stdint.h>
#include <vector>
#include <deque>

#define REQ 20925646.04             ///!< earth's equatoral radius in feet      
//#define E 0.081991889                                                         
#define E 0.0818191908426215        ///!< Eccentricity of the earth (WGS-84)    
#define PI 3.1415926535897932       ///!< pi r !square, pi r round; cake r squae

namespace lib {
namespace math {

double abs(double val);
double round(const double val, const double prec, bool ipow10=true);
double round2Whole(const double val);
bool closeEnough(const double v1, const double v2, const double prec);

void perpCalendar(uint16_t jday, uint16_t *month, uint16_t *day, bool isleapyear=false);
uint16_t perpCalendar(uint16_t month, uint16_t day, bool isleapyear=false);

double MakeNaN();
double MakeSNaN();
double MakePosInf();
double MakeNegInf();

bool isNaN(double val);
bool isSNaN(double val);
bool isQNaN(double val);

std::valarray<double> add2x2(const std::valarray<double>& one, const std::valarray<double>& other);
std::valarray<double> mult2Squares(const std::valarray<double>& left, const std::valarray<double>& right, int order);
std::valarray<double> multRowBySquare(const std::valarray<double>& row, const std::valarray<double>& square, int order);
std::valarray<double> multSquareByCol(const std::valarray<double>& square, const std::valarray<double>& col, int order);

std::valarray<double> convLLE2EFG(double lat, double lon, double msl);
std::valarray<double> convLLE2EFG(const std::valarray<double>& in);
std::valarray<double> rotateEFG2NED(const std::valarray<double>& in, double lat, double lon);
std::valarray<double> rotateEB(const std::valarray<double>& earth, double pitch, double roll, double hdg);

double RSS(const std::valarray<double>& in, int n);

uint32_t reflect(uint32_t in, uint16_t sz);

double makeDouble(uint32_t msw, uint32_t lsw);

uint32_t checksum(void *data, uint16_t bufsize, uint16_t width); // width in bytes
uint32_t checksum(std::vector<uint32_t>& data, const uint16_t bufsize, uint16_t width);

double mean(std::deque<double> deq);
double stddev(std::deque<double> deq);
double interpolate( const double t1
                  , const double t2
                  , const double tx
                  , const double v1
                  , const double v2
                  );

} // namespace math                                                             
} // namespace lib                                                              

#endif // #ifndef LIB_MATH_HIMATH_H                                             
