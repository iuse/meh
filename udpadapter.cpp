#include "udpadapter.h"

UDPAdapter::UDPAdapter(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 20320);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void UDPAdapter::readyRead() {
    QByteArray Buffer;
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

    // Opengazer sends a string formatted as 'x X_VALUE\ny Y_VALUE'
    // where X_VALUE and Y_VALUE are not padded.
    // Therefore we cut the string where space and newline characters are
    // and the x and y coordinates are respectively at the index 1 and 3
    // of coordList
    QRegExp rx("(\\ |\\n)");
    QStringList coordList = QString(Buffer.data()).split(rx);

//    qDebug() << "Message from: " << sender.toString();
//    qDebug() << "Message port: " << senderPort;
//    qDebug() << "coord x:" << coordList[1];
//    qDebug() << "coord y:" << coordList[3];

    emit coordChanged(coordList[1].toInt(), coordList[3].toInt());
}
