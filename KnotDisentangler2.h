#ifndef KnotDisentangler2_H
#define KnotDisentangler2_H

#include <QtGui/QMainWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtWidgets/QOpenGLWidget>

class KnotDisentangler2 : public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT

public:
  KnotDisentangler2(QWidget* parent = Q_NULLPTR);
  virtual ~KnotDisentangler2();
  
protected:
  void initializeGL();
  void paintGL();
  
  QOpenGLContext *m_context;
};

#endif // KnotDisentangler2_H
