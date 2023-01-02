#include <QtGui/QApplication>
#include "dialog.h"
#include "pesfxcreator.h"

class CommandLine
{
public:
	CommandLine( int argc, char* argv[] )
		: m_argc(argc)
		, m_argv(argv)
	{}

	~CommandLine() {}

	int CheckArg( const QString& s )
	{
		for( int i=0; i < m_argc; ++i )
		{
			if( s.compare( m_argv[i] ) == 0 )
				return i;
		}
		return -1;
	}


private:
	int m_argc;
	char** m_argv;
};


int main(int argc, char *argv[])
{
	CommandLine cl( argc, argv );
	QApplication a(argc, argv);
	PeSfxCreator pc;

	//
	// parse command line
	//
	int arg = -1;
	if( cl.CheckArg( "/?" ) > 0 )
	{
		QMessageBox::information( NULL, "PeSfxCreator",
								  "PeSfxCreator\n"\
								  "(c) F.Muecke, 2009\n"\
								  "\n"\
								  "\nCommand line parameters:\n"\
								  "/? \tshows this help\n"\
								  "/basedir [dir]\tdirectory containing the files\n"\
								  "/exefile [file]\tfile that should be started after extraction\n"\
								  "/outfile [file]\toutput filename\n"\
								  "/intdir [dir]\tintermediade directory (default: %TMP%)\n"\
								  "/q \tquiet mode\n"
								  "");
		return 0;
	}
	arg = cl.CheckArg( "/basedir" );
	if( arg > 0 )
	{
		pc.SetBaseDir( QString(argv[arg+1]) );
	}
	arg = cl.CheckArg( "/exefile" );
	if( arg > 0 )
	{
		pc.SetExeFile( QString(argv[arg+1]) );
	}
	arg = cl.CheckArg( "/outfile" );
	if( arg > 0 )
	{
		pc.SetOutputFile( QString(argv[arg+1]) );
	}
	//TODO: "/intdir"

	Dialog w(&pc);
    w.show();
    return a.exec();
}
