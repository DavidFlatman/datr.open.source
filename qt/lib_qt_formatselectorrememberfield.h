//------------------------------------------------------------------------------
///@file lib_qt_formatselectorrememberfield.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------
#ifndef LIB_QT_FORMATSELECTORREMEMBERFIELD_H_FILE_GUARD
#define LIB_QT_FORMATSELECTORREMEMBERFIELD_H_FILE_GUARD

#include "lib_qt_rememberfield.h"

#include <QComboBox>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///
///@par Class: FormatSelectorRememberField
///
///@brief
///
///@par Thread Safety:  none (Qt GUI)
///
///@par Class Knowledge
///         -   This class knows about:
///             -   RememberField class
///             -   When to store/restore the RememberField
///         -   This class hides:
///         -   This class is agnostic about:
///             -   The underlying field-name/field-value storage mechanism
///
///@par Expected Usage:
///     @code
///         struct MyDateTimePicker::Data {
///             QGridLayout*                    m_Layout;
///             QLineEdit*                      m_LineEdit;
///             FormatSelectorRememberField*    m_FormatSelector;
///         Data()
///             : m_Layout(new QGridLayout)
///             , m_LineEdit(new QLineEdit)
///             , m_FormatSelector(new FormatSelectorRememberField(
///                                       "format"
///                                     , nullptr)
///                                 )
///             {
///                 m_Layout->addWidget(m_TimeSelector);
///             }
///         }
///
///     @endcode
///
///@version 2020-08-21  PN     File creation
//------------------------------------------------------------------------------

class FormatSelectorRememberField
    : public RememberField
    , public QComboBox
{
    public:
        explicit FormatSelectorRememberField(
              QString const& field_name
            , QWidget* parent = nullptr
        );
        FormatSelectorRememberField(
              QString const& field_name
            , QString const& field_value
            , QWidget* parent = nullptr
        );
        virtual ~FormatSelectorRememberField();

        void addFormat(QString const& text, QString const& format);
        void removeFormat(QString const& text);

        virtual QString value() const override;
        virtual void setValue(QString const& new_value) override;
}; // class FormatSelectorRememberField //
} // namespace qt //
} // namespace lib //

#endif // LIB_QT_FORMATSELECTORREMEMBERFIELD_H_FILE_GUARD
