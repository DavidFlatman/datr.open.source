//------------------------------------------------------------------------------
///                                                                             
///@file lib_cast.h                                                             
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Way too much typing can be required while doing a cast.             
///                                                                             
///     Instead of typing:                                                      
///         @code                                                               
///             message = dynamic_cast<lib::ms::1553::Message*>(ch8);           
///         @endcode                                                            
///                                                                             
///     Just type:                                                              
///         @code                                                               
///             lib::cast_dynamic(message, ch8);                                
///         @endcode                                                            
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2013-02-12  DHF     File creation.                                  
///                                                                             
//------------------------------------------------------------------------------


#ifndef LIB_CAST_FILE_GUARD
#define LIB_CAST_FILE_GUARD

namespace lib {

//------------------------------------------------------------------------------
///@brief   This is probably a bad cast.  You really should know what you're    
///         doing.                                                              
//------------------------------------------------------------------------------
template<typename DESTINATION, typename SOURCE>
DESTINATION* cast(DESTINATION*& destination, SOURCE* source)
{
    destination = (DESTINATION*) source;
    return destination;
}

//------------------------------------------------------------------------------
///@briefA  Peform a static cast to the pointer of the given type.              
///                                                                             
///@warning The only reason to use this method (versus cast_dynmaic) is when    
///         the pointers involved are (1) not virutal, and/or (2) not derived   
///         form each other.                                                    
//------------------------------------------------------------------------------
template<typename DESTINATION, typename SOURCE>
DESTINATION* cast_static(DESTINATION*& destination, SOURCE* source)
{
    destination = static_cast<DESTINATION*>(source);
    return destination;
}

//------------------------------------------------------------------------------
///@brief
//------------------------------------------------------------------------------
template<typename DESTINATION, typename SOURCE>
DESTINATION* cast_dynamic(DESTINATION*& destination, SOURCE* source)
{
    destination = dynamic_cast<DESTINATION*>(source);
    return destination;
}

}

#endif // #ifndef LIB_CAST_FILE_GUARD

