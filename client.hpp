#pragma once

#include <QObject>
#include <QTimer>
#include <qqml.h>

class Client : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Client(QObject *parent = nullptr);

//private:
//    QTimer *timer;

public slots:
    void connect(const QString &server, bool on);

//    void disconnect();

//    void send(const std::vector<float> &channels);
//    void send(float );
    void send_channels(float a);

signals:

};

