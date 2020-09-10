//------------------------------------------------------------------------------
///@file lib_qt_datetimepicker.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-09-10  PN     Removed isValidDate(). Added cancel(), m_Time,
///                            m_ButtonBox, m_BoxLayout, and m_Box.
///@version 2020-09-09  PN     Implemented struct Data, constructor, destructor
///                            , setMinimumDate(), setMaximumDate(), value(),
///                            , setValue(), name(), setName(), openDialog()
///                            , onDateChange(), and isValidDate().
///@version 2020-09-03  PN     File creation.
//------------------------------------------------------------------------------

#include "lib_qt_datetimepicker.h"
#include "lib_qt_lineeditrememberfield.h"

#include <iostream>
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
struct DateTimePicker::Data {
    QHBoxLayout*            m_Layout;
    LineEditRememberField*  m_Field;
    QGroupBox*              m_Box;
    QGridLayout*            m_BoxLayout;
    QCalendarWidget*        m_Calendar;
    QTimeEdit*              m_Time;
    QDialogButtonBox*       m_ButtonBox;
    QPushButton*            m_ThreeDots;
    QDateTime               m_MinimumDateTime;
    QDateTime               m_MaximumDateTime;

    Data(QString const& field_name)
        : m_Layout(new QHBoxLayout)
        , m_Field(new LineEditRememberField(field_name, nullptr))
        , m_Box(new QGroupBox)
        , m_BoxLayout(new QGridLayout)
        , m_Calendar(new QCalendarWidget)
        , m_Time(new QTimeEdit)
        , m_ButtonBox(
              new QDialogButtonBox(
                   QDialogButtonBox::Cancel | QDialogButtonBox::Save
              )
          )
        , m_ThreeDots(new QPushButton("..."))
    {
        m_Layout->addWidget(m_Field);
        m_Layout->addWidget(m_ThreeDots);
        m_BoxLayout->addWidget(m_Calendar);
        m_BoxLayout->addWidget(m_Time);
        m_BoxLayout->addWidget(m_ButtonBox);
        m_Box->setLayout(m_BoxLayout);
    }

}; // Struct DateTimePicker::Data

//------------------------------------------------------------------------------
///@brief Initialize inherited classes, connecting slot and signals, and layout.
//------------------------------------------------------------------------------
DateTimePicker::DateTimePicker(
      QString const& field_name
    , QWidget* parent
)   : QWidget(parent)
    , Field(field_name)
    , m_Data(new Data(field_name))
{
    setLayout(m_Data->m_Layout);
    connect(
          m_Data->m_ThreeDots, SIGNAL(clicked())
        , this, SLOT(openDialog())
    );
    connect(
        m_Data->m_ButtonBox, SIGNAL(accepted())
        , this, SLOT(onDateChange())
    );
    connect(
        m_Data->m_ButtonBox, SIGNAL(rejected())
        , this, SLOT(cancel())
    );
} // DateTimePicker::DateTimePicker() //

//------------------------------------------------------------------------------
///@brief Reclaim resources held by object.
//------------------------------------------------------------------------------
DateTimePicker::~DateTimePicker()
{
} // DateTimePicker::~DateTimePicker() //

//------------------------------------------------------------------------------
///@brief Set m_MinimumDateTime to a new value.
///@param new_date  QDateTime containing the new minimum date and time.
//------------------------------------------------------------------------------
void DateTimePicker::setMinimumDateTime(QDateTime const& new_date)
{
    m_Data->m_MinimumDateTime = new_date;
} // DateTimePicker::setMinimumDateTime() //

//------------------------------------------------------------------------------
///@brief Set m_MaximumDateTime to a new value.
///@param new_date  QDateTime containing the new maximum date and time.
//------------------------------------------------------------------------------
void DateTimePicker::setMaximumDateTime(QDateTime const& new_date)
{
    m_Data->m_MaximumDateTime = new_date;
} // DateTimePicker::setMaximumDateTime() //

//------------------------------------------------------------------------------
///@brief Get value of m_Field.
//------------------------------------------------------------------------------
QString DateTimePicker::value() const
{
    return m_Data->m_Field->value();
} // DateTimePicker::value() //

//------------------------------------------------------------------------------
///@brief Set value of m_Field to a new value.
///@param new_value  QString containing the new value for m_Field.
//------------------------------------------------------------------------------
void DateTimePicker::setValue(QString const& new_value)
{
    return m_Data->m_Field->setValue(new_value);
} // DateTimePicker::setValue() //

//------------------------------------------------------------------------------
///@brief Get name of m_Field.
//------------------------------------------------------------------------------
QString DateTimePicker::name() const
{
    return m_Data->m_Field->name();
} // DateTimePicker::name() //

//------------------------------------------------------------------------------
///@brief Set name of m_Field to a new value.
///@param new_value  QString containing the new name for m_Field.
//------------------------------------------------------------------------------
void DateTimePicker::setName(QString const& new_name)
{
    return m_Data->m_Field->setName(new_name);
} // DateTimePicker::setName() //

//------------------------------------------------------------------------------
///@brief Open dialog for selecting a date and time.
//------------------------------------------------------------------------------
void DateTimePicker::openDialog()
{
    m_Data->m_Calendar->setMinimumDate(m_Data->m_MinimumDateTime.date());
    m_Data->m_Calendar->setMaximumDate(m_Data->m_MaximumDateTime.date());
    m_Data->m_Time->setMinimumTime(m_Data->m_MinimumDateTime.time());
    m_Data->m_Time->setMaximumTime(m_Data->m_MaximumDateTime.time());
    m_Data->m_Box->show();
} // DateTimePicker::openDialog() //

//------------------------------------------------------------------------------
///@brief Update the value of the m_Field based on user dialog selection.
///@pre The "save" button is pressed in the m_Box dialog.
//------------------------------------------------------------------------------
void DateTimePicker::onDateChange()
{
    QDateTime new_date_time;
    new_date_time.setDate(m_Data->m_Calendar->selectedDate());
    new_date_time.setTime(m_Data->m_Time->time());
    setValue(new_date_time.toString());
    m_Data->m_Box->hide();
} // DateTimePicker::onDateChange() //

//------------------------------------------------------------------------------
///@brief Close the m_Box dialog.
///@pre The "cancel" button is pressed in the m_Box dialog.
//------------------------------------------------------------------------------
void DateTimePicker::cancel()
{
    m_Data->m_Box->hide();
} // DateTimePicker::cancel() //
} // namespace qt //
} // namespace lib //
