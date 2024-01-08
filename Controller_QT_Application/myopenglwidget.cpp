#include "myopenglwidget.h"


/*!
 * \file
 * \brief Zawiera definicje metod klasy MyOpenGLWidget
 *
 * W pliku znajdują się definicje klasy MyOpenGLWidget
 * Są one odpowiedzialne za wizualizacje połozenia manipulatora
 * W momencie wykrycia reakcju ze strony uzytkownika
 * emitowany jest odpowieni sygnal odczytywane jest nowe wychylenie
 * danego przegubu a nastepnie zaktualizowanie polozenia
 * manipulatora na scenie
 */


MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    :QOpenGLWidget (parent)

{
    _Q0_deg = _Q1_deg = _Q2_deg = 0;
}
/*!
 * Aktualizuje odebrane dane dotyczace wychylenia poszczegolnyh
 * serwomechanizmow
 * \param[in] Q0_deg- Wartosc konta w stopniach
 */
void MyOpenGLWidget::SetQ0_deg(int Q0_deg)
{
    _Q0_deg = float(Q0_deg);
    update();
  //  qDebug() << " deg1 " << _Q0_deg ;
}

/*!
 * Aktualizuje odebrane dane dotyczace wychylenia poszczegolnyh
 * serwomechanizmow
 * \param[in] Q1_deg- Wartosc konta w stopniach
 */
void MyOpenGLWidget::SetQ1_deg(int Q1_deg)
{
    _Q1_deg = float(Q1_deg);
    update();
   // qDebug() << " deg2 " << _Q1_deg ;
}

/*!
 * Aktualizuje odebrane dane dotyczace wychylenia poszczegolnyh
 * serwomechanizmow
 * \param[in] Q2_deg- Wartosc konta w stopniach
 */
void MyOpenGLWidget::SetQ2_deg(int Q2_deg)
{
    _Q2_deg = float(Q2_deg);
    update();
   // qDebug() << " deg3 " << _Q2_deg ;
}

/*!
 * Aktualizuje odebrane dane dotyczace wychylenia poszczegolnyh
 * serwomechanizmow
 * \param[in] Q3_deg- Wartosc konta w stopniach
 */
void MyOpenGLWidget::SetQ3_deg(int Q3_deg)
{
    _Q3_deg = float(Q3_deg);
    update();
}

/*!
 * Aktualizuje odebrane dane dotyczace wychylenia poszczegolnyh
 * serwomechanizmow
 * \param[in] Q4_deg- Wartosc konta w stopniach
 */
void MyOpenGLWidget::SetQ4_deg(int Q4_deg)
{
    _Q4_deg = float(Q4_deg);
    update();
}



/*!
 * Tworzy szescian oraz dokonuje jego
 * przeskalowania wzgledem poszczegolnego osi układu spolrzednych
 * \param[in] Size_X - Wartość przeskalowania w kierunku osi x
 * \param[in] Size_Y - Wartość przeskalowania w kierunku osi y
 * \param[in] Size_Z - Wartość przeskalowania w kierunku osi z
 */
void GLCreateBox( float Size_X,  float Size_Y,  float Size_Z )
{
 glPushMatrix();
 glScalef( Size_X, Size_Y, Size_Z );

 glBegin(GL_POLYGON);
  glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 is red
  glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 is green
  glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 is blue
  glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 is purple
 glEnd();


 glBegin(GL_POLYGON);
  glColor3f(   1.0,  1.0, 1.0 );
  glVertex3f(  0.5, -0.5, 0.5 );
  glVertex3f(  0.5,  0.5, 0.5 );
  glVertex3f( -0.5,  0.5, 0.5 );
  glVertex3f( -0.5, -0.5, 0.5 );
 glEnd();

// Purple side - RIGHT
 glBegin(GL_POLYGON);
  glColor3f(  1.0,  0.0,  1.0 );
  glVertex3f( 0.5, -0.5, -0.5 );
  glVertex3f( 0.5,  0.5, -0.5 );
  glVertex3f( 0.5,  0.5,  0.5 );
  glVertex3f( 0.5, -0.5,  0.5 );
 glEnd();

// Green side - LEFT
 glBegin(GL_POLYGON);
  glColor3f(   0.0,  1.0,  0.0 );
  glVertex3f( -0.5, -0.5,  0.5 );
  glVertex3f( -0.5,  0.5,  0.5 );
  glVertex3f( -0.5,  0.5, -0.5 );
  glVertex3f( -0.5, -0.5, -0.5 );
 glEnd();

  // Blue side - TOP
 glBegin(GL_POLYGON);
   glColor3f(   0.0,  0.0,  1.0 );
   glVertex3f(  0.5,  0.5,  0.5 );
   glVertex3f(  0.5,  0.5, -0.5 );
   glVertex3f( -0.5,  0.5, -0.5 );
   glVertex3f( -0.5,  0.5,  0.5 );
  glEnd();

// Red side - BOTTOM
  glBegin(GL_POLYGON);
   glColor3f(   1.0,  0.0,  0.0 );
   glVertex3f(  0.5, -0.5, -0.5 );
   glVertex3f(  0.5, -0.5,  0.5 );
   glVertex3f( -0.5, -0.5,  0.5 );
   glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();
  glPopMatrix();
}


/*!
 * Tworzy model serwomechanizmu
 * przeskalowania wzgledem poszczegolnego osi układu spolrzednych
 * \param[in] Size_X - Wartość przeskalowania w kierunku osi x
 * \param[in] Size_Y - Wartość przeskalowania w kierunku osi y
 * \param[in] Size_Z - Wartość przeskalowania w kierunku osi z
 */
void GLCreateServo( float Size_X,  float Size_Y,  float Size_Z )
{
    glRotatef( -90, 1.0, 0.0, 0.0 );
    glPushMatrix();
    glScalef( Size_X, Size_Y, Size_Z );

    glBegin(GL_POLYGON);
     glColor3f( 0.0, 0.0, 0.0 );
     glVertex3f(  0.025f, -0.05f, -0.0625f );
     glVertex3f(  0.025f,  0.05f, -0.0625f );
     glVertex3f( -0.025f,  0.05f, -0.0625f );
     glVertex3f( -0.025f, -0.05f, -0.0625f );
    glEnd();


    glBegin(GL_POLYGON);
     glColor3f( 0.0, 0.0, 0.0 );
     glVertex3f(  0.025f, -0.05f, 0.0375f );
     glVertex3f(  0.025f,  0.05f, 0.0375f );
     glVertex3f( -0.025f,  0.05f, 0.0375f );
     glVertex3f( -0.025f, -0.05f, 0.0375f );
    glEnd();

   // Purple side - RIGHT
    glBegin(GL_POLYGON);
     glColor3f( 0.0, 0.0, 0.0 );
     glVertex3f( 0.025f, -0.05f, -0.0625f );
     glVertex3f( 0.025f,  0.05f, -0.0625f );
     glVertex3f( 0.025f,  0.05f,  0.0375f );
     glVertex3f( 0.025f, -0.05f,  0.0375f );
    glEnd();

   // Green side - LEFT
    glBegin(GL_POLYGON);
     glColor3f( 0.0, 0.0, 0.0 );
     glVertex3f( -0.025f, -0.05f,  0.0375f );
     glVertex3f( -0.025f,  0.05f,  0.0375f );
     glVertex3f( -0.025f,  0.05f, -0.0625f );
     glVertex3f( -0.025f, -0.05f, -0.0625f );
    glEnd();

     // Blue side - TOP
    glBegin(GL_POLYGON);
      glColor3f( 0.0, 0.0, 0.0 );
      glVertex3f(  0.025f,  0.05f,  0.0375f );
      glVertex3f(  0.025f,  0.05f, -0.0625f );
      glVertex3f( -0.025f,  0.05f, -0.0625f );
      glVertex3f( -0.025f,  0.05f,  0.0375f );
     glEnd();

   // Red side - BOTTOM
     glBegin(GL_POLYGON);
      glColor3f( 0.0, 0.0, 0.0 );
      glVertex3f(  0.025f, -0.05f, -0.0625f );
      glVertex3f(  0.025f, -0.05f,  0.0375f );
      glVertex3f( -0.025f, -0.05f,  0.0375f );
      glVertex3f( -0.025f, -0.05f, -0.0625f );
     glEnd();
     glPopMatrix();

     glColor3f( 1.0, 0.84f, 0.0 );  // zolty walec
     glTranslatef( 0.0, 0.025f, 0.0375f );
     glutWireCylinder(0.00625,0.0125, 50,1);

     glColor3f( 0.0, 0.0, 0.0 );
     glTranslatef( 0.0, 0.0, 0.0125f );
     glutWireCylinder(0.01,0.00625, 100,1);

     glTranslatef( 0.0, 0.0, 0.00625f );
     glutWireCylinder(0.025,0.00625, 150,1);
     glTranslatef( 0.0, 0.0, 0.00625f );

     float theta;
     glBegin(GL_POLYGON);
     for(int i=0; i<360; i++){

         theta = i*3.142f/180.0f;
         glVertex3f( 0.025f* cos(theta), 0.025f*sin(theta), 0.0f );
     }
     glEnd();
}


/*!
 * Tworzy model serwomechanizmu
 * przeskalowania wzgledem poszczegolnego osi układu spolrzednych
 * \param[in] Size_X - Wartość przeskalowania w kierunku osi x
 * \param[in] Size_Y - Wartość przeskalowania w kierunku osi y
 * \param[in] Size_Z - Wartość przeskalowania w kierunku osi z
 */
void GLCreateServoSmall( float Size_X,  float Size_Y,  float Size_Z )
{
    glRotatef( -90, 1.0, 0.0, 0.0 );
    glPushMatrix();
    glScalef( Size_X, Size_Y, Size_Z );

    glBegin(GL_POLYGON);
     glColor3f( 0.0, 0.0, 0.0 );
     glVertex3f(  0.05f, -0.025f, -0.05f );
     glVertex3f(  0.05f,  0.025f, -0.05f );
     glVertex3f( -0.05f,  0.025f, -0.05f );
     glVertex3f( -0.05f, -0.025f, -0.05f );
    glEnd();


    glBegin(GL_POLYGON);
     glColor3f( 0.0, 0.0, 0.0 );
     glVertex3f(  0.05f, -0.025f, 0.0125f );
     glVertex3f(  0.05f,  0.025f, 0.0125f );
     glVertex3f( -0.05f,  0.025f, 0.0125f );
     glVertex3f( -0.05f, -0.025f, 0.0125f );
    glEnd();

   // Purple side - RIGHT
    glBegin(GL_POLYGON);
     glColor3f( 0.0, 0.0, 0.0 );
     glVertex3f( 0.05f, -0.025f, -0.05f );
     glVertex3f( 0.05f,  0.025f, -0.05f );
     glVertex3f( 0.05f,  0.025f,  0.0125f );
     glVertex3f( 0.05f, -0.025f,  0.0125f );
    glEnd();

   // Green side - LEFT
    glBegin(GL_POLYGON);
     glColor3f( 0.0, 0.0, 0.0 );
     glVertex3f( -0.05f, -0.025f,  0.0125f );
     glVertex3f( -0.05f,  0.025f,  0.0125f );
     glVertex3f( -0.05f,  0.025f, -0.05f );
     glVertex3f( -0.05f, -0.025f, -0.05f );
    glEnd();

     // Blue side - TOP
    glBegin(GL_POLYGON);
      glColor3f( 0.0, 0.0, 0.0 );
      glVertex3f(  0.05f,  0.025f,  0.0125f );
      glVertex3f(  0.05f,  0.025f, -0.05f );
      glVertex3f( -0.05f,  0.025f, -0.05f );
      glVertex3f( -0.05f,  0.025f,  0.0125f );
     glEnd();

   // Red side - BOTTOM
     glBegin(GL_POLYGON);
      glColor3f( 0.0, 0.0, 0.0 );
      glVertex3f(  0.05f, -0.025f, -0.05f );
      glVertex3f(  0.05f, -0.025f,  0.0125f );
      glVertex3f( -0.05f, -0.025f,  0.0125f );
      glVertex3f( -0.05f, -0.025f, -0.05f );
     glEnd();
     glPopMatrix();

     glColor3f( 1.0, 0.84f, 0.0 );  // zolty walec
     glTranslatef( 0.0, 0.00f, 0.00625f );
     glutWireCylinder(0.00625,0.0125, 50,1);

     glColor3f( 0.0, 0.0, 1.0 );
     glTranslatef( 0.0, 0.0, 0.0125f );
     glutWireCylinder(0.01,0.00625, 100,1);

     glTranslatef( 0.0, 0.0, 0.00625f );
     glutWireCylinder(0.025,0.00625, 150,1);
     glTranslatef( 0.0, 0.0, 0.00625f );

    float theta;
     glBegin(GL_POLYGON);
     for(int i=0; i<360; i++){

         theta = i*3.142f/180.0f;
         glVertex3f( 0.025f* cos(theta), 0.025f*sin(theta), 0.0f );
     }
     glEnd();



}


/*!
 * Tworzy model efektora - czesc obracajaca
 * przeskalowania wzgledem poszczegolnego osi układu spolrzednych
 * \param[in] Size_X - Wartość przeskalowania w kierunku osi x
 * \param[in] Size_Y - Wartość przeskalowania w kierunku osi y
 * \param[in] Size_Z - Wartość przeskalowania w kierunku osi z
 */
void GLCreateEfector( float Size_X,  float Size_Y,  float Size_Z ){

    glPushMatrix();
    glScalef( Size_X, Size_Y, Size_Z );

    glBegin(GL_POLYGON);
     glColor3f( 0.853f, 0.853f, 0.853f );
     glVertex3f(  0.0375f, -0.03125f, -0.03125f );
     glVertex3f(  0.0375f,  0.03125f, -0.03125f );
     glVertex3f( -0.0f,  0.03125f, -0.03125f );
     glVertex3f( -0.0f, -0.03125f, -0.03125f );
    glEnd();


    glBegin(GL_POLYGON);

     glVertex3f(  0.0375f, -0.03125f, 0.03125f );
     glVertex3f(  0.0375f,  0.03125f, 0.03125f );
     glVertex3f( -0.0f,  0.03125f, 0.03125f );
     glVertex3f( -0.0f, -0.03125f, 0.03125f );
    glEnd();

   // Purple side - RIGHT
    glBegin(GL_POLYGON);

     glVertex3f( 0.0375f, -0.03125f, -0.03125f );
     glVertex3f( 0.0375f,  0.03125f, -0.03125f );
     glVertex3f( 0.0375f,  0.03125f,  0.03125f );
     glVertex3f( 0.0375f, -0.03125f,  0.03125f );
    glEnd();

   // Green side - LEFT
    glBegin(GL_POLYGON);

     glVertex3f( -0.0f, -0.03125f,  0.03125f );
     glVertex3f( -0.0f,  0.03125f,  0.03125f );
     glVertex3f( -0.0f,  0.03125f, -0.03125f );
     glVertex3f( -0.0f, -0.03125f, -0.03125f );
    glEnd();

     // Blue side - TOP
    glBegin(GL_POLYGON);

      glVertex3f(  0.0375f,  0.03125f,  0.03125f );
      glVertex3f(  0.0375f,  0.03125f, -0.03125f );
      glVertex3f( -0.0f,  0.03125f, -0.03125f );
      glVertex3f( -0.0f,  0.03125f,  0.03125f );
     glEnd();

   // Red side - BOTTOM
     glBegin(GL_POLYGON);
      glVertex3f(  0.0375f, -0.03125f, -0.03125f );
      glVertex3f(  0.0375f, -0.03125f,  0.03125f );
      glVertex3f( -0.0f, -0.03125f,  0.03125f );
      glVertex3f( -0.0f, -0.03125f, -0.03125f );
     glEnd();

     // dodatek pierwszy

     glBegin(GL_POLYGON);
      glColor3f( 0.853f, 0.853f, 0.853f );
      glVertex3f(  0.0375f, 0.0125f, -0.03125f );
      glVertex3f(  0.0375f,  0.03125f, -0.03125f );
      glVertex3f( 0.0750f,  0.03125f, -0.03125f );
      glVertex3f( 0.0750f, 0.0125f, -0.03125f );
     glEnd();


     glBegin(GL_POLYGON);

      glVertex3f(  0.0375f, 0.0125f, 0.03125f );
      glVertex3f(  0.0375f,  0.03125f, 0.03125f );
      glVertex3f( 0.0750f,  0.03125f, 0.03125f );
      glVertex3f( 0.0750f, 0.0125f, 0.03125f );
     glEnd();

    // Purple side - RIGHT
     glBegin(GL_POLYGON);

      glVertex3f( 0.0375f, 0.0125f, -0.03125f );
      glVertex3f( 0.0375f,  0.03125f, -0.03125f );
      glVertex3f( 0.0375f,  0.03125f,  0.03125f );
      glVertex3f( 0.0375f, 0.0125f,  0.03125f );
     glEnd();

    // Green side - LEFT
     glBegin(GL_POLYGON);

      glVertex3f( 0.0750f, 0.0125f,  0.03125f );
      glVertex3f( 0.0750f,  0.03125f,  0.03125f );
      glVertex3f( 0.0750f,  0.03125f, -0.03125f );
      glVertex3f( 0.0750f, 0.0125f, -0.03125f );
     glEnd();

      // Blue side - TOP
     glBegin(GL_POLYGON);

       glVertex3f(  0.0375f,  0.03125f,  0.03125f );
       glVertex3f(  0.0375f,  0.03125f, -0.03125f );
       glVertex3f( 0.0750f,  0.03125f, -0.03125f );
       glVertex3f( 0.0750f,  0.03125f,  0.03125f );
      glEnd();

    // Red side - BOTTOM
      glBegin(GL_POLYGON);
       glVertex3f(  0.0375f, 0.0125f, -0.03125f );
       glVertex3f(  0.0375f, 0.0125f,  0.03125f );
       glVertex3f( 0.0750f, 0.0125f,  0.03125f );
       glVertex3f( 0.0750f, 0.0125f, -0.03125f );
      glEnd();



    // dodatek drugi


      glBegin(GL_POLYGON);
       glVertex3f(  0.0375f, -0.0125f, -0.03125f );
       glVertex3f(  0.0375f,  -0.03125f, -0.03125f );
       glVertex3f( 0.0750f,  -0.03125f, -0.03125f );
       glVertex3f( 0.0750f, -0.0125f, -0.03125f );
      glEnd();


      glBegin(GL_POLYGON);

       glVertex3f(  0.0375f, -0.0125f, 0.03125f );
       glVertex3f(  0.0375f,  -0.03125f, 0.03125f );
       glVertex3f( 0.0750f,  -0.03125f, 0.03125f );
       glVertex3f( 0.0750f, -0.0125f, 0.03125f );
      glEnd();

     // Purple side - RIGHT
      glBegin(GL_POLYGON);

       glVertex3f( 0.0375f, -0.0125f, -0.03125f );
       glVertex3f( 0.0375f,  -0.03125f, -0.03125f );
       glVertex3f( 0.0375f,  -0.03125f,  0.03125f );
       glVertex3f( 0.0375f, -0.0125f,  0.03125f );
      glEnd();

     // Green side - LEFT
      glBegin(GL_POLYGON);

       glVertex3f( 0.0750f, -0.0125f,  0.03125f );
       glVertex3f( 0.0750f,  -0.03125f,  0.03125f );
       glVertex3f( 0.0750f,  -0.03125f, -0.03125f );
       glVertex3f( 0.0750f, -0.0125f, -0.03125f );
      glEnd();

       // Blue side - TOP
      glBegin(GL_POLYGON);

        glVertex3f(  0.0375f,  -0.03125f,  0.03125f );
        glVertex3f(  0.0375f,  -0.03125f, -0.03125f );
        glVertex3f( 0.0750f,  -0.03125f, -0.03125f );
        glVertex3f( 0.0750f,  -0.03125f,  0.03125f );
       glEnd();

     // Red side - BOTTOM
       glBegin(GL_POLYGON);
        glVertex3f(  0.0375f, -0.0125f, -0.03125f );
        glVertex3f(  0.0375f, -0.0125f,  0.03125f );
        glVertex3f( 0.0750f, -0.0125f,  0.03125f );
        glVertex3f( 0.0750f, -0.0125f, -0.03125f );
       glEnd();



     glPopMatrix();
}


/*!
 * Tworzy model efektora - czesc chwytajaca
 * przeskalowania wzgledem poszczegolnego osi układu spolrzednych
 * \param[in] Size_X - Wartość przeskalowania w kierunku osi x
 * \param[in] Size_Y - Wartość przeskalowania w kierunku osi y
 * \param[in] Size_Z - Wartość przeskalowania w kierunku osi z
 */
void GLCreateEfector_v2( float Size_X,  float Size_Y,  float Size_Z ){


    glPushMatrix();
    glScalef( Size_X, Size_Y, Size_Z );

    glBegin(GL_POLYGON);
     glColor3f( 0.0f, 0.0f, 0.0f );
     glVertex3f(  0.0625f, -0.01f, -0.01f );
     glVertex3f(  0.0625f,  0.01f, -0.01f );
     glVertex3f( -0.0375f,  0.01f, -0.01f );
     glVertex3f( -0.0375f, -0.01f, -0.01f );
    glEnd();


    glBegin(GL_POLYGON);
     glVertex3f(  0.0625f, -0.01f, 0.01f );
     glVertex3f(  0.0625f,  0.01f, 0.01f );
     glVertex3f( -0.0375f,  0.01f, 0.01f );
     glVertex3f( -0.0375f, -0.01f, 0.01f );
    glEnd();

   // Purple side - RIGHT
    glBegin(GL_POLYGON);
     glVertex3f( 0.0625f, -0.01f, -0.01f );
     glVertex3f( 0.0625f,  0.01f, -0.01f );
     glVertex3f( 0.0625f,  0.01f,  0.01f );
     glVertex3f( 0.0625f, -0.01f,  0.01f );
    glEnd();

   // Green side - LEFT
    glBegin(GL_POLYGON);
     glVertex3f( -0.0375f, -0.01f,  0.01f );
     glVertex3f( -0.0375f,  0.01f,  0.01f );
     glVertex3f( -0.0375f,  0.01f, -0.01f );
     glVertex3f( -0.0375f, -0.01f, -0.01f );
    glEnd();

     // Blue side - TOP
    glBegin(GL_POLYGON);
      glVertex3f(  0.0625f,  0.01f,  0.01f );
      glVertex3f(  0.0625f,  0.01f, -0.01f );
      glVertex3f( -0.0375f,  0.01f, -0.01f );
      glVertex3f( -0.0375f,  0.01f,  0.01f );
     glEnd();

   // Red side - BOTTOM
     glBegin(GL_POLYGON);
      glVertex3f(  0.0625f, -0.01f, -0.01f );
      glVertex3f(  0.0625f, -0.01f,  0.01f );
      glVertex3f( -0.0375f, -0.01f,  0.01f );
      glVertex3f( -0.0375f, -0.01f, -0.01f );
     glEnd();
     glPopMatrix();
}



/*!
 * Tworzy model mocowania - w ksztalcie litery U
 * przeskalowania wzgledem poszczegolnego osi układu spolrzednych
 * \param[in] Size_X - Wartość przeskalowania w kierunku osi x
 * \param[in] Size_Y - Wartość przeskalowania w kierunku osi y
 * \param[in] Size_Z - Wartość przeskalowania w kierunku osi z
 */
void GLCreate_U_model( float Size_X,  float Size_Y,  float Size_Z )
{
   // glRotatef( -90, 1.0, 0.0, 0.0 );
    glPushMatrix();
    glScalef( Size_X, Size_Y, Size_Z );


   // podstawa
    glBegin(GL_POLYGON);
     glColor3f( 0.853f, 0.853f, 0.853f );   // 0.753
     glVertex3f(  0.03125f, -0.0025f, -0.06875f );
     glVertex3f(  0.03125f,  0.0025f, -0.06875f );
     glVertex3f( -0.03125f,  0.0025f, -0.06875f );
     glVertex3f( -0.03125f, -0.0025f, -0.06875f );
    glEnd();


    glBegin(GL_POLYGON);

     glVertex3f(  0.03125f, -0.0025f, 0.06875f );
     glVertex3f(  0.03125f,  0.0025f, 0.06875f );
     glVertex3f( -0.03125f,  0.0025f, 0.06875f );
     glVertex3f( -0.03125f, -0.0025f, 0.06875f );
    glEnd();

   //  side - RIGHT
    glBegin(GL_POLYGON);

     glVertex3f( 0.03125f, -0.0025f, -0.06875f );
     glVertex3f( 0.03125f,  0.0025f, -0.06875f );
     glVertex3f( 0.03125f,  0.0025f,  0.06875f );
     glVertex3f( 0.03125f, -0.0025f,  0.06875f );
    glEnd();

   // side - LEFT
    glBegin(GL_POLYGON);

     glVertex3f( -0.03125f, -0.0025f,  0.06875f );
     glVertex3f( -0.03125f,  0.0025f,  0.06875f );
     glVertex3f( -0.03125f,  0.0025f, -0.06875f );
     glVertex3f( -0.03125f, -0.0025f, -0.06875f );
    glEnd();

     //side - TOP
    glBegin(GL_POLYGON);

      glVertex3f(  0.03125f,  0.0025f,  0.06875f );
      glVertex3f(  0.03125f,  0.0025f, -0.06875f );
      glVertex3f( -0.03125f,  0.0025f, -0.06875f );
      glVertex3f( -0.03125f,  0.0025f,  0.06875f );
     glEnd();

   // side - BOTTOM
     glBegin(GL_POLYGON);

      glVertex3f(  0.03125f, -0.0025f, -0.06875f );
      glVertex3f(  0.03125f, -0.0025f,  0.06875f );
      glVertex3f( -0.03125f, -0.0025f,  0.06875f );
      glVertex3f( -0.03125f, -0.0025f, -0.06875f );
     glEnd();

   // lewe ramie  uchwytu


     glBegin(GL_POLYGON);

      glVertex3f(  0.03125f, -0.0f,    -0.06875f );
      glVertex3f(  0.03125f,  0.1375f, -0.06875f );
      glVertex3f( -0.03125f,  0.1375f, -0.06875f );
      glVertex3f( -0.03125f, -0.0f,    -0.06875f );
     glEnd();


     glBegin(GL_POLYGON);

      glVertex3f(  0.03125f, -0.0f, -0.06375f );
      glVertex3f(  0.03125f,  0.1375f, -0.06375f );
      glVertex3f( -0.03125f,  0.1375f, -0.06375f );
      glVertex3f( -0.03125f, -0.0f, -0.06375f );
     glEnd();


     //  side - RIGHT
      glBegin(GL_POLYGON);

       glVertex3f( 0.03125f, -0.0f, -0.06875f );
       glVertex3f( 0.03125f,  0.1375f, -0.06875f );
       glVertex3f( 0.03125f,  0.1375f,  -0.06375f );
       glVertex3f( 0.03125f, -0.0f,  -0.06375f );
      glEnd();

     // side - LEFT
      glBegin(GL_POLYGON);

       glVertex3f( -0.03125f, -0.0f,  -0.06375f );
       glVertex3f( -0.03125f,  0.1375f,  -0.06375f );
       glVertex3f( -0.03125f,  0.1375f, -0.06875f );
       glVertex3f( -0.03125f, -0.0f, -0.06875f );
      glEnd();

      //side - TOP
     glBegin(GL_POLYGON);

       glVertex3f(  0.03125f,  0.1375f,  -0.06375f );
       glVertex3f(  0.03125f,  0.1375f, -0.06875f );
       glVertex3f( -0.03125f,  0.1375f, -0.06875f );
       glVertex3f( -0.03125f,  0.1375f,  -0.06375f );
      glEnd();

    // side - BOTTOM
      glBegin(GL_POLYGON);

       glVertex3f(  0.03125f, -0.0f, -0.06875f );
       glVertex3f(  0.03125f, -0.0f,  -0.06375f );
       glVertex3f( -0.03125f, -0.0f,  -0.06375f );
       glVertex3f( -0.03125f, -0.0f, -0.06875f );
    glEnd();

// prawe ramie uchwytu

    glBegin(GL_POLYGON);

     glVertex3f(  0.03125f, -0.0f,    0.06875f );
     glVertex3f(  0.03125f,  0.1375f, 0.06875f );
     glVertex3f( -0.03125f,  0.1375f, 0.06875f );
     glVertex3f( -0.03125f, -0.0f,    0.06875f );
    glEnd();


    glBegin(GL_POLYGON);

     glVertex3f(  0.03125f, -0.0f, 0.06375f );
     glVertex3f(  0.03125f,  0.1375f, 0.06375f );
     glVertex3f( -0.03125f,  0.1375f, 0.06375f );
     glVertex3f( -0.03125f, -0.0f, 0.06375f );
    glEnd();


    //  side - RIGHT
     glBegin(GL_POLYGON);

      glVertex3f( 0.03125f, -0.0f, 0.06875f );
      glVertex3f( 0.03125f,  0.1375f, 0.06875f );
      glVertex3f( 0.03125f,  0.1375f,  0.06375f );
      glVertex3f( 0.03125f, -0.0f,  0.06375f );
     glEnd();

    // side - LEFT
     glBegin(GL_POLYGON);

      glVertex3f( -0.03125f, -0.0f,  0.06375f );
      glVertex3f( -0.03125f,  0.1375f,  0.06375f );
      glVertex3f( -0.03125f,  0.1375f, 0.06875f );
      glVertex3f( -0.03125f, -0.0f, 0.06875f );
     glEnd();

     //side - TOP
    glBegin(GL_POLYGON);

      glVertex3f(  0.03125f,  0.1375f,  0.06375f );
      glVertex3f(  0.03125f,  0.1375f, 0.06875f );
      glVertex3f( -0.03125f,  0.1375f, 0.06875f );
      glVertex3f( -0.03125f,  0.1375f,  0.06375f );
     glEnd();

   // side - BOTTOM
     glBegin(GL_POLYGON);

      glVertex3f(  0.03125f, -0.0f, 0.06875f );
      glVertex3f(  0.03125f, -0.0f,  0.06375f );
      glVertex3f( -0.03125f, -0.0f,  0.06375f );
      glVertex3f( -0.03125f, -0.0f, 0.06875f );
   glEnd();

//

    // walec


     glTranslatef( 0.0, 0.1375f, 0.06375f );
     glutWireCylinder(0.03125,0.005, 150,1);

     float theta;

     glBegin(GL_POLYGON);
     for(int i=0; i<360; i++){

         theta = i*3.142f/180.0f;
         glVertex3f( 0.03121f* cos(theta), 0.03121f*sin(theta), 0.0f );
     }
     glEnd();
    glTranslatef( 0.0, 0.0, 0.005f );

     glBegin(GL_POLYGON);
     for(int i=0; i<360; i++){

         theta = i*3.142f/180.0f;
         glVertex3f( 0.03121f* cos(theta), 0.03121f*sin(theta), 0.0f );
     }
     glEnd();

     //*************************************************************

     glPushMatrix();

     glColor3f( 1.0, 0.84f, 0.0 );  // zolty walec
      glTranslatef( 0.0, 0.0f, 0.001f );
     glBegin(GL_POLYGON);
     for(int i=0; i<360; i++){

         theta = i*3.142f/180.0f;
         glVertex3f( 0.02f* cos(theta), 0.02f*sin(theta), 0.0f );
     }
     glEnd();
     glutWireCylinder(0.0125,0.005, 150,1);
     glTranslatef( 0.0, 0.0f, 0.005f );
      glColor3f( 0.5f, 0.5f, 0.5f );
     glutWireSphere(0.0125,200,200);

     glPopMatrix();

     glColor3f( 0.853f, 0.853f, 0.853f );   // 0.753
     glTranslatef( 0.0, 0.0f, -0.06875f ); // powrot na srodek

     glTranslatef( 0.0, 0.0f, -0.06875f );
     glutWireCylinder(0.03125,0.005, 150,1);

     glBegin(GL_POLYGON);
     for(int i=0; i<360; i++){

         theta = i*3.142f/180.0f;
         glVertex3f( 0.03121f* cos(theta), 0.03121f*sin(theta), 0.0f );
     }
     glEnd();

    glTranslatef( 0.0, 0.0, 0.005f );
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i++){

        theta = i*3.142f/180.0f;
        glVertex3f( 0.03121f* cos(theta), 0.03121f*sin(theta), 0.0f );
    }
    glEnd();





    glPushMatrix();
    glTranslatef( 0.0, 0.0f, -0.01f );
    glColor3f( 1.0, 0.84f, 0.0 );  // zolty walec
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i++){

        theta = i*3.142f/180.0f;
        glVertex3f( 0.02f* cos(theta), 0.02f*sin(theta), 0.0f );
    }
    glEnd();
     glutWireCylinder(0.02,0.01, 150,1);




    glPopMatrix();





    glTranslatef( 0.0, 0.0f, 0.06375f );   // powrót na srodek







     glPopMatrix();


}



/*!
 * Tworzy model mocowania - przedluzka
 * przeskalowania wzgledem poszczegolnego osi układu spolrzednych
 * \param[in] Size_X - Wartość przeskalowania w kierunku osi x
 * \param[in] Size_Y - Wartość przeskalowania w kierunku osi y
 * \param[in] Size_Z - Wartość przeskalowania w kierunku osi z
 */
void GLCreate_I_model( float Size_X,  float Size_Y,  float Size_Z ){


    glPushMatrix();
    glScalef( Size_X, Size_Y, Size_Z );


   // podstawa
    glBegin(GL_POLYGON);
     glColor3f( 0.853f, 0.853f, 0.853f );   // 0.753
     glVertex3f(  0.1375f, -0.005f, -0.0375f );
     glVertex3f(  0.1375f,  0.005f, -0.0375f );
     glVertex3f( -0.1375f,  0.005f, -0.0375f );
     glVertex3f( -0.1375f, -0.005f, -0.0375f );
    glEnd();


    glBegin(GL_POLYGON);

     glVertex3f(  0.1375f, -0.005f, 0.0375f );
     glVertex3f(  0.1375f,  0.005f, 0.0375f );
     glVertex3f( -0.1375f,  0.005f, 0.0375f );
     glVertex3f( -0.1375f, -0.005f, 0.0375f );
    glEnd();

   //  side - RIGHT
    glBegin(GL_POLYGON);

     glVertex3f( 0.1375f, -0.005f, -0.0375f );
     glVertex3f( 0.1375f,  0.005f, -0.0375f );
     glVertex3f( 0.1375f,  0.005f,  0.0375f );
     glVertex3f( 0.1375f, -0.005f,  0.0375f );
    glEnd();

   // side - LEFT
    glBegin(GL_POLYGON);

     glVertex3f( -0.1375f, -0.005f,  0.0375f );
     glVertex3f( -0.1375f,  0.005f,  0.0375f );
     glVertex3f( -0.1375f,  0.005f, -0.0375f );
     glVertex3f( -0.1375f, -0.005f, -0.0375f );
    glEnd();

     //side - TOP
    glBegin(GL_POLYGON);

      glVertex3f(  0.1375f,  0.005f,  0.0375f );
      glVertex3f(  0.1375f,  0.005f, -0.0375f );
      glVertex3f( -0.1375f,  0.005f, -0.0375f );
      glVertex3f( -0.1375f,  0.005f,  0.0375f );
     glEnd();

   // side - BOTTOM
     glBegin(GL_POLYGON);

      glVertex3f(  0.1375f, -0.005f, -0.0375f );
      glVertex3f(  0.1375f, -0.005f,  0.0375f );
      glVertex3f( -0.1375f, -0.005f,  0.0375f );
      glVertex3f( -0.1375f, -0.005f, -0.0375f );
     glEnd();

     glPopMatrix();
}


/*!
 * Inicjalizuje openGl
 */
void MyOpenGLWidget::initializeGL(){
    float r, g, b, a = normaliza_0_1(255.0f, RGB_MIN, RGB_MAX);
    initializeOpenGLFunctions();
    qColorToRGB(Qt::darkCyan, r, g, b);
    glClearColor(r, g, b, a);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_COLOR_MATERIAL);


/*

  initializeOpenGLFunctions();
    glClearColor(static_cast<GLclampf>(0.2),
                static_cast<GLclampf>(0.2),
                static_cast<GLclampf>(0.2),
    1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
*/

}


/*!
 * Dokonuje odrysowania manipulatora
 */
void MyOpenGLWidget::paintGL(){

 //  float r,g,b;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.0,1.0,5,0,0,-100,0,1,0);
 //   glRotatef(20, 1, 0,0);
//    glRotated(0,0,0,0);
//    glRotatef(0,0,0,0);

 //   glBegin(GL_LINES);      // Dodanie przykładowej zielonej linii do projektu
  //  qColorToRGB(Qt::green, r, g, b);
 //   glColor3f(r, g, b);
  //  glVertex3f(0.0f, 1.0f, 0.0f);
  //  glVertex3f(1.0f, 0.0f, 0.0f);
  //  glEnd();
  //  glutWireSphere(1,20,20);

    glScalef( 5.0, 5.0, 5.0 );

//  glRotatef( GetQ0_deg(), 1.0, 0.0, 0.0 );
 // glRotatef( GetQ1_deg(), 0.0, 1.0, 0.0 );
//  glRotatef( GetQ2_deg(), 0.0, 0.0, 1.0 );


  GLCreateServo(1.0,1.0,1.0);

    glRotatef( 90, 1.0, 0.0, 0.0 );
    glRotatef( GetQ0_deg(), 0.0, 1.0, 0.0 );
    GLCreate_U_model(1.0, 1.0, 1.0);   // mamy w tym momencie serwo a na nim kontownik

    glRotatef( 90, 1.0, 0.0, 0.0 );
    glTranslatef( -0.0f, 0.0, -0.1375f );
    glRotatef( -90, 0.0, 1.0, 0.0 );


    glRotatef( GetQ1_deg(), 0.0, 1.0, 0.0 );
    glTranslatef( 0.0f, 0.0, 0.025f );
    GLCreateServo(1.0,1.0,1.0);    // mam 2 serwo dodane



    glRotatef( 90, 0.0, 0.0, 1.0 );
    glTranslatef( 0.0625f, -0.03f, -0.075f );

    GLCreate_I_model(1.0, 1.0, 1.0);   // mamy łacznik miedzy 2 a 3 servem

    glRotatef( 90, 1.0, 0.0, 0.0 );
    glRotatef( -90, 0.0, 1.0, 0.0 );
    glTranslatef( -0.03f, 0.0125f, -0.0875f );
    GLCreateServo(1.0,1.0,1.0);     // mamy 3 serwa :D


    glRotatef( 180, 0.0, 0.0, 1.0 );
    glTranslatef( 0.0f, 0.0, -0.0625f );


    glRotatef( GetQ2_deg(), 0.0, 0.0, 1.0 );
    glTranslatef( 0.0f, -0.1375f, 0.0f );
    GLCreate_U_model(1.0, 1.0, 1.0);   // 3 serwa i ramie U



    glRotatef( 90, 0.0, 1.0, 0.0 );
    glRotatef( 180, 0.0, 0.0, 1.0 );
    GLCreateServoSmall(1.0,1.0,1.0);


    glRotatef( -90, 0.0, 1.0, 0.0 );
    glRotatef( GetQ3_deg(), 1.0, 0.0, 0.0 );
    GLCreateEfector(1.0,1.0,1.0);    // jest mocowanie pod efektor

     glTranslatef( 0.050f, 0.0f, 0.015f );

     glPushMatrix();
      glRotatef( GetQ4_deg()-30, 0.0, 1.0, 0.0 );
    GLCreateEfector_v2(1.0,1.0,1.0);
    glPopMatrix();

    glTranslatef( 0.0f, 0.0f, -0.030f );
    glRotatef( -GetQ4_deg()+30, 0.0, 1.0, 0.0 );
    GLCreateEfector_v2(1.0,1.0,1.0);  // jest wszystko

      //  qColorToRGB(Qt::green, r, g, b);
   //   glColor3f(r, g, b);



}


/*!
 * Funkcja przeskalowywuje scene w momencie zmiany jej rozmiaru
 *
 * \param[in] w - szerokosc okna
 * \param[in] h - wysokosc okna
 */
void MyOpenGLWidget::resizeGL(int w, int h){


   /*
    glViewport(0, 0, w, h );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
*/

  /*  glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);

    qreal aspect = qreal(w) / qreal(h ? h:1);

    const qreal  zNear = 3.0, zFar = 7.0, fov = 45.0;

    glLoadIdentity();

  //  gluPerspective(fov, aspect, zNear, zFar);
*/


    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-0.5,0,4,0,0,-100,0,1,0);



  /*  float ratio = 1.0* w / h;
            //use projection mode
            glMatrixMode(GL_PROJECTION);
            // reset matrix

            glLoadIdentity();
            // Set the viewport to be the entire window
            glViewport(0,0,w,h);
            // Set the correct perspective.
            //gluPerspective(45,ratio,1,100);

            gluPerspective(45.0f,ratio,0.5f,100.0f);
            //glOrtho(-40/3.0f,40/3.0f,-10,10,-10,10);
            // Get Back to the Modelview
            glMatrixMode(GL_MODELVIEW);

*/


}

void MyOpenGLWidget::qColorToRGB(const QColor &C, float &r, float &g, float &b) const
{
   r = normaliza_0_1(C.red(),  RGB_MIN, RGB_MAX);
   g = normaliza_0_1(C.green(),  RGB_MIN, RGB_MAX);
   b = normaliza_0_1(C.blue(),  RGB_MIN, RGB_MAX);
}

float MyOpenGLWidget::normaliza_0_1(float val, float min, float max) const
{
    return (val-min)/(max-min);
};
