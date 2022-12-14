#include "definitions.h"
#include "io.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/** EXTERNAL VARIABLES **/

extern GLdouble _window_ratio;
extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern object3d *_first_object;
extern object3d *_selected_object;

extern object3d *_first_kamera;
extern object3d *_selected_kamera;

extern char aldaketa_mota;
extern char zer_aldatu;
extern char erreferentzi_sistema;
extern char kam_mota;

extern double ezker,eskuin,behekoa,goikoa,near,far;
extern bool obj_ikuspegia;

void esam_matrizea_lortu(double *esamptr, double *mptr){ //mptr zutabetan dagoena errenkadetan jarri

    esamptr[0] = mptr[0];  esamptr[4] = mptr[1];  esamptr[8] = mptr[2];   esamptr[12] = -(mptr[0]*mptr[12] + mptr[1]*mptr[13] + mptr[2]*mptr[14]);
    esamptr[1] = mptr[4];  esamptr[5] = mptr[5];  esamptr[9] = mptr[6];   esamptr[13] = -(mptr[4]*mptr[12] + mptr[5]*mptr[13] + mptr[6]*mptr[14]);
    esamptr[2] = mptr[8];  esamptr[6] = mptr[9];  esamptr[10] = mptr[10]; esamptr[14] = -(mptr[8]*mptr[12] + mptr[9]*mptr[13] + mptr[10]*mptr[14]);
    esamptr[3] = 0;        esamptr[7] = 0;        esamptr[11] = 0;        esamptr[15] = 1;
    
}

/**
 * @brief Function to draw the axes
 */
void draw_axes()
{
    /*Draw X axis*/
    glColor3f(KG_COL_X_AXIS_R,KG_COL_X_AXIS_G,KG_COL_X_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(1,0,0);
    glVertex3d(0,0,0);
    glEnd();
    /*Draw Y axis*/
    glColor3f(KG_COL_Y_AXIS_R,KG_COL_Y_AXIS_G,KG_COL_Y_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,1,0);
    glVertex3d(0,0,0);
    glEnd();
    /*Draw Z axis*/
    glColor3f(KG_COL_Z_AXIS_R,KG_COL_Z_AXIS_G,KG_COL_Z_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,0,1);
    glVertex3d(0,0,0);
    glEnd();
}

void drawText(const char *text, int length, int x, int y){
    int i;
    
    glMatrixMode(GL_PROJECTION);
    double matrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x,y);
    for(i = 0; i < length; i++) 
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);

}

/**
 * @brief Callback reshape function. We just store the new proportions of the window
 * @param width New width of the window
 * @param height New height of the window
 */
void reshape(int width, int height) {
    // we will use a rectangular viewport allways
    if (width > height)
        glViewport(0, 0, height, height);
      else 
        glViewport(0, 0, width, width);
    //  old code
    //  glViewport(0, 0, width, height);
    //  Take care, the width and height are integer numbers, but the ratio is a GLdouble so, in order to avoid
    //  rounding the ratio to integer values we need to cast width and height before computing the ratio */
    // _window_ratio = (GLdouble) width / (GLdouble) height;
}


/**
 * @brief Callback display function
 */
void display(void) {
    double ESAM[16];
    GLint v_index, v, f;
    object3d *aux_obj = _first_object;

    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Define the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*When the window is wider than our original projection plane we extend the plane in the X axis* /
    if ((_ortho_x_max - _ortho_x_min) / (_ortho_y_max - _ortho_y_min) < _window_ratio) {
        /* New width * /
        GLdouble wd = (_ortho_y_max - _ortho_y_min) * _window_ratio;
        /* Midpoint in the X axis * /
        GLdouble midpt = (_ortho_x_min + _ortho_x_max) / 2;
        /*Definition of the projection* /
        glOrtho(midpt - (wd / 2), midpt + (wd / 2), _ortho_y_min, _ortho_y_max, _ortho_z_min, _ortho_z_max);
    } else {/* In the opposite situation we extend the Y axis * /
        /* New height * /
        GLdouble he = (_ortho_x_max - _ortho_x_min) / _window_ratio;
        /* Midpoint in the Y axis * /
        GLdouble midpt = (_ortho_y_min + _ortho_y_max) / 2;
        /*Definition of the projection* /
        glOrtho(_ortho_x_min, _ortho_x_max, midpt - (he / 2), midpt + (he / 2), _ortho_z_min, _ortho_z_max);
    }*/
    //glOrtho(_selected_kamera->min.x, _selected_kamera->max.x, _selected_kamera->min.y, _selected_kamera->max.y, _selected_kamera->min.z, _selected_kamera->max.z);
    //glOrtho(_ortho_x_min, _ortho_x_max, _ortho_y_min, _ortho_y_max, _ortho_z_min, _ortho_z_max);
    //glFrustum(ezker(-x),eskubi(+x),behekoa(-y), goikoa(+y), near(-z), far(+z))
    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    //glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);
    glFrustum(_selected_kamera->min.x, _selected_kamera->max.x, _selected_kamera->min.y, _selected_kamera->max.y, _selected_kamera->min.z, _selected_kamera->max.z);

    //glFrustum(_selected_kamera->min.x, _selected_kamera->max.x, _selected_kamera->min.y, _selected_kamera->max.y, _selected_kamera->min.z, _selected_kamera->max.z);
    /* Now we start drawing the object */

    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    /*First, we draw the axes*/
    draw_axes();

    /*Now each of the objects in the list*/
    while (aux_obj != 0) {

        /* Select the color, depending on whether the current object is the selected one or not */
        if (aux_obj == _selected_object){
            glColor3f(KG_COL_SELECTED_R,KG_COL_SELECTED_G,KG_COL_SELECTED_B);
        }else{
            glColor3f(KG_COL_NONSELECTED_R,KG_COL_NONSELECTED_G,KG_COL_NONSELECTED_B);
        }

        if(kam_mota == 'o'){
            esam_matrizea_lortu(&(ESAM[0]),_selected_object->mzptr->matrize);
            glLoadMatrixd(ESAM);
        }else{
            esam_matrizea_lortu(&(ESAM[0]),_selected_kamera->mzptr->matrize);
            glLoadMatrixd(ESAM);
        }
        /* esam_matrizea_lortu(&(ESAM[0]),_selected_kamera->mzptr->matrize);
            glLoadMatrixd(ESAM);
            glLoadIdentity();
            esam_matrizea_lortu(&(ESAM[0]),_selected_object->mzptr->matrize);
            glLoadMatrixd(ESAM);*/
        glMultMatrixd(aux_obj->mzptr->matrize);

        /* Draw the object; for each face create a new polygon with the corresponding vertices */
        //glMultMatrixd(aux_obj->mzptr->matrize); //behekoagatik aldatu
        //glLoadMatrixd(aux_obj->mzptr->matrize);

        for (f = 0; f < aux_obj->num_faces; f++) {
            glBegin(GL_POLYGON);
            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                v_index = aux_obj->face_table[f].vertex_table[v];
                glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                           aux_obj->vertex_table[v_index].coord.y,
                           aux_obj->vertex_table[v_index].coord.z);
            }
            glEnd();
        }
        aux_obj = aux_obj->next;
    }

    /*Do the actual drawing*/
    glFlush();
}

