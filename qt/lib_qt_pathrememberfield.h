//------------------------------------------------------------------------------
///@file lib_qt_pathrememberfield.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------

#ifndef LIB_QT_PATHREMEMBERFIELD_H_FILE_GUARD
#define LIB_QT_PATHREMEMBERFIELD_H_FILE_GUARD

#include "lib_qt_lineeditrememberfield.h"

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
///             -   LineEditRememberField class.
///         -   This class hides:
///             -   The autocomplete mechanism.
///         -   This class is agnostic about:
///             -   How the <DOWN-ARROW> and <UP-ARROW> key press is detected.
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
///@version 2020-09-30  PN     Moved updateFilesList() to private.
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

    protected:
        virtual void keyPressEvent(QKeyEvent *event) override;

    private:
        QStringList filesList;
        int index;
        void updateFilesList();
        void autoCompletePath();
}; // class PathRememberField //
} // namespace lib //
} // namespace qt //
#endif // LIB_QT_PATHREMEMBERFIELD_H_FILE_GUARD //
