import QtQuick 2.15
import QtQuick.Window 2.15
import CalcTest.gui 1.0
import MainWindow.gui 1.0

MainWindow {
    id: mainWindow
    width: 900
    height: 650
    visible: true
    title: qsTr("Ship Path Calculations")

    Item {
        id: updater
        property double prevDateTime: new Date().getTime()
        property double diffMsTime: 0
        property double currTime: new Date().getTime()
        Timer {
            interval: 500;
            running: true;
            repeat: true;
            onTriggered: updater.updateTimerTriggered()
        }

        function updateTimerTriggered() {
            updater.currTime = new Date().getTime();
            updater.diffMsTime = updater.currTime - updater.prevDateTime;
            updater.prevDateTime = updater.currTime;

            calculationsTest.timerUpdate(updater.diffMsTime, updater.currTime)
        }

    }

    MouseArea {
        anchors.fill: parent
        onClicked: calculationsTest.clearCalculation()
    }

    Text {
        anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter; bottomMargin: 20 }
        text: "Click anywhere to clear the chart"
    }

    Grid {
        id: mainGrid
        width: parent.width
        height: parent.height
        rows: 2
        columns: 1

        CalculationsTest {
            id: calculationsTest
            width: parent.width;
            height: parent.height - 50

            Canvas {
                id: grid
                anchors.fill : parent
                property int wgrid: 100
                onPaint: {
                    wgrid = calculationsTest.gridCellSize
                    var ctx = getContext("2d")
                    ctx.lineWidth = 1
                    ctx.strokeStyle = Qt.rgba(0.3, 0.7, 1, 0.3)
                    ctx.beginPath()
                    var nrows = height/wgrid;
                    for(var i=0; i < nrows+1; i++) {
                        ctx.moveTo(0, wgrid*i);
                        ctx.lineTo(width, wgrid*i);
                    }

                    var ncols = width/wgrid
                    for(var j=0; j < ncols+1; j++) {
                        ctx.moveTo(wgrid*j, 0);
                        ctx.lineTo(wgrid*j, height);
                    }
                    ctx.closePath()
                    ctx.stroke()
                }
            }
            Canvas {
                id: shipPath
                anchors.fill : parent
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.lineWidth = 1
                    ctx.strokeStyle = "red"
                    ctx.beginPath()
                    for (var i = 0; i < calculationsTest.positions.length-1; i++) {
                        ctx.moveTo(calculationsTest.positions[i].x, calculationsTest.positions[i].y)
                        ctx.lineTo(calculationsTest.positions[i+1].x, calculationsTest.positions[i+1].y)
                    }
                    ctx.closePath()
                    ctx.stroke()
                }
            }
            Connections {
                target: mainWindow
                function onResizeEnded(width, height) {
                    calculationsTest.resizeEnded(width, height)
                }
            }

            onCalculationCleared: {
                update()
                console.log("The calculation has been cleared")
            }

            onPositionsChanged: {
                shipPath.requestPaint()
            }
        }
    }


}
