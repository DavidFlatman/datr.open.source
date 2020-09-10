//------------------------------------------------------------------------------
///@file lib_qt_datetimepicker.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------

#ifndef LIB_QT_DATETIMEPICKER_H_FILE_GUARD
#define LIB_QT_DATETIMEPICKER_H_FILE_GUARD

#include "lib_qt_field.h"

#include <memory>
#include <QCalendarWidget>
#include <QStringList>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///
///@par Class: DateTimePicker
///
///@brief   Prompt the user for a start time and a end time.
///
///@par Thread Safety:  none (Qt GUI)
///
///@par Class Knowledge
///         -   This class knows about:
///             -   Field Class.
///         -   This class hides:
///             -   Struct Data containing the componenets of
///                 DateTimePicker Class and containing class state data.
///         -   This class is agnostic about:
///             -   Whether the date is entered or searched
///                 from QCalenderWidget.
///
///@par Expected Usage:
///     @code
///         QFormLayout*        m_Layout(new QFormLayout);
///         DateTimePicker*     m_DateTimePicker(new DateTimePicker);
///         m_Layout->addRow("Date Selector:", m_DateTimePicker);
///         setLayout(m_Layout);
///
///         struct MyDialogClass::Data
///         {
///                QFormLayout*                 m_Layout;
///                lib::qt::PathField*          m_InputFile;
///                lib::qt::DateTimePicker*     m_StartTime;
///                lib::qt::DateTimePicker*     m_EndTime;
///                Data()
///                     : m_StartTime(new lib::qt::DateTimePicker("Start"))
///                     , m_EndTime(new lib::qt::DateTimePicker("End"))
///                {
///                     m_Layout->addRow("Input", m_InputFile);
///                     m_Layout->addRow("Start", m_StartTime);
///                     m_Layout->addRow("End", m_EndTime);
///                }
///         };
///     @endcode
///
///@version 2020-09-10  PN     Removed isValidDate(). Added cancel().
///@version 2020-09-03  PN     File creation. Detailed design. Added constructor
///                            , destructor, setMinimumDate(), setMaximumDate()
///                            , openDialog(), isValidDate(), name(), setName()
///                            , value(), and setValue().
//------------------------------------------------------------------------------
class DateTimePicker
        : public QWidget
        , public Field
{
    Q_OBJECT

    public:
        explicit DateTimePicker(
              QString const& field_name
            , QWidget* parent = nullptr
        );
        DateTimePicker(DateTimePicker&& that) = default;
        DateTimePicker(const DateTimePicker& that) = delete;
        DateTimePicker& operator=(const DateTimePicker& that) = delete;
        ~DateTimePicker();

//------------------------------------------------------------------------------
// Each QCalenderWidget and QTimeEdit will have a min and max date.
//------------------------------------------------------------------------------
        void setMinimumDateTime(QDateTime const& new_date);
        void setMaximumDateTime(QDateTime const& new_date);

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
// SIGNAL(m_Data->m_ButtonBox->accepted()) , SLOT(onPathChange())
//------------------------------------------------------------------------------
        void onDateChange();
//------------------------------------------------------------------------------
// SIGNAL(m_Data->m_ButtonBox->rejected()) , SLOT(cancel())
//------------------------------------------------------------------------------
        void cancel();

    private:
        struct Data;
        std::unique_ptr<Data> m_Data;

}; // class DateTimePicker //
} // namespace qt //
} // namespace lib //

#endif // LIB_QT_DATETIMEPICKER_H //
