//------------------------------------------------------------------------------
///@file lib_qt_field.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-08-25  PN     Update documentation to DATR standards.
///@version 2020-08-17  PN     Removed logic for value() and setValue(), since
///                            they are now pure virtual functions.
///@version 2020-08-14  PN     Add constructor logic to initialize field name.
///                            Add logic to setter and getter functions.
///@version 2020-08-13  PN     File creation
//------------------------------------------------------------------------------

#include "lib_qt_field.h"

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///@brief Initialize m_Name for the Field class.
///@param name    QString containing name of the field initialized.
//------------------------------------------------------------------------------
Field::Field(QString const& name)
{
    setName(name);
} // Field::Field() //

//------------------------------------------------------------------------------
///@brief Reclaim resources held by object.
//------------------------------------------------------------------------------
Field::~Field()
{
} // Field::~Field() //

//------------------------------------------------------------------------------
///@brief Set m_Name to new value.
///@param new_name    QString containing the new field name.
//------------------------------------------------------------------------------
void Field::setName(QString const& new_name)
{
    m_Name = new_name;
} // Field::value() //

//------------------------------------------------------------------------------
///@brief Return the QString m_Name.
//------------------------------------------------------------------------------
QString Field::name() const
{
    return m_Name;
} // Field::name() //


} //namespace qt //
} //namespace lib //
