import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import Deepin.Widgets 1.0

Column{
    width: parent.width
    height: childrenRect.height

    property int connectionStatus: 100

    property var infos: vpnConnections[index]

    function goToEditConnection(){
        stackView.push({
            "item": stackViewPages["connectionPropertiesPage"],
            "properties": { "uuid": infos["Uuid"], "connectionPath": infos.Path, "devicePath": "/" },
            "destroyOnPop": true
        })
        stackView.currentItemId = "connectionPropertiesPage"
    }

    DBaseLine {
        id: lineBox

        property bool hovered: false
        property bool selected: false
        color: dconstants.contentBgColor

        MouseArea{
            z:-1
            anchors.fill:parent
            hoverEnabled: true
            onEntered: parent.hovered = true
            onExited: parent.hovered = false

            onClicked: {
                if (connectionStatus == 100){
                    goToEditConnection()
                }
                else{
                    activateThisConnection()
                }
            }
        }

        leftLoader.sourceComponent: Item {
            width: parent.width
            height: parent.height

            DImageButton {
                anchors.verticalCenter: parent.verticalCenter
                normal_image: "img/check_1.png"
                hover_image: "img/check_2.png"
                visible: connectionStatus == 100
                onClicked: {
                }
            }

            DLabel {
                anchors.left: parent.left
                anchors.leftMargin: 24
                anchors.verticalCenter: parent.verticalCenter
                text: infos["Id"]
                font.pixelSize: 12
                color: {
                    if(lineBox.selected){
                        return dconstants.activeColor
                    }
                    else if(lineBox.hovered){
                        return dconstants.hoverColor
                    }
                    else{
                        return dconstants.fgColor
                    }
                }
            }
        }

        rightLoader.sourceComponent: DArrowButton {
            onClicked: {
            }
        }
    }
}
