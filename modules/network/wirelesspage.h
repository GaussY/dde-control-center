#ifndef WIRELESSPAGE_H
#define WIRELESSPAGE_H

#include "contentwidget.h"

#include <QMap>
#include <QTimer>
#include <QPointer>
#include <QJsonObject>

namespace dcc {

namespace widgets {
class SettingsGroup;
class SwitchWidget;
}

namespace network {

class ConnectHiddenPage;
class ConnectionEditPage;
class AccessPointWidget;
class NetworkModel;
class WirelessDevice;
class WirelessPage : public ContentWidget
{
    Q_OBJECT

public:
    explicit WirelessPage(WirelessDevice *dev, QWidget *parent = 0);
    ~WirelessPage();

    void setModel(NetworkModel *model);

public slots:
    void onAPAdded(const QJsonObject &apInfo);
    void onAPChanged(const QJsonObject &apInfo);
    void onAPRemoved(const QString &ssid);

signals:
    void requestEditAP(const QString &devPath, const QString &apPath) const;
    void requestConnectAp(const QString &devPath, const QString &apPath, const QString ssid) const;
    void requestCreateConnection(const QString &type, const QString &devPath) const;
    void requestNextPage(ContentWidget * const w) const;
    void requestDeviceAPList(const QString &devPath) const;
    void requestDeviceEnabled(const QString &devPath, const bool enabled) const;

private slots:
    void onDeviceRemoved();
    void sortAPList();
    void onActiveConnInfoChanged(const QList<QJsonObject> &activeConns);
    void showConnectHidePage();
    void showAPEditPage(const QString &session);

private:
    void updateActiveAp();

private:
    WirelessDevice *m_device;
    NetworkModel *m_model;

    dcc::widgets::SettingsGroup *m_listGroup;
    AccessPointWidget *m_connectHideSSID;

    QPointer<ConnectionEditPage> m_apEditPage;

    QString m_activeApName;
    QTimer m_sortDelayTimer;
    QMap<QString, AccessPointWidget *> m_apItems;
};

}   // namespace dcc

}   // namespace network

#endif // WIRELESSPAGE_H
