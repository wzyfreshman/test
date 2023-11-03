import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "WebView"
    Depends { name: "Qt"; submodules: ["core", "gui"]}

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
    libNameForLinkerDebug: "Qt5WebView"
    libNameForLinkerRelease: "Qt5WebView"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5WebView.so.5.11.3"
    cpp.defines: ["QT_WEBVIEW_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtWebView"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["webview"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
