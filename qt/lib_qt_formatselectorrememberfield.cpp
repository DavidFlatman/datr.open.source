//------------------------------------------------------------------------------
///@file lib_qt_formatselectorrememberfield.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-08-21  PN     File creation. Implemented constructor
///                            , destructor, addFormat(), removeFormat()
///                            , value(), and setValue().
//------------------------------------------------------------------------------

#include "lib_qt_formatselectorrememberfield.h"

#include <iostream>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///@brief Initialize inherited classes and populate the QComboBox with default
///       formats. Set format from remembered value.
//------------------------------------------------------------------------------
FormatSelectorRememberField::FormatSelectorRememberField(
    QString const& field_name
  , QWidget* parent
) : RememberField(field_name)
  , QComboBox(parent)
{
    addItem("TextDate", "ddd MMM d yyyy hh:mm:ss");
    addItem("ISODate", "yyyy-MM-ddTHH:mm:ss");
    addItem("RFC2822Date", "dd MMM yyyy HH:mm:ss zzz");
    addItem("DATR", "yyyy-MM-dd HH:mm:ss");
    if(!rememberedValue().isEmpty())
    {
        setValue(rememberedValue());
    }
} // // FormatSelectorRememberField::FormatSelectorRememberField() //

//------------------------------------------------------------------------------
///@brief Initialize inherited classes and populate the QComboBox with default
///       formats. Set format from parameter value.
//------------------------------------------------------------------------------
FormatSelectorRememberField::FormatSelectorRememberField(
    QString const& field_name
  , QString const& field_value
  , QWidget* parent
) : RememberField(field_name)
  , QComboBox(parent)
{
    addItem("TextDate", "ddd MMM d yyyy hh:mm:ss");
    addItem("ISODate", "yyyy-MM-ddTHH:mm:ss");
    addItem("RFC2822Date", "dd MMM yyyy HH:mm:ss zzz");
    addItem("DATR", "yyyy-MM-dd HH:mm:ss");
    addFormat(field_name, field_value);
}
//------------------------------------------------------------------------------
///@brief Reclaim resources held by object. Set current format to be remembered.
//------------------------------------------------------------------------------
FormatSelectorRememberField::~FormatSelectorRememberField()
{
    setRememberedValue(value());
} // FormatSelectorRememberField::~FormatSelectorRememberField

//------------------------------------------------------------------------------
///@brief Add a new format to the format selector.
///@param text      QString containing the text to be displayed on format
///                 selector.
///       format    QString containing the format.
//------------------------------------------------------------------------------
void FormatSelectorRememberField::addFormat(
      QString const& text
    , QString const& format
)
{
    if(text.isEmpty() || format.isEmpty())
    {
        std::cout << "Warning: addFormat() inputs cannot be empty" << std::endl;
        return;
    }
    if(findData(QVariant(format), Qt::UserRole, Qt::MatchExactly) == -1)
    {
        addItem(text, format);
        setCurrentIndex(count()-1);
    } else {
        std::cout << "Warning: Format already in selector!" << std::endl;
    }
} // FormatSelectorRememberField::addFormat() //

//------------------------------------------------------------------------------
///@brief Remove a format from the format selector.
///@param row    Int containing the row of the format to be removed.
//------------------------------------------------------------------------------
void FormatSelectorRememberField::removeFormat(int const& row)
{
    if(row < count() && row >= 0)
    {
        removeItem(row);
    } else {
        std::cout << "Warning: removeFormat() out of range" << std::endl;
    }
} // FormatSelectorRememberField::removeFormat() //

//------------------------------------------------------------------------------
///@brief Get the format of the current selection in the format selector. If
///       current index is set to placeholder, return the first format in the
///       selector.
//------------------------------------------------------------------------------
QString FormatSelectorRememberField::value() const
{
    return itemData(currentIndex(), Qt::UserRole).toString();
} // FormatSelectorRememberField::value() //

//------------------------------------------------------------------------------
///@brief Set a new format to be added to the format selector. Will not add a
///       format that is already in the list.
///@param new_value    QString containing the new format to be added.
//------------------------------------------------------------------------------
void FormatSelectorRememberField::setValue(QString const& new_value)
{
    addFormat("Remembered Format", new_value);
} // FormatSelectorRememberField::setValue()
} // namespace qt //
} // namespace lib //
