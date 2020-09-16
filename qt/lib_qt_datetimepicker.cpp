//------------------------------------------------------------------------------
///@file lib_qt_datetimepicker.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
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

#include <iostream>
#include <QCalendarWidget>
#include <QDateTime>
#include <QGridLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QList>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>


namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///@brief Data struct for the DateTimePicker class
//------------------------------------------------------------------------------
struct DateTimePicker::Data {
    QGridLayout*            m_Layout;
    QLineEdit*              m_LineEdit;
    QCalendarWidget*        m_Calendar;
    QHeaderView*            m_Header;
//------------------------------------------------------------------------------
// Note: We are using QTreeWidget instead of QListWidget because QListWidget
// does not support headers.
//------------------------------------------------------------------------------
    QTreeWidget*            m_TimeSelector;
    QPushButton*            m_Today;
    QPushButton*            m_Now;
    QPushButton*            m_Cancel;
    QPushButton*            m_Done;
    QDate                   m_Date;
    QTime                   m_Time;

    Data()
        : m_Layout(new QGridLayout)
        , m_LineEdit(new QLineEdit)
        , m_Calendar(new QCalendarWidget)
        , m_TimeSelector(new QTreeWidget)
        , m_Today(new QPushButton("Today"))
        , m_Now(new QPushButton("Now"))
        , m_Cancel(new QPushButton("Cancel"))
        , m_Done(new QPushButton("Ok"))
    {
        m_TimeSelector->setHeaderLabel("Time");
        m_TimeSelector->header()->setDefaultAlignment(Qt::AlignCenter);
//------------------------------------------------------------------------------
//  Setting header color for both m_Calendar and m_Time for consistency.
//------------------------------------------------------------------------------
        m_TimeSelector->header()->setStyleSheet(
            "QHeaderView::section { background-color:blue }"
        );
        m_Calendar->setStyleSheet(
            "QCalendarWidget QWidget#qt_calendar_navigationbar"
            "{ background-color:blue }"
        );
        m_Layout->setVerticalSpacing(25);
        m_Layout->addWidget(m_LineEdit, 0, 0, 1, 5);
        m_Layout->addWidget(m_Calendar, 1, 0, 1, 1);
        m_Layout->addWidget(m_TimeSelector, 1, 2, 1, 2);
        m_Layout->addWidget(m_Today, 2, 0);
        m_Layout->addWidget(m_Now, 2, 1);
        m_Layout->addWidget(m_Cancel, 2, 2);
        m_Layout->addWidget(m_Done, 2, 3);
        m_Date = m_Calendar->selectedDate();
    }
};

//------------------------------------------------------------------------------
///@brief Initialize inherited classe, connecting slot and signals, and layout.
//------------------------------------------------------------------------------
DateTimePicker::DateTimePicker(QWidget* parent)
  : QGroupBox(parent)
  , m_Data(new Data())
{
    setLayout(m_Data->m_Layout);
    populateList();
//------------------------------------------------------------------------------
// Select first item in m_TimeSelector to avoid nullptr when calling
// m_TimeSelector->currentItem();
//------------------------------------------------------------------------------
    m_Data->m_TimeSelector->setCurrentItem(m_Data->m_TimeSelector->itemAt(1,1));
    connect(
          m_Data->m_Today, SIGNAL(clicked())
        , this, SLOT(todayButtonPressed())
    );
    connect(
          m_Data->m_Now, SIGNAL(clicked())
        , this, SLOT(nowButtonPressed())
    );
    connect(
          m_Data->m_Done, SIGNAL(clicked())
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
          currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)
      ) , this, SLOT(onTimeChange())
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
        QTreeWidgetItem* item = findItemInTree(new_time);
        QTreeWidgetItemIterator iter(m_Data->m_TimeSelector);
        while((*iter) != item)
        {
            (*iter)->setHidden(true);
            ++iter;
            if((*iter) == nullptr)
            {
                break;
            }
        }
    } else {
        std::cout << "setMinimumTime: Invalid Input" << std::endl;
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
        QTreeWidgetItem* item = findItemInTree(new_time);
        QTreeWidgetItemIterator iter(m_Data->m_TimeSelector);
        bool belowItem = false;
        while((*iter) != nullptr)
        {
            if(belowItem)
            {
                (*iter)->setHidden(true);
            }
            if((*iter) == item)
            {
                belowItem = true;
            }
            ++iter;
        }
    } else {
        std::cout << "setMaximumTime: Invalid Input" << std::endl;
    }
} // DateTimePicker::setMaximumTime //

//------------------------------------------------------------------------------
///@brief Shows the widget window.
//------------------------------------------------------------------------------
void DateTimePicker::showPicker()
{
    QGroupBox::show();
} // DateTimePicker::showPicker() //

//------------------------------------------------------------------------------
///@brief Hides the widget window.
//------------------------------------------------------------------------------
void DateTimePicker::hidePicker()
{
    QGroupBox::hide();
} // DateTimePicker::hidePicker() //

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
    int hours = m_Data->m_TimeSelector->currentItem()->text(0)
            .split(":")[0].toInt();
    QString endString = m_Data->m_TimeSelector->currentItem()->text(0)
            .split(":")[1];
    int minutes = endString.split(" ")[0].toInt();
    bool beforeNoon = (endString.split(" ")[1] == "AM");
    if(beforeNoon)
    {
        if(hours == 12)
        {
            hours = 0;
        }
    } else {
        if(hours != 12)
        {
            hours += 12;
        }
    }
    setTime(QTime(hours, minutes, 0));
} // DateTimePicker::onTimeChange() //

//------------------------------------------------------------------------------
///@brief Populate m_Time of available times that the user can select.
//------------------------------------------------------------------------------
void DateTimePicker::populateList()
{
    bool am = true;
    QString text;
    QTreeWidgetItem* item;
    item = new QTreeWidgetItem(m_Data->m_TimeSelector);
    item->setText(0, "12:00 AM");
    item->setTextAlignment(0, Qt::AlignCenter);
    item = new QTreeWidgetItem(m_Data->m_TimeSelector);
    item->setText(0, "12:30 AM");
    item->setTextAlignment(0, Qt::AlignCenter);
    for(int i = 0; i < 2; i++)
    {
        for(int j = 1; j < 13; j++)
        {
            if(j == 12)
            {
                if(!am)
                {
                    continue;
                } else {
                    am = false;
                }
            }
            for(int k = 0; k < 60; k += 30)
            {
                item = new QTreeWidgetItem(m_Data->m_TimeSelector);
//------------------------------------------------------------------------------
// Format time. Preappends a '0' in front of the second digit. Example: 1:00 PM
//------------------------------------------------------------------------------
                text = (QString::number(j) + ":" +
                QString("%1").arg(k, 2, 10, QChar('0')));
                if(am)
                {
                    text += " AM";
                } else {
                    text += " PM";
                }
                item->setText(0, text);
                item->setTextAlignment(0, Qt::AlignCenter);
            }
        }
    }
} // DateTimePicker::populateList() //

//------------------------------------------------------------------------------
///@brief Sets m_LineEdit to the current values of m_Date and m_Time.
//------------------------------------------------------------------------------
void DateTimePicker::updateLineEdit()
{
    setLineEdit(QDateTime(m_Data->m_Date, m_Data->m_Time, Qt::UTC).toString());
} // DateTimePicker::updateLineEdit() //

//------------------------------------------------------------------------------
///@brief Returns the pointer to QTreeWidgetItem that is in the m_TimeSelector
///       based on the given QTime.
///@param time    QTime containing the time of the QTreeWidget item we are
///               searching for in m_TimeSelector.
//------------------------------------------------------------------------------
QTreeWidgetItem* DateTimePicker::findItemInTree(QTime const& time)
{
    int hours = time.hour();
    int minutes = time.minute();
    bool beforeNoon = true;
    if(hours > 12)
    {
        hours -= 12;
        beforeNoon = false;
    }
    QString findItem = (QString::number(hours) + ":" +
                        QString("%1").arg(minutes, 2, 10, QChar('0')));
    if(beforeNoon)
    {
        findItem.append(" AM");
    } else {
        findItem.append(" PM");
    }
    return m_Data->m_TimeSelector
            ->findItems(findItem, Qt::MatchExactly, 0).first();

} // DateTimePicker::findItemInTree() //
} // namespace qt //
} // namespace lib //
