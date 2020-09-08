//------------------------------------------------------------------------------
///@file lib_qt_rememberfield.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-08-25  PN     Update documentation to DATR standards.
///@version 2020-08-19  PN     Removed m_Name. Changed name() to internalName().
///                            Made sure RememberField class initalizes
///                            Field's name.
///@version 2020-08-16  PN     Add logic to constructor, rememberedValue(), and
///                            setRememberedValue(). Added m_Name and name().
///@version 2020-08-13  PN     File creation
//------------------------------------------------------------------------------

#include "lib_qt_rememberfield.h"

#include <iostream>
#include <QSettings>
#include <QString>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///@brief Initialize fieldName for the RememberField Class.
///@param fieldName    QString containing name of the field initialized.
//------------------------------------------------------------------------------
RememberField::RememberField(QString const& fieldName)
    : Field(fieldName)
{
} // RememberField::RememberField()

//------------------------------------------------------------------------------
///@brief Set fieldName and fieldValue for the RememberField Class.
//------------------------------------------------------------------------------
RememberField::RememberField(
    QString const& fieldName
  , QString const& fieldValue
) : Field(fieldName)
{
    setRememberedValue(fieldValue);
} // RememberField::RememberField() //

//------------------------------------------------------------------------------
///@brief Reclaim resources held by object.
//------------------------------------------------------------------------------
RememberField::~RememberField()
{
} // RememberField::~RememberField() //

//------------------------------------------------------------------------------
///@brief Return the remembered value from file.
//------------------------------------------------------------------------------
QString RememberField::rememberedValue() const
{
    QSettings settings;
    return settings.value(internalName()).toString();
} // RememberField::value() //

//------------------------------------------------------------------------------
///@brief Save the remembered field name and field value to file.
///@pre Requires the following to be set:
///     QCoreApplication::setOrganizationName,
///     QCoreApplication::setOrganizationDomain,
///     QCoreApplication::setApplicationName.
///@post Saves value to a .conf file in the .config/ directory.
//------------------------------------------------------------------------------
void RememberField::setRememberedValue(QString const& value)
{
    QSettings settings;
    settings.setValue(internalName(), value);
} // RememberField::setValue() //

//------------------------------------------------------------------------------
///@brief Return remembered field name with the "remember-field" prepended to
///       avoid name collisions.
//------------------------------------------------------------------------------
QString RememberField::internalName() const
{
    return "remembered-field/" + name();
} // RememberField::name() //
} // namespace qt //
} // namespace lib //
