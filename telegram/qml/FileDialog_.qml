import QtCore
import QtQuick 2.15
import QtQuick.Dialogs

FileDialog {
    id:file
    title: "选择文件"
    currentFolder:StandardPaths.writableLocation(StandardPaths.DownloadLocation)
    fileMode: FileDialog.OpenFiles
    nameFilters: ["所有文件 (*.*)"]
    signal done
    onAccepted: {
        backend.send_file(selectedFile)
        visible = false
        close()
    }
    onRejected: {
        console.log("Canceled");
    }
    Component.onCompleted: {
        visible = false
        close()
    }
}
