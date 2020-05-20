#pragma once

#include <QObject>
#include <QTcpServer>

class ServerSocketDao : public QObject
{
	Q_OBJECT

public:
	~ServerSocketDao();
	static ServerSocketDao* GetInstance();


private:
	ServerSocketDao(QObject* parent = nullptr);

	QTcpServer m_server;

signals:

public slots:
	void NewConnectionSlot();
	void AcceptErrorSlot(QAbstractSocket::SocketError socketError);
};

class Client : public QObject
{
	Q_OBJECT

public:
	Client(QObject* parent = nullptr);
	~Client();

private:
	QString m_name;

public slots:
	void ReadDataSlot();
	void StateChangedSLot(QAbstractSocket::SocketState socketState);
	void ErrorSlot(QAbstractSocket::SocketError socketError);
};
