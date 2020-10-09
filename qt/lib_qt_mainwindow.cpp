//------------------------------------------------------------------------------
///@file lib_qt_mainwindow.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-10-07  PN     File creation.
//------------------------------------------------------------------------------

#include "lib_qt_mainwindow.h"

#include <QAction>
#include <QMdiArea>
#include <QMenu>

namespace lib {
namespace qt {

struct MainWindow::Data
{
    QMdiArea*       m_MdiArea;
    QMenu*          m_Menu;
    QAction*        m_NewAction;
    QAction*        m_SaveAction;
    QAction*        m_SaveAsAction;
    QAction*        m_CutAction;
    QAction*        m_CopyAction;
    QAction*        m_PasteAction;
    QAction*        m_CloseAction;
    QAction*        m_MenuSeparator;

    Data(QWidget* parent)
        : m_MdiArea(new QMdiArea())
        , m_Menu(new QMenu("Main Window", nullptr))
        , m_NewAction(new QAction("new", parent))
        , m_SaveAction(new QAction("save", parent))
        , m_SaveAsAction(new QAction("save as", parent))
        , m_CutAction(new QAction("cut", parent))
        , m_CopyAction(new QAction("copy", parent))
        , m_PasteAction(new QAction("paste", parent))
        , m_CloseAction(new QAction("close", parent))
        , m_MenuSeparator(new QAction(parent))
    {
        m_Menu->addMenu("File");
        m_MdiArea->setAcceptDrops(false);
    }
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_Data(new Data(this))
{
    setCentralWidget(m_Data->m_MdiArea);
    setAcceptDrops(true);

} // MainWindow::MainWindow //

MainWindow::~MainWindow()
{
    //Placeholder
}

void MainWindow::openFile(QString const& path)
{
    //Placeholder
}

void MainWindow::newWindow()
{
    //Placeholder
}

void MainWindow::openFile()
{
    //Placeholder
}
void MainWindow::save()
{
    //Placeholder
}

void MainWindow::saveAs()
{
    //Placeholder
}

void MainWindow::cut()
{
    //Placeholder
}

void MainWindow::copy()
{
    //Placeholder
}

void MainWindow::paste()
{
    //Placeholder
}

void MainWindow::about()
{
    //Placeholder
}

void MainWindow::aboutDatr()
{
    //Placeholder
}

void MainWindow::updateMenus()
{
    //Placeholder
}
} // namespace qt //
} // namespace lib //
