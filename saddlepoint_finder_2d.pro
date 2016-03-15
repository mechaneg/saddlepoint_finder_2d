QMAKE_CXXFLAGS += -std=c++11 -Wall

CONFIG+=debug_and_release

CONFIG(release, debug|release){
  DESTDIR = build/build
  OBJECTS_DIR = $$DESTDIR
  MOC_DIR = $$DESTDIR
}else{
  DESTDIR = build/build.dbg
  OBJECTS_DIR = $$DESTDIR
  MOC_DIR = $$DESTDIR
}

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
    src/disp_relation.cpp \
    src/file_system.cpp \
    src/branch_on_real_axe.cpp \
    src/equip_line.cpp \
    src/om_k_evaluator.cpp \
    src/om_k.cpp

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
    src/disp_relation.h \
    src/file_system.h \
    src/branch_on_real_axe.h \
    src/equip_line.h \
    src/real_axe_intersection_state.h \
    src/om_k_evaluator.h
