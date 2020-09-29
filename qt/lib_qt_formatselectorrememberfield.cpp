//------------------------------------------------------------------------------
///@file lib_qt_formatselectorrememberfield.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-08-25  PN     Updated to work with Qt4.8.7.
///                            Removed currentData() and setPlaceholder().
///@version 2020-08-21  PN     File creation. Implemented constructor
///                            , destructor, addFormat(), removeFormat()
///                            , value(), and setValue().
//------------------------------------------------------------------------------

#include "lib_qt_formatselectorrememberfield.h"

#include <iostream>
#include <QDebug>

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
    addItem("RFC2822Date", "dd MMM yyyy HH:mm:ss");
    addItem("DATR", "yyyy-MM-dd HH:mm:ss");
    if(!rememberedValue().isEmpty())
    {
        addItem("Remembered Format", rememberedValue());
    }
} // FormatSelectorRememberField::FormatSelectorRememberField() //

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
    addItem("RFC2822Date", "dd MMM yyyy HH:mm:ss");
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
        qDebug() << "Warning: addFormat() arguments cannot be empty";
        return;
    } else if(findData(QVariant(format), Qt::UserRole, Qt::MatchExactly) == -1){
        addItem(text, format);
        setCurrentIndex(count()-1);
    } else {
        qDebug() << "Warning: addFormat() format already in selector!"
                 << format;
    }
} // FormatSelectorRememberField::addFormat() //

//------------------------------------------------------------------------------
///@brief Remove a format from the format selector based on displayed text.
///@param text    QString containing the displayed text of the format to be
///               removed.
//------------------------------------------------------------------------------
void FormatSelectorRememberField::removeFormat(QString const& text)
{
    int row = findText(text, Qt::MatchExactly);
    if(row == -1)
    {
        qDebug() << "Warning: removeFormat() format not in the selector"
                 << text;
    } else {
        removeItem(row);
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
///@brief Set the current selected format to a new format.
///@param new_value    QString containing the new format to be added.
//------------------------------------------------------------------------------
void FormatSelectorRememberField::setValue(QString const& new_value)
{
    setItemData(currentIndex(), new_value);
} // FormatSelectorRememberField::setValue()
} // namespace qt //
} // namespace lib //
