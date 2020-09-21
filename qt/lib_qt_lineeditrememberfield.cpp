//------------------------------------------------------------------------------
///@file lib_qt_lineeditrememberfield.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-08-25  PN     Update documentation to DATR standards.
///@version 2020-08-24  PN     Changed setValue() to use QLineEdit::setText()
///                            and value() to use QLineEdit::text().
///@version 2020-08-20  PN     Added logic for constructor, destructor, value()
///                            and setValue().
///@version 2020-08-19  PN     File creation
//------------------------------------------------------------------------------

#include "lib_qt_lineeditrememberfield.h"

#include <iostream>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///@brief Set m_Name and restore value for the LineEditRememberField Class.
//------------------------------------------------------------------------------
LineEditRememberField::LineEditRememberField(
    QString const& field_name
  , QWidget* parent
) : RememberField(field_name)
  , QLineEdit(parent)
{
    setValue(rememberedValue());
} // LineEditRememberField::LineEditRememberField() //

//------------------------------------------------------------------------------
///@brief Set m_Name and m_Value for the LineEditRememberField Class.
//------------------------------------------------------------------------------
LineEditRememberField::LineEditRememberField(       
    QString const& field_name
  , QString const& field_value
  , QWidget* parent
) : RememberField(field_name)
  , QLineEdit(parent)
{
    setValue(field_value);
} // LineEditRememberField::LineEditRememberField() //

//------------------------------------------------------------------------------
///@brief Reclaim resources held by object.
//------------------------------------------------------------------------------
LineEditRememberField::~LineEditRememberField()
{
    setRememberedValue(value());
} //LineEditRememberField::!LineEditRememberField //

//------------------------------------------------------------------------------
///@brief Returns m_Value in LineEditRememberLine class.
//------------------------------------------------------------------------------
QString LineEditRememberField::value() const
{
    return QLineEdit::text();
} // LineEditRememberField::value() //

//------------------------------------------------------------------------------
///@brief Set value for the QLineEdit text field.
///@param new_value    QString containing the path for user selected file.
//------------------------------------------------------------------------------
void LineEditRememberField::setValue(QString const& new_value)
{
    QLineEdit::setText(new_value);
} // LineEditRememberField::setValue() //
} // namespace qt //
} // namespace lib //
