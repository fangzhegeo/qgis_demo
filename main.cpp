#include <QtWidgets/QApplication>
#include <qgsapplication.h>
#include <QtMath>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QgsApplication a(argc, argv, true);
    QgsApplication::setPrefixPath("D:/OSGeo4W64/apps/qgis", true);
    QgsApplication::initQgis();
    MainWindow w;
    w.show();
    return a.exec();
}
