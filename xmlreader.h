#ifndef XMLREADER_H
#define XMLREADER_H

#include <QXmlStreamReader>
#include <QStandardItemModel>

class XmlReader
{
public:
    XmlReader();
    void readXml(QByteArray data, QStandardItemModel *model);
    QString checkCode(QString code);
};

#endif // XMLREADER_H
