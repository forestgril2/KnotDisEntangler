#include "KnotDisentangler2.h"

#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

KnotDisentangler2::KnotDisentangler2(QWidget* parent) : QOpenGLWidget(parent)
{
  //QMenuBar* menuBar;
  //QAction* action = new QAction(this);
  //action->setText("Quit");
  
  //connect(action, SIGNAL(triggered()), SLOT(close()));
  //menuBar->addMenu("File")->addAction(action);
}

KnotDisentangler2::~KnotDisentangler2()
{
}

#include "KnotDisentangler2.moc"
