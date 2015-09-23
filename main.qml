import QtQuick 2.3
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import Vk 1.0

ApplicationWindow {
    id: mainWindow
    title: qsTr("Prakt_Photo")
    visible: true
    width: 400
    height: 400

    statusBar: StatusBar {
        RowLayout {Label { id: status }}
    }

    VkSession {
        id: vk

        onAuthSuccess: {
            mainView.state = "normal"
            load_profile()
            load_photos()
        }

        onAuthLogout: {
            viewModel.restartApp()
        }
    }

    Component.onCompleted: {
        vk.login()
    }

    QtObject {
        id: profile
        property string userId
        property string userName
        property string status
        property url avatar
    }

    function load_profile() {
        var req = vk.request("users.get")
            .arg("fields", "status, photo_50")
            .exec()

        req.done.connect(function(response) {
            response = response[0];

            profile.userId = response.id;
            profile.avatar = response.photo_50;
            profile.status = response.status;

            profile.userName = "%1 %2"
                .arg(response.first_name)
                .arg(response.last_name)
        })
    }

    function load_photos() {
        docsView.state = "wait"

        var req = vk.request("photos.get")
            .arg("album_id","profile")
            .arg("rev", "1")
            .exec()

        req.done.connect(function(response) {
            docsView.state = "normal"

            docsModel.clear()
            for (var i = 0; i < response.count; ++i){
                docsModel.append(response.items[i])
            }
        })

        req.error.connect(function(error) {
            console.log("photos load error");
        })
    }

    ColumnLayout {
        id: mainView
        anchors.fill: parent
        anchors.margins: 5
        state: "wait"

        states: [
            State {
                name: "normal"
                PropertyChanges { target: mainView; opacity: 1}
            },
            State {
                name: "wait"
                PropertyChanges { target: mainView; opacity: 0}
            }
        ]

        RowLayout {
            id: header
            anchors.margins: 5
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Image {
                source: profile.avatar
                width: 50
                height: 50
            }

            ColumnLayout {
                Label {
                    text: profile.userName
                    font.pointSize: 14;
                    font.family: "Segoe UI"
                }
                Label {
                    text: profile.status;
                    font.pointSize: 7;
                    font.family: "Segoe UI";
                    font.italic: true
                }
           }

            Button {
                anchors.right: parent.right
                text: qsTr("Logout")
                onClicked: {
                    vk.logout()
                }
           }
        }

        Rectangle {
            color: "transparent"
            border { width: 1; color: "black" }
            anchors.top : header.bottom
            anchors.bottom: controls.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5

           ScrollView {
               anchors.fill : parent

               ListView {
                   id: docsView
                   model: ListModel { id: docsModel }
                   focus: true
                   highlight: Rectangle { color: "lightsteelblue"; radius: 2 }
                   state: "normal"

                   states: [
                       State {
                           name: "normal"
                            PropertyChanges { target: docsView; opacity: 1}
                            PropertyChanges { target: docsView; enabled: true}
                       },
                       State {
                           name: "wait"
                           PropertyChanges { target: docsView; opacity: 0.4}
                           PropertyChanges { target: docsView; enabled: false}
                       }

                   ]

                   delegate: Item {

                       property var view: ListView.view
                       property var isCurrent: ListView.isCurrentItem

                       width: view.width
                       height: 75

                       Rectangle {
                           color: "transparent"
                           anchors.fill: parent

                           RowLayout{
                               anchors.fill: parent

                               Image {
                                 anchors.centerIn: parent
                                   anchors.margins: 5
                                   source:model.photo_75
                                   width: 75
                                   height: 75

                                   MouseArea {
                                       anchors.fill: parent
                                       cursorShape: Qt.PointingHandCursor
                                       onClicked: {
                                           view.currentIndex = model.index
                                           viewModel.downloadDoc(model.photo_604)
                                       }
                                   }

                               }
                               MouseArea {
                                   anchors.fill: parent
                                   onClicked: {
                                       view.currentIndex = model.index
                                   }
                               }

                           }
                       }
                   }
               }
           }
        }

        RowLayout {
           id: controls
           state: "normal"
           anchors.horizontalCenter: parent.horizontalCenter
           anchors.bottom: parent.bottom

           states: [
               State {
                   name: "normal"
                   PropertyChanges {target: controls; enabled: true}
               },
               State {
                   name: "wait"
                   when: docsView.state == "wait"
                   PropertyChanges {target: controls; enabled: false}
              }
           ]

           Button {
               text: qsTr("Update List")
               onClicked: load_photos()
          }
       }

    }
}
