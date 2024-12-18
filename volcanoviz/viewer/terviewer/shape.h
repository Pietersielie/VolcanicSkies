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


// shape.h: generate openGL buffer objects for different simple shapes
// author: James Gain
// date: 24 January 2014

#ifndef _shape_h
#define _shape_h

#include "glheaders.h"

#include "view.h"

struct ShapeDrawData
{
    GLuint VAO;             // vertex array object id
    GLfloat diffuse[4];     // diffuse colour
    GLfloat specular[4];    // specular colour
    GLfloat ambient[4];     // ambient colour
    GLuint indexBufSize;    // index buffer size - as required by DrawElements
    GLsizei numInstances;   // number of instances of the object to render - as require by DrawElementsInstanced
    GLuint texID;           // texture identifier - largely unused at the moment
    bool   current;         // set to true if this is part of current manipulator, controls alpha transparency on rendering
};

class Shape
{
private:
    std::vector<float> verts;   //< vertex, texture and normal data
    GLuint vaoConstraint;       //< openGL handles for various buffers
    GLuint vboConstraint;
    GLuint iboConstraint;
    GLuint iBuffer;             //< handle for the transform instance buffer
    GLuint cBuffer;             //< handle for the colour variation instance buffer
    GLfloat diffuse[4], ambient[4], specular[4]; // material properties
    int numInstances;

    /**
     * Create a sphere vertex at specified integer latitude and longitude with a transformation matrix applied and append to existing geometry
     * @param radius    radius of sphere
     * @param lat       latitude as proportion
     * @param lon       longitude as proportion
     * @param trm       model transformation matrix
     */
    void genSphereVert(float radius, float lat, float lon, glm::mat4x4 trm);

    /**
      * Create a hemisphere vertex at specified latitude (starting at equator) and longitude with a transformation matrix applied and append to existing geometry
      * @param radius    radius of sphere
      * @param lat       latitude as proportion
      * @param lon       longitude as proportion
      * @param trm       model transformation matrix
      */
    void genHemisphereVert(float radius, float lat, float lon, glm::mat4x4 trm);

public:

    std::vector<unsigned int> indices;   // vertex indices for triangles

    Shape()
    {
        vaoConstraint = 0;
        vboConstraint = 0;
        iboConstraint = 0;

        // default colour
        diffuse[0] = 0.325f; diffuse[1] = 0.235f; diffuse[3] = diffuse[2] = 1.0f;
        numInstances = 1;
    }

    ~Shape()
    {
        clear();
    }

    void clear()
    {
        verts.clear();
        indices.clear();
    }

    void removeAllInstances()
    {
        numInstances = 0;
    }

    /// getter for shape colour
    GLfloat * getColour(){ return diffuse; }

    /// setter for shape colour
    void setColour(GLfloat * col);

    /// setter for flat-shaded colour where ambient, diffuse and specular all have the same values
    void setFlatColour(GLfloat * col);

    /**
     * Create a cylinder originally lying along the positive z-axis and append to existing geometry
     * @param radius      radius of cylinder
     * @param height    length of cylinder
     * @param slices    number of arcs subdividing the cylinder circle
     * @param stacks    number of subdivision along the z-axis
     * @param trm       model transformation matrix
     */
    void genCylinder(float radius, float height, int slices, int stacks, glm::mat4x4 trm);

    /**
     * Create a cylinder with closed capped ends, originally lying along the positive z-axis and append to existing geometry
     * @param startradius   radius of cylinder at its beginning
     * @param endradius     radius of cylinder at its end
     * @param height    length of cylinder
     * @param slices    number of arcs subdividing the cylinder circle
     * @param stacks    number of subdivision along the z-axis
     * @param trm       model transformation matrix
     * @param clip      clip against the edges of the terrain, if true
     */
    void genCappedCylinder(float startradius, float endradius, float height, int slices, int stacks, glm::mat4x4 trm, bool clip);

    /**
     * Create a cone with a closed capped based, originally lying along the positive z-axis and append to existing geometry
     * @param startradius   radius of cone at its beginning
     * @param height    length of cone
     * @param slices    number of arcs subdividing the cone circle
     * @param stacks    number of subdivision along the z-axis
     * @param trm       model transformation matrix
     * @param clip      clip against the edges of the terrain, if true
     */
    void genCappedCone(float startradius, float height, int slices, int stacks, glm::mat4x4 trm, bool clip);

    /**
     * Create a truncated square-based pyramid, originally lying along the positive z-axis and append to existing geometry
     * @param baselen   length on a side for base
     * @param toplen    length on a side for top
     * @param height    height of pyramid
     * @param trm       model transformation matrix
     */
    void genPyramid(float baselen, float toplen, float height, glm::mat4x4 trm);

    /**
     * Create a sphere with a transformation matrix applied and append to existing geometry
     * @param radius    radius of sphere
     * @param slices    number of azimuth subdivisions
     * @param stacks    number of elevation subdivisions
     * @param trm       model transformation matrix
     */
    void genSphere(float radius, int slices, int stacks, glm::mat4x4 trm);

    /**
     * Draw a thick smooth line given a vector of 3D positions
     * @param curve     point positions along curve
     * @param view      current viewpoint
     * @param thickness width of the curve
     * @param tol       determines the gap between point to prevent oversaturation
     * @param closed    true if this represents a closed loop
     * @param offset    move the line closer to the viewpoint if true to avoid z-fighting
     * @param viewadapt scale line width according to distance from the viewpoint if true
     */
    void genCurve(std::vector<vpPoint> &curve, View * view, float thickness, float tol, bool closed, bool offset, bool viewadapt);

    /**
     * Draw a thick smooth generalized cylinder given a vector of 3D positions
     * @param curve     point positions along curve
     * @param radius    half width of the curve
     * @param tol       determines the gap between points to prevent oversaturation
     * @param slices    number of subdivisionsions around the cylinder
     */
    void genCylinderCurve(std::vector<vpPoint> &curve, float radius, float tol, int slices);

    /**
      * Draw a thick smooth generalized cylinder given a vector of 3D positions, with some cylinders suppressed for a dashed effect
      * @param curve     point positions along curve
      * @param radius    half width of the curve
      * @param tol       determines the gap between points to prevent oversaturation
      * @param dashlen   length of a dash segment
      * @param slices    number of subdivisionsions around the cylinder
      */
     void genDashedCylinderCurve(std::vector<vpPoint> &curve, float radius, float tol, float dashlen, int slices);

     /**
       * Draw a thick smooth generalized cylinder given a vector of 3D positions, with some cylinders suppressed for a dashed effect
       * Each cylinder will have a set number of points included before the dash gap starts as an indicator of speed
       * @param curve     point positions along curve
       * @param radius    half width of the curve
       * @param tol       determines the gap between points to prevent oversaturation
       * @param dashcnt   number of points in a dash
       * @param gaplen    length of the gap between dashes
       * @param jumptol   supress drawing if the gap between points is longer than this threshold
       * @param slices    number of subdivisionsions around the cylinder
       */
      void genSpeedCylinderCurve(std::vector<vpPoint> &curve, float radius, float tol, int dashcnt, float gaplen, float jumptol, int slices);
    
    /**
      * Draw a thick smooth generalized cylinder given a vector of 3D positions, with some cylinders suppressed for a dashed effect. Each cylinder widens from the start radius to the end radius along its length.
      * Each cylinder will have a set number of points included before the dash gap starts as an indicator of speed
      * @param curve     point positions along curve
      * @param startradius    half width of the curve at the start of a dash
      * @param endradius      half width of the curve at the end of a dash
      * @param tol       determines the gap between points to prevent oversaturation
      * @param dashcnt   number of points in a dash
      * @param gaplen    length of the gap between dashes
      * @param jumptol   supress drawing if the gap between points is longer than this threshold
      * @param slices    number of subdivisionsions around the cylinder
      */
      void genSpeedCylinderCurveWedge(std::vector<vpPoint> &curve, float startradius, float endradius, float tol, int dashcnt, float gaplen, float jumptol, int slices);

    /**
     * draw vertices of a curve as spheres
     * @param curve        point positions along curve
     * @param thickness    radius of spheres
     */
    void genSphereCurve(std::vector<vpPoint> &curve, float thickness);

    /**
       * Create a hemisphere with a transformation matrix applied and append to existing geometry
       * @param radius    radius of sphere
       * @param slices    number of azimuth subdivisions
       * @param stacks    number of elevation subdivisions
       * @param trm       model transformation matrix
       */
    void genHemisphere(float radius, int slices, int stacks, glm::mat4x4 trm);
    
    /**
       * Create a plane with a transformation matrix applied and append to existing geometry
       * @param trm       model transformation matrix
       */
    void genPlane(glm::mat4x4 trm);

    /**
     * Convert a mesh structure to openGL geometry
     * @param points    list of vertices
     * @param norms     list of vertex normals
     * @param faces     flattened list of vertex indices, with each group of 3 indices representing a triangle
     * @param trm       model transformation matrix
     */
    void genMesh(std::vector<vpPoint> * points, std::vector<Vector> * norms, std::vector<int> * faces, glm::mat4x4 trm);

    /**
     * test case for creating geomatry. In this case a single triangle in the x-z plane
     */
    void genTest();

    /**
     * Return data required for a draw call, such as the VAO, colour, etc.
     */
    ShapeDrawData getDrawParameters();

    /**
     * Bind the appropriate OpenGL buffers for rendering instances. Only needs to be done if
     * the instances attributes change.
     * @param iforms    transformation applied to each instance. If this is empty assume a single instance with identity transformation.
     * @param icols     colour offset applied to each instance. Must match the size of iforms.
     * @retval @c true if buffers successfully bound
     */
    bool bindInstances(std::vector<glm::mat4> * iforms, std::vector<glm::vec4> * icols);
};

#endif
