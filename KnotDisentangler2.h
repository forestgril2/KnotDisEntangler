#ifndef KnotDisentangler2_H
#define KnotDisentangler2_H

#include <QtGui/QMainWindow>
#include <QOpenGLFunctions>
#include <QtWidgets/QOpenGLWidget>

class KnotDisentangler2 : public QMainWindow, public QOpenGLWidget, 
protected QOpenGLFunctions
{
Q_OBJECT

public:
  KnotDisentangler2(QScreen *screen = 0);
  virtual ~KnotDisentangler2();
  
protected:
  void initializeGL();
  void paintGL();
  
  QOpenGLContext *m_context;
};

#endif // KnotDisentangler2_H
