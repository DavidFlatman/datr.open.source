//------------------------------------------------------------------------------
///@file lib_qt_pathrememberfield.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-09-30  PN     Update autoCompletePath() and updateFilesList().
///@version 2020-09-29  PN     Implement the constructors, destructor,
///                            autoCompletePath(), eventFilter(),
///                            and updateFilesList().
///@version 2020-09-28  PN     File creation
//------------------------------------------------------------------------------

#include "lib_qt_pathrememberfield.h"

#include <QDebug>
#include <QDir>
#include <QEvent>
#include <QKeyEvent>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///@brief Initialize the parent classes.
//------------------------------------------------------------------------------
PathRememberField::PathRememberField(
    QString const& field_name
  , QWidget* parent
) : LineEditRememberField(field_name, parent)
  , index(-1)
{
    updateFilesList();
}

//------------------------------------------------------------------------------
///@brief Initialize the parent classes.
//------------------------------------------------------------------------------
PathRememberField::PathRememberField(
    QString const& field_name
  , QString const& field_value
  , QWidget* parent
) : LineEditRememberField(field_name, field_value, parent)
  , index(-1)
{
    updateFilesList();
}

//------------------------------------------------------------------------------
///@brief Reclaim resources held by object.
//------------------------------------------------------------------------------
PathRememberField::~PathRememberField()
{
}

//------------------------------------------------------------------------------
///@brief Set PathRememberField to detect key press events of <UP-ARROW> and
///       <DOWN-ARROW>.
//------------------------------------------------------------------------------
void PathRememberField::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        autoCompletePath();
    } else {
        QLineEdit::keyPressEvent(event);
        updateFilesList();
    }
}

//------------------------------------------------------------------------------
///@brief Check the current path in the line edit and populate the filesList
///       with all the files and directories.
//------------------------------------------------------------------------------
void PathRememberField::updateFilesList()
{
    filesList.clear();
    QString path = value();
    if(!QDir(path).exists())
    {
        QString fileName = QDir(path).dirName();
        QString dirPath = QFileInfo(path).dir().path();
        filesList = QDir(dirPath).entryList(
                          QStringList(fileName + "*")
                        , QDir::NoDotAndDotDot|QDir::AllEntries
                        , QDir::DirsFirst
                    );
    } else {
        filesList = QDir(path).entryList(
                          QDir::NoDotAndDotDot|QDir::AllEntries
                        , QDir::DirsFirst
                    );
    }
}

//------------------------------------------------------------------------------
///@brief Iterate through filesList and update the QLineEdit to new path.
//------------------------------------------------------------------------------
void PathRememberField::autoCompletePath()
{
    QString dirPath = QFileInfo(value()).dir().path();
    if(!filesList.isEmpty())
    {
        if(++index >= filesList.length())
        {
           index = 0;
        }
        setValue(dirPath + "/" + filesList.value(index));
    }
}
} // namespace lib //
} // namespace qt //
