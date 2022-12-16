#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "display.h"
#include "io.h"
#include "definitions.h"
#include <math.h>
#include <stdbool.h>

/** GLOBAL VARIABLES **/

GLdouble _window_ratio;                     /*Control of window's proportions */
GLdouble _ortho_x_min,_ortho_x_max;         /*Variables for the control of the orthographic projection*/
GLdouble _ortho_y_min ,_ortho_y_max;        /*Variables for the control of the orthographic projection*/
GLdouble _ortho_z_min,_ortho_z_max;         /*Variables for the control of the orthographic projection*/

object3d * _first_object= 0;                /*List of objects*/
object3d * _selected_object = 0;            /*Object currently selected*/

object3d *_first_kamera = 0;
object3d *_selected_kamera = 0;

char aldaketa_mota; 
char erreferentzi_sistema;
char zer_aldatu;
char kam_mota;

double ezker,eskuin,behekoa,goikoa,near,far;

/** GENERAL INITIALIZATION **/
void initialization (){
    double mat[16];
    int i;
    object3d *aux_kamera;

    ezker = -0.1;
    eskuin = 0.1;
    behekoa = -0.1;
    goikoa = 0.1;
    near = 0.1;
    far = 1000;
    kam_mota = 'l';

    /*Initialization of all the variables with the default values*/
    _ortho_x_min = KG_ORTHO_X_MIN_INIT;
    _ortho_x_max = KG_ORTHO_X_MAX_INIT;
    _ortho_y_min = KG_ORTHO_Y_MIN_INIT;
    _ortho_y_max = KG_ORTHO_Y_MAX_INIT;
    _ortho_z_min = KG_ORTHO_Z_MIN_INIT;
    _ortho_z_max = KG_ORTHO_Z_MAX_INIT;

    _window_ratio = (GLdouble) KG_WINDOW_WIDTH / (GLdouble) KG_WINDOW_HEIGHT;

    /*Definition of the background color*/
    glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);

    /*Definition of the method to draw the objects*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    erreferentzi_sistema = 'g';
    zer_aldatu = 'o';
    aldaketa_mota = 't';

    //(5, 5, 5) posiziotik (0, 0, 0)-ra begira

    aux_kamera = (object3d *)malloc(sizeof(object3d));
    aux_kamera->min.x = ezker;
    aux_kamera->max.x = eskuin;
    aux_kamera->min.y = behekoa;
    aux_kamera->max.y = goikoa;
    aux_kamera->min.z = near;
    aux_kamera->max.z = far;

    mat[0] = sqrt(2)/2;  mat[4] = -sqrt(2)/(2*sqrt(3)); mat[8] = 1/sqrt(3);  mat[12] = 5;
    mat[1] = 0;          mat[5] = sqrt(2)/sqrt(3);      mat[9] = 1/sqrt(3);  mat[13] = 5;
    mat[2] = -sqrt(2)/2; mat[6] = -sqrt(2)/(2*sqrt(3)); mat[10] = 1/sqrt(3); mat[14] = 5;
    mat[3] = 0;          mat[7] = 0;                    mat[11] = 0;         mat[15] = 1;

    aux_kamera->mzptr =(mz *)malloc(sizeof(mz));
    for(i = 0; i<16; i++) aux_kamera->mzptr->matrize[i] = mat[i];
    aux_kamera->next = 0;

    _first_kamera = aux_kamera;
    _selected_kamera = aux_kamera;

    //Goikoa 

    aux_kamera = (object3d *)malloc(sizeof(object3d));
    aux_kamera->min.x = ezker;
    aux_kamera->max.x = eskuin;
    aux_kamera->min.y = behekoa;
    aux_kamera->max.y = goikoa;
    aux_kamera->min.z = near;
    aux_kamera->max.z = far;

    mat[0] = 1;  mat[4] = 0; mat[8] = 0;  mat[12] = 0;
    mat[1] = 0;  mat[5] = 0; mat[9] = 1;  mat[13] = 5;
    mat[2] = 0;  mat[6] = -1;mat[10] = 0; mat[14] = 0;
    mat[3] = 0;  mat[7] = 0; mat[11] = 0; mat[15] = 1;

    aux_kamera->mzptr =(mz *)malloc(sizeof(mz));
    for(i = 0; i<16; i++) aux_kamera->mzptr->matrize[i] = mat[i];
    aux_kamera->next = _first_kamera;

    _first_kamera = aux_kamera;
    _selected_kamera = aux_kamera;

    //Albokoa 

    aux_kamera = (object3d *)malloc(sizeof(object3d));
    aux_kamera->min.x = ezker;
    aux_kamera->max.x = eskuin;
    aux_kamera->min.y = behekoa;
    aux_kamera->max.y = goikoa;
    aux_kamera->min.z = near;
    aux_kamera->max.z = far;

    mat[0] = 0;  mat[4] = 0; mat[8] = 1;  mat[12] = 5;
    mat[1] = 0;  mat[5] = 1; mat[9] = 0;  mat[13] = 0;
    mat[2] = -1; mat[6] = 0; mat[10] = 0; mat[14] = 0;
    mat[3] = 0;  mat[7] = 0; mat[11] = 0; mat[15] = 1;

    aux_kamera->mzptr =(mz *)malloc(sizeof(mz));
    for(i = 0; i<16; i++) aux_kamera->mzptr->matrize[i] = mat[i];
    aux_kamera->next = _first_kamera;

    _first_kamera = aux_kamera;
    _selected_kamera = aux_kamera;

    printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);
}


/** MAIN FUNCTION **/
int main(int argc, char** argv) {

    /*First of all, print the help information*/
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);

    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutSpecialFunc(tekla_berezien_arretarako_funtzioa);

    /* this initialization has to be AFTER the creation of the window */
    initialization();

    /* start the main loop */
    glutMainLoop();
    return 0;
}
