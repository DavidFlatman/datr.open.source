//------------------------------------------------------------------------------
///@file lib_qt_pathrememberfield.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------

#ifndef LIB_QT_PATHREMEMBERFIELD_H_FILE_GUARD
#define LIB_QT_PATHREMEMBERFIELD_H_FILE_GUARD

#include "lib_qt_lineeditrememberfield.h"

#include <QList>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///
///@par Class: PathRememberField
///
///@brief QLineEdit that can autocomplete a file path based on files listed in
///       current directory when a <DOWN-ARROW> or <UP-ARROW> key pressed.
///
///@par Thread Safety:  none (Qt GUI)
///
///@par Class Knowledge
///         -   This class knows about:
///             -   LineEditRememberField
///         -   This class hides:
///         -   This class is agnostic about:
///             -   The underlying handler for <DOWN-ARROW> and <UP-ARROW> key
///                 press.
///
///@par Expected Usage:
///     @code
///         struct MyField::Data
///         {
///              QHBoxLayout*                m_Layout;
///              PathRememberField*          m_Path;
///
///             MyField() {
///                 m_Layout->addWidget(m_Path);
///             }
///         }
///     @endcode
///
///@version 2020-09-29  PN     Added QList<QString> filesList() and
///                            updateFilesList(). Removed onPathChange().
///@version 2020-09-28  PN     File creation. Added constructors, destructor,
///                            autoCompletePath(), onPathChange(), and
///                            eventFilter().
//------------------------------------------------------------------------------
class PathRememberField
    : public LineEditRememberField
{
    public:
        explicit PathRememberField(
              QString const& field_name
            , QWidget* parent = nullptr
          );
        PathRememberField(
              QString const& field_name
            , QString const& field_value
            , QWidget* parent = nullptr
          );
        ~PathRememberField();

        void autoCompletePath();
        void updateFilesList();

    protected:
        bool eventFilter(QObject* obj, QEvent *event) override;

    private:
        QList<QString> filesList();
}; // class PathRememberField //
} // namespace lib //
} // namespace qt //
#endif // LIB_QT_PATHREMEMBERFIELD_H_FILE_GUARD //
