//------------------------------------------------------------------------------
///@file lib_qt_pathfield.h                                                     
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_QT_PATHFIELD_H_FILE_GUARD
#define LIB_QT_PATHFIELD_H_FILE_GUARD

#include "lib_qt_rememberfield.h"

#include <memory>
#include <QWidget>

namespace lib {
namespace qt {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: PathField                                                        
///                                                                             
///@brief   Prompt the user the for a file name and display [...] button for    
///         file dialog box.                                                    
///                                                                             
///@par Thread Safety:  none (Qt GUI)                                           
///                                                                             
///@par Class Knowledge                                                         
///         -   This class knows about:
///                 -   Field class
///         -   This class hides:
///                 -   Struct Data containing the componenets of
///                     PathField Class and containing class state data.
///         -   This class is agnostic about:
///                 -   Whether the file path is entered or searched
///                     from QFileDialog
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///         QFormLayout*    m_Layout(new QFormLayout);                          
///         PathField*      m_PathField(new PathField);                         
///         m_Layout->addRow("File name:", m_PathField);                        
///         setLayout(m_Layout);
///
///         struct MyDialogClass::Data
///         {
///                QFormLayout*                  m_Layout;
///                lib::qt::PathField*            m_InputPath;
///                lib::qt::PathField*            m_OutputPath;
///                lib::qt::PathField*            m_ConfigurationPath;
///                Data()
///                     : m_InputPath(new lib::qt::PathField("input"))
///                     , m_OutputPath(new lib::qt::PathField("output"))
///                     , m_ConfigutationPath(
///                         new lib::qt::PathField("configuration")
///                         )
///                 {
///                     m_InputPath->setType(
///                         tr("CH8 Files (*.dat); All Files (*.*)")
///                         );
///                     m_InputPath->setMustExist(true);
///                     m_InputPath->setTitle("Input CH8 File");
///                     m_OutputPath->setType(
///                         tr("Report (*.html); All Files (*.*)")
///                         );
///                     m_OutputPath->setWarnIfExists(true);
///                     m_OutputPath->setTitle("Report Output FIle");
///                     m_ConfigurtationFile->setType(
///                         tr("Configuration (*.xml); AllFiles(*.*)")
///                         );
///                     m_ConfigutationFIle->setMustExist(true);
///                     m_ConfigurtationFIle->setTitle("Configuration File");
///                     m_Layout->addRow("Input", m_InputPath);
///                     m_Layout->addRow("Output", m_OutputPath);
///                     m_Layout->addRow("Configuration", m_ConfigurationPath);
///                }
///         };
///     @endcode
///
///@version 2020-10-05  PN     Removed verifyCritera().
///@version 2020-08-25  PN     Update documentation to DATR standards.
///@version 2020-08-18  PN     Removed openDialogHelper(), and close(). Changed
///                            isAbsolutePath() to isRelativePath() and
///                            isValidPath() to verifyCriteria(). Changed parent
///                            class to Field.
///@version 2020-08-13  PN     Added setValue(), parameters for previous
///                            functions, openDialogHelper(), and close().
///                            Removed fileSelected().
///@version 2020-08-12  PN     Added constructor, destructor, isValidPath(),
///                            isAbsolutePath(), openDialog(), setter functions
///                            for fields in m_Data, and fileSelected().
///@version 2020-08-11  PN     File creation
///
///@TODO
/// PathField currently does not enforce mustExist. It is detected and a message
/// is written to std::cout. A QMessageBox (or TBD) needs to be done to properly
/// handle the error.
//------------------------------------------------------------------------------

class PathField 
        : public QWidget
        , public Field
{
    Q_OBJECT

    public:
        explicit PathField(
              QString const& field_name
            , QWidget* parent = nullptr
        );
        PathField(PathField&& that) = default;
        PathField(const PathField& that) = delete;
        PathField& operator=(const PathField& that) = delete;
        ~PathField();

        void setType(QString const& new_type);
        void setTitle(QString const& new_title);
        void setMustExist(bool state = true);
        void setWarnIfExists(bool state = true);

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
// SIGNAL(m_Data->m_Path->textChanged()) , SLOT(onPathChange())
//------------------------------------------------------------------------------
        void onPathChange();

    private:
        struct Data;
        std::unique_ptr<Data> m_Data;

}; // class PathField //
} // namespace qt //
} // namespace lib //


#endif // #ifndef LIB_QT_PATHFIELD_H_FILE_GUARD

