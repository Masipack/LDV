#---------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------
QT += core gui widgets opengl serialport sql webengine webenginewidgets printsupport

DEFINES += _TESTE_
DEFINES += _DISPLAY_HORIZONTAL_
DEFINES += _USE_DISK_

TRANSLATIONS = res/translations/lang.ts

#---------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------
win32 {
    QMAKE_LFLAGS += /INCREMENTAL:NO
    THIRDY_PARTY = c:/3rdparty
}

linux {
    THIRDY_PARTY = $(HOME)/3rdparty

   # QMAKE_CXXFLAGS += -Wabi-tag
  #  QMAKE_CXXFLAGS += -Wno-unknown-pragmas
  #  QMAKE_CXXFLAGS += -Wno-unused-parameter
  #  QMAKE_CXXFLAGS += -Wno-unused-but-set-variable
  #  QMAKE_CXXFLAGS += -Wno-unused-variable
  #  QMAKE_CXXFLAGS += -Wno-unused-function
  #  QMAKE_CXXFLAGS += -Wno-expansion-to-defined

    QMAKE_CXXFLAGS += -Wno-unknown-pragmas
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-unused-but-set-variable
    QMAKE_CXXFLAGS += -Wno-unused-variable
    #QMAKE_CXXFLAGS += -Wno-abi-tag
}

#---------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------
INCLUDEPATH += $$THIRDY_PARTY/pylon/include
INCLUDEPATH += $$THIRDY_PARTY/genicam/library/cpp/include
INCLUDEPATH += $$THIRDY_PARTY/genicam/library/cpp/include
INCLUDEPATH += $$THIRDY_PARTY/opencv4/include
INCLUDEPATH += $$THIRDY_PARTY/MasipackControls
INCLUDEPATH += $$THIRDY_PARTY/qzxing-master/src/zxing
INCLUDEPATH += $$THIRDY_PARTY/qzxing-master/src
INCLUDEPATH += $$THIRDY_PARTY/libdmtx
INCLUDEPATH += $$THIRDY_PARTY/ZBar/include
INCLUDEPATH += $$THIRDY_PARTY/cognex/include
INCLUDEPATH += $$THIRDY_PARTY/AVL/include
#INCLUDEPATH += $$THIRDY_PARTY/avlibrary_lite/include
INCLUDEPATH += $$PWD

#---------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------
TARGET      = LDFM-V2
TEMPLATE    = app

#QMAKE_LFLAGS += -no-pie

DEPLOY_PATH = $${PWD}/../$${TARGET}_DEPLOY

#---------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------
OBJECTS_DIR = $${DEPLOY_PATH}/C_OBJ
MOC_DIR     = $${DEPLOY_PATH}/C_MOC
UI_DIR      = $${DEPLOY_PATH}/C_UI

#---------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release) {
    DESTDIR     = $${DEPLOY_PATH}/Debug
}

CONFIG(release, debug|release) {
    DESTDIR     = $${DEPLOY_PATH}/Release
}

#---------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------
SOURCES += \
        main.cpp \
        maindialog.cpp \
    util/dlgkeyboard.cpp \
    util/serialcontrol.cpp \
    util/dlginfo.cpp \
    util/alarmmanager.cpp \
    util/imagequeue.cpp \
    util/systemsettings.cpp \
    interface/windowmanager.cpp \
    util/dialogpart11.cpp \
    interface/formmainmenu.cpp \
    mv/camera/cam_aca1300_60gc.cpp \
    mv/camera/cameramanager.cpp \
    mv/camera/mvcamera.cpp \
    interface/form_login.cpp \
    interface/formalarms.cpp \
    interface/formusbexport.cpp \
    interface/formversion.cpp \
    interface/formprinter.cpp \
    interface/formpart11.cpp \
    interface/dlgdatetime.cpp \
    util/pdfmaker.cpp \
    interface/form_newuser.cpp \
    interface/form_users.cpp \
    interface/formconfig.cpp \
    interface/formproducts.cpp \
    mv/imagedevice.cpp \
    mv/inspectionbuffer.cpp \
    mv/mvtool.cpp \
    interface/formnewproduct.cpp \
    interface/widgets/mvlistwidget.cpp \
    interface/dlginsptype.cpp \
    mv/application/ibconfig.cpp \
    mv/tools/mvabstracttool.cpp \
    mv/tools/mvfiducial.cpp \
    mv/tools/mvdatamatrix.cpp \
    mv/tools/mvbarcode.cpp \
    mv/tools/mvocr.cpp \
    interface/widgets/paramsocr.cpp \
    interface/widgets/paramsfiducial.cpp \
    interface/widgets/paramsbarcode.cpp \
    interface/widgets/paramsdatamatrix.cpp \
    mv/PO/mvocrpo.cpp \
    interface/formnewproductcontent.cpp \
    interface/formnewproductfinish.cpp \
    interface/formprocess.cpp \
    interface/formprocesscontent.cpp \
    mv/application/ibprocess.cpp \
    interface/formstatistics.cpp \
    interface/formimagelog.cpp \
    util/dlginfoprocess.cpp \
    util/MWResult.cpp \
    util/msqlsingleton.cpp \
    util/factorymssql.cpp \
    util/dlgDataBase.cpp \
    interface/configdatabase.cpp \
    interface/formsaidas.cpp \
    interface/widgets/widgetoutput.cpp \
    mv/application/ibimagelog.cpp \
    interface/formfirstaccess.cpp \
    interface/widgets/widgetuser.cpp \
    interface/formconfiguser.cpp

HEADERS += \
        maindialog.h \
    util/systemsettings.h \
    util/fileutil.h \
    util/sys_log.h \
    util/serialcontrol.h \
    util/permission_check.h \
    util/alarmmanager.h \
    util/imagequeue.h \
    util/dlgkeyboard.h \
    util/dlginfo.h \
    interface/windowmanager.h \
    util/dialogpart11.h \
    interface/formmainmenu.h \
    mv/camera/cam_aca1300_60gc.h \
    mv/camera/cameramanager.h \
    mv/camera/mvcamera.h \
    interface/form_login.h \
    interface/formalarms.h \
    interface/formusbexport.h \
    interface/formversion.h \
    interface/formprinter.h \
    interface/formpart11.h \
    interface/dlgdatetime.h \
    util/pdfmaker.h \
    interface/form_newuser.h \
    interface/form_users.h \
    interface/formconfig.h \
    interface/formproducts.h \
    mv/imagedevice.h \
    mv/inspectionbuffer.h \
    mv/mvtool.h \
    interface/formnewproduct.h \
    interface/widgets/mvlistwidget.h \
    global_defines.h \
    interface/dlginsptype.h \
    mv/application/ibconfig.h \
    mv/tools/mvabstracttool.h \
    mv/tools/mvfiducial.h \
    mv/tools/mvdatamatrix.h \
    mv/tools/mvbarcode.h \
    mv/tools/mvocr.h \
    TO/abstracttoolto.h \
    TO/fiducialto.h \
    interface/widgets/paramsocr.h \
    interface/widgets/paramsfiducial.h \
    interface/widgets/paramsbarcode.h \
    interface/widgets/paramsdatamatrix.h \
    mv/PO/mvocrpo.h \
    TO/ocrto.h \
    TO/barcodeto.h \
    TO/datamatrixto.h \
    TO/productto.h \
    interface/formnewproductcontent.h \
    interface/formnewproductfinish.h \
    TO/multicamto.h \
    interface/formprocess.h \
    interface/formprocesscontent.h \
    mv/application/ibprocess.h \
    interface/formstatistics.h \
    interface/formimagelog.h \
    util/dlginfoprocess.h \
    util/MWResult.h \
    util/msqlsingleton.h \
    util/abstractdatabase.h \
    util/factorymssql.h \
    TO/databaseto.h \
    util/dlgDataBase.h \
    interface/configdatabase.h \
    interface/formsaidas.h \
    interface/widgets/widgetoutput.h \
    mv/application/ibimagelog.h \
    interface/formfirstaccess.h \
    interface/widgets/widgetuser.h \
    interface/formconfiguser.h

FORMS += \
        maindialog.ui \
    util/dlginfo.ui \
    util/dlgkeyboard.ui \
    util/dialogpart11.ui \
    interface/form_mainmenuH.ui \
    interface/form_login.ui \
    interface/formalarms.ui \
    interface/formusbexport.ui \
    interface/form_versionH.ui \
    interface/form_versionV.ui \
    interface/formprinter.ui \
    interface/formpart11.ui \
    interface/dlgdatetime.ui \
    interface/form_newuser.ui \
    interface/form_users.ui \
    interface/form_configH.ui \
    interface/form_configV.ui \
    interface/form_productsH.ui \
    interface/form_productsV.ui \
    interface/formnewproduct.ui \
    interface/widgets/mvlistwidget.ui \
    interface/dlginsptype.ui \
    interface/widgets/paramsocr.ui \
    interface/widgets/paramsfiducial.ui \
    interface/widgets/paramsbarcode.ui \
    interface/widgets/paramsdatamatrix.ui \
    interface/formnewproductcontent.ui \
    interface/formnewproductfinish.ui \
    interface/formprocess.ui \
    interface/formprocesscontent.ui \
    interface/formstatistics.ui \
    interface/formimagelog.ui \
    util/dlginfoprocess.ui \
    util/dlgDataBase.ui \
    interface/configdatabase.ui \
    interface/formsaidas.ui \
    interface/widgets/widgetoutput.ui \
    interface/formfirstaccess.ui \
    interface/widgets/widgetuser.ui \
    interface/formconfiguser.ui

#---------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------
#LIBS += -L$$THIRDY_PARTY/"opencv4/lib"              -lopencv_highgui
#LIBS += -L$$THIRDY_PARTY/"opencv4/lib"              -lopencv_core
#LIBS += -L$$THIRDY_PARTY/"opencv4/lib"              -lopencv_imgproc
#LIBS += -L$$THIRDY_PARTY/"opencv4/lib"              -lopencv_ml
#LIBS += -L$$THIRDY_PARTY/"opencv4/lib"              -lopencv_objdetect
#LIBS += -L$$THIRDY_PARTY/"opencv4/lib"              -lopencv_features2d
#                                                    -lopencv_imgcodecs

LIBS += -L$$THIRDY_PARTY/"opencv4/lib"      -lopencv_core \
                                            -lopencv_highgui \
                                            -lopencv_imgproc \
                                            -lopencv_imgcodecs\
                                            -lopencv_ml \
                                            -lopencv_objdetect \
                                            -lopencv_features2d

LIBS += -L$$THIRDY_PARTY/"pylon/lib64"              -lGCBase_gcc_v3_0_Basler_pylon_v5_0 \
                                                    -lGenApi_gcc_v3_0_Basler_pylon_v5_0 \
                                                    -lpylonbase-5.0.5 \
                                                    -lMathParser_gcc_v3_0_Basler_pylon_v5_0 \
                                                    -lXmlParser_gcc_v3_0_Basler_pylon_v5_0 \
                                                    -lLog_gcc_v3_0_Basler_pylon_v5_0 \
                                                    -lNodeMapData_gcc_v3_0_Basler_pylon_v5_0 \
                                                    -lpylonutility

LIBS += -L$$THIRDY_PARTY/"MasipackControls"        -lmasipackcontrolsplugin

LIBS += -L$$THIRDY_PARTY/"qzxing-master/src"       -lQZXing

LIBS += -L$$THIRDY_PARTY/"libdmtx/.libs"           -ldmtx

LIBS += -L$$THIRDY_PARTY/"ZBar/libs"               -lzbar

LIBS += -L$$THIRDY_PARTY/"cognex/lib"              -lBarcodeScanner

#LIBS += -L$$THIRDY_PARTY/"cognex/lib"              -lDataManSDK
LIBS += -L$$THIRDY_PARTY/"AVL/lib"                 -lAVL

#LIBS += -L$$THIRDY_PARTY/"avlibrary_lite/lib"      -lAVL_Lite


RESOURCES += \
    res/images.qrc \
    res/icons.qrc \
    res/styles.qrc

DISTFILES += \
    ../LDF_DEPLOY/Debug/data/settings.dat \
    ../LDF_DEPLOY/Release/data/settings.dat \
    ../LDFM-V2_DEPLOY/Release/data/settings.dat


