//------------------------------------------------------------------------------
///@file lib_qt_mainwindow.h
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------

#ifndef LIB_QT_MAINWINDOW_H_FILE_GUARD
#define LIB_QT_MAINWINDOW_H_FILE_GUARD

#include <memory>
#include <QMainWindow>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///
///@par Class: MainWindow
///
///@brief   Main Window to display
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

class MainWindow
    : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget* parent);
        ~MainWindow();

        void openFile(QString const& path);

    private slots:
        void newWindow();
        void openFile();
        void save();
        void saveAs();
        void cut();
        void copy();
        void paste();
        void about();
        void aboutDatr();
        void updateMenus();

    private:
        struct Data;
        std::unique_ptr<Data> m_Data;

};
} // namespace qt //
} // namespace lib //

#endif // LIB_QT_MAINWINDOW_H_FILE_GUARD
