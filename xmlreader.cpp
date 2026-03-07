#include "xmlreader.h"
#include <qdebug.h>
#include <QRegularExpression>

XmlReader::XmlReader(){}

void XmlReader::readXml(QByteArray data, QStandardItemModel *model)
{
    qDebug()<<"In ReadXml function";
    model->clear();
    model->setHorizontalHeaderLabels({
        "Pallet", "Container","Code", "Height", "Length/Diameter", "Breadth", "Weight"
    });

    QXmlStreamReader xml(data);

    if (!xml.readNextStartElement() || xml.name() != "Pallets") {
        qWarning() << "Expected <Pallets> root element, got" << xml.name().toString();
        return;
    }

    while (xml.readNextStartElement()) {
        if (xml.name() == "Pallet") {
            QString palletNumber = xml.attributes().value("Number").toString();

            while (xml.readNextStartElement()) {
                QString containerType = xml.name().toString(); // "Box" or "Cylinder"

                if (containerType == "Box" || containerType == "Cylinder") {
                    QString code;
                    int weight = 0, length = 0, breadth = 0, height = 0, diameter = 0;

                    // parse children of container
                    while (xml.readNextStartElement()) {
                        if (xml.name() == "code") {
                            code = xml.readElementText();
                            code = checkCode(code);
                        } else if (xml.name() == "weight") {
                            weight = xml.readElementText().toInt();
                        } else if (xml.name() == "length") {
                            length = xml.readElementText().toInt();
                        } else if (xml.name() == "breadth") {
                            breadth = xml.readElementText().toInt();
                        } else if (xml.name() == "height") {
                            height = xml.readElementText().toInt();
                        } else if (xml.name() == "diameter") {
                            diameter = xml.readElementText().toInt();
                        } else {
                            xml.skipCurrentElement();
                        }
                    }

                    // build row
                    QList<QStandardItem*> row;
                    row << new QStandardItem(palletNumber);
                    row << new QStandardItem(containerType);
                    row << new QStandardItem(code);
                    row << new QStandardItem(QString::number(height));

                    if (containerType == "Box")
                        row << new QStandardItem(QString::number(length));
                    else // Cylinder
                        row << new QStandardItem(QString::number(diameter));

                    row << new QStandardItem(QString::number(breadth));
                    row << new QStandardItem(QString::number(weight));

                    model->appendRow(row);
                }
                else {
                    xml.skipCurrentElement();
                }
            }
        }
        else {
            xml.skipCurrentElement();
        }
    }

    if (xml.hasError()) {
        qWarning() << "XML parse error:" << xml.errorString();
    }
}

QString XmlReader::checkCode(QString code)
{
    QRegularExpression re("^\\d{4}/\\d{1,2}/[BC]\\d+$");
    QRegularExpressionMatch match = re.match(code);

    if (match.hasMatch())
        return code;
    else
        return "*******";
}
