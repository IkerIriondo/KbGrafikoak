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

argia *bonbila, *eguzkia, *fokoa, *_selected_argia;

char aldaketa_mota; 
char erreferentzi_sistema;
char zer_aldatu;
char kam_mota;
char poligonoak;
char argiak, argi1, argi2, argi3, argi4, argi5, argi6, argi7, argi8;
char flat_smooth;

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
    poligonoak = 'b';
    argiak = 'b';
    argi1 = 'i';
    argi2 = 'i';
    argi3 = 'i';
    argi4 = 'i';
    argi5 = 'i';
    argi6 = 'i';
    argi7 = 'i';
    argi8 = 'i';

    flat_smooth = 'f';

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
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
    aux_kamera->kam_mota = '0'; //paraleloan, 1 perspektiba

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
    aux_kamera->kam_mota = '1'; //paraleloan, 1 perspektiba

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
    aux_kamera->kam_mota = '0'; //paraleloan, 1 perspektiba

    _first_kamera = aux_kamera;
    _selected_kamera = aux_kamera;

    printf("Erreferentzi sistema: %c, Zer aldatu: %c, Aldaketa mota: %c\n", erreferentzi_sistema, zer_aldatu, aldaketa_mota);

    //ARGIAK
    _selected_argia = 0;
    GLfloat horia [4] = {0.0 , 1.0 , 1.0 , 1.0};
    GLfloat grisa [4] = {0.2 , 0.2 , 0.2 , 1.0};
    GLfloat txuria [4] = {1.0 , 1.0 , 1.0 , 1.0};

    //BONBILA

    bonbila = (argia *)malloc(sizeof(argia));

    bonbila->argi_zenb = GL_LIGHT0;
    bonbila->kokapena[0] = 0.0; bonbila->kokapena[1] = 10.0;
    bonbila->kokapena[2] = 0.0; bonbila->kokapena[3] = 1.0;
    bonbila->angelua = 180.0;
    bonbila->argi_mota = 'b';

    glLightfv(bonbila->argi_zenb, GL_AMBIENT, grisa);
    glLightfv(bonbila->argi_zenb, GL_DIFFUSE, horia);
    glLightfv(bonbila->argi_zenb, GL_SPECULAR, txuria);

    glLightfv(bonbila->argi_zenb, GL_POSITION, bonbila->kokapena);
    glLightf(bonbila->argi_zenb, GL_SPOT_CUTOFF, bonbila->angelua);

    //EGUZKIA

    eguzkia = (argia *)malloc(sizeof(argia));
    eguzkia->argi_zenb = GL_LIGHT1;
    eguzkia->norabidea[0] = 1.0; eguzkia->norabidea[1] = 0.0;
    eguzkia->norabidea[2] = 0.0; eguzkia->norabidea[3] = 0.0;
    eguzkia->argi_mota = 'e';

    glLightfv(eguzkia->argi_zenb, GL_AMBIENT, grisa);
    glLightfv(eguzkia->argi_zenb, GL_DIFFUSE, horia);
    glLightfv(eguzkia->argi_zenb, GL_SPECULAR, txuria);

    glLightfv(eguzkia->argi_zenb, GL_POSITION, eguzkia->norabidea);

    //FOKOA

    fokoa = (argia *)malloc(sizeof(argia));
    fokoa->argi_zenb = GL_LIGHT2;

    fokoa->kokapena[0] = _selected_kamera->mzptr->matrize[12];
    fokoa->kokapena[1] = _selected_kamera->mzptr->matrize[13];
    fokoa->kokapena[2] = _selected_kamera->mzptr->matrize[14];
    fokoa->kokapena[3] = _selected_kamera->mzptr->matrize[15];

    fokoa->norabidea[0] = -_selected_kamera->mzptr->matrize[8];
    fokoa->norabidea[1] = -_selected_kamera->mzptr->matrize[9];
    fokoa->norabidea[2] = -_selected_kamera->mzptr->matrize[10];

    fokoa->angelua = 45.0;
    fokoa->intentsitatea = 0.7;
    fokoa->argi_mota = 'f';

    glLightfv(fokoa->argi_zenb, GL_AMBIENT, grisa);
    glLightfv(fokoa->argi_zenb, GL_DIFFUSE, horia);
    glLightfv(fokoa->argi_zenb, GL_SPECULAR, txuria);

    glLightfv(fokoa->argi_zenb, GL_POSITION, fokoa->kokapena);
    glLightfv(fokoa->argi_zenb, GL_SPOT_DIRECTION, fokoa->norabidea2);
    glLightf(fokoa->argi_zenb, GL_SPOT_CUTOFF , fokoa->angelua);
    glLightf(fokoa->argi_zenb, GL_SPOT_EXPONENT, fokoa->intentsitatea);

}


/** MAIN FUNCTION **/
int main(int argc, char** argv) {

    /*First of all, print the help information*/
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);


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
