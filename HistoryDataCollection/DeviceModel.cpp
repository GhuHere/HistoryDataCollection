#include "DeviceModel.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

DeviceModel::DeviceModel(QObject* parent)
	: QObject(parent)
{
}

DeviceModel::~DeviceModel()
{
}

void DeviceModel::SetId(int id)
{
	m_id = id;
}

int DeviceModel::GetId()
{
	return m_id;
}

void DeviceModel::SetType(int type)
{
	m_type = type;
}

int DeviceModel::GetType()
{
	return m_type;
}

void DeviceModel::SetName(QString name)
{
	m_name = name;
}

QString DeviceModel::GetName()
{
	return m_name;
}

void DeviceModel::SetPath(QString path)
{
	m_path = path;
}

QString DeviceModel::GetPath()
{
	return m_path;
}

void DeviceModel::SetOutfile(QString outfile)
{
	m_outfile = outfile;
}

QString DeviceModel::GetOutfile()
{
	return m_outfile;
}

QJsonDocument DeviceModel::DeviceToJson(QList<QSharedPointer<DeviceModel>>& devices)
{
	QJsonObject jsonObject;
	QJsonArray jsonArray;
	jsonObject.insert("tableName", QJsonValue("device"));
	for (int i = 0; i < devices.size(); ++i)
	{
		QJsonObject jsonDev;
		jsonDev.insert("id", QJsonValue(devices[i]->GetId()));
		jsonDev.insert("type", QJsonValue(devices[i]->GetType()));
		jsonDev.insert("name", QJsonValue(devices[i]->GetName()));
		jsonDev.insert("path", QJsonValue(devices[i]->GetPath()));
		jsonDev.insert("outFile", QJsonValue(devices[i]->GetOutfile()));
		jsonArray.append(jsonDev);
	}
	jsonObject.insert("data", jsonArray);
	return QJsonDocument(jsonObject);
}

QList<QSharedPointer<DeviceModel>> DeviceModel::JsonToDevice(QJsonDocument jsonDoc)
{
	QList<QSharedPointer<DeviceModel>> devices;
	QJsonObject jsonObject = jsonDoc.object();
	QJsonArray jsonArray = jsonObject.value("data").toArray();
	for (int i = 0; i < jsonArray.count(); ++i)
	{
		QJsonObject jsonDev = jsonArray.at(i).toObject();
		QSharedPointer<DeviceModel> dev(new DeviceModel());
		dev->SetId(jsonDev.value("id").toInt());
		dev->SetType(jsonDev.value("type").toInt());
		dev->SetName(jsonDev.value("name").toString());
		dev->SetPath(jsonDev.value("path").toString());
		dev->SetOutfile(jsonDev.value("outFile").toString());
		devices.append(dev);
	}
	return devices;
}
