//==============================================================================
//  The content of this file is UNCLASSIFIED                                    
//==============================================================================
//                                                                              
//  File:  lib_math_himath.cpp                                                  
//                                                                              
//  Purpose:                                                                    
//                                                                              
//      This file contains functions that perform mathematical computations.    
//                                                                              
//  Author:                                                                     
//                                                                              
//      Andrew Borman       AGB     andrew.g.borman@boeing.com                  
//                                                                              
//==============================================================================
#include "lib_math_himath.h"

#include <boost/static_assert.hpp>
#include <math.h>
#include <stdexcept>

namespace lib {
namespace math {

//------------------------------------------------------------------------------
//  Function:  Abs                                                              
//  Purpose:  Return the absolute value of a double, ensuring that non-zero     
//              input values have non-zero output values.                       
//------------------------------------------------------------------------------
/// \brief Return the absolute value of a double.                               
///                                                                             
/// The function is guaranteed to return a non-zero value for any non-zero      
/// input.  (As I write this, I don't remember what sort of casting issue       
/// burned me, but something in VStudio 7.1 did.)                               
///                                                                             
/// An almost totally useless feature of this function is that it will return   
/// positive infinity if the input is negative infinity.                        
//------------------------------------------------------------------------------
double abs(double val)
{
    union LETMESEE {double d; uint64_t i;};

    LETMESEE foo;
    foo.d = val;

    if (val < 0) {foo.i &= 0x7fffffffffffffff;}
    return foo.d;
}

//------------------------------------------------------------------------------
//  Function:  round                                                            
//  Purpose:  Return a value that is rounded to the precision specified in      
//              the second argument.                                            
//------------------------------------------------------------------------------
/// \brief Round a double to the specified precision.                           
///                                                                             
/// Rounding floating point values perfectly is not always possible.  This      
/// function biases the rounding so that any future truncation results in the   
/// appropriate value.                                                          
///                                                                             
/// The function will determine the precision represented by the second         
/// argument.  If the third argument is true, the function will define          
/// precision by integer powers of 10.  To round to the second digit after the  
/// decimal point, specify a second argument of 0.01.  If you do not use an     
/// integer power of 10 as the second argument, the function will choose a      
/// precision for you.  It will forgive small errors (e.g. 0.010001 == 0.01),   
/// but may behave unpredictably for larger ones (0.014 could go either way).   
///                                                                             
/// If the third argument is false, the function will round to whatever value   
/// is placed in the second argument.  Small errors will have consequences.     
//------------------------------------------------------------------------------
double round(const double val, const double prec, bool ipow10)
{
    double ans, realPrec, shifted;

    if (prec == 0)
    {
        return val;
    }
    if (ipow10)
    {
        realPrec = pow(10.0, round2Whole(log(prec)/log(10.0)));
        shifted = val/realPrec;
        ans = round2Whole(shifted) * (double)realPrec;
    }
    else
    {
        realPrec = prec;
        shifted = val/realPrec;
        ans = round2Whole(shifted) * realPrec;
    }

    //--------------------------------------------------------------------------
    //  Check the answer's 14th least significant digit.  If it is a 9, the     
    //  rounding was less than perfect.  Apply the smallest practical increment.
    //--------------------------------------------------------------------------
    double fourteenth, modulo, whocares;
    double magnitude;

    magnitude = round2Whole(log(ans)/log(10.0)) - 14;
    modulo = pow(10.0, magnitude);
    fourteenth = modf(ans/modulo, &whocares);

    while (fourteenth > 0.9) 
    {
        ans += modulo;
        fourteenth = modf(ans/modulo, &whocares);
    }

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  closeEnough                                                      
///@brief Return true if two doubles are very close or the same                 
///                                                                             
/// The function exists because I haven't figured out how to make the Round     
/// function perfect.  It isn't always necessary for two double values to be    
/// perfectly identical in order for them to be equal (e.g. 86400.000000000000  
/// and 86399.999999999999).  This function does a simple +//- check to see if  
/// they are close enough for government work.                                  
///                                                                             
/// The specific check is                                                       
///                                                                             
///@code                                                                        
/// return ( (v1 < v2 + prec) && (v1 > v2 - prec) )                             
///@endcode                                                                     
///                                                                             
/// Of course, a negative value for prec will cause the function to return      
/// false in all cases                                                          
//------------------------------------------------------------------------------
bool closeEnough(const double v1, const double v2, const double prec)
{
    return ( (v1 < v2 + prec) && (v1 > v2 - prec) );
}

//------------------------------------------------------------------------------
//  Function:  perpCalendar                                                     
//  Purpose:  Convert Julian day to month/date (both 1-indexed)                 
//------------------------------------------------------------------------------
/// @brief Convert a Julian day value to a month/date pair (Jan = 1).           
//------------------------------------------------------------------------------
void perpCalendar(
    uint16_t jday
  , uint16_t *month
  , uint16_t *day
  , bool isleapyear
) {
    int16_t DaysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int16_t accum=1;
    int16_t ignorant_debugger = 1;

    if (jday > 366 || (!isleapyear && jday > 365))
    {
        throw std::runtime_error("lib::math::perpCalendar: Julian Day value too large");
    }

    if (isleapyear) {DaysPerMonth[1] = 29;}

    while (accum <= jday && ignorant_debugger <= 12)
    {
        *day = jday - accum + 1;
        accum += DaysPerMonth[ignorant_debugger-1];
        if (*day > DaysPerMonth[ignorant_debugger-1]) {ignorant_debugger++;}
    }

    *month = ignorant_debugger;
}

//------------------------------------------------------------------------------
//  Function:  PerpCalendar                                                     
//  Purpose:  Convert a month and year to Julain date (all 1-indexed)           
//------------------------------------------------------------------------------
/// @brief Convert a month/year to Julian date (all 1-indexed)                  
//------------------------------------------------------------------------------
uint16_t perpCalendar(uint16_t month, uint16_t day, bool isleapyear)
{
    int16_t ans=0;
    int16_t DaysPerMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month > 12 || month == 0)
    {
        throw std::runtime_error(
            "lib::math::perpCalendar:  Invalid value for month"
        );
    }

    if (isleapyear) {DaysPerMonth[2] = 29;}

    if (day > DaysPerMonth[month] || day == 0)
    {
        throw std::runtime_error(
            "lib::math::PerpCalendar:  Invalid value for day"
        );
    }

    for (int i=1 ; i < month ; i++) {ans += DaysPerMonth[i];}

    ans += day;

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  Round2Whole                                                      
//  Purpose:  Return a value that is rounded to the nearest whole number.       
//------------------------------------------------------------------------------
/// @brief Round a double to the nearest whole number                           
//------------------------------------------------------------------------------
double round2Whole(const double val)
{
    return floor(val + 0.5);

    //  Andy, check out if you approve the above code.                          

    /*
    double wholenum = val/1.0;
    double frac = fmod(val, 1.0);
    if (abs(frac) >= 0.5)
    {
        if (val > 0) {wholenum++;}
        else {wholenum--;}
    }

    return int64_t(wholenum);
    */
}

//------------------------------------------------------------------------------
//  Function:  MakeNaN                                                          
//  Purpose:  Return a value that is a Quiet NaN.                               
//------------------------------------------------------------------------------
/// @brief Returns the double representation of Quiet NaN.                      
//------------------------------------------------------------------------------
double makeNaN()
{
    union {
        uint32_t nan[2];
        double  d;
    } x = {{0x00000000, 0x7fff0000}};
    BOOST_STATIC_ASSERT(sizeof(double) == 8);

    return x.d;
}

//------------------------------------------------------------------------------
//  Function:  MakeSNaN                                                         
//  Purpose:  Return a value that is a Signal NaN.                              
//  Note:  VStudio doesn't seem to distinguish between Quiet and Signal NaN's.  
//          This function may be superfluous.                                   
//  Correction (25 May 07):  VStudio (or Windows or the Intel chipset)          
//      prohibits the bit pattern that makes up an SNaN.  Bit 3 of the MSW is   
//      forced high if the rest of the byte indicates a NaN.  The output of     
//      this function is converted to QNaN whether I like it or not             
//------------------------------------------------------------------------------
/// @brief  Returns the double representation of Quiet NaN.  (You read that     
///         correctly)                                                          
///                                                                             
/// @warning    VStudio v7.1 (or Windows or the Intel chipset) won't allow the  
///             SNaN bit pattern to be stored in a double.  The output of this  
///             function is converted to a QNaN whether I like it or not!       
//------------------------------------------------------------------------------
double makeSNaN()
{
    union {
        uint32_t nan[2];
        double   d;
    } x = {{0x00000000, 0x7ff70000}};
    return x.d;
}

//------------------------------------------------------------------------------
//  Function:  IsSNaN                                                           
//------------------------------------------------------------------------------
/// @brief Returns false under VStudio 7.1/Windows/Intel.                       
///                                                                             
/// @warning    I'm not sure who the culprit is, but any attempt to store a bit 
///             pattern that represents an SNaN fails.  The bit pattern is      
///             changed to QNaN.  (See the makeSNaN function.)                  
//------------------------------------------------------------------------------
bool isSNaN(double val)
{
    union LETMESEE {double d; uint64_t i;};
    bool ans=false;

    LETMESEE foo;
    foo.d = val;

    if (foo.i == 0x7ff7000000000000) {ans = true;}
    return ans;
}

//------------------------------------------------------------------------------
//  Function:  IsQNaN                                                           
//------------------------------------------------------------------------------
/// @brief Returns true if the argument is a Quiet Not-a-Number.                
//------------------------------------------------------------------------------
bool isQNaN(double val)
{
    union LETMESEE {double d; uint64_t i;};
    bool ans=false;

    LETMESEE foo;
    foo.d = val;

    if (foo.i == 0x7fff000000000000) {ans = true;}
    return ans;
}

//------------------------------------------------------------------------------
//  Function:  IsNaN                                                            
//------------------------------------------------------------------------------
/// @brief Returns true if the argument is a Not-a-Number (NaN).                
//------------------------------------------------------------------------------
bool isNaN(double val)
{
    union LETMESEE {double d; uint64_t i;};
    bool ans=false;

    LETMESEE foo;
    foo.d = val;

    ans = (foo.i == 0x7fff000000000000);

    return ans || isQNaN(val) || isSNaN(val);
}

//------------------------------------------------------------------------------
//  Function:  MakePosInf                                                       
//  Purpose:  Return a value that is positive infinity.                         
//------------------------------------------------------------------------------
/// @brief Returns the double representation of positive infinity.              
//------------------------------------------------------------------------------
double makePosInf()
{
    union {
        uint32_t pi[2];
        double d;
    } x = {{0x00000000, 0x7ff00000}};

    return x.d;
}

//------------------------------------------------------------------------------
//  Function:  MakeNegInf                                                       
//  Purpose:  Return a value that is negative infinity.                         
//------------------------------------------------------------------------------
/// @brief Returns the double representation of negative infinity.              
//------------------------------------------------------------------------------
double makeNegInf()
{
    union {
        uint32_t ni[2];
        double d;
    } x = {{0x00000000, 0xfff00000}};

    return x.d;
}

//------------------------------------------------------------------------------
//  Function:  Add2x2                                                           
//  Purpose:  Add a pair of 2x2 matrices                                        
//  Design Note:  The function performs no checks on the size of the arguments. 
//                  It simply assumes the calling routine was smart enough to   
//                  specify appropriately sized valarrays.                      
//  Reality Check:  The implmentation makes it clear that this is an            
//                  unnecessary function.                                       
//------------------------------------------------------------------------------
///@brief Returns the sum of 2 2x2 matrices as a valarray containing 4 doubles. 
///                                                                             
/// This function is entirely superfluous.  If you need to add two matrices     
/// represented by a pair of valarrays named "one" and "other", I would suggest 
/// one of the following two approaches:                                        
///                                                                             
///@code                                                                        
/// one += other;                                                               
///@endcode                                                                     
///                                                                             
/// or                                                                          
///                                                                             
///@code                                                                        
/// std::valarray foo(n); // where n is the number of elements in the matrix    
/// foo = one + other;                                                          
///@endcode                                                                     
///                                                                             
/// The only reason I haven't deleted the function because its documentation    
/// gives me the opportunity to point out that matrix addition functions are    
/// unnecessary when you use valarrays.                                         
///                                                                             
/// The function will throw a runtime_error if either of the input valarrays    
/// is empty.                                                                   
//------------------------------------------------------------------------------
std::valarray<double> add2x2(
    const std::valarray<double>& one
  , const std::valarray<double>& other
)
{
    if (one.size() == 0 || other.size() == 0)
    {
        throw std::runtime_error(
            "lib::math::add2x2: Attempt to operate on an empty valarray"
        );
    }

    std::valarray<double> ans = one + other;
    return ans;
}

//------------------------------------------------------------------------------
//  Function:  Mult2Squares                                                     
//  Purpose:  Multiply a pair of square matrices of any order                   
//------------------------------------------------------------------------------
/// @brief  Returns the product of two square matrices of equal size as a       
///         valarray containing order*order elements.                           
///                                                                             
/// The third argument defines the number of rows in the matrix (i.e. the order 
/// of the matrix).  The first two arguments are the matrices to be multiplied  
/// (remember that matrix multiplication is order dependent).                   
///                                                                             
/// The function will throw a runtime_error if either of the input valarrays    
/// is empty.                                                                   
//------------------------------------------------------------------------------
std::valarray<double> mult2Squares(
    const std::valarray<double>& left
  , const std::valarray<double>& right
  , int order
)
{
    if (left.size() == 0 || right.size() == 0)
    {
        throw std::runtime_error(
            "lib::math::mult2Squares: Attempt to operate on an empty valarray"
        );
    }

    std::valarray<double> ans(0.0, order*order);
    std::valarray<double> somevec(order);

    for (int i=0 ; i < order*order ; i++)
    {
        somevec = left[std::slice(i%order, order, order)];
        somevec *= right[std::slice(i/order * order, order, 1)];

        for (int j=0 ; j < order ; j++) {ans[i] += somevec[j];}
    }

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  MultRowBySquare                                                  
//  Purpose:  Multiply a row matrix by a square matrix of the same order        
//------------------------------------------------------------------------------
/// \brief Returns the product of a 1xn row matrix and an nxn square matrix     
/// as a valarray containing n^2 elements.                                      
///                                                                             
/// The third argument, order, indicates the number of elements in the row      
/// matrix and the order of the square matrix.  As with any matrix              
/// multiplication, remember that the operation is order dependent.             
///                                                                             
/// The function will throw a runtime_error if either of the input valarrays    
/// is empty.                                                                   
//------------------------------------------------------------------------------
std::valarray<double> multRowBySquare(
    const std::valarray<double>& row
  , const std::valarray<double>& square
  , int order
)
{
    if (row.size() == 0 || square.size() == 0)
    {
        throw std::runtime_error(
            "lib::math::multRowBySquare: Attempt to operate on empty valarray"
        );
    }

    std::valarray<double> ans(0.0, order);
    std::valarray<double> somevec(order);

    for (int i=0 ; i < order ; i++)
    {
        somevec = row;
        somevec *= square[std::slice(i * order, order, 1)];

        for (int j=0 ; j < order ; j++) {ans[i] += somevec[j];}
    }

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  MultSquareByCol                                                  
//  Purpose:  Multiply a square matrix by a column matrix of the same order     
//------------------------------------------------------------------------------
/// \brief Returns the product of an nxn square matrix and an nx1 column matrix 
/// as a column matrix containing n elements.                                   
///                                                                             
/// The third argument, order, indicates the number of elements in the column   
/// matrix and the order of the square matrix.  As with any matrix              
/// multiplication, remember that the operation is order dependent.             
///                                                                             
/// The function will throw a runtime_error if either of the input valarrays    
/// is empty.                                                                   
//------------------------------------------------------------------------------
std::valarray<double> multSquareByCol(
    const std::valarray<double>& square
  , const std::valarray<double>& col
  , int order
)
{
    if (square.size() == 0 || col.size() == 0)
    {
        throw std::runtime_error(
            "lib::math::multSquareByCol: Attempt to operate on empty valarray"
        );
    }

    std::valarray<double> ans(0.0, order);
    std::valarray<double> somevec(order);

    for (int i=0 ; i < order ; i++)
    {
        somevec = square[std::slice(i%order, order, order)];
        somevec *= col;

        for (int j=0 ; j < order ; j++) {ans[i] += somevec[j];}
    }

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  ConvLLE2EFG                                                      
//  Purpose:  Convert a coordinate defined by geodetic latitude and longitude   
//              and altitude above MSL to earth-centered-earth-fixed            
//------------------------------------------------------------------------------
/// \brief Converts a coordinate made up of geodetic latitude, geodetic         
/// longitude, hieght above MSL to earth-center-earth-fixed coordinates         
///                                                                             
/// The function expects to see latitude and longitude in degrees and altitude  
/// above MSL in feet.  The function returns a valarray with three members:  E, 
/// F, and G.  The E axis starts at the center of the Earth and points to the   
/// equator a 0 degrees longitude.  F points to the equator at 90 degrees       
/// longitude.  G points to Santa's Workshop at the North Pole.                 
//------------------------------------------------------------------------------
std::valarray<double> convLLE2EFG(double lat, double lon, double msl)
{
    std::valarray<double> ans(0.0, 3);

    double xyfact = sqrt(1 - pow(E*sin(lat * PI/180.0), 2.0));
    double zfact = REQ*(1 - pow(E, 2.0)) / xyfact;

    ans[0] = (REQ/xyfact + msl) * cos(lon * PI/180.0) * cos(lat * PI/180.0);
    ans[1] = (REQ/xyfact + msl) * sin(lon * PI/180.0) * cos(lat * PI/180.0);
    ans[2] = (zfact + msl) * sin(lat * PI/180.0);

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  ConvLLE2EFG                                                      
//------------------------------------------------------------------------------
/// \brief Converts lat, lon, msl coordinates to earth-center-earth fixed.      
///                                                                             
/// This function overloads ConvLLE2EFG with a valarray input.  The function    
/// assumes that element 0 is latitude in degrees, 1 is longitude in degrees,   
/// and 2 is altitude above msl in feet.                                        
///                                                                             
/// The function will throw a runtime_error if the input valarray is empty.     
//------------------------------------------------------------------------------
std::valarray<double> convLLE2EFG(const std::valarray<double>& in)
{
    if (in.size() == 0)
    {
        throw std::runtime_error(
            "lib::math::convLLE2EFG: Attempt to operate on an empty valarray"
        );
    }

    return convLLE2EFG(in[0], in[1], in[2]);
}

//------------------------------------------------------------------------------
//  Function:  RotateEFG2NED                                                    
//  Purpose:  Rotate a vector in earth-centered-earth-fixed coordinates to      
//              a local NED coordinate system                                   
//------------------------------------------------------------------------------
/// \brief Rotates a vector in earth-center-earth-fixed coordinates to a local  
/// north-east-down system.                                                     
///                                                                             
/// The function expects latitude and longitude to be expressed in degrees.     
/// The function will return a valarray with element 0 representing the north   
/// component of the vector, 1 representing the east, and 2 representing down.  
/// The units of the vector output by the function will be identical to the     
/// units input to the function.                                                
///                                                                             
/// The function will throw a runtime_error if the input valarray is empty.     
//------------------------------------------------------------------------------
std::valarray<double> rotateEFG2NED(
    const std::valarray<double>& in
  , double lat
  , double lon
)
{
    if (in.size() == 0)
    {
        throw std::runtime_error(
            "lib::math::rotateEFG2NED: Attempt to operate on an empty valarray"
        );
    }

    std::valarray<double> ans(0.0, 3);
    std::valarray<double> rotmat(0.0, 9);

    double latrad = lat*PI/180.0;
    double lonrad = lon*PI/180.0;
    double m1 = -1.0;   // force the compiler to use double precision everywhere, even on -1

    rotmat[0] = m1 * sin(latrad) * cos(lonrad);
    rotmat[1] = m1 * sin(lonrad);
    rotmat[2] = m1 * cos(latrad) * cos(lonrad);
    rotmat[3] = m1 * sin(latrad) * sin(lonrad);
    rotmat[4] = cos(lonrad);
    rotmat[5] = m1 * cos(latrad) * sin(lonrad);
    rotmat[6] = cos(latrad);
    rotmat[7] = 0;
    rotmat[8] = m1 * sin(latrad);

    ans = multSquareByCol(rotmat, in, 3);

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  RotateEB                                                         
//  Purpose:  Rotate a vector from local NED to aircraft body coordinates       
//              (i.e. Forward-Right-Down)                                       
//------------------------------------------------------------------------------
/// \brief Rotates a vector in north-east-down coordinates to aircraft          
/// forward-right-down.                                                         
///                                                                             
/// The function expects all angles in degrees.  The function will return a     
/// valarray with element 0 representing forward, element 1 right, and element  
/// 2 down.                                                                     
///                                                                             
/// The function will throw a runtime_error if the input valarray is empty.     
//------------------------------------------------------------------------------
std::valarray<double> rotateEB(
    const std::valarray<double>& earth
  , double pitch
  , double roll
  , double hdg
)
{
    if (earth.size() == 0)
    {
        throw std::runtime_error(
            "lib::math::rotateEB: Attempt to operate on an empty valarray"
        );
    }

    std::valarray<double> ans(0.0, 3);
    std::valarray<double> rotmat(0.0, 9);

    double prad = pitch*PI/180.0;
    double rrad = roll*PI/180.0;
    double hrad = hdg*PI/180.0;
    double m1 = -1.0;

    //--------------------------------------------------------------------------
    //  First, rotate by the heading angle                                      
    //--------------------------------------------------------------------------
    rotmat[0] = cos(hrad);
    rotmat[1] = m1 * sin(hrad);
    rotmat[2] = 0;
    rotmat[3] = sin(hrad);
    rotmat[4] = cos(hrad);
    rotmat[5] = 0;
    rotmat[6] = 0;
    rotmat[7] = 0;
    rotmat[8] = 1;

    ans = multSquareByCol(rotmat, earth, 3);

    //--------------------------------------------------------------------------
    //  Next, rotate by the pitch angle                                         
    //--------------------------------------------------------------------------
    rotmat[0] = cos(prad);
    rotmat[1] = 0;
    rotmat[2] = sin(prad);
    rotmat[3] = 0;
    rotmat[4] = 1;
    rotmat[5] = 0;
    rotmat[6] = m1 * sin(prad);
    rotmat[7] = 0;
    rotmat[8] = cos(prad);

    ans = multSquareByCol(rotmat, ans, 3);

    //--------------------------------------------------------------------------
    //  Last, rotate by the roll angle and return the answer                    
    //--------------------------------------------------------------------------
    rotmat[0] = 1;
    rotmat[1] = 0;
    rotmat[2] = 0;
    rotmat[3] = 0;
    rotmat[4] = cos(rrad);
    rotmat[5] = m1 * sin(rrad);
    rotmat[6] = 0;
    rotmat[7] = sin(rrad);
    rotmat[8] = cos(rrad);

    ans = multSquareByCol(rotmat, ans, 3);

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  RSS                                                              
//  Purpose:  Compute the root-sum-of-squares of the first n elements of a      
//              valarray                                                        
//------------------------------------------------------------------------------
/// \brief Computes the root-sum-of-squares of the first n elements of a        
/// valarray                                                                    
///                                                                             
/// Think of this function as a way to calculate vector magnitude.  If the      
/// second argument is 0, the function will return 0.                           
///                                                                             
/// The function will throw a runtime_error if the input valarray is empty.     
//------------------------------------------------------------------------------
double RSS(const std::valarray<double>& in, int n)
{
    if (in.size() == 0)
    {
        throw std::runtime_error(
            "lib::math::RSS: Attempt to operate on an empty valarray"
        );
    }

    double ans=0;

    for (int i=0 ; i < n ; i++) {ans += pow(in[i], 2.0);}

    return sqrt(ans);
}

//------------------------------------------------------------------------------
//  Function:  Reflect                                                          
//  Purpose:  Reverse the order of the bits in an unsigned long.  The width     
//          of the bit string to be reversed is given as the second argument.   
//------------------------------------------------------------------------------
/// \brief Reverse the order of the bits in an unsigned long.                   
///                                                                             
/// The second argument contains the width of the bit string to be reversed,    
/// beginning with the LSB.  MSB's above that string of bits in the input       
/// value will be ignored.  For example:                                        
///                                                                             
///@code                                                                        
/// unsigned long int test = 0x03300033;                                        
/// ASSERT(Reflect(test, 32) == 0xcc000cc0);    // expected                     
/// ASSERT(Reflect(test, 16) == 0x0000cc00);    // probably a surprise          
///@endcode                                                                     
//------------------------------------------------------------------------------
uint32_t reflect(uint32_t in, uint16_t sz)
{
    uint32_t ans=0, mask;

    for (int i=0 ; i < sz ; i++)
    {
        mask = (1L << i);
        if (in & mask)
        {
            ans |= (1L << (sz-i-1) );
        }
    }

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  MakeDouble                                                       
//  Purpose:  Concatenate two longs into a single double                        
//------------------------------------------------------------------------------
/// \brief Concatenate two long values into a double-precision floating-point   
//------------------------------------------------------------------------------
double makeDouble(uint32_t msw, uint32_t lsw)
{
    union {
        uint32_t i[2];
        double   d;
    } x;
    x.i[0] = lsw;
    x.i[1] = msw;

    return x.d;
}

//------------------------------------------------------------------------------
//  Function:  Checksum                                                         
//  Purpose:  Computes the arithmetic checksum of the content of a buffer       
//------------------------------------------------------------------------------
/// \brief Computes the arithmetic checksum of the content of a buffer          
///                                                                             
/// The first argument is a pointer to the buffer.  The second describes how    
/// large the buffer is.  The third describes the width of the buffer in bytes. 
/// What that all means is that the last argument tells us whether we're        
/// dealing with bytes, 16-bit words, or 32-bit words (nothing else is          
/// implemented yet).  The second argument tells us how many numbers we're      
/// going to have to sum.  The first argument tells us where they are.          
///                                                                             
/// If the checksum cannot be computed, the value 0 is returned.  This is a     
/// little dangerous, but I'm willing to take the risk.                         
//------------------------------------------------------------------------------
uint32_t checksum(void *data, uint16_t bufsize, uint16_t width)
{
    uint32_t i, ans=0;

    if (width != 1 && width != 2 && width != 4) {return ans;}
    else if (width == 1)
    {
        uint8_t *zData = (uint8_t*) data;
        uint8_t csum = zData[0];

        for (i=1 ; i < bufsize ; i++) {csum += zData[i];}
        ans = csum;
    }
    else if (width == 2)
    {
        uint16_t *zData = (uint16_t*) data;
        uint16_t csum = zData[0];

        for (i=1 ; i < bufsize ; i++) {csum += zData[i];}
        ans = csum;
    }
    else if (width == 4)
    {
        uint32_t *zData = (uint32_t*) data;
        ans = zData[0];
        for (i=1 ; i < bufsize ; i++) {ans += zData[i];}
    }

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  Checksum                                                         
//------------------------------------------------------------------------------
/// \brief Computes the arithmetic checksum of the content of a vector of       
///     32-bit unsigned int                                                     
///                                                                             
/// This time around, we've got a vector of uint32_t that we may or may not     
/// want to treat as uint32_t (we may want to treat it as uint16_t or uint8_t). 
/// The third argument indicates the width to be used in bytes.  The second     
/// argument still tells us how many numbers to sum and the first argument      
/// still tells us where to find the data.                                      
///                                                                             
/// If the checksum cannot be computed, the value 0 is returned.  By the same   
/// token, if the values of bufsize and width indicate more data than there is  
/// in the vector, the value 0 will be returned.  This is a little dangerous,   
/// but I'm willing to take the risk.                                           
//------------------------------------------------------------------------------
uint32_t checksum(
    std::vector<uint32_t>& data
  , uint16_t bufsize
  , uint16_t width
)
{
    uint32_t ans=0;

    if (width != 1 && width != 2 && width != 4) {return ans;}
    else if (width == 1)
    {
        if (size_t(bufsize/4) > data.size()) {return ans;}
        for (uint16_t i=0 ; i < bufsize ; i+=4)
        {
            ans += uint8_t(data[i/4] & 0x000000ff);
            if (i+1 < bufsize) {ans += uint8_t( (data[i/4] & 0x0000ff00) >> 8);}
            if (i+2 < bufsize) {ans += uint8_t( (data[i/4] & 0x00ff0000) >> 16);}
            if (i+3 < bufsize) {ans += uint8_t( (data[i/4] & 0xff000000) >> 24);}
        }
    }
    else if (width == 2)
    {
        if (size_t(bufsize/2) > data.size()) {return ans;}
        for (uint16_t i=0 ; i < bufsize ; i+=2)
        {
            ans += uint16_t(data[i/2] & 0x0000ffff);
            if (i+1 < bufsize) {ans += uint16_t( (data[i/2] & 0xffff0000) >> 16);}
        }
    }
    else if (width == 4)
    {
        if (bufsize > data.size()) {return ans;}
        for (int i=0 ; i < bufsize ; i++) {ans += data[i];}
    }

    return ans;
}

//------------------------------------------------------------------------------
//  Function:  mean                                                             
///@brief Computes the arithmetic mean of of all the elements of a deque        
//------------------------------------------------------------------------------
double mean(std::deque<double> deq)
{
    double total=0;

    for (size_t i=0 ; i < deq.size() ; i++) {total += deq[i];}

    return total/((double)deq.size());
}

//------------------------------------------------------------------------------
//  Function:  stddev                                                           
///@brief Computes the standard deviation of all elements of a deque            
///                                                                             
/// It should go without saying, but to compute the sample deviation of the     
/// elements of the deque, square the output of this function.                  
//------------------------------------------------------------------------------
double stddev(std::deque<double> deq)
{
    double sumSquares=0, sumDeq=0, sampvar, divisor=deq.size()-1;
    for (size_t i=0 ; i < deq.size() ; i++)
    {
        sumDeq += deq[i];
        sumSquares += pow(deq[i], 2.0);
    }
    sampvar = (sumSquares - (pow(sumDeq, 2.0))/deq.size()) / divisor;
    return sqrt(sampvar);
}

//------------------------------------------------------------------------------
//  Function:  Interpolate                                                      
///@brief Interpolates the values of the fourth and fifth argument as dictated  
///     by the first three.                                                     
///                                                                             
/// The function assumes that you are interpolating discrete data values to a   
/// time value that lies between them.  The first two arguments are the time    
/// values for which data values are known.  The third argument is the time     
/// value for which the data value is unknown.  The fourth and fifth arguments  
/// are the data values sampled at the times represented by the first and       
/// second arguments, respectively.                                             
///                                                                             
/// Of course, this function does not require the input values to be either     
/// time-based or interpolated.  It will just as happily perform spatial        
/// extrapolation.                                                              
///                                                                             
/// The function is also smart enough to not try to divide by zero.  If the     
/// values of the first two arguments are identical, the function will return   
/// the value of the fifth argument.                                            
//------------------------------------------------------------------------------
double interpolate( const double t1
                  , const double t2
                  , const double tx
                  , const double v1
                  , const double v2
                  )
{
    double ans=v2;

    if (t1 != t2) {ans = (v2 - v1) * (tx - t1) / (t2 - t1) + v1;}

    return ans;
}

} // namespace math                                                             
} // namespace lib                                                              
