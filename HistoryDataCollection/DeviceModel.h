#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QList>
#include <QSharedPointer>

/*
Device JSON
{
	"tableName" : "device",
	"data" : [
		{
			"id" : "deviceId",
			"type" : "deviceType",
			"name" : "deviceName",
			"path" : "devicePath",
			"outFile" : "deviceOutFile"
		},
		...
	]
}
*/

class DeviceModel : public QObject
{
	Q_OBJECT

public:
	DeviceModel(QObject* parent = nullptr);
	~DeviceModel();
	void SetId(int id);
	int GetId();
	void SetType(int type);
	int GetType();
	void SetName(QString name);
	QString GetName();
	void SetPath(QString path);
	QString GetPath();
	void SetOutfile(QString outfile);
	QString GetOutfile();

	static QJsonDocument DeviceToJson(QList<QSharedPointer<DeviceModel>>& devices);
	static QList<QSharedPointer<DeviceModel>> JsonToDevice(QJsonDocument jsonDoc);

private:
	int m_id;
	int m_type;
	QString m_name;
	QString m_path;
	QString m_outfile;
};

