//------------------------------------------------------------------------------
///@file lib_qt_lineeditrememberfield.h                                                     
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_QT_LINEEDITREMEMBERFIELD_H_FILE_GUARD
#define LIB_QT_LINEEDITREMEMBERFIELD_H_FILE_GUARD

#include "lib_qt_rememberfield.h"

#include <QLineEdit>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: LineEditRememberField                                                       
///                                                                             
///@brief Line edit widget to be used by other classes. All values are saved
///       upon exiting the application. Previous values are restored from local
///       file.
///                                                                             
///@par Thread Safety:  none (Qt GUI)                                           
///                                                                             
///@par Class Knowledge                                                         
///         -   This class knows about:
///             -   RememberedField class
///             -   QLineEdit class
///             -   When to store/restore the RememberField
///         -   This class hides:
///         -   This class is agnostic about:
///             -   The underlying field-name/field-value storage mechanism.
///                                                                             
///@par Expected Usage:                                                         
///     @code
///         struct MyField::Data
///         {
///              QHBoxLayout*                m_Layout;
///              LineEditRememberField*      m_Path;
///
///             MyField() {
///                 m_Layout->addWidget(m_Path);
///             }
///         }
///     @endcode
///
///@version 2020-08-25  PN     Update documentation to DATR standards.
///@version 2020-08-19  PN     Update constructor paramters.
///@version 2020-08-17  PN     Added constructor, destructor, value(),
///                            setValue(), setName(), and m_Value.
///@version 2020-08-16  PN     File creation
//------------------------------------------------------------------------------

class LineEditRememberField 
        : public RememberField
        , public QLineEdit
{
    public:
        explicit LineEditRememberField(
              QString const& field_name
            , QWidget* parent = nullptr
          );
        LineEditRememberField(
              QString const& field_name
            , QString const& field_value
            , QWidget* parent = nullptr
          );
        virtual ~LineEditRememberField();
    
        virtual QString value() const override; 
        virtual void setValue(QString const& new_value) override;
}; // class LineEditRememberField
} // namespace qt
} // namespace lib

#endif // LIB_QT_LINEEDITREMEMBERFIELD_H_FILE_GUARD