//------------------------------------------------------------------------------
///@file lib_qt_datetimepicker.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------

#ifndef LIB_QT_DATETIMEPICKER_H_FILE_GUARD
#define LIB_QT_DATETIMEPICKER_H_FILE_GUARD

#include "lib_qt_field.h"
#include "lib_qt_rememberfield.h"

#include <memory>
#include <QAbstractButton>
#include <QCalendarWidget>
#include <QGroupBox>
#include <QStringList>
#include <QTreeWidgetItem>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///
///@par Class: DateTimePicker
///
///@brief   Dialog to prompt the user for a date and time.
///
///@par Thread Safety:  none (Qt GUI)
///
///@par Class Knowledge
///         -   This class knows about:
///             -   The selection of QDate and QTime
///             -   The conversion from QString to QDateTime
///         -   This class hides:
///             -   How m_TimeSelector is populated
///             -   Struct data containing widget componenets
///         -   This class is agnostic about:
///             -   Whether the user correctly sets the min and max of
///                 m_TimeSelector.
///
///@par Expected Usage:
///     @code
///         struct MyDateTimeField::Data {
///             QHBoxLayout*            m_Layout;
///             LineEditRememberField*  m_Field;
///             DateTimePicker*         m_DateTimePicker;
///             QPushButton*            m_ThreeDots;
///
///             Data(QString const& field_name)
///                 : m_Layout(new QHBoxLayout)
///                 , m_Field(new LineEditRememberField(field_name, nullptr))
///                 , m_DateTimePicker(new DateTimePicker(m_Field->value(), nullptr))
///                 , m_ThreeDots(new QPushButton("..."))
///             {
///                 m_Layout->addWidget(m_Field);
///                 m_Layout->addWidget(m_ThreeDots);
///             }
///         }; // Struct MyDateTimeField::Data
///
///         MyDateTimeField::MyDateTimeField(
///               QString const& field_name
///             , QWidget* parent
///         )   : QWidget(parent)
///             , Field(field_name)
///             , m_Data(new Data(field_name))
///         {
///             setLayout(m_Data->m_Layout);
///             connect(
///                 m_Data->m_ThreeDots, SIGNAL(clicked())
///                 , this, SLOT(showPicker())
///             );
///         } // MyDateTimeField::MyDateTimeField() //
///     @endcode
///
///@version 2020-09-15  PN     Added setLineEdit(), todayButtonPressed()
///                            , nowButtonPressed(), onDateChange()
///                            , onTimeChange(), populateList()
///                            , updateLineEdit(), findItemInTree().
///@version 2020-09-14  PN     Added setMinimumDate(), setMaximumDate()
///                            , setMinimumTime(), setMaximumTime, setDate()
///                            , setTime(), showPicker(), hidePicker()
///                            , date(), and time().
///@version 2020-09-11  PN     Moved all functions to DateTimeField class.
///                            Updated DateTimePicker to be a dialog container
///                            for handling user selection of date and time.
///@version 2020-09-10  PN     Removed isValidDate(). Added cancel().
///@version 2020-09-03  PN     File creation. Detailed design. Added constructor
///                            , destructor, setMinimumDate(), setMaximumDate()
///                            , openDialog(), isValidDate(), name(), setName()
///                            , value(), and setValue().
///
//------------------------------------------------------------------------------
class DateTimePicker : public QGroupBox
{
    Q_OBJECT

public:
    DateTimePicker(QWidget* parent = nullptr);
    ~DateTimePicker();

//------------------------------------------------------------------------------
// Each QCalenderWidget and QTimeEdit will have a min and max.
//------------------------------------------------------------------------------
    void setMinimumDate(QDate const& new_date);
    void setMaximumDate(QDate const& new_date);
    void setMinimumTime(QTime const& new_time);
    void setMaximumTime(QTime const& new_time);
    void setDate(QDate const& new_date);
    void setTime(QTime const& new_time);
    void setLineEdit(QString const& new_date_time);
    void showPicker();
    void hidePicker();
    QDate date() const;
    QTime time() const;

public slots:
    void todayButtonPressed();
    void nowButtonPressed();
    void okButtonPressed();
    void onDateChange();
    void onTimeChange();

signals:
    void acceptChanges();

private:
    struct Data;
    std::unique_ptr<Data> m_Data;
    void populateList();
    void updateLineEdit();
    QTreeWidgetItem* findItemInTree(QTime const& time);
}; // class DateTimePicker //
} // namespace qt //
} // namespace lib //

#endif // LIB_QT_DATETIMEPICKER_H //
