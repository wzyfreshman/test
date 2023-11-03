import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "SerialPort"
    Depends { name: "Qt"; submodules: ["core"]}

    architectures: ["arm_64"]
    targetPlatform: "linux"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt5SerialPort"
    libNameForLinkerRelease: "Qt5SerialPort"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5SerialPort.so.5.11.3"
    cpp.defines: ["QT_SERIALPORT_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtSerialPort"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["serialport"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
