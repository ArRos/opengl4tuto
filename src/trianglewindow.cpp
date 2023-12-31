#include "trianglewindow.h"

#include <QMatrix4x4>
#include <QScreen>
#include <QtMath>
#include <QDebug>

void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/basic_vs.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/basic_fs.frag");

    if(!m_program->link())
    {
        qCritical() << m_program->log();
    }
    else
    {
        m_posAttr = m_program->attributeLocation("posAttr");
        Q_ASSERT(m_posAttr != -1);
        m_colAttr = m_program->attributeLocation("colAttr");
        Q_ASSERT(m_colAttr != -1);
        m_matrixUniform = m_program->uniformLocation("matrix");
        Q_ASSERT(m_matrixUniform != -1);
    }
}

void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.7f, 0.7f, 0.8f, 1.0f);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2.5);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    static const GLfloat vertices[] = {
        0.0f,  0.707f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.707f, 0.0f,
        0.5f, -0.5f, 0.0f,
        1.0f, 0.707f, 0.0f
    };

    static const GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f
    };

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_program->release();

    ++m_frame;
}
