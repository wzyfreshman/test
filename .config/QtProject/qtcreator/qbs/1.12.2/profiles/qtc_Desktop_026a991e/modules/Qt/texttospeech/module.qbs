import qbs 1.0
import '../QtModule.qbs' as QtModule

QtModule {
    qtModuleName: "TextToSpeech"
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
    libNameForLinkerDebug: "Qt5TextToSpeech"
    libNameForLinkerRelease: "Qt5TextToSpeech"
    libFilePathDebug: ""
    libFilePathRelease: "/usr/lib/aarch64-linux-gnu/libQt5TextToSpeech.so.5.11.3"
    cpp.defines: ["QT_TEXTTOSPEECH_LIB"]
    cpp.includePaths: ["/usr/include/aarch64-linux-gnu/qt5", "/usr/include/aarch64-linux-gnu/qt5/QtTextToSpeech"]
    cpp.libraryPaths: []
    Group {
        files: [Qt["texttospeech"].libFilePath]
        filesAreTargets: true
        fileTags: ["dynamiclibrary"]
    }
}
