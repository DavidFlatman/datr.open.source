//------------------------------------------------------------------------------
///@file lib_qt_field.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------

#ifndef LIB_QT_FIELD_H_FILE_GUARD
#define LIB_QT_FIELD_H_FILE_GUARD

#include <QString>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///
///@par Class: Field
///
///@brief   Base class for input fields.
///
///@par Thread Safety:  class
///
///@par Class Knowledge
///         -   This class knows about:
///                 -   The name of the field.
///         -   This class hides:
///                 -   QString m_Name.
///         -   This class is agnostic about:
///                 -   How the value is stored.
///                 -   The type of the derived classes input.
///                 -   How to convert from the actual field type
///                     to/from a QString.
///
///@par Expected Usage:
///     @code
///         class MyField : public lib:qt:Field
///         {
///             public:
///                 virtual QString value() const overrride;
///                 virtual void setValue(QString const& new_value) override;
///         }
///     @endcode
///
///@version 2020-08-25  PN     Update documentation to DATR standards.
///@version 2020-08-19  PN     Moved name() to public.
///@version 2020-08-18  PN     Made value() and setValue() as pure virtual
///                            function. Removed value paramter from
///                            constructor. Moved functions to protected.
///@version 2020-08-13  PN     Update parameters for constructor.
///@version 2020-08-12  PN     Added constructor, destructor, value(),
///                            setValue(), valueChanged(), name(), and
///                            setName().
///@version 2020-08-11  PN     File creation
//------------------------------------------------------------------------------

class Field
{
    public:
        virtual QString name() const;
        virtual QString value() const = 0;
        virtual void setValue(QString const& new_value) = 0;
        virtual void setName(QString const& new_name);

    protected:
        explicit Field(QString const& name = "field");
        virtual ~Field();

    private:
        QString m_Name;

}; // class Field //
} // namespace qt //
} // namespace lib //
#endif // #ifndef LIB_QT_FIELD_H_FILE_GUARD
