#pragma once
#include "data.h"
#include <iostream>
#include <QInputDialog>
#include <QMessageBox>

class View : public QGLWidget, protected QGLFunctions
{
    Data data;
    GLuint VBOtexture;
    QImage textureImage;
    int vWidth, vHeight;
    enum vis { kVisualisationQuads, kVisualisationQuadStrip, kVisualisationTexture } visualisation_state;
    enum axis { x, y, z } axis_cut;
    int layer;
public:
    View::View(QWidget* parent) : QGLWidget(parent), data(), layer(0), visualisation_state(kVisualisationQuads), axis_cut(x) 
    {
        setMinimumSize(800, 500);
    }

    void initializeGL()
    {
        glGenTextures(1, &VBOtexture);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        // устанавливаем заполняющий цвет
        qglClearColor(Qt::black);
        // устанавливаем режим сглаживания
        glShadeModel(GL_SMOOTH);
        // задаем модельно-видовую матрицу
        glMatrixMode(GL_MODELVIEW);
        // загрузка единичной матрицы
        glLoadIdentity();
    }

    void resizeGL(int nWidth, int nHeight)
    {
        vWidth = nWidth;
        vHeight = nHeight;
        // установка режима матрицы
        glMatrixMode(GL_PROJECTION);
        // загрузка единичной матрицы
        glLoadIdentity();
        // установка ортогонального преобразования
        glOrtho(0.0f, data.getWidth() - 1, 0.0f, data.getHeight() - 1, -1.0f, 1.0f);
        // установка окна обзора 
        glViewport(10, 10, nWidth-20, nHeight-20);
        update();
    }

    void VisualizationQuads()
    {
        QColor c;
        int w = data.getWidth();
        int h = data.getHeight();
        int d = data.getDepth();

        switch (axis_cut)
        {
        case x:
            for (int y = 0; y < h - 1; y++)
            {
                for (int x = 0; x < w - 1; x++)
                {
                    glBegin(GL_QUADS);
                    c = TransferFunction(data[layer * w * h + y * w + x]);
                    qglColor(c);
                    glVertex2i(x, y);

                    c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
                    qglColor(c);
                    glVertex2i(x, y + 1);

                    c = TransferFunction(data[layer * w * h + (y + 1) * w + x + 1]);
                    qglColor(c);
                    glVertex2i(x + 1, y + 1);

                    c = TransferFunction(data[layer * w * h + y * w + x + 1]);
                    qglColor(c);
                    glVertex2i(x + 1, y);
                    glEnd();
                }
            }
            break;

        case y:
            for (int y = 0; y < h - 1; y++)
            {
                for (int z = 0; z < d - 1; z++)
                {
                    glBegin(GL_QUADS);
                    c = TransferFunction(data[w * h * z + y * w + layer]);
                    qglColor(c);
                    glVertex2i(z, y);

                    c = TransferFunction(data[w * h * z + (y + 1) * w + layer]);
                    qglColor(c);
                    glVertex2i(z, y + 1);

                    c = TransferFunction(data[w * h * (z + 1) + (y + 1) * w + layer]);
                    qglColor(c);
                    glVertex2i(z + 1, y + 1);

                    c = TransferFunction(data[w * h * (z + 1) + y * w + layer]);
                    qglColor(c);
                    glVertex2i(z + 1, y);
                    glEnd();
                }
            }
            break;

        case z:
            for (int z = 0; z < d - 1; z++)
            {
                for (int x = 0; x < w - 1; x++)
                {
                    glBegin(GL_QUADS);
                    c = TransferFunction(data[x + z * w * h + layer * w]);
                    qglColor(c);
                    glVertex2i(x, z);

                    c = TransferFunction(data[x + (z + 1) * w * h + layer * w]);
                    qglColor(c);
                    glVertex2i(x, z + 1);

                    c = TransferFunction(data[(x + 1) + (z + 1) * w * h + layer * w]);
                    qglColor(c);
                    glVertex2i(x + 1, z + 1);

                    c = TransferFunction(data[(x + 1) + z * w * h + layer * w]);
                    qglColor(c);
                    glVertex2i(x + 1, z);
                    glEnd();
                }
            }
            break;
        }
    }

    void VizualizationTexture()
    {
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2i(0, 0);
        glTexCoord2f(0, 1);
        glVertex2i(0, data.getHeight());
        glTexCoord2f(1, 1);
        glVertex2i(data.getWidth(), data.getHeight());
        glTexCoord2f(1, 0);
        glVertex2i(data.getWidth(), 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    void VisualizationQuadStrip()
    {
        QColor c;
        int w = data.getWidth();
        int h = data.getHeight();
        int d = data.getDepth();

        switch (axis_cut)
        {
        case x:
            for (int y = 0; y < h - 1; y++)
            {
                glBegin(GL_QUAD_STRIP);
                for (int x = 0; x < w; x++)
                {
                    c = TransferFunction(data[layer * w * h + y * w + x]);
                    qglColor(c);
                    glVertex2i(x, y);

                    c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
                    qglColor(c);
                    glVertex2i(x, y + 1);
                }
                glEnd();
            }
            break;

        case y:
            for (int y = 0; y < h - 1; y++)
            {
                glBegin(GL_QUAD_STRIP);
                for (int z = 0; z < d; z++)
                {
                    c = TransferFunction(data[w * h * z + y * w + layer]);
                    qglColor(c);
                    glVertex2i(z, y);

                    c = TransferFunction(data[w * h * z + (y + 1) * w + layer]);
                    qglColor(c);
                    glVertex2i(z, y + 1);
                }
                glEnd();
            }
            break;

        case z:
            for (int z = 0; z < d - 1; z++)
            {
                glBegin(GL_QUAD_STRIP);
                for (int x = 0; x < w; x++)
                {
                    c = TransferFunction(data[x + z * w * h + layer * w]);
                    qglColor(c);
                    glVertex2i(x, z);

                    c = TransferFunction(data[x + (z + 1) * w * h + layer * w]);
                    qglColor(c);
                    glVertex2i(x, z + 1);
                }
                glEnd();
            }
            break;
        }
    }

    void paintGL()
    {
        qDebug() << "p: " << visualisation_state;
        //очистка экрана
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        switch (visualisation_state)
        {
        case kVisualisationQuads:
            VisualizationQuads();
            break;
        case kVisualisationQuadStrip:
            VisualizationQuadStrip();
            break;
        case kVisualisationTexture:
            VizualizationTexture();
            break;
        }
        //qDebug() << "MIN: " << data.GetMin();
        //qDebug() << "MAX: " << data.GetMax();
    }

    QColor TransferFunction(short value)
    {
        int c = (value - data.getMin()) * 255 / (data.getMax() - data.getMin());
        return QColor(c, c, c);
    }

    void keyPressEvent(QKeyEvent* event) 
    {
        /*if (event->nativeVirtualKey() == Qt::Key_U)
        {
            // Подняться на слой выше
            for (int i(0); i < data.getHeight(); ++i)
            {
                if (layer < data.getDepth() - 1)
                {
                    layer++;
                    Sleep(500);
                }
            }

        }

        else if (event->nativeVirtualKey() == Qt::Key_D)
        {
            // Опуститься на слой ниже
            if (layer != 0)
            {
                layer--;
            }
        }

        else if (event->nativeVirtualKey() == Qt::Key_N)
        {
            if (visualisation_state == kVisualisationQuads)
            {
                visualisation_state = kVisualisationQuadStrip;
            }

            else if (visualisation_state == kVisualisationQuadStrip)
            {
                visualisation_state = kVisualisationTexture;
            }

            else
            {
                visualisation_state = kVisualisationQuads;
            }

            layer = 0;
        }

        else if (event->nativeVirtualKey() == Qt::Key_W)
        {
            if (axis_cut == axis::x)
            {
                axis_cut = axis::y;
            }
            else if (axis_cut == axis::y)
            {
                axis_cut = axis::z;
            }
            else if (axis_cut == axis::z)
            {
                axis_cut = axis::x;
            }

            layer = 0;
        }

        else if (event->nativeVirtualKey() == Qt::Key_B)
        {
            std::string path = data.getPath();
            if (path == "data/Join.bin")
            {
                path = "data/FOURDIX-1.bin";
                data.clear();
                data.readFile(path);
            }
            else if (path == "data/FOURDIX-1.bin")
            {
                path = "data/FOURDIX-systolic.bin";
                data.clear();
                data.readFile(path);
            }
            else if (path == "data/FOURDIX-systolic.bin")
            {
                path = "data/p3-after2operation1.bin";
                data.clear();
                data.readFile(path);
            }
            else if (path == "data/p3-after2operation1.bin")
            {
                path = "data/torso1.bin";
                data.clear();
                data.readFile(path);
            }
            else if (path == "data/torso1.bin")
            {
                path = "data/Join.bin";
                data.clear();
                data.readFile(path);
            }

            //resizeGL(data.getWidth(), data.getHeight());
            resizeGL(600, 400);
        }
        */

        bool bOk;

        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        ke->key();
        auto ev = event->nativeVirtualKey();
        int num;
            switch (ev)
            {
            case Qt::Key_E:
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, QString::fromUtf8("Scale"),
                    QString::fromUtf8("Yes - Increase \nNo - Decrease"),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                if (reply == QMessageBox::Yes)
                {
                    glScalef(1.1, 1.1, 1.1);
                }
                else if (reply == QMessageBox::No)
                {
                    glScalef(0.9, 0.9, 0.9);
                }
                else return;
                break;
            case Qt::Key_A:
                num = QInputDialog::getInt(0, "Set max", "New max is:", 1, 1);
                if(bOk) //нажата ОК
                    data.SetMax(num);
                break;
            case Qt::Key_C:
                num = QInputDialog::getInt(0, "Set min", "New min is:", 1, 1);
                if (bOk)
                    data.SetMin(num);
                break;
            case Qt::Key_U:
                // Подняться на слой выше
                if (layer < data.getDepth() - 1)
                {
                    layer++;
                }
                break;

            case Qt::Key_D:
                // Опуститься на слой ниже
                if (layer != 0)
                {
                    layer--;
                }
                break;

            case Qt::Key_N:
                if (visualisation_state == kVisualisationQuads)
                {
                    visualisation_state = kVisualisationQuadStrip;
                }

                else if (visualisation_state == kVisualisationQuadStrip)
                {
                    visualisation_state = kVisualisationTexture;
                }

                else
                {
                    visualisation_state = kVisualisationQuads;
                }

                layer = 0;
                break;

            case Qt::Key_W:
                if (axis_cut == axis::x)
                {
                    axis_cut = axis::y;
                }
                else if (axis_cut == axis::y)
                {
                    axis_cut = axis::z;
                }
                else if (axis_cut == axis::z)
                {
                    axis_cut = axis::x;
                }

                layer = 0;
                break;

            case Qt::Key_B:
                std::string path = data.getPath();
                if (path == "data/Join.bin")
                {
                    path = "data/FOURDIX-1.bin";
                    data.clear();
                    data.readFile(path);
                }
                else if (path == "data/FOURDIX-1.bin")
                {
                    path = "data/FOURDIX-systolic.bin";
                    data.clear();
                    data.readFile(path);
                }
                else if (path == "data/FOURDIX-systolic.bin")
                {
                    path = "data/p3-after2operation1.bin";
                    data.clear();
                    data.readFile(path);
                }
                else if (path == "data/p3-after2operation1.bin")
                {
                    path = "data/torso1.bin";
                    data.clear();
                    data.readFile(path);
                }
                else if (path == "data/torso1.bin")
                {
                    path = "data/Join.bin";
                    data.clear();
                    data.readFile(path);
                }
                resizeGL(data.getWidth(), data.getHeight());
                //resizeGL(600, 400);
                break;
                }
            
        update();
    }

    void View::Load2DTexture()
    {
        glBindTexture(GL_TEXTURE_2D, VBOtexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(),
            0, GL_BGRA, GL_UNSIGNED_BYTE, textureImage.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    float View::TransferFunction1(short value)
    {
        return float(value - data.getMin()) / float(data.getMax() - data.getMin());
    }

    void genTextureImage()
    {
        float c;
        int w = data.getWidth();
        int h = data.getHeight();
        int d = data.getDepth();
        textureImage = QImage(w, h, QImage::Format_RGB32);
        qDebug() << "GEN_TEXTURE";

        switch (axis_cut)
        {
        case x:
            for (int y = 0; y < h; y++)
                for (int x = 0; x < w; x++)
                {
                    c = TransferFunction1(data[layer * w * h + w * y + x]) * 255;
                    if (c > 255)
                        c = 255;
                    else if (c < 0)
                        c = 0;
                    textureImage.setPixelColor(x, y, QColor(c, c, c));
                }
            break;

        case y:
            for (int y = 0; y < h; y++)
                for (int z = 0; z < d; z++)
                {
                    c = TransferFunction1(data[w * h * z + y * w + layer]) * 255;
                    if (c > 255)
                        c = 255;
                    else if (c < 0)
                        c = 0;
                    textureImage.setPixelColor(z, y, QColor(c, c, c));
                }
            break;
        case z:
            for (int z = 0; z < d; z++)
                for (int x = 0; x < w; x++)
                {
                    c = TransferFunction1(data[x + z * w * h + layer * w]) * 255;
                    if (c > 255)
                        c = 255;
                    else if (c < 0)
                        c = 0;
                    textureImage.setPixelColor(x, z, QColor(c, c, c));
                }
            break;
        }
    }

    void LoadData(std::string str)
    {
        data.readFile(str);
    }

    int getState() { return visualisation_state; }

    int getLayer() { return layer; }

    int* getDim() {
        int dims[3];
        dims[0] = data.getWidth();
        dims[1] = data.getHeight();
        dims[2] = data.getDepth();
        return dims;
    }

    short GetMin()
    {
        return data.getMax();
    }

    short GetMax()
    {
        return data.getMax();
    }
    
    int getWidth() { return vWidth; }
    int getHeight() { return vHeight; }
};


