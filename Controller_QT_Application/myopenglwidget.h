#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QColor>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <QtOpenGL>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QDebug>

#include <math.h>

/*!
 * \file
 * \brief Definicja metody MyOpenGLWidget
 *
 * Zawiera definicję klasy MyOpenGLWidger
 */

#define RGB_MIN 1
#define RGB_MAX 255


/*!
* \brief Modeluje okno dla grafiki OpenGL
*
* Klasa modeluje pojęcie okna dla grafiki manipulatora. Jest odpowiedzialna
* za wizualizację położenia manipulatora . Reaguje na sygnały i wywołuje
* odpowieni slot
*/
class MyOpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
    /*!
    * \brief Kat wychylenia przegubu
    *
    * Przechowuje w stopniach kat wychylenia danego przegubu
    */
    float  _Q4_deg;
    /*!
    * \brief Kat wychylenia przegubu
    *
    * Przechowuje w stopniach kat wychylenia danego przegubu
    */
    float  _Q3_deg;
    /*!
    * \brief Kat wychylenia przegubu
    *
    * Przechowuje w stopniach kat wychylenia danego przegubu
    */
    float  _Q2_deg;
    /*!
    * \brief Kat wychylenia przegubu
    *
    * Przechowuje w stopniach kat wychylenia danego przegubu
    */
    float  _Q1_deg;
    /*!
    * \brief Kat wychylenia przegubu
    *
    * Przechowuje w stopniach kat wychylenia danego przegubu
    */
    float  _Q0_deg;
public:
    MyOpenGLWidget(QWidget *parent = nullptr);


public slots:

    /*!
    * \brief Aktualizuje nowy kat wychylenia
    */
void SetQ0_deg(int Q0_deg);
/*!
* \brief Aktualizuje nowy kat wychylenia
*/
void SetQ1_deg(int Q1_deg);
/*!
* \brief Aktualizuje nowy kat wychylenia
*/
void SetQ2_deg(int Q2_deg);
/*!
* \brief Aktualizuje nowy kat wychylenia
*/
void SetQ3_deg(int Q3_deg);
/*!
* \brief Aktualizuje nowy kat wychylenia
*/
void SetQ4_deg(int Q4_deg);

/*!
* \brief Zwraca aktualny kat wychylenia danego przegubu
*/
 float GetQ0_deg() const { return _Q0_deg; }
 /*!
 * \brief Zwraca aktualny kat wychylenia danego przegubu
 */
 float GetQ1_deg() const { return _Q1_deg; }
 /*!
 * \brief Zwraca aktualny kat wychylenia danego przegubu
 */
 float GetQ2_deg() const { return _Q2_deg; }
 /*!
 * \brief Zwraca aktualny kat wychylenia danego przegubu
 */
 float GetQ3_deg() const { return _Q3_deg; }
 /*!
 * \brief Zwraca aktualny kat wychylenia danego przegubu
 */
 float GetQ4_deg() const { return _Q4_deg; }

protected:
 /*!
 * \brief Inicjalizacja Opengl
 */
    void initializeGL() override;

    /*!
    * \brief Modeluje wskazane ksztalty
    */
    void paintGL() override;

    /*!
    * \brief Dokonuje przeskalowania  w momencie zmiany rozmiaru okna
    */
    void resizeGL(int w, int h) override;
private:
    void qColorToRGB(const QColor &C, float &r, float &g, float &b) const;
    float normaliza_0_1(float val, float min, float max) const;

};

/*!
* \brief Modeluje szescian
*/
void GLCreateBox( float Size_X,  float Size_Y,  float Size_Z );

/*!
* \brief Modeluje mocowanie w kształcie U
*/
void GLCreate_U_model( float Size_X,  float Size_Y,  float Size_Z );

/*!
* \brief Modeluje przedluzenie mocowania
*/
void GLCreate_I_model( float Size_X,  float Size_Y,  float Size_Z );


/*!
* \brief Modeluje Serwomechanizm
*/
void GLCreateServo( float Size_X,  float Size_Y,  float Size_Z );

/*!
* \brief Modeluje Serwomechanizm
*/
void GLCreateServoSmall( float Size_X,  float Size_Y,  float Size_Z );

/*!
* \brief Modeluje czesc obrotowa efektora
*/
void GLCreateEfector( float Size_X,  float Size_Y,  float Size_Z );

/*!
* \brief Modeluje czesc chwytaka efektora
*/
void GLCreateEfector_v2( float Size_X,  float Size_Y,  float Size_Z );
#endif // MYOPENGLWIDGET_H
