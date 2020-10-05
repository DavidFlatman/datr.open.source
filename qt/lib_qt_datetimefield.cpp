//------------------------------------------------------------------------------
///@file lib_qt_datetimefield.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-09-21  PN     Add addFormat() and removeFormat().
///@version 2020-09-14  PN     Update connect() to handle DateTimePicker
///                            acceptChanges() signal. Add setMaximumDate(),
///                            setMinimumDate, setMaximumTime() and
///                            setMinimumTime().
///@version 2020-09-11  PN     File creation.
//------------------------------------------------------------------------------

#include "lib_qt_datetimefield.h"
#include "lib_qt_lineeditrememberfield.h"
#include "lib_qt_datetimepicker.h"

#include <QCalendarWidget>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimeEdit>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///@brief Field data for the data struct.
//------------------------------------------------------------------------------
struct DateTimeField::Data {
    QHBoxLayout*            m_Layout;
    LineEditRememberField*  m_Field;
    DateTimePicker*         m_DateTimePicker;
    QPushButton*            m_ThreeDots;

    Data(QString const& field_name)
        : m_Layout(new QHBoxLayout)
        , m_Field(new LineEditRememberField(field_name, nullptr))
        , m_DateTimePicker(new DateTimePicker("Date Time Picker",nullptr))
        , m_ThreeDots(new QPushButton("..."))
    {
        m_Layout->addWidget(m_Field);
        m_Layout->addWidget(m_ThreeDots);
    }
}; // Struct DateTimeField::Data

//------------------------------------------------------------------------------
///@brief Initialize inherited classes, connecting slot and signals, and layout.
//------------------------------------------------------------------------------
DateTimeField::DateTimeField(
      QString const& field_name
    , QWidget* parent
)   : QWidget(parent)
    , Field(field_name)
    , m_Data(new Data(field_name))
{
    m_Data->m_DateTimePicker->setParent(this, Qt::Window);
    setLayout(m_Data->m_Layout);
    connect(
          m_Data->m_ThreeDots, SIGNAL(clicked())
        , this, SLOT(openDialog())
    );
    connect(
          m_Data->m_DateTimePicker, SIGNAL(acceptChanges())
        , this, SLOT(onDateTimeChange())
    );
} // DateTimeField::DateTimeField() //

//------------------------------------------------------------------------------
///@brief Reclaim resources held by object. Explicitly call DateTimePicker to
///       save format upon exit.
//------------------------------------------------------------------------------
DateTimeField::~DateTimeField()
{
} // DateTimeField::~DateTimeField() //

//------------------------------------------------------------------------------
///@brief Set the minimum date available for user selection in m_DateTimePicker.
///@param new_date    QDate containing the minimum date available for user
///                   selection.
//------------------------------------------------------------------------------
void DateTimeField::setMinimumDate(QDate const& new_date)
{
    m_Data->m_DateTimePicker->setMinimumDate(new_date);
} // DateTimeField::setMinimumDateTime() //

//------------------------------------------------------------------------------
///@brief Set the maximum date available for user selection in m_DateTimePicker.
///@param new_date    QDate containing the maximum date available for user
///                   selection.
//------------------------------------------------------------------------------
void DateTimeField::setMaximumDate(QDate const& new_date)
{
    m_Data->m_DateTimePicker->setMaximumDate(new_date);
} // DateTimeField::setMaximumDateTime() //

//------------------------------------------------------------------------------
///@brief Set the minimum time available for user selection in m_DateTimePicker.
///@param new_time    QTime containing the minimum time available for user
///                   selection. Only QTime.minute = 0 or QTime.minute = 30 is
///                   allowed.
//------------------------------------------------------------------------------
void DateTimeField::setMinimumTime(QTime const& new_time)
{
    m_Data->m_DateTimePicker->setMinimumTime(new_time);
} // DateTimeField::setMinimumDateTime() //

//------------------------------------------------------------------------------
///@brief Set the maximum time available for user selection in m_DateTimePicker.
///@param new_time    QTime containing the maximum time available for user
///                   selection. Only QTime.minute = 0 or QTime.minute = 30 is
///                   allowed.
//------------------------------------------------------------------------------
void DateTimeField::setMaximumTime(QTime const& new_time)
{
    m_Data->m_DateTimePicker->setMaximumTime(new_time);
} // DateTimeField::setMaximumDateTime() //

//------------------------------------------------------------------------------
///@brief Add a new format to the format selector.
///@param text    QString containing the text to be displayed on format
///               selector.
///       format  QString containing the QDateTime format.
//------------------------------------------------------------------------------
void DateTimeField::addFormat(QString const& text, QString const& format)
{
    m_Data->m_DateTimePicker->addFormat(text, format);
} // DateTimePicker::addFormat() //

//------------------------------------------------------------------------------
///@brief Remove a format from the format selector.
///@param text   QString containing the displayed text in the selector of the
///              format to be removed.
//------------------------------------------------------------------------------
void DateTimeField::removeFormat(QString const& text)
{
    m_Data->m_DateTimePicker->removeFormat(text);
} // DateTimePicker::removeFormat() //

//------------------------------------------------------------------------------
///@brief Get value of m_Field.
//------------------------------------------------------------------------------
QString DateTimeField::value() const
{
    return m_Data->m_Field->value();
} // DateTimeField::value() //

//------------------------------------------------------------------------------
///@brief Set value of m_Field to a new value.
///@param new_value    QString containing the new value for m_Field.
//------------------------------------------------------------------------------
void DateTimeField::setValue(QString const& new_value)
{
    return m_Data->m_Field->setValue(new_value);
} // DateTimeField::setValue() //

//------------------------------------------------------------------------------
///@brief Get name of m_Field.
//------------------------------------------------------------------------------
QString DateTimeField::name() const
{
    return m_Data->m_Field->name();
} // DateTimeField::name() //

//------------------------------------------------------------------------------
///@brief Set name of m_Field to a new value.
///@param new_value    QString containing the new name for m_Field.
//------------------------------------------------------------------------------
void DateTimeField::setName(QString const& new_name)
{
    m_Data->m_Field->setName(new_name);
} // DateTimeField::setName() //

//------------------------------------------------------------------------------
///@brief Open dialog for selecting a date and time.
//------------------------------------------------------------------------------
void DateTimeField::openDialog()
{
    m_Data->m_DateTimePicker->setLineEdit(value());
    m_Data->m_DateTimePicker->showPicker();
} // DateTimeField::openDialog() //

//------------------------------------------------------------------------------
///@brief Update the value of the m_Field based on user selection in
///       m_DateTimePicker.
///@pre The "ok" button is pressed in the m_DateTimePicker window.
//------------------------------------------------------------------------------
void DateTimeField::onDateTimeChange()
{
    QDateTime new_date_time;
    new_date_time.setDate(m_Data->m_DateTimePicker->date());
    new_date_time.setTime(m_Data->m_DateTimePicker->time());
    setValue(new_date_time.toString());
} // DateTimeField::onDateChange() //
} // namespace qt //
} // namespace lib //