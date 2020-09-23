//------------------------------------------------------------------------------
///@file lib_string.cpp                                                         
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///@brief A collection of funcitons that operate on std::string                 
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2017-05-31  MNT         Renamed                                     
///                                   - setFixedCharArray() toFixedCharArray()  
///                                   - getFixedCharArray() fromFixedCharArray()
///                                                                             
///@version 2017-05-17  MNT         Added setFixedCharArray & getFixedCharArray 
///                                                                             
///@version 2014-12-03  AGB         If toUnsigned receives an empty string,     
///                                 immediately return 0.                       
///                                                                             
///@version 2014-11-20  DHF         Modified lib::toUnsigned based on bug found 
///                                 by AGB; under Visual Studio, it would crash 
///                                 on:  lib::toUnsigned("0",0);                
///                                                                             
///@version 2014-10-01  DHF         Rewrote toSigned / toUnsigned.              
///                                                                             
///@version 2014-01-29  DHF         Changed from unsafe vsprintf to vsnprintf.  
///                                                                             
///@version 2012-04-04  AGB v1.00   Merged Andy's string manipulation stuff     
///                                 with Dave's                                 
//------------------------------------------------------------------------------
#include "lib_compiler_info.h"
#include "lib_string.h"

#include <assert.h>
#include <boost/algorithm/string.hpp>
#include <math.h>                       // pow                  
#include <stdarg.h>                     // va<mumble> routines  
#include <stdio.h>
#include <stdexcept>
#include <string.h>

namespace lib {

//----------------------------------------------------------------------------- 
///@brief Returns true if the string contains at least one digit (0-9).         
//----------------------------------------------------------------------------- 
bool hasDigit(const std::string& str)
{
    bool ans=false;
    for (size_t i = 0 ; i < str.length() && !ans ; i++)
    {
        if (isdigit(str[i])) {ans = true;}
    }
    return ans;
}

//----------------------------------------------------------------------------- 
///@brief Returns true if the string contains only letters.                     
///                                                                             
/// The function applies the C Standard Library functions isalpha, isspace, and 
/// ispunct to make its determination.                                          
//----------------------------------------------------------------------------- 
bool isAllAlpha(const std::string& str, bool allowspace, bool allowpunct)
{
    bool ans=true;
    for (size_t i=0 ; i < str.length() && ans ; i++)
    {
        if (!isalpha(str[i]))
        {
            if (allowspace && isspace(str[i])) {ans = true;}
            else if (allowpunct && ispunct(str[i])) {ans = true;}
            else {ans = false;}
        }
    }
    return ans;
}

//----------------------------------------------------------------------------- 
///@brief Returns true if the string contains only the digits 0-9.              
//----------------------------------------------------------------------------- 
bool isAllDigit(const std::string& str)
{
    std::string::const_iterator s;
    bool ans = true;

    for (s = str.begin(); ans && s != str.end(); ++s) 
    {
        ans = isdigit(*s);
    }

    return ans;

    #if 0
    bool ans=true;
    for (int i=0 ; ans && i < str.length() && ans ; i++)
    {
        ans = isdigit(str[i]);
        if (!isdigit(str[i])) {ans = false;}
    }
    return ans;
    #endif
}

//------------------------------------------------------------------------------
/// Return true if the given string constains only alpha and numeric characters 
//------------------------------------------------------------------------------
bool isAlphaNumeric(const std::string& str)
{
    std::string::const_iterator s;
    bool ans = true;

    for (s = str.begin(); ans && s != str.end(); ++s) 
    {
        ans = 
            (*s >= '0' && *s <= '9') 
         || (*s >= 'a' || *s <= 'z') 
         || (*s >= 'A' || *s <= 'Z') 
        ;
    }
    return ans;
}

//----------------------------------------------------------------------------- 
///@brief Returns true if the string contains a binary value                    
//----------------------------------------------------------------------------- 
bool isBinary(const std::string& str)
{
    #if 0
    bool ans=false;
    std::string AllowedChars = "01";

    if (str.find_first_not_of(AllowedChars) == str.npos) {ans = true;}

    return ans;
    #endif
    return (str.find_first_not_of("01") == str.npos);
}

//----------------------------------------------------------------------------- 
///@brief Returns true if the string contains a hex value                       
///                                                                             
/// The method will allow a leading 0x but does not require it.  The check is   
/// case insensitive.                                                           
//----------------------------------------------------------------------------- 
bool isHex(const std::string& str)
{
//  bool ans=false;                                                              
    unsigned int startat=0;
    static const std::string AllowedChars = "0123456789abcdefABCDEF";

    if (str.substr(0, 2) == "0x") {startat=2;}
    #if 0
    if (str.find_first_not_of(AllowedChars, startat) == str.npos) {ans = true;}

    return ans;
    #endif
    return (str.find_first_not_of(AllowedChars, startat) == str.npos);
}

//----------------------------------------------------------------------------- 
///@brief Returns true if the string contains a valid representation of a       
/// number.                                                                     
///                                                                             
/// The function is smart enough to recognize scientific notation (case-        
/// insensitive on the "e").  It's a bit flawed in that it doesn't pay close    
/// attention where minus signs occur.  The function will return true for both  
/// of these strings:  "5-5", "4e-5-8".  The function does not consider spaces  
/// to be valid components of a number.  The function will return false for     
/// each of the following strings:  " 42", "42 ", "123 456".                    
///                                                                             
/// Added in v1.15 - The "5-5" and "4e-5-8" bugs have been fixed.               
///                                                                             
///@todo Rewrite the stuff above and jetison the historical commentary.         
//----------------------------------------------------------------------------- 
bool isNumber(const std::string& str, bool allowcomma)
{
    bool ans=true;

    if (str.substr(0,2) != "0x" || !isHex(str)) 
    {
        int dotct=0, signct=0, scinotect=0;

        for (size_t i=0 ; i < str.length() && ans ; i++)
        {
            if (allowcomma && str[i] == ',') {ans = true;}
            else if (isdigit(str[i]))
            {
                if (signct == 0) {signct = 1;}  // don't allow a sign after the first digit
            }
            else if (str[i] == '+' || str[i] == '-')
            {
                signct++;
                if (signct > 1) {ans = false;}
            }
            else if (str[i] == '.')
            {
                dotct++;
                if (dotct > 1) {ans = false;}
            }
            else if (str[i] == 'e' || str[i] == 'E')
            {
                if (i+1 >= str.length()) {ans=false;}
                else
                {
                    if (str[i+1] == '+' || str[i+1] == '-' || isdigit(str[i+1]))
                    {
                        scinotect++;
                        if (signct == 0) {signct++;} // to fix the "4e-5-8" bug
                        if (!isdigit(str[i+1])) {i++;}
                        if (scinotect > 1) {ans = false;}
                    }
                    else {ans = false;}
                }
            }
            else {ans = false;}
        }
    }

    return ans;
}

//----------------------------------------------------------------------------- 
///@brief Returns true if the string contains an octal value                    
//----------------------------------------------------------------------------- 
bool isOctal(const std::string& str)
{
    #if 0
    bool ans=false;
    std::string AllowedChars = "01234567";

    if (str.find_first_not_of(AllowedChars) == str.npos) {ans = true;}

    return ans;
    #endif

    return str.find_first_not_of("01234567") == str.npos;
}

//------------------------------------------------------------------------------
///@brief Inserts commas (or other characters) in a string                      
///                                                                             
/// The function is smart enough to recognize a decimal point (.) and omit      
/// commas to the right of it.  It's also smart enough to recognize a leading   
/// minus sign and suppress commas to its right.                                
///                                                                             
/// On the other hand, the function does not pay attention to the content of    
/// the other characters in the string.  As a result, commas("abcdefg") will    
/// return the string "a,bcd,efg".  One benefit of this is that by changing the 
/// default arguments for delimiter and count you can convert (c4b890ed to      
/// c4b8 90ed).                                                                 
///                                                                             
/// The function is overloaded to accept string, signed and unsigned int, and   
/// double.  For a double, the formatting directive %f is applied rather than   
/// %e or %g.  The rationale is that no one would want to add commas to a       
/// scientific notation representation of a number.                             
///                                                                             
/// Also, because the %f directive's default behavior is actually %.6f, the     
/// value will be zero-padded to six digits after the decimal.  Don't like it?  
/// Try this:                                                                   
///                                                                             
///@code                                                                        
/// double dontPadMe = 12345.67;                                                
/// std::string butIWantCommas = lib::comma(lib::format("%.2f", dontPadMe));    
///@endcode                                                                     
//------------------------------------------------------------------------------
std::string comma(const std::string& str, const std::string& delim, int count)
{
    std::string ans=str;
    int decimal = static_cast<int>(str.size());
    int negative = 0;

    if (str.find('.') != str.npos) {decimal = static_cast<int>(str.find('.'));}
    if (str[0] == '-') {negative=1;}

    for (int i=decimal-count ; i > negative ; i -= count)
    {
        ans.insert(i, delim);
    }
    
    return ans;
}

#if 0
std::string comma(int64_t val, const std::string& delim, int count)
{
    return comma(format("%" PRId64, val), delim, count);
}

std::string comma(uint64_t val, const std::string& delim, int count)
{
    return comma(format("%" PRIu64, val), delim, count);
}

std::string comma(double val, const std::string& delim, int count)
{
    int dig=6;
    std::string directive = format("%%.%if", dig);
    std::string ans = format("%f", val);
    return comma(ans, delim, count);
}
#endif

//----------------------------------------------------------------------------- 
/// @brief Applies the format command to a std::string.                         
///                                                                             
/// Use this function with caution.  Unlike commands like sprintf and           
/// CString::Format the compiler cannot validate your argument list.  If you    
/// make a mistake, you will likely discover it at runtime when your software   
/// crashes.                                                                    
///                                                                             
/// Why const char* and not const std::string& as the input?  The vsprintf      
/// function doesn't seem to like reading its formatting directive out of       
/// std::string::c_str().                                                       
//----------------------------------------------------------------------------- 
#ifdef IS_VISUAL_STUDIO
std::string format(const std::string format, ...)
#else
std::string format(const std::string& format, ...)
#endif
{
    char ans[32768];
    
    va_list my_va_list;
    va_start(my_va_list, format);
    vsnprintf(ans, sizeof(ans), format.c_str(), my_va_list);
    va_end(my_va_list);

    return std::string(ans);
}

//------------------------------------------------------------------------------
///@brief Converts a string value to a double                                   
//------------------------------------------------------------------------------
double toDouble(const std::string& str)
{
    double ans;
    assert(isNumber(str));
    sscanf(str.c_str(), "%lg", &ans);
    return ans;
}

//------------------------------------------------------------------------------
///@brief Converts a string value to a signed int                               
///                                                                             
///         If the value of the individual character is greater than the base   
///         then parsing stops.                                                 
///                                                                             
///         If the number begins with a minus sign, it is assumed to be a       
///         negative number.                                                    
///                                                                             
///         If the string begins with 0x or 0X and the base is not specified    
///         (or is 0) then base 16 is assumed.                                  
///                                                                             
///         If the string begins with 0 (and not 0x) and the base is not        
///         speicifed then base 8 is assumed.                                   
///                                                                             
///@par Expected Usage                                                          
///         @code                                                               
///             int64_t value = toSigned(input_string);                         
///             assert(toSigned("42") == 42);                                   
///             assert(toSigned("042") == 34);                                  
///             assert(toSigned("0x42") == 66);                                 
///                                                                             
///             assert(toSigned("42", 10) == 42);                               
///             assert(toSigned("42",  5) == 22);                               
///             assert(toSigned("42",  0) == 42);                               
///                                                                             
///             assert(toSigned("1492", 8) == 12);                              
///         @endcode                                                            
///                                                                             
///@param str   The string to convert to a signed value.                        
///                                                                             
///@return      The signed value represented in the string.                     
///                                                                             
//------------------------------------------------------------------------------
int64_t toSigned(const std::string& str, int base)
{

    int64_t ans(0);
    if (str.empty()) {
        //  answer is 0 for empty string    

    } else if (str[0] == '-') {
        ans = int64_t(toUnsigned(str.substr(1), base)) * -1; 

    } else {
        ans = int64_t(toUnsigned(str, base));

    }

    return ans;

} // int64_t toSigned(const std::string& str, int base)

//------------------------------------------------------------------------------
///@brief Converts a string value to a signed int                               
///                                                                             
/// \sa toSigned                                                                
//------------------------------------------------------------------------------
uint64_t toUnsigned(const std::string& str, int base)
{
    if (str.empty()) return 0;

    uint64_t ans(0);
    bool negative = false;

    std::string::const_iterator current = str.begin();

    //--------------------------------------------------------------------------
    //  Get the preliminaries out of the way (negative sign and base indicators)
    //--------------------------------------------------------------------------
    if (*current == '-') {
        negative = true;
        ++current;
    }

    if (base == 0) {
        if (*current == '0') {
            ++current;
            if (current == str.end()) return 0;

            if (*current == 'x' || *current == 'X') {
                ++current;
                if (current == str.end()) return 0;
                base = 16;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }

    } 
    
    //--------------------------------------------------------------------------
    //  If the base was specified, ignore the base indicator in the stirng.     
    //--------------------------------------------------------------------------
    else if (str.substr(0,2) == "0x" || str.substr(0,2) == "0X") 
    {
        current += 2;

    } 
    
    for (bool done = false; !done && current != str.end(); ++current) {
        if (*current >= '0' && *current <= '9') {
            int64_t value = (*current) - '0';
            if (value < base) {
                ans = ans * base + value;
            } else {
                done = true;
            }
        } else if (base > 10 && *current >= 'a' && *current <= 'z') {
            int64_t value = (*current) - 'a' + 10;
            if (value < base) {
                ans = ans * base + value;
            } else {
                done = true;
            }

        } else if (base > 10 && *current >= 'A' && *current <= 'Z') {
            int64_t value = (*current) - 'A' + 10;
            if (value < base) {
                ans = ans * base + value;
            } else {
                done = true;
            }

        } else {
            done = true;
        }
    }

    //--------------------------------------------------------------------------
    //  If the number is negative, make it the 2's compliment of the number.    
    //--------------------------------------------------------------------------
    if (negative) ans = ~ans + 1;

    return ans;

} // uint64_t toUnsigned(const std::string& str, int base)

//------------------------------------------------------------------------------
///@brief Return the left-most ct characters of the first argument.             
//------------------------------------------------------------------------------
std::string left(const std::string&str, int ct)
{
    std::string ans;
    ans = str.substr(0, ct);
    return ans;
}

//----------------------------------------------------------------------------- 
///@brief Return the right-most ct characters of the first argument.            
//----------------------------------------------------------------------------- 
std::string right(const std::string& str, int ct)
{
    int len = static_cast<int>(str.length());

    return  (len <= ct || ct < 0) ? str : str.substr(len-ct, ct);
}

//------------------------------------------------------------------------------
///@brief Removes all internal white space from the specified string            
///                                                                             
/// The function removes all whitespace that appears between characters in the  
/// string.  It will optionally remove whitespace characters at the beginning   
/// and end of the string.                                                      
//------------------------------------------------------------------------------
std::string collapse(const std::string& str, bool trim)
{
    std::string ans, white(" \t\n\v\r\f");
    std::string leadingWhite, trailingWhite;
    if (!trim && str.find_first_not_of(white) != str.npos)
    {
        leadingWhite = str.substr(0, str.find_first_not_of(white));
        trailingWhite = str.substr(str.find_last_not_of(white)+1, str.size());
    }

    for (size_t i=0 ; i < str.size() ; i++) {if (!isspace(str[i])) {ans += str[i];} }
    
    if (!trim) {ans = leadingWhite + ans + trailingWhite;}
    return ans;
}

//------------------------------------------------------------------------------
///@brief Removes superfluous internal white space from the specified string    
//------------------------------------------------------------------------------
std::string compress(const std::string& str, bool trim)
{
    std::string ans, temp, white(" \t\n\v\r\f");
    std::string leadingWhite, trailingWhite;
    size_t begin, end;
    bool lastWasWhite=true;

    if (str.find_first_not_of(white) != str.npos)
    {
        begin = str.find_first_not_of(white);
        end = str.find_last_not_of(white)+1;
        temp = str.substr(begin, end-begin);

        leadingWhite = str.substr(0, begin);
        trailingWhite = str.substr(end, str.size());
    }
    else {temp = "";}

    for (size_t i=0 ; i < temp.size() ; i++) 
    {
        if (!lastWasWhite || !isspace(temp[i])) {ans += temp[i];}
        if (isspace(temp[i])) {lastWasWhite = true;}
        else {lastWasWhite=false;}
    }
    
    if (!trim) {ans = leadingWhite + ans + trailingWhite;}
    return ans;
}

#if 0

//------------------------------------------------------------------------------
///@brief   Trim the "white" space off the beginning and end of the given string
//------------------------------------------------------------------------------
//void trim(std::string& str, const std::string& white)                         
std::string trim(const std::string& str)
{
    std::string ans, white(" \t\n\v\r\f");
    size_t pos = str.find_first_not_of(white);
    if (pos == std::string::npos) {
        ans = "";
    } else {
        ans = str.substr(pos, str.size());
        ans = ans.substr(0, ans.find_last_not_of(white)+1);
    }
    return ans;
}

static inline bool is_upper(char c) { return (c) >= 'A' && (c) <= 'Z'; }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//void toLower(const std::string& source, std::string& destination)             
std::string toLower(const std::string& source)
{
    std::string ans;
    ans = source;

    for (size_t i=0; i < ans.size(); ++i) {
        if (is_upper(ans[i])) ans[i] ^= 32;
    }
    return ans;
}

static inline bool is_lower(char c) { return (c) >= 'a' && (c) <= 'z'; }
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//void toUpper(const std::string& source, std::string& destination)             
std::string toUpper(const std::string& source)
{
    std::string ans;
    ans = source;

    for (size_t i=0; i < ans.size(); ++i) {
        if (is_lower(ans[i])) ans[i] ^= 32;
    }
    return ans;
}
#endif

static const struct SUBSTITUTION_PAIRS {
    const char* str1;
    const char* str2;
} substitution_pairs[] = {
    { "&", "&amp;"}
  , { "'", "&apos;"}
  , { "\"", "&quot;"}
  , { "<", "&lt;"}
  , { ">", "&gt;"}
};

static const uint32_t substitution_pairs_size(
    sizeof(substitution_pairs) / sizeof(substitution_pairs[0])
);

//------------------------------------------------------------------------------
///@brief   Encode the given string to make it acceptable as a value to an XML  
///         attribute.                                                          
//------------------------------------------------------------------------------
std::string xmlEncode(const std::string& str)
{
    std::string ans(str);

    for (uint32_t s=0; s < substitution_pairs_size; ++s) {
        boost::replace_all(
            ans
          , substitution_pairs[s].str1
          , substitution_pairs[s].str2
        );
    }

    return ans;
}

//------------------------------------------------------------------------------
///@brief   Decode the given xmlEncoded string to return it to "normal".        
//------------------------------------------------------------------------------
std::string xmlDecode(const std::string& str)
{
    std::string ans(str);

    uint32_t s = substitution_pairs_size;

    while (s) {
        --s;
        boost::replace_all(
            ans
          , substitution_pairs[s].str2
          , substitution_pairs[s].str1
        );
    }

    return ans;
}

//------------------------------------------------------------------------------
///@brief   Sets a right hand side padded char array w/o null termination,      
///         from string.                                                        
///@param   destination             Pointer to destination char array.          
///@param   destination_max_size    Length of destination array                 
///@param   source                  Source string                               
///@param   pad                     The padding to insert after the source      
///                                 string to reach destination_max_size        
///                                 number of characters.                       
///@return  true = source.length() <= destination_max_size                      
///         false = The given source string was too long; no changes made.      
//------------------------------------------------------------------------------
bool toFixedCharArray(
    char*               destination
  , size_t              destination_max_size
  , const std::string&  source
  , char                pad
) {
    bool result = false;

    if(source.length() <= destination_max_size) {
        memset(destination, pad, destination_max_size);
        strncpy(destination, source.c_str(), source.size());
        result = true;
    }
    return result;
}

//------------------------------------------------------------------------------
///@brief   Retrieves a string from a right hand side padded char array w/o     
///         null termination.                                                   
///@param   source          Pointer to source array                             
///@param   source_max_size Maximum length of source array, optionally          
///                         including padding characters.                       
///@param   trim_trailing   Trailing padding character to trim.                 
///@return  A string without any trailing padding characters.                   
//------------------------------------------------------------------------------
std::string fromFixedCharArray(
    const char* source
  , size_t      source_max_size
  , char        trim_trailing
) {
    std::string s(source, source_max_size);
    return s.erase(s.find_last_not_of(trim_trailing)+1);
}

} // lib namespace

