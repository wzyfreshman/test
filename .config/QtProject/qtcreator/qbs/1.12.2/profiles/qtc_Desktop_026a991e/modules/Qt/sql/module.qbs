import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "Sql"
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
    libNameForLinkerDebug: "Qt5Sql"
    libNameForLinkerRelease: "Qt5Sql"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5Sql.so.5.11.3"
    cpp.defines: ["QT_SQL_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtSql"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["sql"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
