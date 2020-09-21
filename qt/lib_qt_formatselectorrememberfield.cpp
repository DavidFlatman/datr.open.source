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
    setPlaceholderText("Select a format");
    addItem("TextDate", "ddd MMM d yyyy hh:mm:ss t");
    addItem("ISODate", "yyyy-MM-ddTHH:mm:sst");
    addItem("RFC2822Date", "dd MMM yyyy HH:mm:ss ±z");
    addItem("DATR", "yyyy-MM-dd HH:mm:ss");
    if(rememberedValue().isEmpty())
    {
//------------------------------------------------------------------------------
// Set m_FormatSelector to placeholder text at the start of the application.
//------------------------------------------------------------------------------
        setCurrentIndex(-1);
    } else {
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
    addItem("TextDate", "ddd MMM d yyyy hh:mm:ss t");
    addItem("ISODate", "yyyy-MM-ddTHH:mm:sst");
    addItem("RFC2822Date", "dd MMM yyyy HH:mm:ss ±z");
    addItem("DATR", "yyyy-MM-dd HH:mm:ss");
    setValue(field_value);
}
//------------------------------------------------------------------------------
///@brief Reclaim resources held by object. Set current format to be remembered
///       on next startup.
//------------------------------------------------------------------------------
FormatSelectorRememberField::~FormatSelectorRememberField()
{
    setRememberedValue(value());
} // FormatSelectorRememberField::~FormatSelectorRememberField

//------------------------------------------------------------------------------
///@brief Add a new format to the format selector.
///@param text    QString containing the text to be displayed on format
///               selector.
///       format    QString containing the format.
//------------------------------------------------------------------------------
void FormatSelectorRememberField::addFormat(
      QString const& text
    , QString const& format
)
{
    addItem(text, format);
} // FormatSelectorRememberField::addFormat() //

//------------------------------------------------------------------------------
///@brief Remove a format from the format selector.
///@param row    Int containing the row of the format to be removed.
//------------------------------------------------------------------------------
void FormatSelectorRememberField::removeFormat(int const& row)
{
    removeItem(row);
} // FormatSelectorRememberField::removeFormat() //

//------------------------------------------------------------------------------
///@brief Get the format of the current selection in the format selector.
//------------------------------------------------------------------------------
QString FormatSelectorRememberField::value() const
{
    if(currentIndex() == -1)
    {
        return itemData(0, Qt::UserRole).toString();
    }
    return currentData(Qt::UserRole).toString();
} // FormatSelectorRememberField::value() //

//------------------------------------------------------------------------------
///@brief Set a new format to be added to the format selector. Will not add a
///       format that is already in the list.
///@param new_value    QString containing the new format to be added.
//------------------------------------------------------------------------------
void FormatSelectorRememberField::setValue(QString const& new_value)
{
    if(new_value.isEmpty())
    {
        return;
    }
    if(findData(QVariant(new_value), Qt::UserRole, Qt::MatchExactly) == -1)
    {
        addFormat(new_value, new_value);
        setCurrentIndex(count()-1);
    }
} // FormatSelectorRememberField::setValue()
} // namespace qt //
} // namespace lib //
