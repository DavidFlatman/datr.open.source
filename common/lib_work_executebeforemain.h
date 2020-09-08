//------------------------------------------------------------------------------
///                                                                             
///@file lib_work_executebeforemain.h                                           
///                                                                             
///@par  Classification:  UNCLASSIFIED                                          
///                                                                             
///@author  David H. Flatman    DHF     david.flatman.ctr@navy.mil              
///                                                                             
///@version 2019-08-15  DHF     Modified class EMBI_ to declare the variable    
///                             static to avoid linker errors.                  
///                                                                             
///@version 2019-07-24  DHF     File creation.                                  
///                                                                             
//------------------------------------------------------------------------------

#ifndef LIB_WORK_EXECUTEBEFOREMAIN_H_FILE_GUARD
#define LIB_WORK_EXECUTEBEFOREMAIN_H_FILE_GUARD

#ifndef CONCAT4
#define CONCAT4(a, b, c, d) a ## b ## c ## d
#endif

//------------------------------------------------------------------------------
///@brief   Execute code before and after int main(int argc, char* argv[])      
///         is called.                                                          
///                                                                             
///@warning There @e really needs to be a compelling reason to use this.  If    
///         it is used too many times, there is a risk of "race" conditions     
///         where one EXECUTE_BEFORE_AFTER_MAIN needs to be processed before    
///         another one.  For the current version of the compiler, things may   
///         work -- but for the next version it does not work.  @e All uses of  
///         this macro should document why -- and why it is considered safe.    
///                                                                             
///@warning If you place EXECUTE_BEFORE_AFTER_MAIN inside a header file, even   
///         with proper file guards, the code will be execute once for each     
///         compilation unit that includes the header file.                     
///                                                                             
///@limitations If the code contains commas (,) then an additional set of       
///             paranthesis needs to be used around the parameter with the      
///             comma.                                                          
///                                                                             
///@par Expected Usage I                                                        
///         @code                                                               
///             //--------------------------------------------------------------
///             //  Assume that my::library has to call initialize before       
///             //  int main is executed and finish after int main has          
///             //  completed.                                                  
///             //--------------------------------------------------------------
///             EXECUTE_BEFORE_AFTER_MAIN(                                      
///                 my::library::initialize()                                   
///               , my::library::finish()                                       
///             );                                                              
///         @endcode                                                            
///                                                                             
///@par Expected Usage II                                                       
///         @code                                                               
///             EXECUTE_BEFORE_AFTER_MAIN(                                      
///                 std::cout << "before main has begun" << std::endl;          
///              ,  std::cout << "after main has completed" << std::endl;       
///             );                                                              
///         @endcode                                                            
///                                                                             
///@param   BEFORE  The C++ code to be executed before main.                    
///@param   AFTER   The C++ code to be executed after main has completed.       
//------------------------------------------------------------------------------
#define EXECUTE_BEFORE_AFTER_MAIN(BEFORE, AFTER)                            \
    EXECUTE_BEFORE_AFTER_MAIN_INTERNAL(BEFORE, AFTER, __COUNTER__, __LINE__)


//------------------------------------------------------------------------------
///@brief   Execute code before int main(int argc, char* argv[]) is called.     
///                                                                             
///@warning There @e really needs to be a compelling reason to use this.  If    
///         it is used too many times, there is a risk of "race" conditions     
///         where one EXECUTE_BEFORE_MAIN needs to be processed before          
///         another one.  For the current version of the compiler, things may   
///         work -- but for the next version it does not work.  @e All uses of  
///         this macro should document why -- and why it is considered safe.    
///                                                                             
///@warning If you place EXECUTE_BEFORE_MAIN inside a header file, even         
///         with proper file guards, the code will be execute once for each     
///         compilation unit that includes the header file.                     
///                                                                             
//------------------------------------------------------------------------------
#define EXECUTE_BEFORE_MAIN(BEFORE) EXECUTE_BEFORE_AFTER_MAIN(BEFORE, )

//------------------------------------------------------------------------------
///@brief   Used internally by EXECUTE_BEFORE_MAIN to translate __COUNTER__     
///         into a integer.                                                     
///                                                                             
///         Effectively the BEFORE code is placed into the constructor and      
///         the AFTER code is placed in the destructor of a class.              
//------------------------------------------------------------------------------
#define EXECUTE_BEFORE_AFTER_MAIN_INTERNAL(BEFORE, AFTER, COUNT, LINE)  \
    static class CONCAT4(EBMI_, COUNT, _, LINE)                         \
    {                                                                   \
        public:                                                         \
            CONCAT4(EBMI_, COUNT, _, LINE)() { BEFORE; }                \
            CONCAT4(~EBMI_, COUNT, _, LINE)() { AFTER; }                \
    } CONCAT4(ebmi_, COUNT, _, LINE);


#endif // #ifndef LIB_WORK_EXECUTEBEFOREMAIN_H_FILE_GUARD

