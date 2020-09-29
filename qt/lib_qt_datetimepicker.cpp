//------------------------------------------------------------------------------
///@file lib_qt_datetimepicker.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-09-21  PN     Added addFormat() and removeFormat(). Replaced
///                            QComboBox with FormatSelectorRememberField.
///@version 2020-09-18  PN     Replaced QTreeWidget with QListWidget and added a
///                            QLabel for the m_TimeSelector. Removed
///                            findItemInTree(). Updated setMinimumTime() and
///                            setMaximumTime().
///@version 2020-09-17  PN     Removed header color change. Updated
///                            nowButtonPressed() to also change m_Calendar.
///                            Replaced implementation for converting from QTime
///                            to QString with the QVariant class. Added
///                            QComboBox* m_FormatSelector to allow user to
///                            select QDateTime format.
///                            Added a selector for changing DateTime format.
///@version 2020-09-15  PN     Added implementation for setLineEdit()
///                            , todayButtonPressed(), nowButtonPressed()
///                            , onDateChange(), onTimeChange(), populateList()
///                            , updateLineEdit(), findItemInTree().
///@version 2020-09-14  PN     Added implementation for setMinimumDate()
///                            , setMaximumDate(), setMinimumTime()
///                            , setMaximumTime, setDate(), setTime()
///                            , showPicker(), hidePicker(), date(), and time().
///@version 2020-09-11  PN     Removed all functions. Updated DateTimePicker
///                            class to be a dialog window for handling user
///                            selection of date and time.
///@version 2020-09-10  PN     Removed isValidDate(). Added cancel(), m_Time,
///                            m_ButtonBox, m_BoxLayout, and m_Box.
///@version 2020-09-09  PN     Implemented struct Data, constructor, destructor
///                            , setMinimumDate(), setMaximumDate(), value(),
///                            , setValue(), name(), setName(), openDialog()
///                            , onDateChange(), and isValidDate().
///@version 2020-09-03  PN     File creation.
///
//------------------------------------------------------------------------------

#include "lib_qt_datetimepicker.h"
#include "lib_qt_formatselectorrememberfield.h"

#include <QCalendarWidget>
#include <QDateTime>
#include <QDebug>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>


namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///@brief Data struct for the DateTimePicker class
//------------------------------------------------------------------------------
struct DateTimePicker::Data {
    QGridLayout*                    m_Layout;
    QLineEdit*                      m_LineEdit;
    FormatSelectorRememberField*    m_FormatSelector;
    QCalendarWidget*                m_Calendar;
    QVBoxLayout*                    m_TimeLayout;
    QLabel*                         m_TimeLabel;
    QListWidget*                    m_TimeSelector;
    QPushButton*                    m_Today;
    QPushButton*                    m_Now;
    QPushButton*                    m_Cancel;
    QPushButton*                    m_Ok;
    QDate                           m_Date;
    QTime                           m_Time;

    Data()
        : m_Layout(new QGridLayout)
        , m_LineEdit(new QLineEdit)
        , m_FormatSelector(new FormatSelectorRememberField("format", nullptr))
        , m_Calendar(new QCalendarWidget)
        , m_TimeLayout(new QVBoxLayout)
        , m_TimeLabel(new QLabel("Time"))
        , m_TimeSelector(new QListWidget)
        , m_Today(new QPushButton("Today"))
        , m_Now(new QPushButton("Now"))
        , m_Cancel(new QPushButton("Cancel"))
        , m_Ok(new QPushButton("Ok"))
        , m_Date(m_Calendar->selectedDate())
    {
        m_TimeLabel->setAlignment(Qt::AlignCenter);
        m_TimeLabel->setStyleSheet(
            "border: 1px solid; padding: 2px"
        );
        m_TimeLayout->addWidget(m_TimeLabel);
        m_TimeLayout->addWidget(m_TimeSelector);
        m_Layout->setVerticalSpacing(25);
        m_Layout->addWidget(m_LineEdit, 0, 0);
        m_Layout->addWidget(m_FormatSelector, 0, 2, 1, 2);
        m_Layout->addWidget(m_Calendar, 1, 0);
        m_Layout->addLayout(m_TimeLayout, 1, 2, 1, 2);
        m_Layout->addWidget(m_Today, 2, 0);
        m_Layout->addWidget(m_Now, 2, 1);
        m_Layout->addWidget(m_Cancel, 2, 2);
        m_Layout->addWidget(m_Ok, 2, 3);   
    }
};

//------------------------------------------------------------------------------
///@brief Initialize inherited classe, connecting slot and signals, and layout.
//------------------------------------------------------------------------------
DateTimePicker::DateTimePicker(QString const& title, QWidget* parent)
  : QGroupBox(title, parent)
  , m_Data(new Data())
{
    setAlignment(Qt::AlignCenter);
    setLayout(m_Data->m_Layout);
    populateList();
//------------------------------------------------------------------------------
// Select first item in m_TimeSelector to avoid nullptr when calling
// m_TimeSelector->currentItem();
//------------------------------------------------------------------------------
    m_Data->m_TimeSelector->setCurrentItem(m_Data->m_TimeSelector->item(1));
    connect(
          m_Data->m_Today, SIGNAL(clicked())
        , this, SLOT(todayButtonPressed())
    );
    connect(
          m_Data->m_Now, SIGNAL(clicked())
        , this, SLOT(nowButtonPressed())
    );
    connect(
          m_Data->m_Ok, SIGNAL(clicked())
        , this, SLOT(okButtonPressed())
    );
    connect(
          m_Data->m_Cancel, SIGNAL(clicked())
        , this, SLOT(hide())
    );
    connect(
          m_Data->m_Calendar, SIGNAL(selectionChanged())
        , this, SLOT(onDateChange())
    );
    connect(
          m_Data->m_TimeSelector, SIGNAL(
          currentItemChanged(QListWidgetItem *, QListWidgetItem *)
      ) , this, SLOT(onTimeChange())
    );
    connect(
          m_Data->m_FormatSelector, SIGNAL(
          currentIndexChanged(const QString &)
      ) , this, SLOT(updateLineEdit())
    );
} // DateTimePicker::DateTimePicker() //

//------------------------------------------------------------------------------
///@brief Reclaim resources held by object.
//------------------------------------------------------------------------------
DateTimePicker::~DateTimePicker()
{
} // DateTimePicker::~DateTimePicker() //

//------------------------------------------------------------------------------
///@brief Set minimum date for m_Calendar.
///@param new_date    QDate containing the minimum available date that the user
///                   can select.
//------------------------------------------------------------------------------
void DateTimePicker::setMinimumDate(QDate const& new_date)
{
    m_Data->m_Calendar->setMinimumDate(new_date);
} // DateTimePicker::setMinimumDate() //

//------------------------------------------------------------------------------
///@brief Set maximum date for m_Calendar.
///@param new_date    QDate containing the maximum available date that the user
///                   can select.
//------------------------------------------------------------------------------
void DateTimePicker::setMaximumDate(QDate const& new_date)
{
    m_Data->m_Calendar->setMaximumDate(new_date);
} // DateTimePicker::setMaximumDate() //

//------------------------------------------------------------------------------
///@brief Set minimum time for m_TimeSelector
///@param new_time    QTime containing the minimum available time that the user
///                   can select in m_TimeSelector. Requires QTime.minute to be
///                   either a 0 or 30 since m_TimeSelector only has the two
///                   instances.
//------------------------------------------------------------------------------
void DateTimePicker::setMinimumTime(QTime const& new_time)
{
    if(new_time.minute() == 0 || new_time.minute() == 30)
    {
        for(int i = 0; i < m_Data->m_TimeSelector->count(); ++i)
        {
            QListWidgetItem* item = m_Data->m_TimeSelector->item(i);
            if(item->data(Qt::AccessibleDescriptionRole) == new_time)
            {
                break;
            }
            item->setHidden(true);
        }
    } else {
        qDebug() << "Warning: setMinimumTime() Invalid Input" << new_time;
    }
} // DateTimePicker::setMinimumTime //

//------------------------------------------------------------------------------
///@brief Set maximum time for m_TimeSelector
///@param new_time    QTime containing the maximum available time that the user
///                   can select in m_TimeSelector. Requires QTime.minute to be
///                   either a 0 or 30 since m_TimeSelector only has the two
///                   instances.
//------------------------------------------------------------------------------
void DateTimePicker::setMaximumTime(QTime const& new_time)
{
    if(new_time.minute() == 0 || new_time.minute() == 30)
    {
        bool belowItem = false;
        for(int i = 0; i < m_Data->m_TimeSelector->count(); ++i)
        {
            QListWidgetItem* item = m_Data->m_TimeSelector->item(i);
            if(belowItem)
            {
                item->setHidden(true);
            }
            if(item->data(Qt::AccessibleDescriptionRole) == new_time)
            {
                belowItem = true;
            }
        }
    } else {
        qDebug() << "Warning: setMaximumTime() Invalid Input" << new_time;
    }
} // DateTimePicker::setMaximumTime //

//------------------------------------------------------------------------------
///@brief Shows the date time picker window.
//------------------------------------------------------------------------------
void DateTimePicker::showPicker()
{
    QGroupBox::show();
} // DateTimePicker::showPicker() //

//------------------------------------------------------------------------------
///@brief Hides the date time picker window.
//------------------------------------------------------------------------------
void DateTimePicker::hidePicker()
{
    QGroupBox::hide();
} // DateTimePicker::hidePicker() //

//------------------------------------------------------------------------------
///@brief Add a new format to the format selector.
///@param text    QString containing the text to be displayed on format
///               selector.
///       format    QString containing the format.
//------------------------------------------------------------------------------
void DateTimePicker::addFormat(QString const& text, QString const& format)
{
    m_Data->m_FormatSelector->addFormat(text, format);
} // DateTimePicker::addFormat() //

//------------------------------------------------------------------------------
///@brief Remove a format from the format selector.
///@param row    Int containing the row of the format to be removed.
//------------------------------------------------------------------------------
void DateTimePicker::removeFormat(QString const& text)
{
    m_Data->m_FormatSelector->removeFormat(text);
} // DateTimePicker::removeFormat() //

//------------------------------------------------------------------------------
///@brief Sets the current selected date for m_Date.
///@param new_date    QDate containing the new date selected.
//------------------------------------------------------------------------------
void DateTimePicker::setDate(QDate const& new_date)
{
    m_Data->m_Date = new_date;
    updateLineEdit();
} // DateTimePicker::setDate() //

//------------------------------------------------------------------------------
///@brief Sets the current select time for m_Time.
///@param new_time    QTime containing the new time selected.
//------------------------------------------------------------------------------
void DateTimePicker::setTime(QTime const& new_time)
{
    m_Data->m_Time = new_time;
    updateLineEdit();
} // DateTimePicker::setTime() //

//------------------------------------------------------------------------------
///@brief Sets the new value for m_LineEdit.
///@param new_date_time    QString containing the new text value.
//------------------------------------------------------------------------------
void DateTimePicker::setLineEdit(QString const& new_date_time)
{
    m_Data->m_LineEdit->setText(new_date_time);
} // DateTimePicker::setLineEdit() //

//------------------------------------------------------------------------------
///@brief Gets the m_Date.
//------------------------------------------------------------------------------
QDate DateTimePicker::date() const
{
    return m_Data->m_Date;
} // DateTimePicker::date() //

//------------------------------------------------------------------------------
///@brief Gets the m_Time.
//------------------------------------------------------------------------------
QTime DateTimePicker::time() const
{
    return m_Data->m_Time;
} // DateTimePicker::time() //

//------------------------------------------------------------------------------
///@brief Sets the m_Date to the current date.
//------------------------------------------------------------------------------
void DateTimePicker::todayButtonPressed()
{
    m_Data->m_Calendar->setSelectedDate(QDate::currentDate());
} // DateTimePicker::todayButtonPressed() //

//------------------------------------------------------------------------------
///@brief Sets the m_Time to the current time.
//------------------------------------------------------------------------------
void DateTimePicker::nowButtonPressed()
{
    setTime(QTime::currentTime());
    todayButtonPressed();
} // DateTimePicker::nowButtonPressed() //

//------------------------------------------------------------------------------
///@brief Hides the window and emits the acceptChanges signal for the
///       DateTimeField class to receive.
//------------------------------------------------------------------------------
void DateTimePicker::okButtonPressed()
{
    hidePicker();
    emit acceptChanges();
} // DateTimePicker::doneButtonPressed() //

//------------------------------------------------------------------------------
///@brief Sets the m_Date to the selected date.
//------------------------------------------------------------------------------
void DateTimePicker::onDateChange()
{
    setDate(m_Data->m_Calendar->selectedDate());
} // DateTimePicker::onDateChange() //

//------------------------------------------------------------------------------
///@brief Sets the m_Time to the selected time.
//------------------------------------------------------------------------------
void DateTimePicker::onTimeChange()
{
    setTime(QVariant(m_Data->m_TimeSelector->currentItem()
                     ->data(Qt::AccessibleDescriptionRole)).toTime());
} // DateTimePicker::onTimeChange() //

//------------------------------------------------------------------------------
///@brief Sets m_LineEdit to the current values of m_Date and m_Time.
//------------------------------------------------------------------------------
void DateTimePicker::updateLineEdit()
{
    setLineEdit(QDateTime(m_Data->m_Date, m_Data->m_Time, Qt::UTC)
                .toString(m_Data->m_FormatSelector->value()));
    m_Data->m_LineEdit->setToolTip(m_Data->m_FormatSelector->value());
} // DateTimePicker::updateLineEdit() //

//------------------------------------------------------------------------------
///@brief Populate m_Time of available times that the user can select.
//------------------------------------------------------------------------------
void DateTimePicker::populateList()
{
    QListWidgetItem* item;
    for(int i = 0; i < 24; i++)
    {
        item = new QListWidgetItem(QTime(i, 0, 0).toString("h:mm A"));
        item->setData(Qt::AccessibleDescriptionRole, QVariant(QTime(i,0,0)));
        item->setTextAlignment(Qt::AlignCenter);
        m_Data->m_TimeSelector->addItem(item);
        item = new QListWidgetItem(QTime(i, 30, 0).toString("h:mm A"));
        item->setData(Qt::AccessibleDescriptionRole, QVariant(QTime(i,30,0)));
        item->setTextAlignment(Qt::AlignCenter);
        m_Data->m_TimeSelector->addItem(item);
    }
} // DateTimePicker::populateList() //
} // namespace qt //
} // namespace lib //
