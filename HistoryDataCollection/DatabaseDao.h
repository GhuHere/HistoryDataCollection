#pragma once

#include "DeviceModel.h"
#include <QObject>
#include <QtSql>
#include <QSharedPointer>
#include <QList>

class DatabaseDao : public QObject
{
	Q_OBJECT

public:
	~DatabaseDao();
	static DatabaseDao* GetInstance();
	QString GetDatabaseName();
	bool OpenDatabase(QString name);
	void CloseDatabase(QString name);
	void InsertDevice(QList<QSharedPointer<DeviceModel>>& devices, QString name);
	QList<QSharedPointer<DeviceModel>> QueryAllDevices(QString name);

private:
	DatabaseDao(QObject* parent = nullptr);
	QStringList m_databaseNames;
};

