#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLayout>

#include <qgssettings.h>

#include <qgsproject.h>

#include <qgslayertreelayer.h>
#include "qgslayertreeviewdefaultactions.h"

MainWindow *MainWindow::sInstance = nullptr;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    sInstance = this;
    ui->setupUi(this);

    QWidget *centralWidget = this->centralWidget();
    QGridLayout *centralLayout = new QGridLayout( centralWidget );
    centralWidget->setLayout( centralLayout );
    centralLayout->setContentsMargins( 0, 0, 0, 0 );

    mMapCanvas = new QgsMapCanvas(centralWidget);
    mMapCanvas->setVisible(true);
    mMapCanvas->enableAntiAliasing(true);


    mCentralContainer = new QStackedWidget;
    mCentralContainer->insertWidget( 0, mMapCanvas );
    centralLayout->addWidget( mCentralContainer, 0, 0, 2, 1 );
    mCentralContainer->setCurrentIndex(0);

    mInfoBar = new QgsMessageBar( centralWidget );
    mInfoBar->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
    centralLayout->addWidget( mInfoBar, 0, 0, 1, 1 );

    mLayerTreeView = new QgsLayerTreeView( this );
    mLayerTreeView->setObjectName( QStringLiteral( "theLayerTreeView" ) ); // "theLayerTreeView" used to find this canonical instance later

    initLayerTreeView();
    creatOverview();

    connect(ui->actionAddVectorLayer, &QAction::triggered, this, &MainWindow::addVectorLayers);
    connect(ui->actionAddRasterLayer, &QAction::triggered, this, &MainWindow::addRasterLayers);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addVectorLayers()
{
    mMapCanvas->freeze();

    QgsSettings settings;
    QString lastUsedDir = settings.value( QStringLiteral( "UI/lastOgrDir" ), QDir::homePath() ).toString();

    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                            tr("Add Vector Layer"),
                            lastUsedDir,
                            "OGR layers(*.shp *.geojson *.gmt *.kml)");

    if(filePaths.isEmpty())
    {
        return;
    }
    for(QStringList::iterator it = filePaths.begin(); it != filePaths.end(); it++)
    {
        QString baseName = QFileInfo(*it).completeBaseName();
        QgsVectorLayer *layer = new QgsVectorLayer(*it, baseName, "ogr");
        if( !layer || !layer->isValid())
        {
            QMessageBox::critical(this, tr("Error"), tr("Failed to open file\n%1").arg(*it));
            return;
        }
        addVectorLayer(layer);
    }
    mMapCanvas->freeze(false);
    mMapCanvas->refresh();

    QFileInfo fi( filePaths.at(0) );
    QString path = fi.path();
    settings.setValue( QStringLiteral( "UI/lastOgrDir" ), path );
}

bool MainWindow::addVectorLayer(QgsVectorLayer *layer)
{
    if(!layer)
    {
        qDebug() << "no vector layer";
        return false;
    }
    if(!layer->isValid())
    {
        qDebug() << "invalid vector layer";
        delete layer;
        return false;
    }

    // register this layer with the central layers registry
    QList<QgsMapLayer *> myList;
    myList << layer;
    QgsProject::instance()->addMapLayers( myList );
    qDebug() << "add layer, crs of project:" << QgsProject::instance()->crs().authid() << "," << QgsProject::instance()->crs().description();

    QgsLayerTreeLayer *layerTreeLayer = new QgsLayerTreeLayer(layer);
    qDebug() << layerTreeLayer->name();

    return true;
}

void MainWindow::addRasterLayers()
{
    mMapCanvas->freeze();

    QgsSettings settings;
    QString lastUsedDir = settings.value( QStringLiteral( "UI/lastGdalDir" ), QDir::homePath() ).toString();

    //QString filter = "NetCDF(*.grd *.GRD)";//默认过滤名测试
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                            tr("Add Raster Layer(s)"),
                            lastUsedDir,
                            "Raster layers(*.tif *.img *.grd *.hgt)");

    if(filePaths.isEmpty())
    {
        return;
    }
    for(QStringList::iterator it = filePaths.begin(); it != filePaths.end(); it++)
    {
        QString baseName = QFileInfo(*it).completeBaseName();
        QgsRasterLayer *layer = new QgsRasterLayer(*it, baseName, "gdal");
        if( !layer || !layer->isValid())
        {
            return;
        }
        addRasterLayer(layer);
    }
    mMapCanvas->freeze(false);
    mMapCanvas->updateScale();
    mMapCanvas->refresh();

    QFileInfo fi( filePaths.at(0) );
    QString path = fi.path();
    settings.setValue( QStringLiteral( "UI/lastGdalDir" ), path );
}

bool MainWindow::addRasterLayer(QgsRasterLayer *layer)
{
    if ( ! layer )
    {
        return false;
    }

    if ( !layer->isValid() )
    {
        delete layer;
        return false;
    }

    // register this layer with the central layers registry
    QList<QgsMapLayer *> myList;
    myList << layer;
    QgsProject::instance()->addMapLayers( myList );

    return true;
}

void MainWindow::initLayerTreeView()
{
    mLayerTreeDock = new QgsDockWidget( tr( "Layers" ), this );
    mLayerTreeDock->setObjectName( QStringLiteral( "Layers" ) );
    mLayerTreeDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

    QgsLayerTreeModel *model = new QgsLayerTreeModel( QgsProject::instance()->layerTreeRoot(), this );

    model->setFlag( QgsLayerTreeModel::AllowNodeReorder );
    model->setFlag( QgsLayerTreeModel::AllowNodeRename );
    model->setFlag( QgsLayerTreeModel::AllowNodeChangeVisibility );
    model->setFlag( QgsLayerTreeModel::ShowLegendAsTree );
    model->setFlag( QgsLayerTreeModel::UseEmbeddedWidgets );
    model->setAutoCollapseLegendNodes( 10 );

    mLayerTreeView->setModel( model );

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setMargin( 0 );
    vboxLayout->setContentsMargins( 0, 0, 0, 0 );
    vboxLayout->setSpacing( 0 );
    vboxLayout->addWidget( mLayerTreeView );

    QWidget *w = new QWidget;
    w->setLayout( vboxLayout );
    mLayerTreeDock->setWidget( w );
    addDockWidget( Qt::LeftDockWidgetArea, mLayerTreeDock );

    mLayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge( QgsProject::instance()->layerTreeRoot(), mMapCanvas, this );
}

void MainWindow::creatOverview()
{
    mOverviewCanvas = new QgsMapOverviewCanvas( nullptr, mMapCanvas );
    mOverviewCanvas->setBackgroundColor(QColor(Qt::white));

    QCursor mOverviewMapCursor( Qt::OpenHandCursor );
    mOverviewCanvas->setCursor( mOverviewMapCursor );
    mOverviewDock = new QgsDockWidget( tr( "Overview" ), this );

    mOverviewDock->setObjectName( QStringLiteral( "Overview" ) );
    mOverviewDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    mOverviewDock->setWidget( mOverviewCanvas );
    addDockWidget( Qt::LeftDockWidgetArea, mOverviewDock );

    mLayerTreeCanvasBridge->setOverviewCanvas( mOverviewCanvas );
}
