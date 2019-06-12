#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include <qgsmapcanvas.h>
#include <qgsmapoverviewcanvas.h>
#include <qgsvectorlayer.h>
#include <qgsrasterlayer.h>

#include <qgslayertreeview.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgslayertreemodel.h>
#include <qgslayertreeregistrybridge.h>
#include <qgsdockwidget.h>
#include <qgsmessagebar.h>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QgsLayerTreeView *layerTreeView()
    {
        return mLayerTreeView;
    }

    QgsLayerTreeMapCanvasBridge *layerTreeCanvasBridge()
    {
        return mLayerTreeCanvasBridge;
    }

public slots:
    void addVectorLayers();
    bool addVectorLayer(QgsVectorLayer *layer);

    void addRasterLayers();
    bool addRasterLayer(QgsRasterLayer *layer);

private slots:


private:
    Ui::MainWindow *ui;
    static MainWindow *sInstance;

    QgsMapCanvas *mMapCanvas = nullptr;
    QgsMapOverviewCanvas *mOverviewCanvas = nullptr;

    QgsLayerTreeView *mLayerTreeView = nullptr;
    QgsLayerTreeMapCanvasBridge *mLayerTreeCanvasBridge = nullptr;

    QgsDockWidget *mLayerTreeDock = nullptr;    
    QgsDockWidget *mOverviewDock = nullptr;     
    QStackedWidget *mCentralContainer = nullptr; 
    QgsMessageBar *mInfoBar = nullptr;

    void initLayerTreeView();
    void creatOverview();


};

#endif // MAINWINDOW_H
