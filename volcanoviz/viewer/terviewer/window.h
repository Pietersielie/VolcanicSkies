/*******************************************************************************
 *
 * EcoSynth - Data-driven Authoring of Large-Scale Ecosystems (Undergrowth simulator)
 * Copyright (C) 2020  J.E. Gain  (jgain@cs.uct.ac.za)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 ********************************************************************************/


/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include "glwidget.h"
#include <QWidget>
#include <QtWidgets>
#include <string>

class QAction;
class QMenu;
class QLineEdit;
class GLWidget;

class Window : public QMainWindow
{
    Q_OBJECT

public:

    Window(string datadir);

    ~Window(){};

    QSize sizeHint() const;

    /// Various getters for rendering and scene context
    View &getView() { return *perspectiveView->getView(); }
    Terrain &getTerrain() { return *perspectiveView->getTerrain(); }
    GLWidget * getGLWidget(){ return perspectiveView; }
    // BrushPalette * getPalette() { return perspectiveView->getPalette(); }

    /// Adjust rendering parameters, grid and contours, to accommodate current scale
    void scaleRenderParams(float scale);

    /// Main entry point for terrain viewer
    void run_viewer(int numframes, int start);

    /// get the ring radius
    float getSliderValue();

public slots:
    void repaintAllGL();

    // menu items
    void showRenderOptions();
    void showContours(int show);
    void showGridLines(int show);
    void showPalette(bool show);

    /// save and load painted type and slope maps
    void saveMaps();
    void loadMaps();

    // render panel
    void lineEditChange();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void optionsChanged();

private:
    GLWidget * perspectiveView; ///< Main OpenGL window
    QWidget * renderPanel;      ///< Side panel to adjust various rendering style parameters
    QWidget * palPanel;         ///< Side panel for painting palette

    // rendering parameters
    float gridSepX, numGridX, gridSepZ, numGridZ, gridWidth, gridIntensity; ///< grid params
    float contourSep, numContours, contourWidth, contourIntensity; ///< contour params
    float radianceTransition, radianceEnhance; ///< radiance scaling params

    // render panel widgets
    QLineEdit * gridSepXEdit, * gridSepZEdit, * gridWidthEdit, * gridIntensityEdit, * contourSepEdit, * contourWidthEdit, * contourIntensityEdit, * radianceEnhanceEdit;

    // palette panel widgets
    QSlider * innerslider;
    QLabel * innerlabel;

    // menu widgets and actions
    QMenu *viewMenu;
    QAction *showRenderAct, *saveAct, *openAct;

    // file management
    std::string scenedirname;

    float sliderscale;

    // For creating a slider widgets
   void addSlider(QVBoxLayout *layout, const QString &label, float defaultValue, int scale, float low, float high);

    // init menu
    void createActions();
    void createMenus();
};

#endif
