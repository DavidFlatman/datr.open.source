//------------------------------------------------------------------------------
///@file lib_qt_rememberfield.h                                                     
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_QT_REMEMBERFIELD_H_FILE_GUARD
#define LIB_QT_REMEMBERFIELD_H_FILE_GUARD

#include "lib_qt_field.h"

#include <QSettings>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///
///@par Class: RememberField
///
///@brief   This class is agnostic about "path".  The first implementation it   
///         will probably be used for path, but this class is agnostic about    
///         it.
///
///@par Thread Safety:  none (Qt GUI)
///
///@par Class Knowledge
///         -   This class knows about:
///                 -   The underlying field-name/field-value storage mechanism.
///                 -   The Field class with limitations; at construction and
///                     destruction, this class does not have access to:
///                         -   lib:qt:Field::value
///                         -   lib:qt:Field::setValue
///         -   This class hides:
///                 -   The underlying field-name/field-value storage mechanism.
///         -   This class is agnostic about:
///                 -   The meaning or purpose of the field-name/field-value.
///                 -   Assumes already set
///                     -   QCoreApplication::OrganizationName
///                     -   QCoreApplication::OrganizationDomainName
///                     -   QCoreApplication::ApplicationName
///                 -   How the value is actually stored
///
///@par Expected Usage:
///     @code
///         class LineEditRememberField
///             : public RememberField
///             , public QLineEdit
///         {
///             public:
///                 explicit LineEditRememberField(
///                       QString const& fieldName
///                     , QLineEdit* parent = nullptr
///                 )   : RememberField(fieldName)
///                     , QLineEdit(parent)
///                 {
///                     setValue(rememberedValue());
///                 }
///
///                 explicit LineEditRememberField(
///                     QString const& fieldName
///                   , QString const& fieldValue
///                   , QLineEdit* parent = nullptr
///                 ) : RememberField(fieldName)
///                   , QLineEdit(parent)
///                 {
///                     //------------------------------------------------------
///                     //  Note:   Since the value was supplied to the
///                     //  constructor, we don't restore the value.
///                     //------------------------------------------------------
///                     setValue(fieldValue);
///                 }
///
///                 virtual ~LineEditRememberField()
///                 {
///                     setRememberedValue();
///                 }
///
///                 //----------------------------------------------------------
///                 //  Override methods from lib::qt::Field.
///                 //----------------------------------------------------------
///                 virtual QString value() const override;
///                 virtual void setValue(QString const& v) override;
///         }
///     @endcode
///
///@version 2020-08-25  PN     Update documentation to DATR standards.
///@version 2020-08-19  PN     Removed paramter for rememberedValue(). Changed
///                            name() to internalName(). Update constructor
///                            paramters.
///@version 2020-08-13  PN     Removed rememberValueChanged(). Added parameters
///                            for setRememberedValue() and rememberedValue().
///@version 2020-08-12  PN     Added constructor, destructor,
///                            setRememberedValue(), rememberedValue(),
///                            and rememberValueChanged().
///@version 2020-08-11  PN     File creation
//------------------------------------------------------------------------------

class RememberField : public Field {
    public:
        explicit RememberField(QString const& fieldName);
        RememberField(
              QString const& fieldName
            , QString const& fieldValue
        );
        virtual ~RememberField();

        virtual QString rememberedValue() const;
        virtual void setRememberedValue(QString const& value);

    private:
        QString internalName() const;

}; // class RememberField //
} // namespace qt //
} // namespace lib //
#endif // #ifndef LIB_QT_REMEMBERFIELD_H_FILE_GUARD
