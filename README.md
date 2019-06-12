# Introduction
An simple QGIS 3.x demo using C++ API containing map canvas and layer tree view. You can use this project to configure your development environment.The following paths of QGIS in **qgis_demo.pro** and **main.cpp** should be replaced by yours.


一个简单的QGIS 3.x 版本二次开发的demo，包含地图画布控件和图层树控件，可以用于开发环境的配置。使用时需要将**qgis_demo.pro**文件与**main.cpp**文件中以下路径替换为自己的下载路径。


```
INCLUDEPATH +=  "D:/OSGeo4W64/apps/qgis/include"
INCLUDEPATH +=  "D:/OSGeo4W64/include"

LIBS += -L"D:/OSGeo4W64/apps/qgis/lib" -lqgis_core -lqgis_gui
```


```
QgsApplication::setPrefixPath("D:/OSGeo4W64/apps/qgis", true);
```
