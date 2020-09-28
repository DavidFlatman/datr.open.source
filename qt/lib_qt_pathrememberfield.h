//------------------------------------------------------------------------------
///@file lib_qt_pathrememberfield.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------

#ifndef LIB_QT_PATHREMEMBERFIELD_H_FILE_GUARD
#define LIB_QT_PATHREMEMBERFIELD_H_FILE_GUARD

#include "lib_qt_lineeditrememberfield.h"

namespace lib {
namespace qt {

class PathRememberField
    : public LineEditRememberField
{
    public:
        explicit PathRememberField(
              QString const& fieldName
            , QWidget* parent = nullptr
          );
        PathRememberField(
              QString const& fieldName
            , QString const& fieldValue
            , QWidget* parent = nullptr
          );
        ~PathRememberField();
    protected:
        bool eventFilter(QObject* obj, QEvent *event) override;
}; // class PathRememberField //
} // namespace lib //
} // namespace qt //
#endif // LIB_QT_PATHREMEMBERFIELD_H_FILE_GUARD //
