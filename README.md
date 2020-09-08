# datr.open.source

    This directory structure holds the files that have been open sourced
    for the DATR project.

# license
    
    The open source license that is used has not yet been determined.  Most
    likley it will be either BSD or MIT.  LGPL v2.x is also a possiblity.

#   Definitions
        -   TitleCase   -- First letter of each word upper case
        -   camelCase   -- Same as TitleCase with initial letter lower case.
        -   lowercase   -- All lowercase
        -   UPPERCASE   -- All UPPERCASE

#   Directories
        common  --  Code files that are expected to work under both Windows and
                    Linux.

        linux   --  Linux build procedures; linux/src is Linux specific code

        qt      --  Qt framework aware code; think of it as common for Qt code.

        qmake   --  Build procedures for Qt framework code; think of it as
                    linux for Qt code.
    
        tools   --  Developer tools

#   Filenames
        -   All lower case;  Only exception is Makefile because of make 
            convention.
        -   Namespaces, separated by underscores, with an underscore before
            the class name.
       
#   Coding Standards
        -   In general, look at the existing code.
        -   Use Doxygen commenting as appropriate; use the Doxygen @ versus \

##  Classes
        -   Should have one, and only one, public, protected, and private
            in that order
        -   Prefer action words 

##  Capitalization
        -   classes     TitleCase
        -   methods     camelCase
        -   functions   

##  Namespaces
### Naming
       -   single word (if possible; see project lead if not)
       -   all lowercase 

### Existing
       dev     --  developer code (unit tests); should not go into production
       lib     --  short for library; production code for multiple applications

       ds      --  data structures.  An inner most namespace; should just be
                   structs (and maybe constructors; assignment operators)
       msg     --  message.  An inner most namespace; is aware of the
                   DATR publisher/subscriber code.  Best practice is msg 
                   classes are thin wrappers around work classes.
       work    --  an inner most namespace; only "knows" about C++ and Boost
                   Most DATR code should be here.

##  C++ Header Files
        -   In general, look at existing code.
        -   Required sections of class header block comments:
                -   @brief (one sentence description)
                -   @par Thread Safety: [none|class|object|special]
                        -   _none_      There is an unprotected global that
                                        prevents the class from being used
                                        on multiple threads.
                        -   _class_     An object's internals are not protected
                                        but the class does not have statics or
                                        other dependencies that force none
                        -   _object_    The object either has no state, the 
                                        state is constant after creation, or
                                        steps are taken to allow multi-threaded
                                        access
                        -   _special_   Explain why it is special in a paragraph
                                        after the Thread Safety.

                        -   Typcially _class_ is used; Qt derived classes are
                            none since the Qt base classes are none.

### Pointer to Implementation (pImpl)
        The DATR code makes extensive use of the pImpl idiom.  Loosely defined,
        it is a mechanism for making private member variables declared and
        defined in the cpp implementation file versus the heaer file.

        In the header file, something like the following:
        ```
            private:
                struct Data;
                std::unique_ptr<Data> m_Data;
        ```

        The cpp implementation file would then define the member variables used:
        ```
            struct MyClass::Data
            {
                uint64_t    m_MyVariable;
            };
        ```

        A typical constructor would then look like:
        ```
            MyClass::MyClass()
                : m_Data(new Data)
            {
                // ... snip ...
            }
        ```

       
#   WARNINGS

##  Boost
        The existing code base is overly dependent on Boost.  Given the choice,
        use standard C++ that works on both gcc and VisualStudio.

        We are actively trying to remove Boost from the code base.  Why?  It
        is one of the bigger pains in trying to move to a new system.

        The DATR project is actively trying to remove dependencies on   
        Boost in general.  The biggest ripple is the removal of 
        boost::shared_ptr.  The code is currently transitioning to 
        using lib::ds::shared_ptr

