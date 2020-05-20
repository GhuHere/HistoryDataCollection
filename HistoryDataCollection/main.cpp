#include "ServerSocketDao.h"
#include <QtWidgets/QApplication>
#include <qdebug.h>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	ServerSocketDao* pServerSocket = ServerSocketDao::GetInstance();
	return a.exec();
}
