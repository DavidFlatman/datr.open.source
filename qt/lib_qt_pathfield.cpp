//------------------------------------------------------------------------------
///@file lib_qt_pathfield.cpp                                                   
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
///
///@version 2020-08-25  PN     Update documentation to DATR standards.
///@version 2020-08-20  PN     Remove QLineEdit from Data struct. Added
///                            lineEditRememberField to Data struct.
///@version 2020-08-19  PN     Remove unecessary user logic. Changed class to
///                            correct implementation. Added onPathChange().
///@version 2020-08-18  PN     Removed openDialogHelper(), and close(). Changed
///                            isAbsolutePath() to isRelativePath() and
///                            isValidPath() to verifyCriteria().
///@version 2020-08-14  PN     Update Data struct. Setup constructor to connect
///                            signals and slots. Add logic to openDialog(),
///                            openDialogHelper(), updatePaths(),
///                            isRelativePath(), isValidPath(), and m_Data
///                            setter functions.
///@version 2020-08-13  PN     File creation
//------------------------------------------------------------------------------

#include "lib_qt_pathfield.h"
#include "lib_qt_pathrememberfield.h"

#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///@brief Field data for the data struct.
//------------------------------------------------------------------------------
struct PathField::Data
{
    QHBoxLayout*                m_Layout;
    PathRememberField*          m_Path;
    QPushButton*                m_ThreeDots;
    QString                     m_Title;
    QString                     m_Directory;
    QString                     m_Type;
    bool                        m_MustExist;
    bool                        m_WarnIfExists;
    bool                        m_ValidFilePath;

    Data(QString const& field_name)
        : m_Layout(new QHBoxLayout)
        , m_Path(new PathRememberField(field_name, nullptr))
        , m_ThreeDots(new QPushButton("..."))
        , m_Title(field_name)
        , m_MustExist(false)
        , m_WarnIfExists(false)
        , m_ValidFilePath(false)
    {
        m_Layout->addWidget(m_Path);
        m_Layout->addWidget(m_ThreeDots);

//------------------------------------------------------------------------------
// Load previously opened directory for QFileDialog to open from.
//------------------------------------------------------------------------------
        m_Directory = QFileInfo(m_Path->text()).dir().path();
    }
}; // struct PathField::Data //

//------------------------------------------------------------------------------
///@brief Set layout, signals, and slots for the PathField class.
//------------------------------------------------------------------------------
PathField::PathField(
     QString const& field_name
   , QWidget* parent
)  : QWidget(parent)
   , Field(field_name)
   , m_Data(new Data(field_name))
{
    setLayout(m_Data->m_Layout);
    connect(
          m_Data->m_ThreeDots, SIGNAL(clicked())
        , this, SLOT(openDialog())
    );
    connect(
          m_Data->m_Path, SIGNAL(textChanged(QString const&))
        , this, SLOT(onPathChange())
    );
} // PathField::PathField //

//------------------------------------------------------------------------------
///@brief Reclaim resources held by object.
//------------------------------------------------------------------------------
PathField::~PathField()
{
} // PathField::~PathField //


//------------------------------------------------------------------------------
///@brief Open file explorer that will be used to populate the text field.
///@pre QPushButton must be initalized and connected to this slot.
//------------------------------------------------------------------------------
void PathField::openDialog()
{
    QFileDialog fileDialog;
    QString filePath;
    filePath = fileDialog.getOpenFileName(
                  this
                , m_Data->m_Title
                , m_Data->m_Directory
                , m_Data->m_Type
               );
    if(!filePath.isEmpty() && !filePath.isNull())
    {
        m_Data->m_Directory = QFileInfo(filePath).dir().path();
        m_Data->m_Path->setText(filePath);
    } else {
        qDebug() << "Warning: openDialog() file not selected";
    }
} // void PathField::openDialog() //

//------------------------------------------------------------------------------
///@brief Update m_Path to contain abosulute path and call to verifyCriteria().
///@pre QLineEdit must be initialized and connected to this slot.
//------------------------------------------------------------------------------
void PathField::onPathChange()
{
    if(!m_Data->m_Path->text().isEmpty() && !m_Data->m_Path->text().isNull())
    {
        if(isRelativePath())
        {
            m_Data->m_Path->setValue(
                        QFileInfo(m_Data->m_Path->text())
                        .absoluteFilePath()
                        );
        }
        verifyCriteria();
    }
} // void PathField::onPathChange() //
//------------------------------------------------------------------------------
///@brief Checks whether the m_Paths is the relative file path.
//------------------------------------------------------------------------------
bool PathField::isRelativePath() const
{
    return QFileInfo(m_Data->m_Path->text()).isRelative();
} // PathField::isRelativePath() //

//------------------------------------------------------------------------------
///@brief Checks whether m_Path is valid given field criteria; mustExist.
//------------------------------------------------------------------------------
bool PathField::verifyCriteria() const
{
    if(m_Data->m_Path->text() == "")
    {
        qDebug() << "Warning: verifyCriteria() file not selected!";
        return false;
    }
    if(m_Data->m_MustExist)
    {
        if(!QFile(m_Data->m_Path->text()).exists())
        {
            qDebug() << "Warning: verifyCriteria() file does not exist!"
                     << m_Data->m_Path->text();
            return false;
        }
    } else if(m_Data->m_WarnIfExists)
    {
        if(QFile(m_Data->m_Path->text()).exists())
        {
            qDebug() << "Warning: verifyCriteria() file exists!"
                     << m_Data->m_Path->text();
        }
    }
    m_Data->m_ValidFilePath = true;
    return true;
} // PathField::isValidPath() //

//------------------------------------------------------------------------------
///@brief Set m_Type in m_Data.
///@param new_type      QString containing type of file.
//------------------------------------------------------------------------------
void PathField::setType(QString const& new_type)
{
    m_Data->m_Type = new_type;
} // PathField::setType() //

//------------------------------------------------------------------------------
///@brief Set m_Title of the pathfield.
///@param new_title    QString containing title of field.
//------------------------------------------------------------------------------
void PathField::setTitle(QString const& new_title)
{
    m_Data->m_Title = new_title;
} // PathField::setTitle() //

//------------------------------------------------------------------------------
///@brief Set m_MustExist criteria for pathfield.
///@param state    If true, set m_MustExist criteria to true else set to false.
//------------------------------------------------------------------------------
void PathField::setMustExist(bool state)
{
    m_Data->m_MustExist = state;
} // PathField::setMustExist() //

//------------------------------------------------------------------------------
///@brief Set m_WarnIfExist criteria for pathfield.
///@param state    If true, set m_WarnIfExist criteria to true else set to
///                false.
//------------------------------------------------------------------------------
void PathField::setWarnIfExists(bool state)
{
    m_Data->m_WarnIfExists = state;
} // PathField::setWarnIfExists() //

//------------------------------------------------------------------------------
///@brief Returns file path.
//------------------------------------------------------------------------------
QString PathField::value() const
{
    return m_Data->m_Path->value();
} // PathField::value() //

//------------------------------------------------------------------------------
///@brief Set value (file path).
///@param new_value    QString containing path of file.
//------------------------------------------------------------------------------
void PathField::setValue(const QString &new_value)
{
    m_Data->m_Path->setValue(new_value);
} // PathField::setValue() //

//------------------------------------------------------------------------------
///@brief Get name of m_Path.
//------------------------------------------------------------------------------
QString PathField::name() const
{
    return m_Data->m_Path->name();
} // PathField::name() //

//------------------------------------------------------------------------------
///@brief Set name for m_Path.
///@param new_value    QString containing name of field.
//------------------------------------------------------------------------------
void PathField::setName(const QString &new_name)
{
    m_Data->m_Path->setName(new_name);
} // PathField::setName() //
} // namespace qt //
} // namespace lib //


