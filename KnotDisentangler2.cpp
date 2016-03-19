#include "KnotDisentangler2.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QSurface>

KnotDisentangler2::KnotDisentangler2(QScreen* screen) 
: QMainWindow(screen), QOpenGLWidget(screen)
{
  QAction* action = new QAction(this);
  action->setText("Quit");
  
  connect(action, SIGNAL(triggered()), SLOT(close()));
  menuBar()->addMenu("File")->addAction(action);
  
  setWindowSurface(surfaceGL);
}

KnotDisentangler2::~KnotDisentangler2()
{
}

#include "KnotDisentangler2.moc"
