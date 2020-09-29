//------------------------------------------------------------------------------
///@file lib_qt_datetimefield.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------

#ifndef LIB_QT_DATETIMEFIELD_H_FILE_GUARD
#define LIB_QT_DATETIMEFIELD_H_FILE_GUARD

#include "lib_qt_field.h"

#include <memory>
#include <QDateTime>
#include <QWidget>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///
///@par Class: DateTimeField
///
///@brief   Field that contains a QLineEdit and a "three dots button" for
///         handling user input of a date.
///
///@par Thread Safety:  none (Qt GUI)
///
///@par Class Knowledge
///         -   This class knows about:
///             -   Field Class.
///         -   This class hides:
///             -   Struct Data containing the componenets of
///                 DateTimeField Class and containing class state data.
///         -   This class is agnostic about:
///             -   Whether the date is entered or searched
///                 from QCalenderWidget.
///
///@par Expected Usage:
///     @code
///         QFormLayout*        m_Layout(new QFormLayout);
///         DateTimeField*     m_DateTimeField(new DateTimeField);
///         m_Layout->addRow("Date Selector:", m_DateTimeField);
///         setLayout(m_Layout);
///
///         struct MyDialogClass::Data
///         {
///                QFormLayout*                 m_Layout;
///                lib::qt::PathField*          m_InputFile;
///                lib::qt::DateTimeField*      m_StartTime;
///                lib::qt::DateTimeField*      m_EndTime;
///                Data()
///                     : m_StartTime(new lib::qt::DateTimeField("Start"))
///                     , m_EndTime(new lib::qt::DateTimeField("End"))
///                {
///                     m_Layout->addRow("Input", m_InputFile);
///                     m_Layout->addRow("Start", m_StartTime);
///                     m_Layout->addRow("End", m_EndTime);
///                }
///         };
///     @endcode
///
///@version 2020-09-21  PN     Added addFormat() and removeFormat().
///@version 2020-09-14  PN     Add setMaximumDate(), setMinimumDate,
///                            setMaximumTime(), and setMinimumTime().
///@version 2020-09-11  PN     File creation. Moved all previous functions from
///                            DateTimePicker class to DateTimeField.
//------------------------------------------------------------------------------

class DateTimeField
        : public QWidget
        , public Field
{
    Q_OBJECT

public:
    explicit DateTimeField(
          QString const& field_name
        , QWidget* parent = nullptr
    );
    DateTimeField(DateTimeField&& that) = default;
    DateTimeField(const DateTimeField& that) = delete;
    DateTimeField& operator=(const DateTimeField& that) = delete;
    ~DateTimeField();

    void setMinimumDate(QDate const& new_date);
    void setMaximumDate(QDate const& new_date);
    void setMinimumTime(QTime const& new_time);
    void setMaximumTime(QTime const& new_time);
    void addFormat(QString const& text, QString const& format);
    void removeFormat(QString const& text);

    virtual QString value() const override;
    virtual void setValue(QString const& new_value) override;
    virtual QString name() const override;
    virtual void setName(const QString &new_name) override;

public slots:
//------------------------------------------------------------------------------
// SIGNAL(m_Data->m_ThreeDots->clicked()) , SLOT(openDialog())
//------------------------------------------------------------------------------
    void openDialog();
//------------------------------------------------------------------------------
// SIGNAL(m_Data->m_DateTimePicker->acceptChanges()) , SLOT(onDateTimeChange())
//------------------------------------------------------------------------------
    void onDateTimeChange();

private:
    struct Data;
    std::unique_ptr<Data> m_Data;

}; // class DateTimeField //
} // namespace qt //
} // namespace lib //
#endif // LIB_QT_DATETIMEFIELD_H_FILE_GUARD
