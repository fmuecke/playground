#include "pesfxcreator.h"
#include "QDir"
#include "QFile"
#include "QTemporaryFile"
#include "QProcess"

PeSfxCreator::PeSfxCreator()
	: m_BaseDir()
	, m_Files()
	, m_ExeFile()
	, m_OutputFileName()
{
}

//------------------
// SetBaseDirectory
//------------------
void PeSfxCreator::SetBaseDir( const QString& pathStr )
{
	m_Files.clear();
	m_BaseDir = pathStr;
	GetDirContent_( m_BaseDir, m_Files );
	for( int i=0; i< m_Files.size(); ++i )
	{
		m_Files[i] = m_Files[i].mid( m_BaseDir.length() + 1 );
	}
	//files.sort();
}

//--------
// Create
//--------
void PeSfxCreator::Create( QObject* parent ) const
{
	// create DDF file for makecab.exe
	QTemporaryFile ddf_file( QDir::tempPath() + "\\pfc" );
	ddf_file.open();
	WriteDDF_( &ddf_file );

	// call makecab.exe
	QProcess makecab_proc( parent );
	makecab_proc.setWorkingDirectory( m_BaseDir );
	QString program = "makecab.exe /F \"";
	program.append( ddf_file.fileName() + "\"" );
	makecab_proc.start( program );
	//makecab_proc.waitForStarted(-1);
	makecab_proc.waitForFinished(-1);
	ddf_file.close();
}

//###################
// private functions
//###################
void PeSfxCreator::GetDirContent_( const QString& dir, QStringList& files ) const
{
	QDir currentDir( dir );

	QStringList dirs_;
	dirs_ = currentDir.entryList( QStringList("*"), QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Dirs );
	for( int i=0; i<dirs_.size(); ++i )
	{
		currentDir.cd( dirs_[i] );

		GetDirContent_( currentDir.absolutePath(), files );
		currentDir.cdUp();
	}

	QStringList files_;
	files_ = currentDir.entryList( QStringList("*"), QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Files );
	if( files_.isEmpty() )
	{
		// makecab.exe can not handle empty dirs so far
		//files.push_back( dir + "\\" );
	}
	else
	{
		for( int i=0; i<files_.size(); ++i )
			files.push_back( currentDir.absoluteFilePath( files_[i] ) );
	}
}

//------------
// CreateDDF_
//------------
void PeSfxCreator::WriteDDF_( QTemporaryFile* ddf ) const
{
	//if( ddf.exists() && !ddf.remove() )
	//	throw; //TODO: define exception

	QByteArray ddf_data = \
		".OPTION EXPLICIT\n"\
		".Set Cabinet=on\n"\
		".Set Compress=on\n"\
		".Set MaxDiskSize=CDROM\n"\
		".Set ReservePerCabinetSize=0\n"\
		".Set DiskDirectoryTemplate=		; no special output direcotroy\n"\
		".Set CompressionType=MSZIP		; LZX or MSZIP\n"\
		".Set CompressionLevel=7\n"\
		".Set CompressionMemory=21\n"\
		".Set CabinetNameTemplate=\"pesfxdata\"\n";
	for( int i=0; i<m_Files.size(); ++i )
	{
		ddf_data.append( "\"" );
		ddf_data.append( m_Files[i] );
		ddf_data.append( "\" \"" );
		ddf_data.append( m_Files[i] );
		ddf_data.append( "\"\n" );
	}
	ddf->write( ddf_data );
}

