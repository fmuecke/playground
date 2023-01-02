#ifndef PESFXCREATOR_H
#define PESFXCREATOR_H

#include <boost/utility.hpp>

#include "QString"
#include "QStringList"
#include "QTemporaryFile"

#include "QMessagebox"

class PeSfxCreator : boost::noncopyable
{
public:
    PeSfxCreator();
	virtual ~PeSfxCreator(){}

	void SetBaseDir( const QString& pathStr );

	const QString& GetBaseDir() const
	{ return m_BaseDir; }

	const QStringList& GetFiles() const
	{ return m_Files; }

	void RemoveFile( const QString& name )
	{
		m_Files.removeOne( name );
	}
	void SetExeFile( const QString& fileName )
	{
		m_ExeFile = fileName;
	}
	void SetOutputFile( const QString& fileName )
	{
		m_OutputFileName = fileName;
	}
	void Create( QObject* parent ) const;

private:
	// uses m_BaseDir and m_Files
	void GetDirContent_( const QString& dir, QStringList& files ) const;

	void WriteDDF_( QTemporaryFile* ddf ) const;

	//
	// private members
	//
	QString m_BaseDir;
	QStringList m_Files;
	QString m_ExeFile;
	QString m_OutputFileName;

};

#endif // PESFXCREATOR_H
