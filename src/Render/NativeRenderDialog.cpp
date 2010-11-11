//
// C++ Implementation: NativeRenderDialog
//
// Description:
//
//
// Author: Chris Browet <cbro@semperpax.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "NativeRenderDialog.h"

#include "MainWindow.h"
#include "Document.h"
#include "MapView.h"
#include "Maps/Projection.h"
#include "Layer.h"
#include "Features.h"
#include "Preferences/MerkaartorPreferences.h"
#include "PaintStyle/MasPaintStyle.h"
#include "Utils/PictureViewerDialog.h"

#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPrintPreviewWidget>

#include <QProgressDialog>
#include <QPainter>
#include <QSvgGenerator>
#include <QFileDialog>

NativeRenderDialog::NativeRenderDialog(Document *aDoc, const CoordBox& aCoordBox, QWidget *parent)
    :QObject(parent), theDoc(aDoc), theOrigBox(aCoordBox)
{
    thePrinter = new QPrinter();
    thePrinter->setDocName(aDoc->title());

    mapview = new MapView(NULL);
    mapview->setDocument(theDoc);

    preview = new QPrintPreviewDialog( thePrinter, parent );
    QMainWindow* mw = preview->findChild<QMainWindow*>();
    prtW = dynamic_cast<QPrintPreviewWidget*>(mw->centralWidget());

    QWidget* myWidget = new QWidget(preview);
    ui.setupUi(myWidget);
    ui.verticalLayout->addWidget(prtW);
    mw->setCentralWidget(myWidget);

    connect(ui.cbShowNodes, SIGNAL(toggled(bool)), prtW, SLOT(updatePreview()));
    connect(ui.cbShowRelations, SIGNAL(toggled(bool)), prtW, SLOT(updatePreview()));
    connect(ui.cbShowGrid, SIGNAL(toggled(bool)), prtW, SLOT(updatePreview()));
    connect(ui.cbShowScale, SIGNAL(toggled(bool)), prtW, SLOT(updatePreview()));
    connect(ui.sbMinLat, SIGNAL(valueChanged(double)), prtW, SLOT(updatePreview()));
    connect(ui.sbMaxLat, SIGNAL(valueChanged(double)), prtW, SLOT(updatePreview()));
    connect(ui.sbMinLon, SIGNAL(valueChanged(double)), prtW, SLOT(updatePreview()));
    connect(ui.sbMaxLon, SIGNAL(valueChanged(double)), prtW, SLOT(updatePreview()));

    connect(ui.btExportPDF, SIGNAL(clicked()), SLOT(exportPDF()));
    connect(ui.btExportSVG, SIGNAL(clicked()), SLOT(exportSVG()));
    connect(ui.btExportRaster, SIGNAL(clicked()), SLOT(exportRaster()));

    connect( preview, SIGNAL(paintRequested(QPrinter*)), SLOT(print(QPrinter*)) );
    setBoundingBox(aCoordBox);
    setOptions(M_PREFS->getRenderOptions());
}

RendererOptions NativeRenderDialog::options()
{
    RendererOptions opt;
    opt.options |= RendererOptions::ForPrinting;
    opt.options |= RendererOptions::BackgroundVisible;
    opt.options |= RendererOptions::ForegroundVisible;
    opt.options |= RendererOptions::TouchupVisible;
    opt.options |= RendererOptions::NamesVisible;

    if (ui.cbShowNodes->isChecked())
        opt.options |= RendererOptions::NodesVisible;
    if (ui.cbShowRelations->isChecked())
        opt.options |= RendererOptions::RelationsVisible;
    if (ui.cbShowScale->isChecked())
        opt.options |= RendererOptions::ScaleVisible;
    if (ui.cbShowGrid->isChecked())
        opt.options |= RendererOptions::LatLonGridVisible;

    return opt;
}

void NativeRenderDialog::setOptions(RendererOptions aOpt)
{
    ui.cbShowNodes->setChecked(aOpt.options & RendererOptions::NodesVisible);
    ui.cbShowRelations->setChecked(aOpt.options & RendererOptions::RelationsVisible);
    ui.cbShowScale->setChecked(aOpt.options & RendererOptions::ScaleVisible);
    ui.cbShowGrid->setChecked(aOpt.options & RendererOptions::LatLonGridVisible);

    prtW->updatePreview();
}

CoordBox NativeRenderDialog::boundingBox()
{
    CoordBox VP(Coord(
                    angToCoord(ui.sbMinLat->value()),
                    angToCoord(ui.sbMinLon->value())
            ), Coord(
                    angToCoord(ui.sbMaxLat->value()),
                    angToCoord(ui.sbMaxLon->value())
                    ));
    return VP;
}

void NativeRenderDialog::setBoundingBox(CoordBox aBBox)
{
    ui.sbMinLat->setValue(coordToAng(aBBox.bottomLeft().lat()));
    ui.sbMaxLat->setValue(coordToAng(aBBox.topLeft().lat()));
    ui.sbMinLon->setValue(coordToAng(aBBox.topLeft().lon()));
    ui.sbMaxLon->setValue(coordToAng(aBBox.topRight().lon()));

    prtW->updatePreview();
}

int NativeRenderDialog::exec()
{
    return preview->exec();
}

void NativeRenderDialog::print(QPrinter* prt)
{
    QPainter P(prt);
    P.setRenderHint(QPainter::Antialiasing);
    QRect theR = prt->pageRect();
    theR.moveTo(0, 0);
    render(P, theR);
}

void NativeRenderDialog::render(QPainter& P, QRect theR)
{
    P.setClipRect(theR);
    P.setClipping(true);
    RendererOptions opt = options();

    mapview->setGeometry(theR);
    mapview->setViewport(boundingBox(), theR);
    mapview->setRenderOptions(opt);
    mapview->invalidate(true, false);
    mapview->buildFeatureSet();
    mapview->drawCoastlines(P);
    mapview->printFeatures(P);
    if (opt.options & RendererOptions::ScaleVisible)
        mapview->drawScale(P);
    if (opt.options & RendererOptions::LatLonGridVisible)
        mapview->drawLatLonGrid(P);
}

void NativeRenderDialog::exportPDF()
{
    QString s = QFileDialog::getSaveFileName(NULL,tr("Output filename"),"",tr("PDF files (*.pdf)"));
    if (s.isNull())
        return;

#if QT_VERSION >= 0x040500
    QPrinter* prt = preview->printer();
#else
    QPrinter* prt = thePrinter;
#endif
    prt->setOutputFormat(QPrinter::PdfFormat);
    prt->setOutputFileName(s);
    print(prt);
}

void NativeRenderDialog::exportRaster()
{
    QString s = QFileDialog::getSaveFileName(NULL,tr("Output filename"),"",tr("Image files (*.png *.jpg)"));
    if (s.isNull())
        return;
#if QT_VERSION >= 0x040500
    QRect theR = preview->printer()->pageRect();
#else
    QRect theR = thePrinter->pageRect();
#endif
    theR.moveTo(0, 0);

    QPixmap pix(theR.size());
    if (M_PREFS->getUseShapefileForBackground())
        pix.fill(M_PREFS->getWaterColor());
    else if (M_PREFS->getBackgroundOverwriteStyle() || !M_STYLE->getGlobalPainter().getDrawBackground())
        pix.fill(M_PREFS->getBgColor());
    else
        pix.fill(M_STYLE->getGlobalPainter().getBackgroundColor());

    QPainter P(&pix);
    P.setRenderHint(QPainter::Antialiasing);
    render(P, theR);

    pix.save(s);
}

void NativeRenderDialog::exportSVG()
{
    QString s = QFileDialog::getSaveFileName(NULL,tr("Output filename"),"",tr("SVG files (*.svg)"));
    if (s.isNull())
        return;

    QSvgGenerator svgg;
#if QT_VERSION >= 0x040500
    QRect theR = preview->printer()->pageRect();
#else
    QRect theR = thePrinter->pageRect();
#endif
    theR.moveTo(0, 0);
    svgg.setSize(theR.size());
    svgg.setFileName(s);
#if QT_VERSION >= 0x040500
        svgg.setViewBox(theR);
#endif

    QPainter P(&svgg);
    P.setRenderHint(QPainter::Antialiasing);
    render(P, theR);
}

