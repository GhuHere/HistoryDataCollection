#include "ServerSocketDao.h"
#include "DatabaseDao.h"
#include "DeviceModel.h"
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedPointer>

ServerSocketDao* g_pThis = nullptr;
DatabaseDao* g_pDb = DatabaseDao::GetInstance();

ServerSocketDao::ServerSocketDao(QObject* parent)
	: QObject(parent)
{
	QThread* pThread = new QThread();
	this->moveToThread(pThread);
	connect(&m_server, &QTcpServer::newConnection, this, &ServerSocketDao::NewConnectionSlot);
	connect(&m_server, &QTcpServer::acceptError, this, &ServerSocketDao::AcceptErrorSlot);
	connect(this, &ServerSocketDao::destroyed, pThread, &QThread::deleteLater);
	pThread->start();
	m_server.listen(QHostAddress::Any, 1434);
}

ServerSocketDao::~ServerSocketDao()
{

}

ServerSocketDao* ServerSocketDao::GetInstance()
{
	if (g_pThis == nullptr) g_pThis = new ServerSocketDao();
	return g_pThis;
}

void ServerSocketDao::NewConnectionSlot()
{
	while (m_server.hasPendingConnections())
	{
		QTcpSocket* pSocket = m_server.nextPendingConnection();
		Client* pClient = new Client();
		QThread* pThread = new QThread();
		pClient->moveToThread(pThread);
		connect(pSocket, &QTcpSocket::readyRead, pClient, &Client::ReadDataSlot);
		connect(pSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), pClient, &Client::ErrorSlot);
		connect(pSocket, &QTcpSocket::stateChanged, pClient, &Client::StateChangedSLot);
		connect(this, &ServerSocketDao::destroyed, pClient, &Client::deleteLater);
		connect(pClient, &QObject::destroyed, pThread, &QThread::deleteLater);
		pThread->start();
		qDebug() << "ServerSocket: New connection!" << endl;
	}
}

void ServerSocketDao::AcceptErrorSlot(QAbstractSocket::SocketError socketError)
{
	qDebug() << "NetSocketDao-AcceptErrorSlot: " << socketError << endl;
}

Client::Client(QObject* parent) : QObject(parent)
{
	m_name = QString("");
}

Client::~Client()
{
	g_pDb->CloseDatabase(m_name);
}

void Client::StateChangedSLot(QAbstractSocket::SocketState socketState)
{
}

void Client::ReadDataSlot()
{
	qDebug() << "ServerSocket: ReadDataSlot" << endl;
	if (m_name == QString("")) m_name = g_pDb->GetDatabaseName();
	QTcpSocket* pSocket = qobject_cast<QTcpSocket*> (sender());
	QByteArray data = pSocket->readAll();
	QJsonParseError* pError = new QJsonParseError();
	QJsonDocument jsonDoc = QJsonDocument::fromJson(data, pError);
	if (jsonDoc.isNull()) qDebug() << "NetSocketDao-ReadDataSlot: " << pError->errorString() << endl;
	else
	{
		QJsonObject jsonObject = jsonDoc.object();
		int order = jsonObject.value("order").toInt();
		if (order == 1)
		{
			QList<QSharedPointer<DeviceModel>> devices = g_pDb->QueryAllDevices(m_name);
			QJsonDocument devJsonDoc = DeviceModel::DeviceToJson(devices);
			pSocket->write(devJsonDoc.toJson());
		}
		else if (order == 2)
		{
			QList<QSharedPointer<DeviceModel>> devices = DeviceModel::JsonToDevice(jsonDoc);
			g_pDb->InsertDevice(devices, m_name);
		}
	}
}

void Client::ErrorSlot(QAbstractSocket::SocketError socketError)
{
	qDebug() << "NetSocketDao-ErrorSlot: " << socketError << endl;
}
