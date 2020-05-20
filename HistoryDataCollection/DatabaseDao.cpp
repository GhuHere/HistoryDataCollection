#include "DatabaseDao.h"
#include <QDebug>
#include <QMutexLocker>

DatabaseDao* g_pThis = nullptr;

DatabaseDao::DatabaseDao(QObject* parent)
	: QObject(parent)
{
	
}

DatabaseDao::~DatabaseDao()
{
	
}

DatabaseDao* DatabaseDao::GetInstance()
{
	if (g_pThis == nullptr) g_pThis = new DatabaseDao();
	return g_pThis;
}

QString DatabaseDao::GetDatabaseName()
{
	QString name = QString("db%1").arg(m_databaseNames.size());
	if (OpenDatabase(name)) return name;
	else return QString("");
}

bool DatabaseDao::OpenDatabase(QString name)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", name);
	db.setDatabaseName("history_data_collection");
	db.setPort(1433);
	db.setUserName("root");
	db.setPassword("123456");
	bool flg = db.open();
	if (flg == false) qDebug() << "DatabaseDao-Open Database fail" << endl;
	else m_databaseNames.append(name);
	return flg;
}

void DatabaseDao::CloseDatabase(QString name)
{
	if (QSqlDatabase::contains(name))
	{
		QSqlDatabase db = QSqlDatabase::database(name);
		db.close();
	}
	m_databaseNames.removeAll(name);
}

void DatabaseDao::InsertDevice(QList<QSharedPointer<DeviceModel>>& devices, QString name)
{
	QSqlDatabase db = QSqlDatabase::database(name);
	QSqlQuery query(db);
	query.prepare("insert into `device` (`Type`, `Name`, `Path`, `OutFile`) values (?, ?, ?, ?)");
	for (int i = 0; i < devices.size(); ++i)
	{
		query.addBindValue(devices[i]->GetType());
		query.addBindValue(devices[i]->GetName());
		query.addBindValue(devices[i]->GetPath());
		query.addBindValue(devices[i]->GetOutfile());
		if (query.exec() == false)
		{
			qDebug() << query.lastError().text() << endl;
		}
	}
}

QList<QSharedPointer<DeviceModel>> DatabaseDao::QueryAllDevices(QString name)
{
	QList<QSharedPointer<DeviceModel>> result;
	QSqlDatabase db = QSqlDatabase::database(name);
	QSqlQuery query(db);
	if (query.exec("select * from device") == false)
	{
		qDebug() << "DatabaseDao-QueryAllDevices: " << query.lastError().text() << endl;
	}
	while (query.next())
	{
		QSharedPointer<DeviceModel> deviceModel(new DeviceModel());
		deviceModel->SetId(query.value(0).toInt());
		deviceModel->SetType(query.value(1).toInt());
		deviceModel->SetName(query.value(2).toString());
		deviceModel->SetPath(query.value(3).toString());
		deviceModel->SetOutfile(query.value(4).toString());
		result.append(deviceModel);
	}
	return result;
}



