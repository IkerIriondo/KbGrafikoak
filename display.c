#include "definitions.h"
#include "io.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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
extern char poligonoak;

extern double ezker,eskuin,behekoa,goikoa,near,far;

void esam_matrizea_lortu(double *esamptr, double *mptr){ //mptr zutabetan dagoena errenkadetan jarri

    esamptr[0] = mptr[0];  esamptr[4] = mptr[1];  esamptr[8] = mptr[2];   esamptr[12] = -(mptr[0]*mptr[12] + mptr[1]*mptr[13] + mptr[2]*mptr[14]);
    esamptr[1] = mptr[4];  esamptr[5] = mptr[5];  esamptr[9] = mptr[6];   esamptr[13] = -(mptr[4]*mptr[12] + mptr[5]*mptr[13] + mptr[6]*mptr[14]);
    esamptr[2] = mptr[8];  esamptr[6] = mptr[9];  esamptr[10] = mptr[10]; esamptr[14] = -(mptr[8]*mptr[12] + mptr[9]*mptr[13] + mptr[10]*mptr[14]);
    esamptr[3] = 0;        esamptr[7] = 0;        esamptr[11] = 0;        esamptr[15] = 1;
    
}


void kam_objri_begira(){

    int i;
    double kam_non[3];
    double kam_nora[3];
    
    kam_non[0] = _selected_kamera->mzptr->matrize[12];
    kam_non[1] = _selected_kamera->mzptr->matrize[13];
    kam_non[2] = _selected_kamera->mzptr->matrize[14];


    if(_selected_object != 0){
        kam_nora[0] = _selected_object->mzptr->matrize[12];
        kam_nora[1] = _selected_object->mzptr->matrize[13];
        kam_nora[2] = _selected_object->mzptr->matrize[14];
    }else{
        kam_nora[0] = 0.0; kam_nora[1] = 0.0; kam_nora[2] = 0.0;
    }


    double x[3];
    double y[3];
    double z[3];

    double ken[3];
    double norma;
    ken[0] = kam_non[0] - kam_nora[0]; ken[1] = kam_non[1] - kam_nora[1]; ken[2] = kam_non[2] - kam_nora[2];

    norma = sqrt(ken[0]*ken[0] + ken[1]*ken[1] + ken[2]*ken[2]);

    z[0] = ken[0]/norma; z[1] = ken[1]/norma; z[2] = ken[2]/norma;

    double bider_bek[3];
    double vup[3];
    vup[0] = 0.0; vup[1] = 1.0; vup[2] = 0.0;

    bider_bek[0] = vup[1]*z[2] - vup[2]*z[1]; bider_bek[1] = vup[2]*z[0] - vup[0]*z[2]; bider_bek[2] = vup[0]*z[1] - vup[1]*z[0];
    norma = sqrt(bider_bek[0]*bider_bek[0] + bider_bek[1]*bider_bek[1] + bider_bek[2]*bider_bek[2]);
    
    x[0] = bider_bek[0]/norma; x[1] = bider_bek[1]/norma; x[2] = bider_bek[2]/norma;

    y[0] =  z[1]*x[2] - z[2]*x[1]; y[1] = z[2]*x[0] - z[0]*x[2]; y[2] = z[0]*x[1] - z[1]*x[0];

    double mat[16];

    mat[0] = x[0]; mat[4] = y[0]; mat[8] = z[0];  mat[12] = kam_non[0];
    mat[1] = x[1]; mat[5] = y[1]; mat[9] = z[1];  mat[13] = kam_non[1];
    mat[2] = x[2]; mat[6] = y[2]; mat[10] = z[2]; mat[14] = kam_non[2];
    mat[3] = 0;    mat[7] = 0;    mat[11] = 0;    mat[15] = 1;

    mz *matberria;
    matberria = (mz *)malloc(sizeof(mz));
    for(i = 0; i<16; i++) matberria->matrize[i] = mat[i];
    matberria->next = _selected_kamera->mzptr;
    _selected_kamera->mzptr = matberria;

}

void kamera_esferan_biratu(double *v){

    double at[3];
    double mat[16];

    int i;
    mz *mzpt;
    at[0] = _selected_object->mzptr->matrize[12];
    at[1] = _selected_object->mzptr->matrize[13];
    at[2] = _selected_object->mzptr->matrize[14];

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(at[0], at[1], at[2]);
    glRotated(10, v[0], v[1], v[2]);
    glTranslated(-at[0], -at[1], -at[2]);
    glMultMatrixd(_selected_kamera->mzptr->matrize);
    glGetDoublev(GL_MODELVIEW_MATRIX, mat);

    mzpt = (mz *)malloc(sizeof(mz));

    for(i = 0; i<16;i++) mzpt->matrize[i] = mat[i];
    mzpt->next = _selected_kamera->mzptr;
    _selected_kamera->mzptr = mzpt;

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
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(_selected_kamera->min.x, _selected_kamera->max.x, _selected_kamera->min.y, _selected_kamera->max.y, _selected_kamera->min.z, _selected_kamera->max.z);

    /* Now we start drawing the object */

    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    if(poligonoak == 'b'){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    /*if (poligonoak == 'h') {
        glShadeModel(GL_SMOOTH);  // Erpin bakoitzaren bektore normala beharrezkoa da
        //printf("Hutsa\n");
    }else{
        glShadeModel(GL_FLAT);  // poligonoen bektore normalarekin nahikoa da kasu honetan
        //printf("Beteta\n");
    } */

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

        if (aux_obj->flat_smooth == 's') {
            glShadeModel(GL_SMOOTH);  // Erpin bakoitzaren bektore normala beharrezkoa da
        }else{
            glShadeModel(GL_FLAT);  // poligonoen bektore normalarekin nahikoa da kasu honetan
        }        

        if(kam_mota == 'o'){
            glFrustum(_selected_kamera->min.x, _selected_kamera->max.x, _selected_kamera->min.y, _selected_kamera->max.y, _selected_kamera->min.z, _selected_kamera->max.z);
            esam_matrizea_lortu(&(ESAM[0]),_selected_object->mzptr->matrize);
        }else{
            if(_selected_kamera->kam_mota == '0'){
                glOrtho(_selected_kamera->min.x, _selected_kamera->max.x, _selected_kamera->min.y, _selected_kamera->max.y, _selected_kamera->min.z, _selected_kamera->max.z);
            }else{
                glFrustum(_selected_kamera->min.x, _selected_kamera->max.x, _selected_kamera->min.y, _selected_kamera->max.y, _selected_kamera->min.z, _selected_kamera->max.z);
            }
            esam_matrizea_lortu(&(ESAM[0]),_selected_kamera->mzptr->matrize);
        }
        glLoadMatrixd(ESAM);
        glMultMatrixd(aux_obj->mzptr->matrize);

        //Erpinen bektore normalak marrazteko
        /*for(f = 0; f < aux_obj->num_vertices; f++){
            glBegin(GL_LINES);
            glVertex3d(aux_obj->vertex_table[f].coord.x,
                       aux_obj->vertex_table[f].coord.y,
                       aux_obj->vertex_table[f].coord.z);
            glVertex3d(aux_obj->vertex_table[f].coord.x + aux_obj->vertex_table[f].bektore_normala[0],
                       aux_obj->vertex_table[f].coord.y + aux_obj->vertex_table[f].bektore_normala[1],
                       aux_obj->vertex_table[f].coord.z + aux_obj->vertex_table[f].bektore_normala[2]);
            glEnd();
        }*/

        for (f = 0; f < aux_obj->num_faces; f++) {
            glBegin(GL_POLYGON);
            if(poligonoak == 'b'){
                glNormal3d(aux_obj->face_table[f].bektore_normala[0], aux_obj->face_table[f].bektore_normala[1], aux_obj->face_table[f].bektore_normala[2]);
            }
            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                v_index = aux_obj->face_table[f].vertex_table[v];
                if(poligonoak == 'h'){
                    glNormal3d(aux_obj->vertex_table[v_index].bektore_normala[0], aux_obj->vertex_table[v_index].bektore_normala[1], aux_obj->vertex_table[v_index].bektore_normala[2]);
                }
                glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                           aux_obj->vertex_table[v_index].coord.y,
                           aux_obj->vertex_table[v_index].coord.z);
            }
            glEnd();
        }
        aux_obj = aux_obj->next;
    }
    /*Do the actual drawing*/
    glutSwapBuffers();  // Buffer bikoitza erabili dugunez bata besteagatik trukatu
    //glFlush();
}

