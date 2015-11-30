QMAKE_CXXFLAGS += -std=c++11 -Werror -Wno=comment

QT += core

SOURCES += \
    src/main.cpp \
    src/model_controller.cpp \
    src/model.cpp \
    src/cmd_params.cpp \
    src/report_system.cpp \
    src/input_output_files.cpp \
    src/params.cpp \
    src/jenkins_traub.cpp \
    src/disp_rel.cpp

HEADERS += \
    src/common_defines.h \
    src/model_controller.h \
    src/params.h \
    src/model.h \
    src/cmd_params.h \
    src/report_system.h \
    src/input_output_files.h \
    src/om_k.h \
    src/jenkins_traub.h \
    src/disp_rel.h
