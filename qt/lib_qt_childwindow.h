//------------------------------------------------------------------------------
///@file lib_qt_childwindow.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------

#ifndef LIB_QT_CHILDWINDOW_H_FILE_GUARD
#define LIB_QT_CHILDWINDOW_H_FILE_GUARD

#include <QTextEdit>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///
///@par Class: ChildWindow
///
///@brief   MDI child window.
///
///@par Thread Safety:  none
///
///@par Class Knowledge
///         -   This class knows about:
///         -   This class hides:
///         -   This class is agnostic about:
///
///@par Expected Usage:
///     @code
///     @endcode
///
///@version 2020-10-07  PN     File creation
//------------------------------------------------------------------------------

class ChildWindow
    : public QTextEdit
{

    public:
         ChildWindow();
        ~ChildWindow();

    private:

};
} // namespace qt //
} // namespace lib //

#endif // LIB_QT_CHILDWINDOW_H_FILE_GUARD
